#include "Chess.h"
#include <graphics.h>
#include<conio.h>
#include "ChessBook.h"
#include <vector>
#include <string>
#include <iostream>
#include "Man.h"
#include "AI.h"

Chess::Chess(int chessMap[15][15])
{
	std::vector<std::vector<int>> chessMapVector(15, std::vector<int>(15));

	for (int i = 0; i < 15; ++i) {
		for (int j = 0; j < 15; ++j) {
			chessMapVector[i][j] = chessMap[i][j];
		}
	}

	this->chessMap = chessMapVector;
}
Chess::Chess(int gradeSize, int marginX, int marginY, float chessSize)
{
	this->gradeSize = gradeSize;
	this->margin_x = marginX;
	this->margin_y = marginY;
	this->chessSize = chessSize;
	playerFlag = CHESS_BLACK;

	for (int i = 0; i < gradeSize; i++) {
		vector<int>row;
		for (int j = 0; j < gradeSize; j++) {
			row.push_back(0);
		}
		chessMap.push_back(row);
	}
}


int Chess::getGradeSize()
{
	return gradeSize;
}

int Chess::getChessData(ChessPos* pos)
{

	return chessMap[pos->row][pos->col];
}

int Chess::getChessData(int row, int col)
{
	return chessMap[row][col];
}

// 实现透明贴图
void putimagePNG(int x, int y, IMAGE* picture) //x为载入图片的X坐标，y为Y坐标
{
	// 变量初始化
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //获取picture的显存指针
	int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
	int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
	int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
	int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
	int dstX = 0;    //在显存里像素的角标

	// 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + x) + (iy + y) * graphWidth; //在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
			}
		}
	}
}

// 换手
void Chess::updateGameMap(ChessPos* pos ,int colour)
{
	lastPos = *pos;

	chessMap[pos->row][pos->col] = colour;
	playerFlag = !playerFlag; // 换手
}

//检查是否胜利

bool Chess::checkWin()
{
	int row = lastPos.row;
	int col = lastPos.col;


	for (int i = 0; i < 5; i++)
	{
		// 往左5个，往右匹配4个子，20种情况
		if (col - i >= 0 &&
			col - i + 4 < gradeSize &&
			chessMap[row][col - i] == chessMap[row][col - i + 1] &&
			chessMap[row][col - i] == chessMap[row][col - i + 2] &&
			chessMap[row][col - i] == chessMap[row][col - i + 3] &&
			chessMap[row][col - i] == chessMap[row][col - i + 4])
			return true;
	}

	// 竖直方向(上下延伸4个)
	for (int i = 0; i < 5; i++)
	{
		if (row - i >= 0 &&
			row - i + 4 < gradeSize &&
			chessMap[row - i][col] == chessMap[row - i + 1][col] &&
			chessMap[row - i][col] == chessMap[row - i + 2][col] &&
			chessMap[row - i][col] == chessMap[row - i + 3][col] &&
			chessMap[row - i][col] == chessMap[row - i + 4][col])
			return true;
	}


	// “/"方向


	for (int i = 0; i < 5; i++)
	{
		if (row + i < gradeSize &&
			row + i - 4 >= 0 &&
			col - i >= 0 &&
			col - i + 4 < gradeSize &&
			// 第[row+i]行，第[col-i]的棋子，与右上方连续4个棋子都相同
			chessMap[row + i][col - i] == chessMap[row + i - 1][col - i + 1] &&
			chessMap[row + i][col - i] == chessMap[row + i - 2][col - i + 2] &&
			chessMap[row + i][col - i] == chessMap[row + i - 3][col - i + 3] &&
			chessMap[row + i][col - i] == chessMap[row + i - 4][col - i + 4])
			return true;
	}

	// “\“ 方向
	for (int i = 0; i < 5; i++)
	{
		// 第[row+i]行，第[col-i]的棋子，与右下方连续4个棋子都相同
		if (row - i >= 0 &&
			row - i + 4 < gradeSize &&
			col - i >= 0 &&
			col - i + 4 < gradeSize &&
			chessMap[row - i][col - i] == chessMap[row - i + 1][col - i + 1] &&
			chessMap[row - i][col - i] == chessMap[row - i + 2][col - i + 2] &&
			chessMap[row - i][col - i] == chessMap[row - i + 3][col - i + 3] &&
			chessMap[row - i][col - i] == chessMap[row - i + 4][col - i + 4])
			return true;
	}

	return false;
}

