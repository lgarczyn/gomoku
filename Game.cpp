//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#include "Game.hpp"
#include "Analyzer.hpp"
#include "ThreadPool.hpp"
#include "Board.hpp"
#include <boost/bind.hpp>

using namespace std;

const double timeMargin = 0.005;
const int initialWidth = 40;
const int deepWidth = 20;
const int threadCount = 8;

Game::Game(const Options& options) :
		_options(options),
		_timeLimit(options.slowMode ? 10 : 0.5),
		_constDepth(6 + options.slowMode),
		_timeTaken()
{
	_turn = PlayerColor::blackPlayer;
	_depth = _constDepth;
	_state = new Board(_turn);
	_state->fillTaboo(_options.limitBlack, _options.doubleThree, _turn);
	_previousState = nullptr;
	_pool = new Pool(threadCount);

	std::random_device rd;
	_randomDevice = std::mt19937(rd());
}

Game::~Game()
{
	delete _pool;
	delete _state;
	delete _previousState;
}

Score Game::negamax(Board& node, int negDepth, Score alpha, Score beta, PlayerColor player)
{
	MoveScore children[BOARD_HEIGHT * BOARD_WIDTH];

	node.fillPriority(player, _options);
	int count = node.getChildren(children, deepWidth);

	if (!count)
		throw std::logic_error("GetChildren returned an empty array");

	size_t i;
	Score bestScore = ninfinity - 100;
	for (i = 0; i < count; i++)
	{
		if (alpha <= beta && !isOverdue())
		{
			//TODO remove need for atomic
			_movesExplored++;
			BoardPos pos = children[i].pos;
			Board *board = new Board(node, pos, player, _options);
			Score score;
			if (board->getVictory().type)
			{
				score = (pinfinity + negDepth) * (board->getVictory().victor * player);
			}
			else if (negDepth <= 1)
			{
				score = player * board->getScore(_options.captureWin);
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

MoveScore Game::negamaxThread(ThreadData data)
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

BoardPos Game::startNegamax(Board *node, PlayerColor player)
{
	std::atomic<Score> alpha(ninfinity);
	MoveScore children[BOARD_HEIGHT * BOARD_WIDTH];

	int count = node->getChildren(children, initialWidth);

	if (!count)
		throw std::logic_error("GetChildren returned an empty array");

	std::vector<ThreadData> threadData(count);

	for (size_t i = 0; i < count; i++)
	{
		threadData[i] =	ThreadData(
				ChildBoard(
						new Board(*node, children[i].pos, player, _options),
						children[i].pos),
				&alpha,
				player);
	}


	std::function<MoveScore(ThreadData)> function = boost::bind(&Game::negamaxThread, this, _1);
	_movesExplored = 0;

#ifdef NONThreadED
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

	std::uniform_int_distribution<int> uni(0, choice.size() - 1);
	return choice[uni(_randomDevice)].pos;
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

	BoardPos pos = startNegamax(_state, _turn);

	_timeTaken = getTimeDiff();

	return pos;
}

Score Game::getCurrentScore() const
{
	return _state->getScore(_options.captureWin);
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

int Game::getMovesExplored() const
{
	return _movesExplored;
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
