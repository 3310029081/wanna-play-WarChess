#pragma once
#include "Chess.h"

class Man
{
public:
	int colour;
	void init(Chess* chess);
	void go(Chess* m_chess, ChessPos* pos);

	bool isMouseClickWithinImage(int mouseX, int mouseY, int imageX, int imageY, int imageWidth, int imageHeight);

private:
	Chess* chess;
};