//棋盘初始化
void Chess::init()
{
	initgraph(740, 712, EW_SHOWCONSOLE);

	loadimage(0, "res/棋盘.png", 740, 712);

	loadimage(&chessBlackImg, "res/black.png", chessSize, chessSize, true);
	loadimage(&chessWhiteImg, "res/white.png", chessSize, chessSize, true);
	loadimage(&chessKongImg, "res/空.png", chessSize, chessSize, true);

	//按钮图片
	loadimage(&chessButton1Img, "res/先手.png", 80, 50, true);
	loadimage(&chessButton2Img, "res/后手.png", 80, 50, true);
	loadimage(&chessButton3Img, "res/悔棋.png", 80, 50, true);
	loadimage(&chessButton4Img, "res/信息.png", 80, 50, true);
	//五手N打的棋子
	loadimage(&chessChooseBlackImg, "res/chooseBlack.png", chessSize, chessSize, true);
	loadimage(&chessButton5Img, "res/棋局恢复.png", 80, 50, true);
	putimagePNG(120, 0, &chessButton1Img);
	putimagePNG(210, 0, &chessButton2Img);
	putimagePNG(300, 0, &chessButton3Img);
	putimagePNG(390, 0, &chessButton4Img);
	putimagePNG(480, 0, &chessButton5Img);

	for (int i = 0; i < chessMap.size(); i++) {
		for (int j = 0; j < chessMap[i].size(); j++) {
			chessMap[i][j] = 0;
		}
	}

}

//是否成功点击
bool Chess::clickBoard(int x, int y, ChessPos* pos)
{
	int col = (x - margin_x) / chessSize;
	int row = (y - margin_y) / chessSize;

	if (!(col >= 0 && col <= 15 && row >= 0 && row <= 15)) {
		return  false;
	}

	int leftTopPosX = margin_x + chessSize * col;
	int leftTopPosY = margin_y + chessSize * row;
	int offset = chessSize * 0.35; // 20 鼠标点击的模糊距离上限

	int len;
	int selectPos = false;

	do {
		len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
		if (len < offset) {
			pos->row = row;
			pos->col = col;
			if (chessMap[pos->row][pos->col] == 0) {
				selectPos = true;
			}
			break;
		}

		// 距离右上角的距离
		len = sqrt((x - leftTopPosX - chessSize) * (x - leftTopPosX - chessSize) + (y - leftTopPosY) * (y - leftTopPosY));
		if (len < offset) {
			pos->row = row;
			pos->col = col + 1;
			if (chessMap[pos->row][pos->col] == 0) {
				selectPos = true;
			}
			break;
		}

		// 距离左下角的距离
		len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - chessSize) * (y - leftTopPosY - chessSize));
		if (len < offset) {
			pos->row = row + 1;
			pos->col = col;
			if (chessMap[pos->row][pos->col] == 0) {
				selectPos = true;
			}
			break;
		}

		// 距离右下角的距离
		len = sqrt((x - leftTopPosX - chessSize) * (x - leftTopPosX - chessSize) + (y - leftTopPosY - chessSize) * (y - leftTopPosY - chessSize));
		if (len < offset) {
			pos->row = row + 1;
			pos->col = col + 1;
			if (chessMap[pos->row][pos->col] == 0) {
				selectPos = true;
			}
			break;
		}
	} while (0);

	return selectPos;
}


