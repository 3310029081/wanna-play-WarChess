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

// ʵ��͸����ͼ
void putimagePNG(int x, int y, IMAGE* picture) //xΪ����ͼƬ��X���꣬yΪY����
{
	// ������ʼ��
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()���������ڻ�ȡ��ͼ�豸���Դ�ָ�룬EASYX�Դ�
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //��ȡpicture���Դ�ָ��
	int picture_width = picture->getwidth(); //��ȡpicture�Ŀ�ȣ�EASYX�Դ�
	int picture_height = picture->getheight(); //��ȡpicture�ĸ߶ȣ�EASYX�Դ�
	int graphWidth = getwidth();       //��ȡ��ͼ���Ŀ�ȣ�EASYX�Դ�
	int graphHeight = getheight();     //��ȡ��ͼ���ĸ߶ȣ�EASYX�Դ�
	int dstX = 0;    //���Դ������صĽǱ�

	// ʵ��͸����ͼ ��ʽ�� Cp=��p*FP+(1-��p)*BP �� ��Ҷ˹���������е���ɫ�ĸ��ʼ���
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //���Դ������صĽǱ�
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA��͸����
			int sr = ((src[srcX] & 0xff0000) >> 16); //��ȡRGB���R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + x) + (iy + y) * graphWidth; //���Դ������صĽǱ�
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //��ʽ�� Cp=��p*FP+(1-��p)*BP  �� ��p=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //��p=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //��p=sa/255 , FP=sb , BP=db
			}
		}
	}
}

// ����
void Chess::updateGameMap(ChessPos* pos ,int colour)
{
	lastPos = *pos;

	chessMap[pos->row][pos->col] = colour;
	playerFlag = !playerFlag; // ����
}

//����Ƿ�ʤ��

