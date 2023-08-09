#include "AI.h"
#include "Node.h"
#include <iostream>

//初始化
void AI::init(Chess* chess)
{
	this->chess = chess;

	int size = chess->getGradeSize();
	for (int i = 0; i < size; i++) {
		vector<int> row;
		for (int j = 0; j < size; j++) {
			row.push_back(0);
		}
		scoreMap.push_back(row);
	}

}

void AI::go46(Chess* m_chess, ChessPos* pos)
{

	ChessPos pos1 = think(m_chess);
	pos->col = pos1.col;
	pos->row = pos1.row;

	m_chess->chessDown(pos, colour);
}

void AI::go(Chess* m_chess, ChessPos* pos) {
	ChessPos should;
	cout <<"AI正在思考中" << endl;
	
	if (colour == -1) {
		// 寻找黑棋的下一步着法
		should = m_chess->findNextMoveForBlack();
	}
	else if (colour == 1) {
		// 寻找白棋的下一步着法
		should = m_chess->findNextMoveForWhite();
	}

	if (think2(m_chess) > 4888)
	{
		should = think(m_chess);
	}
	else if (should.row != -1 && should.col != -1) {
		// 找到合适的着法，执行该着法
		pos->col = should.col;
		pos->row = should.row;
		m_chess->chessDown(pos, colour);
		return;
	}

	else {
		Node* node = new Node();
		int map[15][15];
		//vectorToStaticArray(m_chess->chessMap, map);
		for (int i = 0; i < 15; ++i) {
			for (int j = 0; j < 15; ++j) {
				map[i][j] = m_chess->chessMap[i][j];
			}
		}
		
		vector<ChessPos> test = node->monte_carlo_tree_search(map, pos, m_chess->count, 30000);

		vector<ChessPos> text2;
		//白棋前10进行防守
		if (colour == -1 && m_chess->count <= 10)
		{
			for (const auto& pos : test) {
				double count = 0.0;
				int x = pos.col;
				int y = pos.row;

				for (int row = (0 < (y - 2) ? (y - 2) : 0); row <= (14 < (y + 2) ? 14 : (y + 2)); row++) {
					for (int col = (0 < (x - 2) ? (x - 2) : 0); col <= (14 < (x + 2) ? 14 : (x + 2)); col++) {
						if (m_chess->chessMap[row][col] != 0) {
							count++;
						}
					}

				}

				if (count >= 3)
				{
					text2.push_back(pos);
				}
			}
		}
		if (text2.empty()) {
			text2 = test;
		}
		for (ChessPos pos : text2) {
			if (colour == 1 && !m_chess->checkForbidden(pos)) {
				//只有黑棋并且没有禁手才走
				should = pos;
				break;
			}
			else if (colour == 1)
			{
				should = pos;
			}
			else
			{
				should = pos;
				break;
			}
		}
	}

	pos->col = should.col;
	pos->row = should.row;
	cout << "AI思考结束" << endl;
	cout << "\n我方下在=======>>>>>>     " << char(pos->col + 65) << " , " << 15 - pos->row << "\n" << endl;
	m_chess->chessDown(pos, colour);

}
//计算分数
void AI::calculateScore(Chess* m_chess)
{
	int personNum = 0; //棋手方（黑棋）多少个连续的棋子
	int aiNum = 0; //AI方（白棋）连续有多少个连续的棋子
	int emptyNum = 0; // 该方向上空白位的个数

	// 评分向量数组清零
	for (int i = 0; i < scoreMap.size(); i++) {
		for (int j = 0; j < scoreMap[i].size(); j++) {
			scoreMap[i][j] = 0;
		}
	}

	int size = m_chess->getGradeSize();
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			//对每个点进行计算
			if (m_chess->getChessData(row, col)) continue;

			for (int y = -1; y <= 0; y++) {        //Y的范围还是-1， 0
				for (int x = -1; x <= 1; x++) {    //X的范围是 -1,0,1
					if (y == 0 && x == 0) continue;
					if (y == 0 && x != 1) continue; //当y=0时，仅允许x=1
					//防止活3计算2次

					personNum = 0;
					aiNum = 0;
					emptyNum = 0;

					// 假设黑棋在该位置落子，会构成什么棋型
					for (int i = 1; i <= 4; i++) {
						int curRow = row + i * y;
						int curCol = col + i * x;

						if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							m_chess->getChessData(curRow, curCol) == -1*colour) {
							personNum++;
						}
						else if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							m_chess->getChessData(curRow, curCol) == 0) {
							emptyNum++;
							break;
						}
						else {
							break;
						}
					}

					// 反向继续计算
					for (int i = 1; i <= 4; i++) {
						int curRow = row - i * y;
						int curCol = col - i * x;

						if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							m_chess->getChessData(curRow, curCol) == -1*colour) {
							personNum++;
						}
						else if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							m_chess->getChessData(curRow, curCol) == 0) {
							emptyNum++;
							break;
						}
						else {
							break;
						}
					}

					if (personNum == 1) { //连2
						scoreMap[row][col] += 10;
					}
					else if (personNum == 2) {
						if (emptyNum == 1) {
							scoreMap[row][col] += 30;
						}
						else if (emptyNum == 2) {
							scoreMap[row][col] += 40;
						}
					}
					else if (personNum == 3) {
						if (emptyNum == 1) {
							scoreMap[row][col] = 60;
						}
						else if (emptyNum == 2) {
							scoreMap[row][col] = 5000; //200
						}
					}
					else if (personNum == 4) {
						scoreMap[row][col] = 20000;
					}

					// 假设白棋在该位置落子，会构成什么棋型
					emptyNum = 0;

					for (int i = 1; i <= 4; i++) {
						int curRow = row + i * y;
						int curCol = col + i * x;

						if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							m_chess->getChessData(curRow, curCol) == colour) {
							aiNum++;
						}
						else if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							m_chess->getChessData(curRow, curCol) == 0) {
							emptyNum++;
							break;
						}
						else {
							break;
						}
					}

					for (int i = 1; i <= 4; i++) {
						int curRow = row - i * y;
						int curCol = col - i * x;

						if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							m_chess->getChessData(curRow, curCol) == colour) {
							aiNum++;
						}
						else if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							m_chess->getChessData(curRow, curCol) == 0) {
							emptyNum++;
							break;
						}
						else {
							break;
						}
					}

					if (aiNum == 0) {
						scoreMap[row][col] += 5;
					}
					else if (aiNum == 1) {
						scoreMap[row][col] += 10;
					}
					else if (aiNum == 2) {
						if (emptyNum == 1) {
							scoreMap[row][col] += 25;
						}
						else if (emptyNum == 2) {
							scoreMap[row][col] += 50;
						}
					}
					else if (aiNum == 3) {
						if (emptyNum == 1) {
							scoreMap[row][col] += 55;
						}
						else if (emptyNum == 2) {
							scoreMap[row][col] += 10000;
						}
					}
					else if (aiNum >= 4) {
						scoreMap[row][col] += 30000;
					}
				}
			}
		}
	}
}
//落子
ChessPos AI::think(Chess* m_chess)
{
	// 计算评分
	calculateScore(m_chess);

	// 从评分中找出最大分数的位置
	int maxScore = 0;
	vector<ChessPos> maxPoints;

	int size = m_chess->getGradeSize();
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			// 前提是这个坐标是空的
			if (m_chess->getChessData(row, col) == 0) {
				ChessPos pos(row, col);
				chess_kind_t kind = m_chess->getCurrentPlayer(); // 获取当前AI执棋的颜色

				//TODO xu 暂时注释
				/*
				if (kind == CHESS_BLACK && chess->checkForbidden(pos, kind)) {
					// AI是黑棋并且位置违反禁手规则，跳过该位置
					continue;
				}
				*/
				if (scoreMap[row][col] > maxScore) {
					maxScore = scoreMap[row][col];
					maxPoints.clear();
					maxPoints.push_back(pos);
				}
				else if (scoreMap[row][col] == maxScore) {
					maxPoints.push_back(pos);
				}
			}
		}
	}

	if (maxPoints.empty()) {
		// 如果没有合法的位置，随机选择一个
		int index = rand() % size;
		int row = index / size;
		int col = index % size;
		return ChessPos(row, col);
	}
	else {
		// 随机选择一个合法的位置
		int index = rand() % maxPoints.size();
		return maxPoints[index];
	}
}
int AI::think2(Chess* m_chess)
{
	// 计算评分
	calculateScore(m_chess);

	// 从评分中找出最大分数的位置
	int maxScore = 0;
	vector<ChessPos> maxPoints;
	

	int size = m_chess->getGradeSize();
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			// 前提是这个坐标是空的
			if (m_chess->getChessData(row, col) == 0) {
				ChessPos pos(row, col);
				chess_kind_t kind = m_chess->getCurrentPlayer(); // 获取当前AI执棋的颜色

				//TODO xu 暂时注释
				/*
				if (kind == CHESS_BLACK && chess->checkForbidden(pos, kind)) {
					// AI是黑棋并且位置违反禁手规则，跳过该位置
					continue;
				}
				*/
				if (scoreMap[row][col] > maxScore) {
					maxScore = scoreMap[row][col];
					maxPoints.clear();
					maxPoints.push_back(pos);
				}
				else if (scoreMap[row][col] == maxScore) {
					maxPoints.push_back(pos);
				}
			}
		}
	}

	if (maxPoints.empty()) {
		// 如果没有合法的位置，随机选择一个
		int index = rand() % size;
		int row = index / size;
		int col = index % size;
		return scoreMap[row][col];
	}
	else {
		return maxScore;
	}
}