//下棋
//下棋
void Chess::chessDown(ChessPos* pos, int colour)
{

	int x = margin_x + pos->col * chessSize - 0.5 * chessSize;
	int y = margin_y + pos->row * chessSize - 0.5 * chessSize;



	while (colour == CHESS_BLACK && checkForbidden(*pos)) {
		MOUSEMSG msg;
		while (true) {
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN) {
				if (clickBoard(msg.x, msg.y, pos)) {
					if (!checkForbidden(*pos)) {
						x = margin_x + pos->col * chessSize - 0.5 * chessSize;
						y = margin_y + pos->row * chessSize - 0.5 * chessSize;
						putimagePNG(x, y, &chessBlackImg);
						cout << "位置合法且不违反禁手规则。" << endl;
						updateGameMap(pos, colour);
						moveHistory.push_back(*pos);
						return;
					}
					else {
						cout << "位置不合法，请确认是否下在该位置 " << endl;
						cout << "1 确认 0 取消" << endl;
						int flag;
						cin >> flag;
						if (flag == 1) {
							x = margin_x + pos->col * chessSize - 0.5 * chessSize;
							y = margin_y + pos->row * chessSize - 0.5 * chessSize;
							putimagePNG(x, y, &chessBlackImg);
							updateGameMap(pos, colour);
							moveHistory.push_back(*pos);
							return;
						}

					}
				}
				else {
					cout << "无效的位置，请重新点击。" << endl;
				}
			}
		}
	}


	if (colour == CHESS_WHITE) {
		putimagePNG(x, y, &chessWhiteImg);
	}
	else {
		putimagePNG(x, y, &chessBlackImg);
	}


	updateGameMap(pos, colour);

	// 将棋子添加到历史记录
	moveHistory.push_back(*pos);
	//将棋谱保存到临时文件中
	ChessBook chessBook;
	chessBook.saveMoveHistory(moveHistory/*,playerFlag*/);
}

//禁手
bool Chess::checkForbidden(const ChessPos& pos)
{// 检查三三禁手
	if (isThreeThreeBan(pos)) {
		cout << "违规三三禁手！请重新下棋。" << endl;
		return true;
	}

	if (isFourFourBan(pos)) {
		cout << "违规四四禁手！请重新下棋。" << endl;
		return true;
	}
	// 检查长连禁手
	if (isLongLongBan(pos)) {
		cout << "违规长禁手！请重新下棋。" << endl;
		return true;
	}


	return false;
}

//33
bool Chess::isThreeThreeBan(ChessPos newPos)
{
	int gradeSize = chessMap.size();

	// 创建棋盘的临时副本
	vector<vector<int>> tempChessMap = chessMap;
	tempChessMap[newPos.row][newPos.col] = CHESS_BLACK;

	int count = 0;
	string text;
	// 检查水平方向是否形成了活三;
	text = chessMapToString(tempChessMap, newPos, 0, 1);
	if (kmpSearch(text, "01110") || kmpSearch(text, "011010") || kmpSearch(text, "010110")) {
		count++;
	}
	//如果满足这个模式的话，说明在这个方向上01110是假的，
	if (kmpSearch(text, "11101") || kmpSearch(text, "10111"))
	{
		count--;
	}
	// 检查竖直方向是否形成了活三
	text = chessMapToString(tempChessMap, newPos, 1, 0);
	if (kmpSearch(text, "01110") || kmpSearch(text, "011010") || kmpSearch(text, "010110")) {
		count++;
	}
	//如果满足这个模式的话，说明在这个方向上01110是假的，
	if (kmpSearch(text, "11101") || kmpSearch(text, "10111"))
	{
		count--;
	}
	// 检查左上到右下对角线方向是否形成了活三
	text = chessMapToString(tempChessMap, newPos, 1, 1);
	if (kmpSearch(text, "01110") || kmpSearch(text, "011010") || kmpSearch(text, "010110")) {
		count++;
	}
	//如果满足这个模式的话，说明在这个方向上01110是假的，
	if (kmpSearch(text, "11101") || kmpSearch(text, "10111"))
	{
		count--;
	}
	// 检查右上到左下对角线方向是否形成了活三
	text = chessMapToString(tempChessMap, newPos, 1, -1);
	if (kmpSearch(text, "01110") || kmpSearch(text, "011010") || kmpSearch(text, "010110")) {
		count++;
	}
	//如果满足这个模式的话，说明在这个方向上01110是假的，
	if (kmpSearch(text, "11101") || kmpSearch(text, "10111"))
	{
		count--;
	}
	return count >= 2;
}

