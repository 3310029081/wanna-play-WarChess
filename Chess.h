#pragma once
#include <graphics.h>
#include <vector>
#include <vector>
#include <string>
using namespace std;

typedef enum {
	CHESS_WHITE = -1,  // �׷�
	CHESS_BLACK = 1  ,  // �ڷ�
	CHESS_CHOOSEBLACK = 666,//����������ͼ
	EMPTY = 0//��
} chess_kind_t;

struct ChessPos {
	int row;
	int col;

	ChessPos(int r = -1, int c = -1) :row(r), col(c) {

	}

};

class Chess
{
public:
	Chess(int chessMap[15][15]);
	Chess(int gradeSize, int marginX, int marginY, float chessSize);
	ChessPos lastPos; //�������λ��
	int count;//�ڼ���

	// ���̵ĳ�ʼ�����������̵�ͼƬ��Դ����ʼ�����̵��������
	void init();

	// �ж���ָ������(x,y)λ�ã��Ƿ�����Ч���
	// �������Ч���������Ч�����λ��(�У��У������ڲ���pos��
	bool clickBoard(int x, int y, ChessPos* pos);

	// �����̵�ָ��λ�ã�pos��, ���ӣ�kind��
	void chessDown(ChessPos* pos, int colour);
	//void chessDown(ChessPos* pos, chess_kind_t kind, int colour);
	//void chessDown(ChessPos* pos, chess_kind_t kind, Man* man, AI* ai);
	//void chessDown(ChessPos* pos, chess_kind_t kind);


	// ��ȡ���̵Ĵ�С��13�ߡ�15�ߡ�19�ߣ�
	int getGradeSize();

	// ��ȡָ��λ���Ǻ��壬���ǰ��壬���ǿհ�
	int getChessData(ChessPos* pos);
	int getChessData(int row, int col);

	// ��ʾ���巽, true:Man��  false: AI ��AI����
	bool playerFlag;


	// �ж�����Ƿ����
	bool checkOver();

	//����
	void undoMove();


	//����
	bool checkForbidden(const ChessPos& pos);

	bool isThreeThreeBan(ChessPos newPos);

	bool isFourFourBan(ChessPos newPos);

	bool isLongLongBan(ChessPos newPos);




	string chessMapToString(const vector<vector<int>>& chessMap, const ChessPos& pos, int dr, int dc);

	ChessPos findNextMoveForWhite();

	bool checkPosition(vector<vector<int>>& chessMap, ChessPos& pos);

	ChessPos findNextMoveForBlack();

	bool checkPositionForBlack(vector<vector<int>>& chessMap, ChessPos& pos);

	vector<int> computePrefixFunction(const string& pattern);

	bool kmpSearch(const string& text, const string& pattern);


	bool checkLongConnForbidden(ChessPos pos);






	//��ȡ��ǰ���������ɫ
	chess_kind_t getCurrentPlayer();	
	//�ָ������õİڷ�����
	void chessRecover(ChessPos* pos, chess_kind_t kind);
	//��ͼ 
	void giveImg(int col, int row, int flag);

	// �洢��ǰ��Ϸ���̺����ӵ����,�հ�Ϊ0������1������-1
	vector<vector<int>> chessMap;

private:
	// ���̳ߴ�
	int gradeSize;
	float margin_x;//49;
	int margin_y;// 49;
	float chessSize; //���Ӵ�С�����̷����С��

	IMAGE chessBlackImg ;
	IMAGE chessWhiteImg;
	IMAGE chessKongImg;
	//��ͼ
	IMAGE  chessChooseBlackImg;

	IMAGE chessButton1Img;
	IMAGE chessButton2Img;
	IMAGE chessButton3Img;
	IMAGE chessButton4Img;
	IMAGE chessButton5Img;


	//�洢������ʷ��¼
	vector<ChessPos> moveHistory;

	//void updateGameMap(ChessPos* pos);
	//void updateGameMap(ChessPos* pos, Man man, AI ai);
	void updateGameMap(ChessPos* pos, int colour);
	bool checkWin();//�����Ӯ ���ʤ���ѷ� true
};