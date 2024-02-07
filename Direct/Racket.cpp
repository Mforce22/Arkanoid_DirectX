#include "Racket.h"

Racket::Racket(VERTEX topLeftVertex, VERTEX topRightVertex, VERTEX bottomLeftVertex, VERTEX bottomRightVertex) {
	this->topLeftVertex = topLeftVertex;
	this->topRightVertex = topRightVertex;
	this->bottomLeftVertex = bottomLeftVertex;
	this->bottomRightVertex = bottomRightVertex;
}

Racket::~Racket() {
}

void Racket::Update() {
	//check if there is a collision with the walls
	//float xCorrection = 0.0f;
	//float yCorrection = 0.0f;
	if (GetKeyState('A') & 0x8000)/*Check if high-order bit is set (1 << 15)*/
	{
		//move left
		if (topLeftVertex.X > -1.0f) {
			topLeftVertex.X -= xSpeed;
			topRightVertex.X -= xSpeed;
			bottomLeftVertex.X -= xSpeed;
			bottomRightVertex.X -= xSpeed;
		}
		
	}
	else if (GetKeyState('D') & 0x8000) {
		if (topRightVertex.X < 1.0f) {
			//move right
			topLeftVertex.X += xSpeed;
			topRightVertex.X += xSpeed;
			bottomLeftVertex.X += xSpeed;
			bottomRightVertex.X += xSpeed;
		}
		
	}
}

VERTEX Racket::GetTopLeftVertex() {
	return topLeftVertex;
}

VERTEX Racket::GetTopRightVertex() {
	return topRightVertex;
}

VERTEX Racket::GetBottomLeftVertex() {
	return bottomLeftVertex;
}

VERTEX Racket::GetBottomRightVertex() {
	return bottomRightVertex;
}

void Racket::SetXSpeed(float xSpeed) {
	this->xSpeed = xSpeed;
}