//44
bool Chess::isFourFourBan(ChessPos newPos)
{
	int gradeSize = chessMap.size();


	// 创建棋盘的临时副本
	vector<vector<int>> tempChessMap = chessMap;
	tempChessMap[newPos.row][newPos.col] = CHESS_BLACK;

	int count = 0;
	string text;
	// 检查水平方向是否形成了活四和冲四
	text = chessMapToString(tempChessMap, newPos, 0, 1);
	if (kmpSearch(text, "011110") || kmpSearch(text, "11101") || kmpSearch(text, "10111") || kmpSearch(text, "-11110") || kmpSearch(text, "01111-") || kmpSearch(text, "01111") || kmpSearch(text, "11110")) {
		count++;
	}

	// 检查竖直方向是否形成了活四和冲四
	text = chessMapToString(tempChessMap, newPos, 1, 0);
	if (kmpSearch(text, "011110") || kmpSearch(text, "11101") || kmpSearch(text, "10111") || kmpSearch(text, "-11110") || kmpSearch(text, "01111-") || kmpSearch(text, "01111") || kmpSearch(text, "11110")) {
		count++;
	}

	// 检查左上到右下对角线方向是否形成了活四和冲四
	text = chessMapToString(tempChessMap, newPos, 1, 1);
	if (kmpSearch(text, "011110") || kmpSearch(text, "11101") || kmpSearch(text, "10111") || kmpSearch(text, "-11110") || kmpSearch(text, "01111-") || kmpSearch(text, "01111") || kmpSearch(text, "11110")) {
		count++;
	}


	// 检查右上到左下对角线方向是否形成了活活四和冲四
	text = chessMapToString(tempChessMap, newPos, 1, -1);
	if (kmpSearch(text, "011110") || kmpSearch(text, "11101") || kmpSearch(text, "10111") || kmpSearch(text, "-11110") || kmpSearch(text, "01111-") || kmpSearch(text, "01111") || kmpSearch(text, "11110")) {
		count++;
	}



	return count >= 2;

}


bool Chess::isLongLongBan(ChessPos newPos)
{
	int gradeSize = chessMap.size();


	// 创建棋盘的临时副本
	vector<vector<int>> tempChessMap = chessMap;
	tempChessMap[newPos.row][newPos.col] = CHESS_BLACK;

	string text;
	// 检查水平方向是否形成了活四和冲四
	text = chessMapToString(tempChessMap, newPos, 0, 1);
	if (kmpSearch(text, "111111") || kmpSearch(text, "1111111") || kmpSearch(text, "1111111") || kmpSearch(text, "11111111") || kmpSearch(text, "111111111")) {
		return true;
	}

	// 检查竖直方向是否形成了活四和冲四
	text = chessMapToString(tempChessMap, newPos, 1, 0);
	if (kmpSearch(text, "111111") || kmpSearch(text, "1111111") || kmpSearch(text, "1111111") || kmpSearch(text, "11111111") || kmpSearch(text, "111111111")) {
		return true;
	}

	// 检查左上到右下对角线方向是否形成了活四和冲四
	text = chessMapToString(tempChessMap, newPos, 1, 1);
	if (kmpSearch(text, "111111") || kmpSearch(text, "1111111") || kmpSearch(text, "1111111") || kmpSearch(text, "11111111") || kmpSearch(text, "111111111")) {
		return true;
	}

	// 检查右上到左下对角线方向是否形成了活三
	text = chessMapToString(tempChessMap, newPos, 1, -1);
	if (kmpSearch(text, "111111") || kmpSearch(text, "1111111") || kmpSearch(text, "1111111") || kmpSearch(text, "11111111") || kmpSearch(text, "111111111")) {
		return true;
	}


	return false;

}