ChessPos AI::thinkFIVE(Chess* m_chess, ChessPos* chessArr)
{
	// 计算评分
	calculateScore(m_chess);



	// 从评分中找出最大分数的位置
	int minScore = 100;
	vector<ChessPos> minPoints;

	int size = m_chess->getGradeSize();
	for (size_t i = 0; i < 3; i++)
	{

		//getChessData return 这个位置是什么颜色

		ChessPos pos(chessArr[i].row, chessArr[i].col);


		//TODO xu 暂时注释
//chess_kind_t kind = m_chess->getCurrentPlayer(); // 获取当前AI执棋的颜色
/*
if (kind == CHESS_BLACK && chess->checkForbidden(pos, kind)) {
	// AI是黑棋并且位置违反禁手规则，跳过该位置
	continue;
}
*/


		if (scoreMap[chessArr[i].row][chessArr[i].col] < minScore) {
			minScore = scoreMap[chessArr[i].row][chessArr[i].col];
			minPoints.clear();
			minPoints.push_back(pos);
		}
		else if (scoreMap[chessArr[i].row][chessArr[i].col] == minScore) {
			minPoints.push_back(pos);
		}

	}

	if (minPoints.empty()) {
		// 如果没有合法的位置，随机选择一个
		int index = rand() % size;
		int row = index / size;
		int col = index % size;
		return ChessPos(row, col);
	}
	else {
		// 随机选择一个合法的位置
		int index = rand() % minPoints.size();
		return minPoints[index];
	}
}



















