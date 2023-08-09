#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include "Chess.h"

class ChessBook
{
public:
	ChessBook();

	std::string getCurrentTimeAsString();

	char convertNumberToChar(int number);

	std::string formatAdjustment(vector<ChessPos> moveHistory);

	void exportRecord(/*const std::string& filename,*/ bool playerFlag, vector<ChessPos> moveHistory);

	// 将moveHistory实时存储到文件中,还有最后一步棋的下棋方
	void saveMoveHistory(const std::vector<ChessPos>& moveHistory/*, bool playerFlag*//*, const std::string& filename*/);

	//清除chessdata的数据
	void cleanChessData();

	// 从文件中读取moveHistory
	std::vector<ChessPos> loadMoveHistory(/*bool& playerFlag*//*const std::string& filename*/);

	//得到movehistory的长度
	int getHistoryChessCount(const std::vector<ChessPos>& HistoryChess);

	//将historyMove中的数据摆放棋子
	bool setChessByMoveHistory(Chess* chess/*std::vector<ChessPos> moveHistory*/);

	// 将一开始选择先后手存储到文件中
	void saveChessChoose(bool playerFlag);

	// 从文件中读取先后手
	bool loadChessChoose();

	//将5手n打的数据写入文件
	void saveFiveHandChess(ChessPos chessArr[], int size);

	//从文件中读取5手n打的信息
	std::vector<ChessPos> loadFiveHandChess();

	//清除5手n打文件中的数据
	void clearFiveHandChess();

	//判断文件是否为空
	bool isFileEmpty(const std::string& filename);

	//将FiveHandNum的数据写入文件
	void saveFiveHandNum(int num);

	//读取FiveHandNum的数据
	int loadFiveHandNum();
};

