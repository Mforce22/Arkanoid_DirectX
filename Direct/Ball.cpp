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

	//check the y collision
	if (topLeftVertex.Y >= boxBottomLeftVertex.Y && topLeftVertex.Y <= boxTopLeftVertex.Y && topLeftVertex.X >= boxTopLeftVertex.X && topLeftVertex.X <= boxTopRightVertex.X && ySpeed > 0) {
		ySpeed = -ySpeed;
		return true;
	}
	else if (topRightVertex.Y >= boxBottomRightVertex.Y && topRightVertex.Y <= boxTopRightVertex.Y && topRightVertex.X >= boxTopLeftVertex.X && topRightVertex.X <= boxTopRightVertex.X && ySpeed > 0) {
		ySpeed = -ySpeed;
		return true;
	}
	else if (bottomLeftVertex.Y <= boxTopLeftVertex.Y && bottomLeftVertex.Y >= boxBottomLeftVertex.Y && bottomLeftVertex.X >= boxTopLeftVertex.X && bottomLeftVertex.X <= boxTopRightVertex.X && ySpeed < 0) {
		ySpeed = -ySpeed;
		return true;
	}
	else if (bottomRightVertex.Y <= boxTopRightVertex.Y && bottomRightVertex.Y >= boxBottomRightVertex.Y && bottomRightVertex.X >= boxTopLeftVertex.X && bottomRightVertex.X <= boxTopRightVertex.X && ySpeed < 0) {
		ySpeed = -ySpeed;
		return true;
	}

	//check the x collision
	if (topLeftVertex.X <= boxTopRightVertex.X && topLeftVertex.X >= boxTopLeftVertex.X && topLeftVertex.Y >= boxBottomLeftVertex.Y && topLeftVertex.Y <= boxTopLeftVertex.Y && xSpeed < 0) {
		xSpeed = -xSpeed;
		return true;
	}
	else if (topRightVertex.X >= boxTopLeftVertex.X && topRightVertex.X <= boxTopRightVertex.X && topRightVertex.Y >= boxBottomRightVertex.Y && topRightVertex.Y <= boxTopRightVertex.Y && xSpeed > 0) {
		xSpeed = -xSpeed;
		return true;
	}
	else if (bottomLeftVertex.X <= boxBottomRightVertex.X && bottomLeftVertex.X >= boxBottomLeftVertex.X && bottomLeftVertex.Y >= boxBottomLeftVertex.Y && bottomLeftVertex.Y <= boxTopLeftVertex.Y && xSpeed < 0) {
		xSpeed = -xSpeed;
		return true;
	}
	else if (bottomRightVertex.X >= boxBottomLeftVertex.X && bottomRightVertex.X <= boxBottomRightVertex.X && bottomRightVertex.Y >= boxBottomRightVertex.Y && bottomRightVertex.Y <= boxTopRightVertex.Y && xSpeed > 0) {
		xSpeed = -xSpeed;
		return true;
	}

	return false;

}
