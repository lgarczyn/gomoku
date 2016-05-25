//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#include "Game.hpp"
#include "Analyzer.hpp"
#include "ThreadPool.hpp"

using namespace std;

const double timeMargin = 0.002;
const int initialWidth = 20;
const int deepWidth = 20;
const int threadCount = 8;

Game::Game(const Options& options) :
		_options(options),
		_timeLimit(options.slowMode ? 10 : 0.5),
		_constDepth(6 + options.slowMode),
		_timeTaken()
{
#ifdef ANALYZER_AVAILABLE
	_analyzer = _options.brainDead ?
			   (IAnalyzer*)new AnalyzerBrainDead() :
			   (IAnalyzer*)new Analyzer();
#else
	_analyzer = new AnalyzerBrainDead();
#endif
	_turn = PlayerColor::blackPlayer;
	_depth = _constDepth;
	_state = new Board(_turn);
	_state->fillTaboo(_options.limitBlack, _options.doubleThree, _turn);
	_previousState = nullptr;
	_pool = new Pool(threadCount);
}

Game::~Game()
{
	delete _pool;
	delete _analyzer;
	delete _state;
	delete _previousState;
}

Score Game::negamax(Board& node, int negDepth, Score alpha, Score beta, PlayerColor player)
{
	node.fillPriority(player, _options);
	auto children = node.getChildren(player, deepWidth);

	if (!children.size())
		throw std::logic_error("GetChildren returned an empty array");

	//std::vector<Score> scores = std::vector<Score>(children.size());
	size_t i;
	Score bestScore = ninfinity - 100;
	for (i = 0; i < children.size(); i++)
	{
		if (alpha <= beta && !isOverdue())
		{
			BoardPos pos = children[i].pos;
			Board *board = new Board(node, pos, player, _options);
			Score score;
			if (board->getVictory().type)
			{
				score = (pinfinity + negDepth) * (board->getVictory().victor * player);
			}
			else if (negDepth <= 1)
			{
				score = player * _analyzer->getScore(*board, _options.captureWin);
			}
			else
			{
				score = -negamax(*board, negDepth - 1, -beta, -alpha, -player);
			}
			bestScore = std::max(bestScore, score);
			alpha = std::max(alpha, score);
			delete board;
		}
		else
		{
			break;
		}
	}

	return bestScore;
}

MoveScore Game::negamax_thread(ThreadData data)
{
	Score score;

	Board* board = data.node.board;
	BoardPos pos = data.node.move;

	if (isOverdue())
	{
		delete board;
		return MoveScore(ninfinity, pos);
	}
	if (data.alpha->load() > pinfinity)
	{
		delete board;
		return MoveScore(ninfinity, pos);
	}

	std::atomic<Score>* alpha = data.alpha;

	if (board->getVictory().type)
	{
		score = (pinfinity + _depth) * (board->getVictory().victor * data.player);
	}
	else
	{
		score = -negamax(*board, _depth - 1, ninfinity, -alpha->load(), -data.player);
	}

	alpha->store(std::max(alpha->load(), score));

	delete board;
	return (MoveScore(score, pos));
}

BoardPos Game::start_negamax(Board *node, PlayerColor player)
{
	std::atomic<Score> alpha(ninfinity);
	std::vector<MoveScore> children = node->getChildren(player, initialWidth);

	if (!children.size())
		throw std::logic_error("GetChildren returned an empty array");

	std::vector<ThreadData> threadData(children.size());

	for (size_t i = 0; i < children.size(); i++)
	{
		threadData[i] =	ThreadData(
				ChildBoard(
						new Board(*node, children[i].pos, player, _options),
						children[i].pos),
				&alpha,
				player);
	}


	std::function<MoveScore(ThreadData)> function = boost::bind(&Game::negamax_thread, this, _1);
#ifdef NON_THREADED
	std::vector<MoveScore> result;
	result.resize(threadData.size());
	for (size_t i = 0; i < threadData.size(); i++)
	{
		result[i] = function(threadData[i]);
	}
#else
	std::vector<MoveScore> result = _pool->run(function, threadData);
#endif

	MoveScore bestMove(ninfinity - 100);
	std::vector<MoveScore>	choice;
	for (size_t i = 0; i < result.size(); i++)
	{
		MoveScore move = result[i];

		if (move.score > bestMove.score)
		{
			choice.clear();
			bestMove = move;
			choice.push_back(bestMove);
		}
		else if (move.score == bestMove.score && move.score > ninfinity)
		{
			bestMove = move;
			choice.push_back(bestMove);
		}
	}
	return choice[rand() % choice.size()].pos;//TODO get fucking rid of rand
}


bool Game::isOverdue() const
{
	using namespace std;
	auto current = std::chrono::high_resolution_clock::now();

	double difference = std::chrono::duration_cast<std::chrono::milliseconds>(current - _start).count();

	return difference > (_timeLimit - timeMargin) * 1000;
}

double Game::getTimeDiff() const
{
	auto current = std::chrono::high_resolution_clock::now();
	double difference = double(std::chrono::duration_cast<std::chrono::milliseconds>(current - _start).count()) / 1000;
	return difference;
}

BoardPos Game::getNextMove()
{
	_start = std::chrono::high_resolution_clock::now();

	BoardPos pos = start_negamax(_state, _turn);

	_timeTaken = getTimeDiff();

	return pos;
}

Score Game::getCurrentScore() const
{
	return _analyzer->getScore(*_state, _options.captureWin);
}

bool Game::play(BoardPos pos)
{
	if (_state->getCase(pos) != BoardSquare::empty)
		return false;
	if (_state->getPriority(pos) < 0)
		return false;

	delete _previousState;
	_previousState = _state;
	_state = new Board(*_previousState, pos, _turn, _options);

	_turn = -_turn;
	_state->fillTaboo(_options.limitBlack, _options.doubleThree, _turn);
	_state->fillPriority(_turn, _options);

	return _state->getVictory().type;
}

bool Game::hasPosChanged(BoardPos pos) const
{
	if (_previousState == nullptr)
		return false;
	return _previousState->getCase(pos) != _state->getCase(pos);
}

double Game::getTimeTaken() const
{
	return _timeTaken;
}

bool Game::play()
{
	return play(getNextMove());
}

bool Game::isPlayerNext() const
{
	return
		(!_options.isBlackAI && getTurn() == blackPlayer) ||
		(!_options.isWhiteAI && getTurn() == whitePlayer);
}

Board *Game::getState()
{
	return (_state);
}

PlayerColor Game::getTurn() const
{
	return  (_turn);
}