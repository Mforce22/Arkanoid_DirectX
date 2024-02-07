#pragma once
#include "Vertex.h"
#include <Windows.h>

class Racket
{
public:
		Racket(VERTEX topLeftVertex, VERTEX topRightVertex, VERTEX bottomLeftVertex, VERTEX bottomRightVertex);
		~Racket();
		void Update();
		VERTEX GetTopLeftVertex();
		VERTEX GetTopRightVertex();
		VERTEX GetBottomLeftVertex();
		VERTEX GetBottomRightVertex();
		void SetXSpeed(float xSpeed);
private:
		float xSpeed;
		VERTEX topLeftVertex;
		VERTEX topRightVertex;
		VERTEX bottomLeftVertex;
		VERTEX bottomRightVertex;
};
