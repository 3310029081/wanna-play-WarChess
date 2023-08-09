#include "AI.h"
#include "Node.h"
#include <iostream>

//��ʼ��
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
	cout <<"AI����˼����" << endl;
	
	if (colour == -1) {
		// Ѱ�Һ������һ���ŷ�
		should = m_chess->findNextMoveForBlack();
	}
	else if (colour == 1) {
		// Ѱ�Ұ������һ���ŷ�
		should = m_chess->findNextMoveForWhite();
	}

	if (think2(m_chess) > 4888)
	{
		should = think(m_chess);
	}
	else if (should.row != -1 && should.col != -1) {
		// �ҵ����ʵ��ŷ���ִ�и��ŷ�
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
		//����ǰ10���з���
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
				//ֻ�к��岢��û�н��ֲ���
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
	cout << "AI˼������" << endl;
	cout << "\n�ҷ�����=======>>>>>>     " << char(pos->col + 65) << " , " << 15 - pos->row << "\n" << endl;
	m_chess->chessDown(pos, colour);

}
//�������
void AI::calculateScore(Chess* m_chess)
{
	int personNum = 0; //���ַ������壩���ٸ�����������
	int aiNum = 0; //AI�������壩�����ж��ٸ�����������
	int emptyNum = 0; // �÷����Ͽհ�λ�ĸ���

	// ����������������
	for (int i = 0; i < scoreMap.size(); i++) {
		for (int j = 0; j < scoreMap[i].size(); j++) {
			scoreMap[i][j] = 0;
		}
	}

	int size = m_chess->getGradeSize();
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			//��ÿ������м���
			if (m_chess->getChessData(row, col)) continue;

			for (int y = -1; y <= 0; y++) {        //Y�ķ�Χ����-1�� 0
				for (int x = -1; x <= 1; x++) {    //X�ķ�Χ�� -1,0,1
					if (y == 0 && x == 0) continue;
					if (y == 0 && x != 1) continue; //��y=0ʱ��������x=1
					//��ֹ��3����2��

					personNum = 0;
					aiNum = 0;
					emptyNum = 0;

					// ��������ڸ�λ�����ӣ��ṹ��ʲô����
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

					// �����������
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

					if (personNum == 1) { //��2
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

					// ��������ڸ�λ�����ӣ��ṹ��ʲô����
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
//����
ChessPos AI::think(Chess* m_chess)
{
	// ��������
	calculateScore(m_chess);

	// ���������ҳ���������λ��
	int maxScore = 0;
	vector<ChessPos> maxPoints;

	int size = m_chess->getGradeSize();
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			// ǰ������������ǿյ�
			if (m_chess->getChessData(row, col) == 0) {
				ChessPos pos(row, col);
				chess_kind_t kind = m_chess->getCurrentPlayer(); // ��ȡ��ǰAIִ�����ɫ

				//TODO xu ��ʱע��
				/*
				if (kind == CHESS_BLACK && chess->checkForbidden(pos, kind)) {
					// AI�Ǻ��岢��λ��Υ�����ֹ���������λ��
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
		// ���û�кϷ���λ�ã����ѡ��һ��
		int index = rand() % size;
		int row = index / size;
		int col = index % size;
		return ChessPos(row, col);
	}
	else {
		// ���ѡ��һ���Ϸ���λ��
		int index = rand() % maxPoints.size();
		return maxPoints[index];
	}
}
int AI::think2(Chess* m_chess)
{
	// ��������
	calculateScore(m_chess);

	// ���������ҳ���������λ��
	int maxScore = 0;
	vector<ChessPos> maxPoints;
	

	int size = m_chess->getGradeSize();
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			// ǰ������������ǿյ�
			if (m_chess->getChessData(row, col) == 0) {
				ChessPos pos(row, col);
				chess_kind_t kind = m_chess->getCurrentPlayer(); // ��ȡ��ǰAIִ�����ɫ

				//TODO xu ��ʱע��
				/*
				if (kind == CHESS_BLACK && chess->checkForbidden(pos, kind)) {
					// AI�Ǻ��岢��λ��Υ�����ֹ���������λ��
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
		// ���û�кϷ���λ�ã����ѡ��һ��
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
	// ��������
	calculateScore(m_chess);



	// ���������ҳ���������λ��
	int minScore = 100;
	vector<ChessPos> minPoints;

	int size = m_chess->getGradeSize();
	for (size_t i = 0; i < 3; i++)
	{

		//getChessData return ���λ����ʲô��ɫ

		ChessPos pos(chessArr[i].row, chessArr[i].col);


		//TODO xu ��ʱע��
//chess_kind_t kind = m_chess->getCurrentPlayer(); // ��ȡ��ǰAIִ�����ɫ
/*
if (kind == CHESS_BLACK && chess->checkForbidden(pos, kind)) {
	// AI�Ǻ��岢��λ��Υ�����ֹ���������λ��
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
		// ���û�кϷ���λ�ã����ѡ��һ��
		int index = rand() % size;
		int row = index / size;
		int col = index % size;
		return ChessPos(row, col);
	}
	else {
		// ���ѡ��һ���Ϸ���λ��
		int index = rand() % minPoints.size();
		return minPoints[index];
	}
}



















// ���ȿɱ�汾
ChessPos AI::thinkFIVELengChange(Chess* m_chess, ChessPos* chessArr, int FiveHandNum)
{



	// ��������
	calculateScore(m_chess);


	
	// ���������ҳ���������λ��
	int minScore = 100;
	vector<ChessPos> minPoints;

	int size = m_chess->getGradeSize();
	for (size_t i = 0; i < FiveHandNum; i++)
	{

		//getChessData return ���λ����ʲô��ɫ

		ChessPos pos(chessArr[i].row, chessArr[i].col);


		//TODO xu ��ʱע��
//chess_kind_t kind = m_chess->getCurrentPlayer(); // ��ȡ��ǰAIִ�����ɫ
/*
if (kind == CHESS_BLACK && chess->checkForbidden(pos, kind)) {
	// AI�Ǻ��岢��λ��Υ�����ֹ���������λ��
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
		// ���û�кϷ���λ�ã����ѡ��һ��
		int index = rand() % size;
		int row = index / size;
		int col = index % size;
		return ChessPos(row, col);
	}
	else {
		// ���ѡ��һ���Ϸ���λ��
		int index = rand() % minPoints.size();
		return minPoints[index];
	}
}
