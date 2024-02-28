#pragma once
#include <D3DX10.h>
#include "Vertex.h"



class Ball
{
public:
	
	Ball(VERTEX topLeftVertex, VERTEX topRightVertex, VERTEX bottomLeftVertex, VERTEX bottomRightVertex);
	~Ball();
	bool Update();
	VERTEX GetTopLeftVertex();
	VERTEX GetTopRightVertex();
	VERTEX GetBottomLeftVertex();
	VERTEX GetBottomRightVertex();

	void SetXSpeed(float xSpeed);
	void SetYSpeed(float ySpeed);

	bool CheckCollision(VERTEX boxTopLeftVertex, VERTEX boxTopRightVertex, VERTEX boxBottomLeftVertex, VERTEX boxBottomRightVertex);
	void CheckRacketCollision(VERTEX boxTopLeftVertex, VERTEX boxTopRightVertex, VERTEX boxBottomLeftVertex, VERTEX boxBottomRightVertex);
	void CheckRacketCollisionPoint(VERTEX boxTopLeftVertex, VERTEX boxTopRightVertex);
	
	
private:
	float xSpeed, ySpeed;
	VERTEX topLeftVertex;
	VERTEX topRightVertex;
	VERTEX bottomLeftVertex;
	VERTEX bottomRightVertex;
};