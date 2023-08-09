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

	// ��moveHistoryʵʱ�洢���ļ���,�������һ��������巽
	void saveMoveHistory(const std::vector<ChessPos>& moveHistory/*, bool playerFlag*//*, const std::string& filename*/);

	//���chessdata������
	void cleanChessData();

	// ���ļ��ж�ȡmoveHistory
	std::vector<ChessPos> loadMoveHistory(/*bool& playerFlag*//*const std::string& filename*/);

	//�õ�movehistory�ĳ���
	int getHistoryChessCount(const std::vector<ChessPos>& HistoryChess);

	//��historyMove�е����ݰڷ�����
	bool setChessByMoveHistory(Chess* chess/*std::vector<ChessPos> moveHistory*/);

	// ��һ��ʼѡ���Ⱥ��ִ洢���ļ���
	void saveChessChoose(bool playerFlag);

	// ���ļ��ж�ȡ�Ⱥ���
	bool loadChessChoose();

	//��5��n�������д���ļ�
	void saveFiveHandChess(ChessPos chessArr[], int size);

	//���ļ��ж�ȡ5��n�����Ϣ
	std::vector<ChessPos> loadFiveHandChess();

	//���5��n���ļ��е�����
	void clearFiveHandChess();

	//�ж��ļ��Ƿ�Ϊ��
	bool isFileEmpty(const std::string& filename);

	//��FiveHandNum������д���ļ�
	void saveFiveHandNum(int num);

	//��ȡFiveHandNum������
	int loadFiveHandNum();
};

