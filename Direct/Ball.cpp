#include "Ball.h"

Ball::Ball(VERTEX topLeftVertex, VERTEX topRightVertex, VERTEX bottomLeftVertex, VERTEX bottomRightVertex) {
	this->topLeftVertex = topLeftVertex;
	this->topRightVertex = topRightVertex;
	this->bottomLeftVertex = bottomLeftVertex;
	this->bottomRightVertex = bottomRightVertex;
}

Ball::~Ball() {
}

void Ball::Update() {

	//check if there is a collision with the walls
	float xCorrection = 0.0f;


	if (topLeftVertex.X + xSpeed <= -1) {
		xCorrection = topLeftVertex.X + xSpeed + 1;
	}
	else if (topRightVertex.X + xSpeed >= 1) {
		xCorrection = topRightVertex.X + xSpeed - 1;
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