// 寻找白棋可以构成两个活三、两个活四、一个活三一个活四、冲四加活三等的位置
ChessPos Chess::findNextMoveForWhite() {
	//int gradeSize = getGradeSize();
	for (int r = 0; r < 15; ++r) {
		for (int c = 0; c < 15; ++c) {
			if (chessMap[r][c] == 0) {
				// 在空白位置下白棋，并检查是否满足条件
				vector<vector<int>> tempMap = chessMap;
				tempMap[r][c] = -1; // 下白棋

				// 将下的白棋转换为字符串，并进行模式匹配检查
				ChessPos pos(r, c);
				if (checkPosition(tempMap, pos)) {
					cout << pos.row << pos.col << "ji" << endl;
					return pos;
				}
			}
		}
	}

	return NULL;
}
bool Chess::checkPosition(vector<vector<int>>& chessMap, ChessPos& pos) {
	int dr[4] = { 1, 1, 0, -1 };
	int dc[4] = { 0, 1, 1, 1 };

	int count = 0;
	for (int i = 0; i < 4; ++i) {
		//是否形成了活四和冲四
		string text = chessMapToString(chessMap, pos, dr[i], dc[i]);
		if (kmpSearch(text, "0----0") || kmpSearch(text, "0----1") || kmpSearch(text, "1----0") || kmpSearch(text, "---0-") || kmpSearch(text, "-0---") || kmpSearch(text, "----0") || kmpSearch(text, "0----")) {
			count++;
		}

		if (kmpSearch(text, "0---0") || kmpSearch(text, "0--0-0") || kmpSearch(text, "0-0--0")) {
			count++;
		}
		//如果满足这个模式的话，说明在这个方向上01110是假的，
		if (kmpSearch(text, "---0-") || kmpSearch(text, "-0---"))
		{
			count--;
		}
	}
	return  count >= 2;
}

//黑棋 寻找34组合
ChessPos Chess::findNextMoveForBlack() {

	int gradeSize = getGradeSize();

	for (int r = 0; r < 15; ++r) {
		for (int c = 0; c < 15; ++c) {
			if (chessMap[r][c] == 0) {
				// 在空白位置下白棋，并检查是否满足条件
				vector<vector<int>> tempMap = chessMap;
				tempMap[r][c] = 1; // 下黑棋

				// 将下的白棋转换为字符串，并进行模式匹配检查
				ChessPos pos(r, c);
				if (checkPositionForBlack(tempMap, pos)) {

					return pos;
				}
			}
		}
	}

	return NULL;
}
bool Chess::checkPositionForBlack(vector<vector<int>>& chessMap, ChessPos& pos) {
	int dr[4] = { 1, 1, 0, -1 };
	int dc[4] = { 0, 1, 1, 1 };


	for (int i = 0; i < 4; ++i) {
		int count = 0;
		int num = 0;
		//一个方向上的冲四
		string text = chessMapToString(chessMap, pos, dr[i], dc[i]);
		// 是否形成了冲四
		if (kmpSearch(text, "011110") || kmpSearch(text, "11101") || kmpSearch(text, "10111") || kmpSearch(text, "-111110") || kmpSearch(text, "01111-1")) {
			num++;
		}
		for (int j = 0; j < 4; j++)
		{
			if (j != i)
			{
				text = chessMapToString(chessMap, pos, dr[j], dc[j]);
				if (kmpSearch(text, "01110") || kmpSearch(text, "011010") || kmpSearch(text, "010110")) {
					count++;
				}
				//如果满足这个模式的话，说明在这个方向上01110是假的，
				if (kmpSearch(text, "11101") || kmpSearch(text, "10111"))
				{
					count--;
				}
			}
		}


		if (count == 1 && num == 1)
		{
			return true;
		}


	}
	return false;

}

// 计算KMP算法中的前缀函数
vector<int> Chess::computePrefixFunction(const string& pattern) {
	int m = pattern.length();
	vector<int> prefixFunction(m, 0);
	int k = 0;

	for (int q = 1; q < m; ++q) {
		while (k > 0 && pattern[k] != pattern[q]) {
			k = prefixFunction[k - 1];
		}
		if (pattern[k] == pattern[q]) {
			++k;
		}
		prefixFunction[q] = k;
	}

	return prefixFunction;
}


// 实现KMP算法进行模式匹配
bool  Chess::kmpSearch(const string& text, const string& pattern) {
	int n = text.length();
	int m = pattern.length();

	if (m == 0) {
		return true; // 空模式在任何文本中都存在
	}

	vector<int> prefixFunction = computePrefixFunction(pattern);
	int q = 0;

	for (int i = 0; i < n; ++i) {
		while (q > 0 && pattern[q] != text[i]) {
			q = prefixFunction[q - 1];
		}
		if (pattern[q] == text[i]) {
			++q;
		}
		if (q == m) {
			return true; // 在文本中找到了模式
		}
	}
	return false; // 在文本中未找到模式
}


