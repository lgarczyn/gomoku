//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#include "Game.hpp"
#include "Analyzer.hpp"

using namespace std;

const bool slowMode = false;
const double time_linit = (slowMode ? 3 : 0.5);
const double time_margin = 0.001;
static const int threadCount = 8;

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
	_depth = 5 + (slowMode ? 1 : 0);
	_state->fillTaboo(_options.limitBlack, _options.doubleThree, _turn);
	_previousState = nullptr;
	_pool = new Pool(threadCount);
}

Game::~Game()
{
	delete _pool;
}

Score Game::negamax(Board* node, int negDepth, Score alpha, Score beta, PlayerColor player)
{
	auto children = node->getChildren(player, _options.capture, 20);

	if (!children.size())
		throw std::logic_error("GetChildren returned an empty array");

	Score bestScore = ninfinity;
	//std::vector<Score> scores = std::vector<Score>(children.size());

	for (int i = 0; i < children.size() && !isOverdue(); i++)
	{
		ChildBoard child = children[i];
		Score score;
		Board* board = child.board;
		BoardPos pos = child.move;

		if (board->isTerminal(pos, _options.captureWin))
		{
			score = pinfinity - _depth + negDepth;
		}
		else if (negDepth <= 1)
		{
			score = player * _analyzer->getScore(*board, _options.captureWin);
		}
		else
		{
			score = -negamax(child.board, negDepth - 1, -beta, -alpha, -player);
		}
		if (score > bestScore)
		{
			bestScore = score;
		}
		delete board;

		alpha = std::max(alpha, score);
		if (alpha > beta)
			break;
	}
	return bestScore;
}

MoveScore Game::negamax_thread(ThreadData data)
{
	if (isOverdue())
	{
		return ninfinity;
	}

	Score score;

	Board* board = data.node.board;
	BoardPos pos = data.node.move;


	std::atomic<int>* alpha = data.alpha;

	if (board->isTerminal(pos, _options.captureWin))
	{
		score = pinfinity;
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
	std::vector<ChildBoard> children = node->getChildren(player, _options.capture, 20);

	if (!children.size())
		throw std::logic_error("GetChildren returned an empty array");

	std::vector<ThreadData> threadData(children.size());

	for (int i = 0; i < children.size(); i++)
	{
		threadData[i] =	ThreadData(
					children[i],
					&alpha,
					player);
	}


	std::function<MoveScore(ThreadData)> function = boost::bind(&Game::negamax_thread, this, _1);
	std::vector<MoveScore> result = _pool->run(function, threadData);

	MoveScore bestMove(ninfinity - 1);
	std::vector<MoveScore>	choice;
	for (int i = 0; i < result.size(); i++)
	{
		Score score = result[i].score;
		BoardPos pos = result[i].pos;

		if (score > bestMove.score)
		{
			choice.clear();
			bestMove = MoveScore(score, pos);
			choice.push_back(bestMove);
		}
		else if (score == bestMove.score)
		{
			bestMove = MoveScore(score, pos);
			choice.push_back(bestMove);
		}
	}
	return choice[rand() % choice.size()].pos;//TODO get fucking rid of rand
}


bool Game::isOverdue()
{
	using namespace std;
	clock_t _current = clock();

	return double(_current - _start) > (time_linit - time_margin) * CLOCKS_PER_SEC;
}

BoardPos Game::getNextMove()
{
	using namespace std;
	_start = clock();

	BoardPos pos = start_negamax(_state, _turn);

	clock_t end = clock();
	_timeTaken = double(end - _start) / CLOCKS_PER_SEC;

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

	return _state->isTerminal(_options.captureWin);
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