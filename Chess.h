#pragma once
#include <graphics.h>
#include <vector>
#include <vector>
#include <string>
using namespace std;

typedef enum {
	CHESS_WHITE = -1,  // 白方
	CHESS_BLACK = 1  ,  // 黑方
	CHESS_CHOOSEBLACK = 666,//五手落子贴图
	EMPTY = 0//空
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
	ChessPos lastPos; //最近落子位置
	int count;//第几手

	// 棋盘的初始化：加载棋盘的图片资源，初始化棋盘的相关数据
	void init();

	// 判断在指定坐标(x,y)位置，是否是有效点击
	// 如果是有效点击，把有效点击的位置(行，列）保存在参数pos中
	bool clickBoard(int x, int y, ChessPos* pos);

	// 在棋盘的指定位置（pos）, 落子（kind）
	void chessDown(ChessPos* pos, int colour);
	//void chessDown(ChessPos* pos, chess_kind_t kind, int colour);
	//void chessDown(ChessPos* pos, chess_kind_t kind, Man* man, AI* ai);
	//void chessDown(ChessPos* pos, chess_kind_t kind);


	// 获取棋盘的大小（13线、15线、19线）
	int getGradeSize();

	// 获取指定位置是黑棋，还是白棋，还是空白
	int getChessData(ChessPos* pos);
	int getChessData(int row, int col);

	// 标示下棋方, true:Man方  false: AI （AI方）
	bool playerFlag;


	// 判断棋局是否结束
	bool checkOver();

	//悔棋
	void undoMove();


	//禁手
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






	//获取当前对象的棋颜色
	chess_kind_t getCurrentPlayer();	
	//恢复棋盘用的摆放棋子
	void chessRecover(ChessPos* pos, chess_kind_t kind);
	//贴图 
	void giveImg(int col, int row, int flag);

	// 存储当前游戏棋盘和棋子的情况,空白为0，黑子1，白子-1
	vector<vector<int>> chessMap;

private:
	// 棋盘尺寸
	int gradeSize;
	float margin_x;//49;
	int margin_y;// 49;
	float chessSize; //棋子大小（棋盘方格大小）

	IMAGE chessBlackImg ;
	IMAGE chessWhiteImg;
	IMAGE chessKongImg;
	//贴图
	IMAGE  chessChooseBlackImg;

	IMAGE chessButton1Img;
	IMAGE chessButton2Img;
	IMAGE chessButton3Img;
	IMAGE chessButton4Img;
	IMAGE chessButton5Img;


	//存储落子历史记录
	vector<ChessPos> moveHistory;

	//void updateGameMap(ChessPos* pos);
	//void updateGameMap(ChessPos* pos, Man man, AI ai);
	void updateGameMap(ChessPos* pos, int colour);
	bool checkWin();//检查输赢 如果胜负已分 true
};