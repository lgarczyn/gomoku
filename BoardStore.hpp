//
// Created by Louis GARCZYNSKI on 4/5/16.
//

/*
#pragma once

#include <unordered_map>
#include "Board.hpp"
#include "Constants.hpp"

class BoardStore
{
	static const int bucket_size = 729;
	static const int bucket_bits = 6;
	void* _universe = nullptr;

	Board**	get_universe_position(Board *state)
	{
		void** 				node = &_universe;
		BoardData* 			data = state->getData();

		int counter = 1;
		int index;

		for (int y = 0; y < BOARD_HEIGHT; y++)
		{
			for (int x = 0; x < BOARD_WIDTH; x++)
			{
				index = 3 * index + (int)data[y][x];
				counter++;

				if (counter == bucket_bits)
				{
					if (*node == nullptr)
					{
						*node = new void *[bucket_size]();
					}
					node = &(static_cast<void **>(*node)[index]);
					counter = 0;
				}
				else
				{
					counter++;
				}
			}
		}
		if (*node == nullptr)
		{
			*node = new void *[3]();
		}
		node = &(static_cast<void **>(*node)[index]);

		return (reinterpret_cast<Board**>(node));
	}
};*/