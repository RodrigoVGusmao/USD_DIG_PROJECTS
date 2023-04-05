#pragma once

#include<cinttypes>

namespace key
{
	int8_t upKey,
		downKey,
		rightKey,
		leftKey;

	int8_t movementState;

	void initializeKey(void)
	{
		upKey = 'W';
		downKey = 'S';
		rightKey = 'D';
		leftKey = 'A';
	}
}