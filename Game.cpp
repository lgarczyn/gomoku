//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#include "Game.hpp"
#include "Analyzer.hpp"

using namespace std;

const bool slowMode = true;
const double time_linit = (slowMode ? 30 : 0.5);
const double time_margin = 0.001;
const int threadCount = 8;
const int initial_width = 200;
const int deep_width = 200;
const int const_depth = 3;

Game::Game(Options options):_options(options), _timeTaken()
{
	_state = new Board();
#ifdef ANALYZER_AVAILABLE
	_analyzer = _options.brainDead ?
			   (IAnalyzer*)new AnalyzerBrainDead() :
			   (IAnalyzer*)new Analyzer();
#else
	_analyzer = new AnalyzerBrainDead();
#endif
	_turn = PlayerColor::blackPlayer;
	_depth = const_depth;
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

Score Game::negamax(Board* node, int negDepth, Score alpha, Score beta, PlayerColor player)
{
	auto children = node->getChildren(player, _options.capture, deep_width);

	if (!children.size())
		throw std::logic_error("GetChildren returned an empty array");

	Score bestScore = ninfinity;
	//std::vector<Score> scores = std::vector<Score>(children.size());
	size_t i;
	for (i = 0; i < children.size(); i++)
	{
		Board* board = children[i].board;

		if (alpha <= beta && !isOverdue())
		{
			Score score;
			if (board->isTerminal(_options.captureWin))
			{
				score = pinfinity + negDepth;
			}
			else if (negDepth <= 1)
			{
				score = player * _analyzer->getScore(*board, _options.captureWin);
			}
			else
			{
				score = -negamax(board, negDepth - 1, -beta, -alpha, -player);
			}
			if (score > bestScore)
			{
				bestScore = score;
			}
			alpha = std::max(alpha, score);
		}
		else

		delete board;
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

	std::atomic<int>* alpha = data.alpha;

	if (board->isTerminal(_options.captureWin))
	{
		score = pinfinity + _depth;
	}
	else
	{
		score = -negamax(board, _depth - 1, ninfinity, -alpha->load(), -data.player);
	}

	alpha->store(std::max(alpha->load(), score));

	delete board;
	return (MoveScore(score, pos));
}

BoardPos Game::start_negamax(Board *node, PlayerColor player)
{
	std::atomic<Score> alpha(ninfinity);
	std::vector<ChildBoard> children = node->getChildren(player, _options.capture, initial_width);

	if (!children.size())
		throw std::logic_error("GetChildren returned an empty array");

	std::vector<ThreadData> threadData(children.size());

	for (size_t i = 0; i < children.size(); i++)
	{
		threadData[i] =	ThreadData(
					children[i],
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
		else if (move.score == bestMove.score)
		{
			bestMove = move;
			choice.push_back(bestMove);
		}
	}
	return choice[rand() % choice.size()].pos;//TODO get fucking rid of rand
}


bool Game::isOverdue()
{
	return false;
	using namespace std;
	auto current = std::chrono::high_resolution_clock::now();

	return double(std::chrono::duration_cast<std::chrono::milliseconds>(current - _start).count()) > (time_linit - time_margin) * 1000;
}

BoardPos Game::getNextMove()
{
	_start = std::chrono::high_resolution_clock::now();

	BoardPos pos = start_negamax(_state, _turn);

	auto current = std::chrono::high_resolution_clock::now();
	_timeTaken = double(std::chrono::duration_cast<std::chrono::milliseconds>(current - _start).count()) / 1000;
	return pos;
}

bool Game::play(BoardPos pos)
{
	if (_state->getCase(pos) != BoardSquare::empty)
		return false;
	if (_state->getPriority(pos) < 0)
		return false;

	delete _previousState;
	_previousState = _state;
	_state = new Board(*_previousState, pos, _turn, _options.capture);

	_turn = -_turn;
	_state->fillTaboo(_options.limitBlack, _options.doubleThree, _turn);
	_state->fillPriority(_turn);

	return _state->isTerminal(pos, _options.captureWin);
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