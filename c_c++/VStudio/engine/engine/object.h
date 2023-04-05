#pragma once

typedef struct
{
	float x;
	float y;
} Position;

typedef struct
{
	uint16_t x;
	uint16_t y;
} Size;

class object {
public:
	Position position;
	Position lastSafePos;
	Size spriteSize;
	float speed;
};

class trig {
public:
	Position position;
	size 
};