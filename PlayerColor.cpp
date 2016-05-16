//
// Created by Louis GARCZYNSKI on 3/31/16.
//

#include "PlayerColor.hpp"

PlayerColor operator-(PlayerColor& rhs)
{
	return (static_cast<PlayerColor >(-static_cast<int>(rhs)));
}
