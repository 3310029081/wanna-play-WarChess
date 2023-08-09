#pragma once
#include "Chess.h"
class AI
{
public:
	int colour;
	void init(Chess* chess);
	void go(Chess* m_chess, ChessPos* pos);
	void go46(Chess* m_chess, ChessPos* pos);
	

public:
	Chess* chess;
	vector<vector<int>> scoreMap;



	//���ֽӿ�
	void calculateScore(Chess* m_chess);
	//˼����� 
	ChessPos think(Chess* m_chess);
	int think2(Chess* m_chess);



	

	ChessPos thinkFIVE(Chess* m_chess, ChessPos* chessArr);

	ChessPos thinkFIVELengChange(Chess* m_chess, ChessPos* chessArr, int FiveHandNum);

	




};