// 长度可变版本
ChessPos AI::thinkFIVELengChange(Chess* m_chess, ChessPos* chessArr, int FiveHandNum)
{



	// 计算评分
	calculateScore(m_chess);


	
	// 从评分中找出最大分数的位置
	int minScore = 100;
	vector<ChessPos> minPoints;

	int size = m_chess->getGradeSize();
	for (size_t i = 0; i < FiveHandNum; i++)
	{

		//getChessData return 这个位置是什么颜色

		ChessPos pos(chessArr[i].row, chessArr[i].col);


		//TODO xu 暂时注释
//chess_kind_t kind = m_chess->getCurrentPlayer(); // 获取当前AI执棋的颜色
/*
if (kind == CHESS_BLACK && chess->checkForbidden(pos, kind)) {
	// AI是黑棋并且位置违反禁手规则，跳过该位置
	continue;
}
*/


		if (scoreMap[chessArr[i].row][chessArr[i].col] < minScore) {
			minScore = scoreMap[chessArr[i].row][chessArr[i].col];
			minPoints.clear();
			minPoints.push_back(pos);
		}
		else if (scoreMap[chessArr[i].row][chessArr[i].col] == minScore) {
			minPoints.push_back(pos);
		}

	}

	if (minPoints.empty()) {
		// 如果没有合法的位置，随机选择一个
		int index = rand() % size;
		int row = index / size;
		int col = index % size;
		return ChessPos(row, col);
	}
	else {
		// 随机选择一个合法的位置
		int index = rand() % minPoints.size();
		return minPoints[index];
	}
}
