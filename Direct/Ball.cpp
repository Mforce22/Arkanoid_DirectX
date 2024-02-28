#include "Ball.h"

Ball::Ball(VERTEX topLeftVertex, VERTEX topRightVertex, VERTEX bottomLeftVertex, VERTEX bottomRightVertex) {
	this->topLeftVertex = topLeftVertex;
	this->topRightVertex = topRightVertex;
	this->bottomLeftVertex = bottomLeftVertex;
	this->bottomRightVertex = bottomRightVertex;
}

Ball::~Ball() {
}

//return if the match has ended
bool Ball::Update() {

	//check if there is a collision with the walls
	float xCorrection = 0.0f;

	float yCorrection = 0.0f;


	if (topLeftVertex.X + xSpeed <= -1) {
		xCorrection = topLeftVertex.X + xSpeed + 1;
	}
	else if (topRightVertex.X + xSpeed >= 1) {
		xCorrection = topRightVertex.X + xSpeed - 1;
	}

	if (topLeftVertex.Y + ySpeed >= 1) {
		yCorrection = topLeftVertex.Y + ySpeed - 1;
	}
	else if (bottomLeftVertex.Y + ySpeed <= -1) {
		yCorrection = bottomLeftVertex.Y + ySpeed + 1;
		return true;
	}


	topLeftVertex.X += xSpeed - xCorrection;
	topLeftVertex.Y += ySpeed;
	topRightVertex.X += xSpeed - xCorrection;
	topRightVertex.Y += ySpeed;
	bottomLeftVertex.X += xSpeed - xCorrection;
	bottomLeftVertex.Y += ySpeed;
	bottomRightVertex.X += xSpeed - xCorrection;
	bottomRightVertex.Y += ySpeed;

	if (xCorrection != 0.0f) {
		xSpeed = -xSpeed;
	}
	if (yCorrection != 0.0f) {
		ySpeed = -ySpeed;
	}

	return false;

	/*topLeftVertex = { 0.2, 0.1 , 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) };
	topRightVertex = { 0.3, 0.1, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) };
	bottomLeftVertex = { 0.2, 0.0, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) };
	bottomRightVertex = { 0.3, 0.0f , 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) };*/
}

VERTEX Ball::GetTopLeftVertex() {
	return topLeftVertex;
}

VERTEX Ball::GetTopRightVertex() {
	return topRightVertex;
}

VERTEX Ball::GetBottomLeftVertex() {
	return bottomLeftVertex;
}

VERTEX Ball::GetBottomRightVertex() {
	return bottomRightVertex;
}

void Ball::SetXSpeed(float xSpeed) {
	this->xSpeed = xSpeed;
}

void Ball::SetYSpeed(float ySpeed) {
	this->ySpeed = ySpeed;
}

bool Ball::CheckCollision(VERTEX boxTopLeftVertex, VERTEX boxTopRightVertex, VERTEX boxBottomLeftVertex, VERTEX boxBottomRightVertex) {

	float topBallX = (topLeftVertex.X + topRightVertex.X) / 2;
	float topBallY = topLeftVertex.Y;

	float bottomBallX = (bottomLeftVertex.X + bottomRightVertex.X) / 2;
	float bottomBallY = bottomLeftVertex.Y;

	float leftBallX = topLeftVertex.X;
	float leftBallY = (topLeftVertex.Y + bottomLeftVertex.Y) / 2;

	float rightBallX = topRightVertex.X;
	float rightBallY = (topRightVertex.Y + bottomRightVertex.Y) / 2;


	//check if the collision is up or down
	if (topBallY >= boxBottomLeftVertex.Y && topBallY <= boxTopLeftVertex.Y && topBallX >= boxBottomLeftVertex.X && topBallX <= boxBottomRightVertex.X) {
		ySpeed = -ySpeed;
		return true;
	}
	else if (bottomBallY >= boxBottomLeftVertex.Y && bottomBallY <= boxTopLeftVertex.Y && bottomBallX >= boxBottomLeftVertex.X && bottomBallX <= boxBottomRightVertex.X) {
		ySpeed = -ySpeed;
		return true;
	}

	//check if the collision is left or right
	if (leftBallX >= boxTopLeftVertex.X && leftBallX <= boxTopRightVertex.X && leftBallY >= boxBottomLeftVertex.Y && leftBallY <= boxTopLeftVertex.Y) {
		xSpeed = -xSpeed;
		return true;
	}
	else if (rightBallX >= boxTopLeftVertex.X && rightBallX <= boxTopRightVertex.X && rightBallY >= boxBottomLeftVertex.Y && rightBallY <= boxTopLeftVertex.Y) {
		xSpeed = -xSpeed;
		return true;
	}

	

	return false;

}

void Ball::CheckRacketCollision(VERTEX boxTopLeftVertex, VERTEX boxTopRightVertex, VERTEX boxBottomLeftVertex, VERTEX boxBottomRightVertex) {
	if (topLeftVertex.Y >= boxBottomLeftVertex.Y && topLeftVertex.Y <= boxTopLeftVertex.Y && topLeftVertex.X >= boxTopLeftVertex.X && topLeftVertex.X <= boxTopRightVertex.X && ySpeed > 0) {
		ySpeed = -ySpeed;
		CheckRacketCollisionPoint(boxTopLeftVertex, boxTopRightVertex);
		return;
	}
	else if (topRightVertex.Y >= boxBottomRightVertex.Y && topRightVertex.Y <= boxTopRightVertex.Y && topRightVertex.X >= boxTopLeftVertex.X && topRightVertex.X <= boxTopRightVertex.X && ySpeed > 0) {
		ySpeed = -ySpeed;
		CheckRacketCollisionPoint(boxTopLeftVertex, boxTopRightVertex);
		return;
	}
	else if (bottomLeftVertex.Y <= boxTopLeftVertex.Y && bottomLeftVertex.Y >= boxBottomLeftVertex.Y && bottomLeftVertex.X >= boxTopLeftVertex.X && bottomLeftVertex.X <= boxTopRightVertex.X && ySpeed < 0) {
		ySpeed = -ySpeed;
		CheckRacketCollisionPoint(boxTopLeftVertex, boxTopRightVertex);
		return;
	}
	else if (bottomRightVertex.Y <= boxTopRightVertex.Y && bottomRightVertex.Y >= boxBottomRightVertex.Y && bottomRightVertex.X >= boxTopLeftVertex.X && bottomRightVertex.X <= boxTopRightVertex.X && ySpeed < 0) {
		ySpeed = -ySpeed;
		CheckRacketCollisionPoint(boxTopLeftVertex, boxTopRightVertex);
		return;
	}

	return;

}


void Ball::CheckRacketCollisionPoint(VERTEX boxTopLeftVertex, VERTEX boxTopRightVertex) {

	//divide boxTopLeftVertex and boxTopRightVertex into 3 parts
	float leftPart = boxTopLeftVertex.X + (boxTopRightVertex.X - boxTopLeftVertex.X) / 3;
	float rightPart = boxTopLeftVertex.X + (boxTopRightVertex.X - boxTopLeftVertex.X) * 2 / 3;

	//check if the ball is in the left part
	if (topLeftVertex.X <= leftPart && topRightVertex.X <= leftPart && xSpeed >= 0) {
		xSpeed = -xSpeed;
	}
	//check if the ball is in the right part
	else if (topLeftVertex.X >= rightPart && topRightVertex.X >= rightPart && xSpeed <= 0) {
		xSpeed = -xSpeed;
	}
}