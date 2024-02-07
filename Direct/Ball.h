#pragma once
#include <D3DX10.h>
#include "Vertex.h"



class Ball
{
public:
	
	Ball(VERTEX topLeftVertex, VERTEX topRightVertex, VERTEX bottomLeftVertex, VERTEX bottomRightVertex);
	~Ball();
	void Update();
	VERTEX GetTopLeftVertex();
	VERTEX GetTopRightVertex();
	VERTEX GetBottomLeftVertex();
	VERTEX GetBottomRightVertex();

	void SetXSpeed(float xSpeed);
	void SetYSpeed(float ySpeed);

	
	
private:
	float xSpeed, ySpeed;
	VERTEX topLeftVertex;
	VERTEX topRightVertex;
	VERTEX bottomLeftVertex;
	VERTEX bottomRightVertex;
};