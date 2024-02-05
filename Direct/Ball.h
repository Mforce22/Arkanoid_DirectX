#pragma once
#include <D3DX10.h>

extern struct VERTEX { FLOAT X, Y, Z; D3DXCOLOR Color;
VERTEX() = default;
};

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