//将棋盘转化为字符串
string Chess::chessMapToString(const vector<vector<int>>& chessMap, const ChessPos& pos, int dr, int dc) {
	string chessboardString;
	int gradeSize = getGradeSize();

	//暂时写死(棋局的一半)
	for (int i = -7; i <= 7; ++i) {
		int r = pos.row + i * dr;
		int c = pos.col + i * dc;

		if (r < 0 || r >= gradeSize || c < 0 || c >= gradeSize)
			continue; // 超出棋盘边界

		int cell = chessMap[r][c];
		if (cell == 1)
			chessboardString.push_back('1');
		else if (cell == -1) {
			chessboardString.push_back('-');
		}
		else
			chessboardString.push_back('0');
	}

	return chessboardString;
}




//判断输赢
bool Chess::checkOver()
{
	//checkWin()

	if (checkWin())
	{
		//Sleep(1500);
		if (playerFlag == false) {

			//刚才走棋的是Man 棋手胜利
			std::cout  << "---胜利--" << "\n";
			//loadimage(0, "res/胜利.png", 740, 712);
		}
		else {
			std::cout << "---失败--" << "\n";
			//loadimage(0, "res/失败.png", 740, 712);
		}

		  
		ChessBook chessBook;
		chessBook.exportRecord(playerFlag, moveHistory);

		//设置是否重新开始


		//return true;

	}

	return false;
}


//悔棋
void Chess::undoMove() {
	cout << "悔棋调用ing" << endl;
	if (!moveHistory.empty()) {
		// 获取最后两步的落子位置
		ChessPos secondLast = moveHistory.back();
		moveHistory.pop_back();
		ChessPos last = moveHistory.back();
		moveHistory.pop_back();

		// 清除棋盘上最后两步的棋子，并从走法记录中移除
		chessMap[secondLast.row][secondLast.col] = 0;
		chessMap[last.row][last.col] = 0;

		// 重新绘制棋盘，以移除棋子
		int x1 = margin_x + last.col * chessSize - 0.5 * chessSize;
		int y1 = margin_y + last.row * chessSize - 0.5 * chessSize;
		int x2 = margin_x + secondLast.col * chessSize - 0.5 * chessSize;
		int y2 = margin_y + secondLast.row * chessSize - 0.5 * chessSize;

		putimagePNG(x1, y1, &chessKongImg);
		putimagePNG(x2, y2, &chessKongImg);
		count -= 2;
	}
}




//获取当前对象的颜色
chess_kind_t Chess::getCurrentPlayer()
{
	if (playerFlag)
		return CHESS_BLACK;
	else
		return CHESS_WHITE;
}

//恢复棋盘用的摆放棋子
void Chess::chessRecover(ChessPos* pos, chess_kind_t kind) {
	int x = margin_x + pos->col * chessSize - 0.5 * chessSize;
	//std::cout << "x = " << x << std::endl;
	int y = margin_y + pos->row * chessSize - 0.5 * chessSize;
	//std::cout << "y = " << y << std::endl;
	//std::cout << "row = " << pos->row << std::endl;
	//std::cout << "col = " << pos->col << std::endl;
	moveHistory.push_back(*pos);//将棋子放进历史记录
	if (kind == CHESS_WHITE) {
		putimagePNG(x, y, &chessWhiteImg);
		chessMap[pos->row][pos->col] = -1;
	}
	else {
		putimagePNG(x, y, &chessBlackImg);
		chessMap[pos->row][pos->col] = 1;
	}
	
}





void Chess::giveImg(int col, int row, int flag) {

	int y = 76 + col * 39 - 0.5 * chessSize;
	int x = 96 + row * 39 - 0.5 * chessSize;
	if (flag == 1) {
		//黑
		putimagePNG(x, y, &chessBlackImg);
	}
	else if (flag == -1) {
		putimagePNG(x, y, &chessWhiteImg);
	}
	else if (flag == 666) {
		putimagePNG(x, y, &chessChooseBlackImg);
	}
	else if (flag == 888) {
		putimagePNG(x, y, &chessKongImg);
	}




}
