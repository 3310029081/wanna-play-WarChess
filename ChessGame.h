#pragma once
#include "Man.h"
#include "AI.h"
#include "Chess.h"

class ChessGame
{
public:
	ChessGame(Man*, AI*, Chess*);
	//³õÊ¼»¯µã»÷
	void handleMouseClick(Chess chess, Man man, AI ai);
	//void handleMouseClick(Chess chess);
	bool isMouseClickWithinImage(int mouseX, int mouseY, int imageX, int imageY, int imageWidth, int imageHeight);
	void play();
	

private:
	Man* man;
	AI* ai;
	Chess* chess;


};