bool Chess::checkWin()
{
	int row = lastPos.row;
	int col = lastPos.col;


	for (int i = 0; i < 5; i++)
	{
		// ����5��������ƥ��4���ӣ�20�����
		if (col - i >= 0 &&
			col - i + 4 < gradeSize &&
			chessMap[row][col - i] == chessMap[row][col - i + 1] &&
			chessMap[row][col - i] == chessMap[row][col - i + 2] &&
			chessMap[row][col - i] == chessMap[row][col - i + 3] &&
			chessMap[row][col - i] == chessMap[row][col - i + 4])
			return true;
	}

	// ��ֱ����(��������4��)
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


	// ��/"����


	for (int i = 0; i < 5; i++)
	{
		if (row + i < gradeSize &&
			row + i - 4 >= 0 &&
			col - i >= 0 &&
			col - i + 4 < gradeSize &&
			// ��[row+i]�У���[col-i]�����ӣ������Ϸ�����4�����Ӷ���ͬ
			chessMap[row + i][col - i] == chessMap[row + i - 1][col - i + 1] &&
			chessMap[row + i][col - i] == chessMap[row + i - 2][col - i + 2] &&
			chessMap[row + i][col - i] == chessMap[row + i - 3][col - i + 3] &&
			chessMap[row + i][col - i] == chessMap[row + i - 4][col - i + 4])
			return true;
	}

	// ��\�� ����
	for (int i = 0; i < 5; i++)
	{
		// ��[row+i]�У���[col-i]�����ӣ������·�����4�����Ӷ���ͬ
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

//���̳�ʼ��
void Chess::init()
{
	initgraph(740, 712, EW_SHOWCONSOLE);

	loadimage(0, "res/����.png", 740, 712);

	loadimage(&chessBlackImg, "res/black.png", chessSize, chessSize, true);
	loadimage(&chessWhiteImg, "res/white.png", chessSize, chessSize, true);
	loadimage(&chessKongImg, "res/��.png", chessSize, chessSize, true);

	//��ťͼƬ
	loadimage(&chessButton1Img, "res/����.png", 80, 50, true);
	loadimage(&chessButton2Img, "res/����.png", 80, 50, true);
	loadimage(&chessButton3Img, "res/����.png", 80, 50, true);
	loadimage(&chessButton4Img, "res/��Ϣ.png", 80, 50, true);
	//����N�������
	loadimage(&chessChooseBlackImg, "res/chooseBlack.png", chessSize, chessSize, true);
	loadimage(&chessButton5Img, "res/��ָֻ�.png", 80, 50, true);
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

//�Ƿ�ɹ����
bool Chess::clickBoard(int x, int y, ChessPos* pos)
{
	int col = (x - margin_x) / chessSize;
	int row = (y - margin_y) / chessSize;

	if (!(col >= 0 && col <= 15 && row >= 0 && row <= 15)) {
		return  false;
	}

	int leftTopPosX = margin_x + chessSize * col;
	int leftTopPosY = margin_y + chessSize * row;
	int offset = chessSize * 0.35; // 20 �������ģ����������

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

		// �������Ͻǵľ���
		len = sqrt((x - leftTopPosX - chessSize) * (x - leftTopPosX - chessSize) + (y - leftTopPosY) * (y - leftTopPosY));
		if (len < offset) {
			pos->row = row;
			pos->col = col + 1;
			if (chessMap[pos->row][pos->col] == 0) {
				selectPos = true;
			}
			break;
		}

		// �������½ǵľ���
		len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - chessSize) * (y - leftTopPosY - chessSize));
		if (len < offset) {
			pos->row = row + 1;
			pos->col = col;
			if (chessMap[pos->row][pos->col] == 0) {
				selectPos = true;
			}
			break;
		}

		// �������½ǵľ���
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


//����
//����
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
						cout << "λ�úϷ��Ҳ�Υ�����ֹ���" << endl;
						updateGameMap(pos, colour);
						moveHistory.push_back(*pos);
						return;
					}
					else {
						cout << "λ�ò��Ϸ�����ȷ���Ƿ����ڸ�λ�� " << endl;
						cout << "1 ȷ�� 0 ȡ��" << endl;
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
					cout << "��Ч��λ�ã������µ����" << endl;
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

	// ��������ӵ���ʷ��¼
	moveHistory.push_back(*pos);
	//�����ױ��浽��ʱ�ļ���
	ChessBook chessBook;
	chessBook.saveMoveHistory(moveHistory/*,playerFlag*/);
}

//����
bool Chess::checkForbidden(const ChessPos& pos)
{// �����������
	if (isThreeThreeBan(pos)) {
		cout << "Υ���������֣����������塣" << endl;
		return true;
	}

	if (isFourFourBan(pos)) {
		cout << "Υ�����Ľ��֣����������塣" << endl;
		return true;
	}
	// ��鳤������
	if (isLongLongBan(pos)) {
		cout << "Υ�泤���֣����������塣" << endl;
		return true;
	}


	return false;
}

//33
bool Chess::isThreeThreeBan(ChessPos newPos)
{
	int gradeSize = chessMap.size();

	// �������̵���ʱ����
	vector<vector<int>> tempChessMap = chessMap;
	tempChessMap[newPos.row][newPos.col] = CHESS_BLACK;

	int count = 0;
	string text;
	// ���ˮƽ�����Ƿ��γ��˻���;
	text = chessMapToString(tempChessMap, newPos, 0, 1);
	if (kmpSearch(text, "01110") || kmpSearch(text, "011010") || kmpSearch(text, "010110")) {
		count++;
	}
	//����������ģʽ�Ļ���˵�������������01110�Ǽٵģ�
	if (kmpSearch(text, "11101") || kmpSearch(text, "10111"))
	{
		count--;
	}
	// �����ֱ�����Ƿ��γ��˻���
	text = chessMapToString(tempChessMap, newPos, 1, 0);
	if (kmpSearch(text, "01110") || kmpSearch(text, "011010") || kmpSearch(text, "010110")) {
		count++;
	}
	//����������ģʽ�Ļ���˵�������������01110�Ǽٵģ�
	if (kmpSearch(text, "11101") || kmpSearch(text, "10111"))
	{
		count--;
	}
	// ������ϵ����¶Խ��߷����Ƿ��γ��˻���
	text = chessMapToString(tempChessMap, newPos, 1, 1);
	if (kmpSearch(text, "01110") || kmpSearch(text, "011010") || kmpSearch(text, "010110")) {
		count++;
	}
	//����������ģʽ�Ļ���˵�������������01110�Ǽٵģ�
	if (kmpSearch(text, "11101") || kmpSearch(text, "10111"))
	{
		count--;
	}
	// ������ϵ����¶Խ��߷����Ƿ��γ��˻���
	text = chessMapToString(tempChessMap, newPos, 1, -1);
	if (kmpSearch(text, "01110") || kmpSearch(text, "011010") || kmpSearch(text, "010110")) {
		count++;
	}
	//����������ģʽ�Ļ���˵�������������01110�Ǽٵģ�
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


	// �������̵���ʱ����
	vector<vector<int>> tempChessMap = chessMap;
	tempChessMap[newPos.row][newPos.col] = CHESS_BLACK;

	int count = 0;
	string text;
	// ���ˮƽ�����Ƿ��γ��˻��ĺͳ���
	text = chessMapToString(tempChessMap, newPos, 0, 1);
	if (kmpSearch(text, "011110") || kmpSearch(text, "11101") || kmpSearch(text, "10111") || kmpSearch(text, "-11110") || kmpSearch(text, "01111-") || kmpSearch(text, "01111") || kmpSearch(text, "11110")) {
		count++;
	}

	// �����ֱ�����Ƿ��γ��˻��ĺͳ���
	text = chessMapToString(tempChessMap, newPos, 1, 0);
	if (kmpSearch(text, "011110") || kmpSearch(text, "11101") || kmpSearch(text, "10111") || kmpSearch(text, "-11110") || kmpSearch(text, "01111-") || kmpSearch(text, "01111") || kmpSearch(text, "11110")) {
		count++;
	}

	// ������ϵ����¶Խ��߷����Ƿ��γ��˻��ĺͳ���
	text = chessMapToString(tempChessMap, newPos, 1, 1);
	if (kmpSearch(text, "011110") || kmpSearch(text, "11101") || kmpSearch(text, "10111") || kmpSearch(text, "-11110") || kmpSearch(text, "01111-") || kmpSearch(text, "01111") || kmpSearch(text, "11110")) {
		count++;
	}


	// ������ϵ����¶Խ��߷����Ƿ��γ��˻���ĺͳ���
	text = chessMapToString(tempChessMap, newPos, 1, -1);
	if (kmpSearch(text, "011110") || kmpSearch(text, "11101") || kmpSearch(text, "10111") || kmpSearch(text, "-11110") || kmpSearch(text, "01111-") || kmpSearch(text, "01111") || kmpSearch(text, "11110")) {
		count++;
	}



	return count >= 2;

}


bool Chess::isLongLongBan(ChessPos newPos)
{
	int gradeSize = chessMap.size();


	// �������̵���ʱ����
	vector<vector<int>> tempChessMap = chessMap;
	tempChessMap[newPos.row][newPos.col] = CHESS_BLACK;

	string text;
	// ���ˮƽ�����Ƿ��γ��˻��ĺͳ���
	text = chessMapToString(tempChessMap, newPos, 0, 1);
	if (kmpSearch(text, "111111") || kmpSearch(text, "1111111") || kmpSearch(text, "1111111") || kmpSearch(text, "11111111") || kmpSearch(text, "111111111")) {
		return true;
	}

	// �����ֱ�����Ƿ��γ��˻��ĺͳ���
	text = chessMapToString(tempChessMap, newPos, 1, 0);
	if (kmpSearch(text, "111111") || kmpSearch(text, "1111111") || kmpSearch(text, "1111111") || kmpSearch(text, "11111111") || kmpSearch(text, "111111111")) {
		return true;
	}

	// ������ϵ����¶Խ��߷����Ƿ��γ��˻��ĺͳ���
	text = chessMapToString(tempChessMap, newPos, 1, 1);
	if (kmpSearch(text, "111111") || kmpSearch(text, "1111111") || kmpSearch(text, "1111111") || kmpSearch(text, "11111111") || kmpSearch(text, "111111111")) {
		return true;
	}

	// ������ϵ����¶Խ��߷����Ƿ��γ��˻���
	text = chessMapToString(tempChessMap, newPos, 1, -1);
	if (kmpSearch(text, "111111") || kmpSearch(text, "1111111") || kmpSearch(text, "1111111") || kmpSearch(text, "11111111") || kmpSearch(text, "111111111")) {
		return true;
	}


	return false;

}

// Ѱ�Ұ�����Թ��������������������ġ�һ������һ�����ġ����ļӻ����ȵ�λ��
ChessPos Chess::findNextMoveForWhite() {
	//int gradeSize = getGradeSize();
	for (int r = 0; r < 15; ++r) {
		for (int c = 0; c < 15; ++c) {
			if (chessMap[r][c] == 0) {
				// �ڿհ�λ���°��壬������Ƿ���������
				vector<vector<int>> tempMap = chessMap;
				tempMap[r][c] = -1; // �°���

				// ���µİ���ת��Ϊ�ַ�����������ģʽƥ����
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
		//�Ƿ��γ��˻��ĺͳ���
		string text = chessMapToString(chessMap, pos, dr[i], dc[i]);
		if (kmpSearch(text, "0----0") || kmpSearch(text, "0----1") || kmpSearch(text, "1----0") || kmpSearch(text, "---0-") || kmpSearch(text, "-0---") || kmpSearch(text, "----0") || kmpSearch(text, "0----")) {
			count++;
		}

		if (kmpSearch(text, "0---0") || kmpSearch(text, "0--0-0") || kmpSearch(text, "0-0--0")) {
			count++;
		}
		//����������ģʽ�Ļ���˵�������������01110�Ǽٵģ�
		if (kmpSearch(text, "---0-") || kmpSearch(text, "-0---"))
		{
			count--;
		}
	}
	return  count >= 2;
}

//���� Ѱ��34���
ChessPos Chess::findNextMoveForBlack() {

	int gradeSize = getGradeSize();

	for (int r = 0; r < 15; ++r) {
		for (int c = 0; c < 15; ++c) {
			if (chessMap[r][c] == 0) {
				// �ڿհ�λ���°��壬������Ƿ���������
				vector<vector<int>> tempMap = chessMap;
				tempMap[r][c] = 1; // �º���

				// ���µİ���ת��Ϊ�ַ�����������ģʽƥ����
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
		//һ�������ϵĳ���
		string text = chessMapToString(chessMap, pos, dr[i], dc[i]);
		// �Ƿ��γ��˳���
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
				//����������ģʽ�Ļ���˵�������������01110�Ǽٵģ�
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

// ����KMP�㷨�е�ǰ׺����
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


// ʵ��KMP�㷨����ģʽƥ��
bool  Chess::kmpSearch(const string& text, const string& pattern) {
	int n = text.length();
	int m = pattern.length();

	if (m == 0) {
		return true; // ��ģʽ���κ��ı��ж�����
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
			return true; // ���ı����ҵ���ģʽ
		}
	}
	return false; // ���ı���δ�ҵ�ģʽ
}


//������ת��Ϊ�ַ���
string Chess::chessMapToString(const vector<vector<int>>& chessMap, const ChessPos& pos, int dr, int dc) {
	string chessboardString;
	int gradeSize = getGradeSize();

	//��ʱд��(��ֵ�һ��)
	for (int i = -7; i <= 7; ++i) {
		int r = pos.row + i * dr;
		int c = pos.col + i * dc;

		if (r < 0 || r >= gradeSize || c < 0 || c >= gradeSize)
			continue; // �������̱߽�

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




//�ж���Ӯ
bool Chess::checkOver()
{
	//checkWin()

	if (checkWin())
	{
		//Sleep(1500);
		if (playerFlag == false) {

			//�ղ��������Man ����ʤ��
			std::cout  << "---ʤ��--" << "\n";
			//loadimage(0, "res/ʤ��.png", 740, 712);
		}
		else {
			std::cout << "---ʧ��--" << "\n";
			//loadimage(0, "res/ʧ��.png", 740, 712);
		}

		  
		ChessBook chessBook;
		chessBook.exportRecord(playerFlag, moveHistory);

		//�����Ƿ����¿�ʼ


		//return true;

	}

	return false;
}


//����
void Chess::undoMove() {
	cout << "�������ing" << endl;
	if (!moveHistory.empty()) {
		// ��ȡ�������������λ��
		ChessPos secondLast = moveHistory.back();
		moveHistory.pop_back();
		ChessPos last = moveHistory.back();
		moveHistory.pop_back();

		// ���������������������ӣ������߷���¼���Ƴ�
		chessMap[secondLast.row][secondLast.col] = 0;
		chessMap[last.row][last.col] = 0;

		// ���»������̣����Ƴ�����
		int x1 = margin_x + last.col * chessSize - 0.5 * chessSize;
		int y1 = margin_y + last.row * chessSize - 0.5 * chessSize;
		int x2 = margin_x + secondLast.col * chessSize - 0.5 * chessSize;
		int y2 = margin_y + secondLast.row * chessSize - 0.5 * chessSize;

		putimagePNG(x1, y1, &chessKongImg);
		putimagePNG(x2, y2, &chessKongImg);
		count -= 2;
	}
}




//��ȡ��ǰ�������ɫ
chess_kind_t Chess::getCurrentPlayer()
{
	if (playerFlag)
		return CHESS_BLACK;
	else
		return CHESS_WHITE;
}

//�ָ������õİڷ�����
void Chess::chessRecover(ChessPos* pos, chess_kind_t kind) {
	int x = margin_x + pos->col * chessSize - 0.5 * chessSize;
	//std::cout << "x = " << x << std::endl;
	int y = margin_y + pos->row * chessSize - 0.5 * chessSize;
	//std::cout << "y = " << y << std::endl;
	//std::cout << "row = " << pos->row << std::endl;
	//std::cout << "col = " << pos->col << std::endl;
	moveHistory.push_back(*pos);//�����ӷŽ���ʷ��¼
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
		//��
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
