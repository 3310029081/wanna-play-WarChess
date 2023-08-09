#include "ChessGame.h"
#include "ChessBook.h"

#include <random>


ChessGame::ChessGame(Man* man, AI* ai, Chess* chess)
{
	this->man = man;
	this->ai = ai;
	this->chess = chess;

	ai->init(chess);
	man->init(chess);
}
// ��Ҫ���õĺ���A
void functionA(Chess chess, Man man, AI ai) {//����
	chess.count = 1;

	chess.playerFlag = true;

	//������Ϣ�����ڻָ���ʱ���ж�
	bool chooseFlag = false;//trueΪai��
	ChessBook chessbook;
	chessbook.saveChessChoose(chooseFlag);

	//����
	//�������� ȷ�������ִ�������

	chess.playerFlag = true;
	man.colour = CHESS_WHITE;
	ai.colour = CHESS_BLACK;

	//chessMap ���к���

	//��chessdown�����ȽϺ�

	ChessPos pos1(7, 7);
	chess.chessDown(&pos1, CHESS_BLACK);
	chess.count++;
	cout << "chess.count : " << chess.count << endl;

	//ԭ��7 8 
	ChessPos pos2(6, 7);
	chess.chessDown(&pos2, CHESS_WHITE);
	chess.count++;
	cout << "chess.count : " << chess.count << endl;
	ChessPos pos3;




	srand(time(0));//�������������

	int r = rand();


	//ԭ����5 6
	cout << "r �� ֵ Ϊ :  " << r << endl;
	if (r % 4 == 0)
	{
		ChessPos pos3(5, 9);
		chess.chessDown(&pos3, CHESS_BLACK);
	}
	else if (r % 2 == 0)
	{
		ChessPos pos3(5, 9);
		chess.chessDown(&pos3, CHESS_BLACK);
	}
	else {
		ChessPos pos3(5, 9);
		chess.chessDown(&pos3, CHESS_BLACK);
	}

	chess.count++;
	cout << "chess.count : " << chess.count << endl;

	//����д��
	//��������Ӵ������ ---> 3


	cout << "�����ִ������Ϊ 3  " << endl;
	chessbook.saveFiveHandNum(3);//��������ִ����

	//������� 
	cout << "�Է��Ƿ�ѡ���� ����� ������1 �������������0  " << endl;




	int flag;
	while (1) {
		cin >> flag;
		cout << "flag : " << flag << endl;
		if (flag == 1 || flag == 0) {
			break;
		}
		cout << "����������������룡" << endl;
	}




	if (flag == 1) {
		//�������
		cout << "---����---" << endl;
		chess.playerFlag = false;
		man.colour = CHESS_BLACK;
		ai.colour = CHESS_WHITE;

		//������Ϣ�����ڻָ���ʱ���ж�
		ChessBook chessbook;
		chessbook.saveChessChoose(!chooseFlag);

		ai.go46(&chess, &chess.lastPos);

		chess.count++;
		cout << "chess.count : " << chess.count << endl;

		//�������������
		//���������̸������ǵ�����¼������  �����
		ChessPos AIThink;


		ChessPos chessArr[3];
		for (size_t i = 0; i < 3; i++)
		{
			MOUSEMSG msg;
			while (1) {
				msg = GetMouseMsg();
				if (msg.uMsg == WM_LBUTTONDOWN && chess.clickBoard(msg.x, msg.y, &chessArr[i])) {
					cout << "λ�úϷ�" << endl;
					break;
				}

			}
			chess.giveImg(chessArr[i].row, chessArr[i].col, CHESS_CHOOSEBLACK);

		}
		//����5��n�򣬻ָ����ʱ����
		chessbook.saveFiveHandChess(chessArr, 3);

		// Ȼ��AI����

		cout << "AI������ѡλ��......  " << endl;
		Sleep(500);
		//AIѡ���������е�һ��-->Ȼ��������� Ȼ���������ʵ�� Ȼ�� ��������������� �ö��汣��
		AIThink = ai.thinkFIVE(&chess, chessArr);

		//����ոյ���ͼ
		for (size_t i = 0; i < 3; i++)
		{
			chess.giveImg(chessArr[i].row, chessArr[i].col, 888);
		}






		cout << "�ҷ�AIѡ���λ����: " << AIThink.row << "�� " << AIThink.col << "�� " << endl;


		chess.chessMap[AIThink.row][AIThink.col] = CHESS_BLACK;
		int fiveChooseRow = AIThink.row;
		int fiveChooseCol = AIThink.col;
		ChessPos fiveChoose(fiveChooseRow, fiveChooseCol);
		chess.chessDown(&fiveChoose, CHESS_BLACK);

		//�Ծ��������У�ɾ��5��n�������
		chessbook.clearFiveHandChess();

		chess.count++;
		cout << "chess.count : " << chess.count << endl;
		ai.go46(&chess, &chess.lastPos);
	
		//��������
		while (1) {


			
			man.go(&chess, &chess.lastPos);
			chess.count++;
			cout << "chess.count : " << chess.count << endl;
			if (chess.checkOver()) {
				chess.init();
				continue;
			}
			ai.go(&chess, &chess.lastPos);
			chess.count++;
			cout << "chess.count : " << chess.count << endl;
			if (chess.checkOver()) {
				chess.init();
				continue;
			}

		}

	}
	else if (flag == 0) {
		//���������

		cout << "---������---" << endl;

		chess.count = 3;

		//�����������


		Sleep(500);

		man.go(&chess, &chess.lastPos);

		chess.count++;
		cout << "chess.count : " << chess.count << endl;


		ChessPos  chessPosClean[3];
		//�³����
		for (size_t i = 0; i < 3; i++)
		{

			ChessPos chessPos = ai.think(&chess);
			chessPosClean[i] = chessPos;
			chess.chessMap[chessPos.row][chessPos.col] = CHESS_CHOOSEBLACK;
			chess.giveImg(chessPos.row, chessPos.col, CHESS_CHOOSEBLACK);
		}

		//����5��n�򣬻ָ����ʱ����
		chessbook.saveFiveHandChess(chessPosClean, 3);

		//������ݲ��� ֻʣ����ͼ


		for (size_t i = 0; i < 3; i++)
		{

			chess.chessMap[chessPosClean[i].row][chessPosClean[i].col] = 0;

		}


		// ����� ֱ�ӻ�ȡλ��Ȼ��chessDown������
		//��Ҫ�����ȷ�� Ȼ����ǿŵ�λ��ȷ��

		ChessPos pos4;
		MOUSEMSG msg;
		while (1) {
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN && chess.clickBoard(msg.x, msg.y, &pos4)) {
				cout << "λ�úϷ�" << endl;
				break;
			}

		}


		//���������
		cout << "��ʼ������ͼ" << endl;



		for (size_t i = 0; i < 3; i++)
		{

			chess.giveImg(chessPosClean[i].row, chessPosClean[i].col, 888);

		}


		// ����
		chess.chessDown(&pos4, CHESS_BLACK);

		//�Ծ��������У�ɾ��5��n�������
		chessbook.clearFiveHandChess();

		chess.count++;
		cout << "chess.count : " << chess.count << endl;


		//��ʼ�����Ծ�


		while (1) {
			man.go(&chess, &chess.lastPos);
			chess.count++;
			cout << "chess.count : " << chess.count << endl;
			if (chess.checkOver()) {
				chess.init();
				continue;
			}

			ai.go(&chess, &chess.lastPos);
			chess.count++;
			cout << "chess.count : " << chess.count << endl;
			if (chess.checkOver()) {
				chess.init();
				continue;
			}
		}


	}

}

// ��Ҫ���õĺ���B
void functionB(Chess chess, Man man, AI ai) {//����
	chess.playerFlag = false;
	chess.count = 1;
	man.colour = CHESS_BLACK;
	ai.colour = CHESS_WHITE;
	//������Ϣ�����ڻָ���ʱ���ж�
	bool chooseFlag = true;
	ChessBook chessbook;
	chessbook.saveChessChoose(chooseFlag);


	//�������������--->��������ķ�ʽ�°� �Լ� �������-->��һ�������洢���� 



	//���������ǰ׷�


	//���������������λ�� ���� �����³�����

	cout << "�����³�ǰ����" << endl;

	ChessPos posThree[3];
	for (size_t i = 0; i < 3; i++)
	{
		MOUSEMSG msg;
		while (1) {
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN && chess.clickBoard(msg.x, msg.y, &posThree[i])) {
				cout << "λ�úϷ�" << endl;
				break;
			}

		}

		if (i == 1)
		{
			//�ڶ��ְ� 
			chess.chessDown(&posThree[i], CHESS_WHITE);
			chess.count++;
			cout << "chess.count : " << chess.count << endl;
		}
		else
		{

			chess.chessDown(&posThree[i], CHESS_BLACK);
			chess.count++;
			cout << "chess.count : " << chess.count << endl;
		}


	}

	Sleep(5000);


	//AI ѡ���Ƿ���  ��������Ǻ����ʤ����-->  ���� ���� 
	cout << "�Է������Ĵ��������?" << endl;
	int FiveHandNum;

	cin >> FiveHandNum;
	cout << "�Է������ĵ����ִ������ : " << FiveHandNum << " �Ѿ��ɹ���¼ " << endl;
	chessbook.saveFiveHandNum(FiveHandNum);//��������ִ����

	cout << "AI����˼����������..." << endl;

	// 1 �� 0 ���� 
	int changeHandFlag;

	if (chess.chessMap[6][8] == CHESS_BLACK)
	{
		changeHandFlag = 1;

	}
	else if (chess.chessMap[6][6] == CHESS_BLACK) {
		changeHandFlag = 1;
	}
	else if (chess.chessMap[8][6] == CHESS_BLACK) {
		changeHandFlag = 1;
	}
	else if (chess.chessMap[8][8] == CHESS_BLACK) {
		changeHandFlag = 1;
	}
	//�����
	else if (chess.chessMap[5][6] == CHESS_BLACK) {
		changeHandFlag = 1;
	}
	else if (chess.chessMap[5][8] == CHESS_BLACK) {
		changeHandFlag = 1;
	}
	else if (chess.chessMap[9][8] == CHESS_BLACK) {
		changeHandFlag = 1;
	}
	else if (chess.chessMap[9][6] == CHESS_BLACK) {
		changeHandFlag = 1;
	}




	else {
		changeHandFlag = 0;
	}





	if (changeHandFlag)
	{
		//����
		cout << "�ҷ�AIѡ����" << endl;
		//�ҷ���� �������ɶԷ���
		man.colour = CHESS_WHITE;
		ai.colour = CHESS_BLACK;

		//������Ϣ�����ڻָ���ʱ���ж�
		ChessBook chessbook;
		chessbook.saveChessChoose(!chooseFlag);

		man.go(&chess, &chess.lastPos);

		chess.count++;
		cout << "chess.count : " << chess.count << endl;

		//�������λ�� ����������Ѿ���¼��

		//�³����

		vector<ChessPos> chessPosClean;
		ChessPos fiveHandChess[10];//�ָ�����ļ��е�����
		for (size_t i = 0; i < FiveHandNum; i++)
		{

			ChessPos chessPos = ai.think(&chess);
			fiveHandChess[i] = chessPos;//copy���ݵ�chessArr
			chessPosClean.push_back(chessPos);

			chess.chessMap[chessPos.row][chessPos.col] = CHESS_CHOOSEBLACK;
			chess.giveImg(chessPos.row, chessPos.col, CHESS_CHOOSEBLACK);
		}

		//����5��n�򣬻ָ����ʱ����
		chessbook.saveFiveHandChess(fiveHandChess, FiveHandNum);

		//������ݲ��� ֻʣ����ͼ


		for (size_t i = 0; i < FiveHandNum; i++)
		{

			chess.chessMap[chessPosClean[i].row][chessPosClean[i].col] = 0;

		}


		// ����� ֱ�ӻ�ȡλ��Ȼ��chessDown������
		//��Ҫ�����ȷ�� Ȼ����ǿŵ�λ��ȷ��

		ChessPos pos4;
		MOUSEMSG msg;
		while (1) {
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN && chess.clickBoard(msg.x, msg.y, &pos4)) {
				cout << "λ�úϷ�" << endl;
				break;
			}

		}


		//���������
		cout << "��ʼ������ͼ" << endl;



		for (size_t i = 0; i < FiveHandNum; i++)
		{

			chess.giveImg(chessPosClean[i].row, chessPosClean[i].col, 888);

		}


		// ����
		chess.chessDown(&pos4, CHESS_BLACK);

		//�Ծ��������У�ɾ��5��n�������
		chessbook.clearFiveHandChess();

		chess.count++;
		cout << "chess.count : " << chess.count << endl;


		//�����Ծ� -->������ �� ���� 

		while (1) {

			man.go(&chess, &chess.lastPos);
			chess.count++;
			cout << "chess.count : " << chess.count << endl;
			if (chess.checkOver()) {
				chess.init();
				continue;
			}



			ai.go(&chess, &chess.lastPos);
			chess.count++;
			cout << "chess.count : " << chess.count << endl;
			if (chess.checkOver()) {
				chess.init();
				continue;
			}


		}


	}
	else {
		//������
		cout << "�ҷ�AIѡ�񲻻���,�����Ծ�" << endl;
		//������
		chess.count = 3;


		ai.go46(&chess, &chess.lastPos);
		chess.count++;
		cout << "chess.count : " << chess.count << endl;
		//ѡ��������������  

		//FiveHandNum
		ChessPos AIThink;

		ChessPos chessArr[10];


		ChessPos nullPos;

		//��chessArr��ʼ��
		for (size_t i = 0; i < FiveHandNum; i++)
		{
			chessArr[i] = 0;
		}


		for (size_t i = 0; i < FiveHandNum; i++)
		{


			MOUSEMSG msg;
			while (1) {
				msg = GetMouseMsg();
				if (msg.uMsg == WM_LBUTTONDOWN && chess.clickBoard(msg.x, msg.y, &chessArr[i])) {
					cout << "λ�úϷ�" << endl;
					break;
				}

			}
			chess.giveImg(chessArr[i].row, chessArr[i].col, CHESS_CHOOSEBLACK);

		}

		//����5��n�򣬻ָ����ʱ����
		chessbook.saveFiveHandChess(chessArr, FiveHandNum);

		// Ȼ��AI����

		cout << "AI������ѡλ��......  " << endl;
		Sleep(500);
		//AIѡ���������е�һ��-->Ȼ��������� Ȼ���������ʵ�� Ȼ�� ��������������� �ö��汣��

		AIThink = ai.thinkFIVELengChange(&chess, chessArr, FiveHandNum);

		//����ոյ���ͼ
		for (size_t i = 0; i < 3; i++)
		{
			chess.giveImg(chessArr[i].row, chessArr[i].col, 888);
		}

		cout << "�ҷ�AIѡ���λ����: " << AIThink.row << "�� " << AIThink.col << "�� " << endl;


		chess.chessMap[AIThink.row][AIThink.col] = CHESS_BLACK;
		int fiveChooseRow = AIThink.row;
		int fiveChooseCol = AIThink.col;
		ChessPos fiveChoose(fiveChooseRow, fiveChooseCol);
		chess.chessDown(&fiveChoose, CHESS_BLACK);

		//�Ծ��������У�ɾ��5��n�������
		chessbook.clearFiveHandChess();

		chess.count++;
		cout << "chess.count : " << chess.count << endl;

		//�����Ծ�  �ҷ��� ���� �� 

		ai.go46(&chess, &chess.lastPos);

		while (1) {


			

			man.go(&chess, &chess.lastPos);
			chess.count++;
			cout << "chess.count : " << chess.count << endl;
			if (chess.checkOver()) {
				chess.init();
				continue;
			}

			ai.go(&chess, &chess.lastPos);
			chess.count++;
			cout << "chess.count : " << chess.count << endl;
			if (chess.checkOver()) {
				chess.init();
				continue;
			}

		}

	}

}

// ��Ҫ���õĺ���C
void functionC(){
	// ��������Ӻ���D��ʵ�ʴ���
}

// ��Ҫ���õĺ���D
void functionD() {//��Ϣ

	// ��������Ӻ���D��ʵ�ʴ���
}

// ��Ҫ���õĺ���E
void functionE(Chess m_chess, Man man, AI ai) {//�ָ�

	//�ָ���һ����
	ChessBook chessbook;
	m_chess.playerFlag = chessbook.setChessByMoveHistory(&m_chess);
	bool Flag = chessbook.loadChessChoose();//trueΪai��
	if (Flag == true) {
		std::cout << "ai�ǰ���: " << std::endl;
		man.colour = CHESS_BLACK;
		ai.colour = CHESS_WHITE;
	}
	else {
		std::cout << "ai�Ǻ���: " << std::endl;
		man.colour = CHESS_WHITE;
		ai.colour = CHESS_BLACK;
	}

	//�ж��Ƿ���Ҫ5��n��
	int HistoryChessSize = chessbook.getHistoryChessCount(chessbook.loadMoveHistory());
	int FiveHandNum = chessbook.loadFiveHandNum();//��ȡ�Ѿ������FiveHandNum
	int fiveHandChessSize = chessbook.getHistoryChessCount(chessbook.loadFiveHandChess());//�Ѿ��µĴ����
	std::string filename = "fiveHandChessData.txt";
	if (HistoryChessSize < 5) {
		if (HistoryChessSize == 0) {
			//û�¹��壡
			cout << "û�¹���!�����ݣ�Ĭ�����ֿ�ʼ" << endl;
		}
		//�¹��壬��Ҫ������д���
		//����
		if (HistoryChessSize <= 3) {
			//��������ǻ���
			if (HistoryChessSize == 1) {
				//��������
				//��������
				if (m_chess.playerFlag == true) {
					//std::cout << "�ֵ��˷�����" << std::endl;
					man.go(&m_chess, &m_chess.lastPos);
					if (m_chess.checkOver()) {
						m_chess.init();
					}
				}
				else
				{
					//std::cout << "�ֵ�ai������" << std::endl;
					ai.go(&m_chess, &m_chess.lastPos);
					if (m_chess.checkOver()) {
						m_chess.init();
					}
				}
				HistoryChessSize++;
			}
			if (HistoryChessSize == 2) {
				//��������
				//��������
				if (m_chess.playerFlag == true) {
					//std::cout << "�ֵ��˷�����" << std::endl;
					man.go(&m_chess, &m_chess.lastPos);
					if (m_chess.checkOver()) {
						m_chess.init();
					}
					//���ǻ���
					cout << "AI����˼����������..." << endl;
					// 1 �� 0 ���� 
					int changeHandFlag;
					if (m_chess.chessMap[6][8] == CHESS_BLACK)
					{
						changeHandFlag = 1;
					}
					else if (m_chess.chessMap[6][6] == CHESS_BLACK) {
						changeHandFlag = 1;
					}
					else if (m_chess.chessMap[8][6] == CHESS_BLACK) {
						changeHandFlag = 1;
					}
					else if (m_chess.chessMap[8][8] == CHESS_BLACK) {
						changeHandFlag = 1;
					}
					else {
						changeHandFlag = 0;
					}
					if (changeHandFlag)
					{
						//����
						cout << "�ҷ�AIѡ����" << endl;
						//�ҷ���� �������ɶԷ���
						man.colour = CHESS_WHITE;
						ai.colour = CHESS_BLACK;
						//������Ϣ�����ڻָ���ʱ���ж�
						ChessBook chessbook;
						chessbook.saveChessChoose(!Flag);
						Flag = !Flag;
					}
				}
				else
				{
					//std::cout << "�ֵ�ai������" << std::endl;
					ai.go(&m_chess, &m_chess.lastPos);
					if (m_chess.checkOver()) {
						m_chess.init();
					}
					//���ǻ���
					cout << "�Է��Ƿ�ѡ���� ����� ������1 �������������0  " << endl;

					int flag;
					while (1) {
						cin >> flag;
						cout << "flag : " << flag << endl;
						if (flag == 1 || flag == 0) {
							break;
						}
						cout << "����������������룡" << endl;
					}
					if (flag == 1) {
						//�������
						cout << "---����---" << endl;
						m_chess.playerFlag = false;
						man.colour = CHESS_BLACK;
						ai.colour = CHESS_WHITE;
						//������Ϣ�����ڻָ���ʱ���ж�
						ChessBook chessbook;
						chessbook.saveChessChoose(!Flag);
						Flag = !Flag;
					}
					cout << "�Է������Ĵ��������?" << endl;
					int FiveHandNum;
					cin >> FiveHandNum;
					cout << "�Է������ĵ����ִ������ : " << FiveHandNum << " �Ѿ��ɹ���¼ " << endl;
					chessbook.saveFiveHandNum(FiveHandNum);//��������ִ����
				}
				HistoryChessSize++;
			}
			if (HistoryChessSize == 3) {
				//��������
				//��������
				if (m_chess.playerFlag == true) {
					//std::cout << "�ֵ��˷�����" << std::endl;
					man.go(&m_chess, &m_chess.lastPos);
					if (m_chess.checkOver()) {
						m_chess.init();
					}
				}
				else
				{
					//std::cout << "�ֵ�ai������" << std::endl;
					ai.go(&m_chess, &m_chess.lastPos);
					if (m_chess.checkOver()) {
						m_chess.init();
					}
				}
				HistoryChessSize++;
			}
		}
		//5��n��
		FiveHandNum = chessbook.loadFiveHandNum();//����FiveHandNum
		if (HistoryChessSize == 4 && chessbook.isFileEmpty(filename)) {
			//��������
			if (m_chess.playerFlag == true) {
				std::cout << "�ֵ��˷�5��" << std::endl;
				//����n��
				ChessPos AIThink;
				ChessPos chessArr[10];
				ChessPos nullPos;
				//��chessArr��ʼ��
				for (size_t i = 0; i < FiveHandNum; i++)
				{
					chessArr[i] = 0;
				}
				for (size_t i = 0; i < FiveHandNum; i++)
				{
					MOUSEMSG msg;
					while (1) {
						msg = GetMouseMsg();
						if (msg.uMsg == WM_LBUTTONDOWN && m_chess.clickBoard(msg.x, msg.y, &chessArr[i])) {
							cout << "λ�úϷ�" << endl;
							break;
						}
					}
					m_chess.giveImg(chessArr[i].row, chessArr[i].col, CHESS_CHOOSEBLACK);
				}
				//����5��n�򣬻ָ����ʱ����
				chessbook.saveFiveHandChess(chessArr, FiveHandNum);
				// Ȼ��AI����
				cout << "AI������ѡλ��......  " << endl;
				Sleep(500);
				//AIѡ���������е�һ��-->Ȼ��������� Ȼ���������ʵ�� Ȼ�� ��������������� �ö��汣��
				AIThink = ai.thinkFIVELengChange(&m_chess, chessArr, FiveHandNum);
				//����ոյ���ͼ
				for (size_t i = 0; i < FiveHandNum; i++)//FiveHandNum \ 3
				{
					m_chess.giveImg(chessArr[i].row, chessArr[i].col, 888);
				}
				cout << "�ҷ�AIѡ���λ����: " << AIThink.row << "�� " << AIThink.col << "�� " << endl;
				m_chess.chessMap[AIThink.row][AIThink.col] = CHESS_BLACK;
				int fiveChooseRow = AIThink.row;
				int fiveChooseCol = AIThink.col;
				ChessPos fiveChoose(fiveChooseRow, fiveChooseCol);
				m_chess.chessDown(&fiveChoose, CHESS_BLACK);
				//�Ծ��������У�ɾ��5��n�������
				chessbook.clearFiveHandChess();
			}
			else
			{
				std::cout << "�ֵ�ai��5��" << std::endl;
				//��ʼ5��n��
				ChessPos  chessPosClean[10];
				//�³����
				for (size_t i = 0; i < FiveHandNum; i++)
				{
					ChessPos chessPos = ai.think(&m_chess);
					chessPosClean[i] = chessPos;
					m_chess.chessMap[chessPos.row][chessPos.col] = CHESS_CHOOSEBLACK;
					m_chess.giveImg(chessPos.row, chessPos.col, CHESS_CHOOSEBLACK);
				}
				//����5��n�򣬻ָ����ʱ����
				chessbook.saveFiveHandChess(chessPosClean, FiveHandNum);//TODO 3Ӧ�û��ɴ������
				//������ݲ��� ֻʣ����ͼ
				for (size_t i = 0; i < FiveHandNum; i++)//TODO 3Ӧ�û��ɴ������
				{
					m_chess.chessMap[chessPosClean[i].row][chessPosClean[i].col] = 0;
				}
				// ����� ֱ�ӻ�ȡλ��Ȼ��chessDown������
				//��Ҫ�����ȷ�� Ȼ����ǿŵ�λ��ȷ��
				ChessPos pos4;
				MOUSEMSG msg;
				while (1) {
					msg = GetMouseMsg();
					if (msg.uMsg == WM_LBUTTONDOWN && m_chess.clickBoard(msg.x, msg.y, &pos4)) {
						cout << "λ�úϷ�" << endl;
						break;
					}
				}
				//���������
				cout << "��ʼ������ͼ" << endl;
				for (size_t i = 0; i < FiveHandNum; i++)//TODO 3Ӧ�û��ɴ������
				{
					m_chess.giveImg(chessPosClean[i].row, chessPosClean[i].col, 888);
				}
				// ����
				m_chess.chessDown(&pos4, CHESS_BLACK);
				//�Ծ��������У�ɾ��5��n�������
				chessbook.clearFiveHandChess();
			}
		}
		if (HistoryChessSize == 4 && !chessbook.isFileEmpty(filename)) {
			std::vector<ChessPos> chessPosClean = chessbook.loadFiveHandChess();//�Ѿ�����Ĵ�������

			//TODO �ж��Ƿ��Ѿ������˴��
			// FiveHandNum

			//��������n���е�ѡ������
			if (m_chess.playerFlag == true) {
				//std::cout << "aiѡ5��" << std::endl;
				/*if (Flag == true) {*/
					//ai�ף��˺�
				ChessPos AIThink;
				ChessPos chessArr[10];
				ChessPos nullPos;
				//��chessArr��ʼ��
				for (size_t i = 0; i < FiveHandNum; i++)
				{
					chessArr[i] = chessPosClean[i];
				}
				for (size_t i = 0; i < FiveHandNum; i++)
				{
					m_chess.giveImg(chessArr[i].row, chessArr[i].col, CHESS_CHOOSEBLACK);
				}
				// Ȼ��AI����
				cout << "AI������ѡλ��......  " << endl;
				Sleep(500);
				//AIѡ���������е�һ��-->Ȼ��������� Ȼ���������ʵ�� Ȼ�� ��������������� �ö��汣��
				AIThink = ai.thinkFIVELengChange(&m_chess, chessArr, FiveHandNum);
				//����ոյ���ͼ
				for (size_t i = 0; i < 3; i++)
				{
					m_chess.giveImg(chessArr[i].row, chessArr[i].col, 888);
				}
				cout << "�ҷ�AIѡ���λ����: " << AIThink.row << "�� " << AIThink.col << "�� " << endl;
				m_chess.chessMap[AIThink.row][AIThink.col] = CHESS_BLACK;
				int fiveChooseRow = AIThink.row;
				int fiveChooseCol = AIThink.col;
				ChessPos fiveChoose(fiveChooseRow, fiveChooseCol);
				m_chess.chessDown(&fiveChoose, CHESS_BLACK);
				//m_chess.playerFlag = !m_chess.playerFlag;//�ָ�����
				//�Ծ��������У�ɾ��5��n�������
				chessbook.clearFiveHandChess();
				/*}*/
			}
			else {
				//std::cout << "��ѡ5��" << std::endl;
				//if (Flag == true) {
				//	//ai�ף��˺�

				//}
				//else {
					//ai��,�˰�
					//���ӵ�����n��
				for (size_t i = 0; i < FiveHandNum; i++)
				{
					ChessPos chessPos = chessPosClean[i];
					m_chess.chessMap[chessPos.row][chessPos.col] = CHESS_CHOOSEBLACK;
					m_chess.giveImg(chessPos.row, chessPos.col, CHESS_CHOOSEBLACK);
				}
				//������ݲ��� ֻʣ����ͼ
				for (size_t i = 0; i < FiveHandNum; i++)
				{
					m_chess.chessMap[chessPosClean[i].row][chessPosClean[i].col] = 0;
				}
				// ����� ֱ�ӻ�ȡλ��Ȼ��chessDown������
				//��Ҫ�����ȷ�� Ȼ����ǿŵ�λ��ȷ��
				ChessPos pos4;
				MOUSEMSG msg;
				while (1) {
					msg = GetMouseMsg();
					if (msg.uMsg == WM_LBUTTONDOWN && m_chess.clickBoard(msg.x, msg.y, &pos4)) {
						cout << "λ�úϷ�" << endl;
						break;
					}
				}
				//���������
				cout << "��ʼ������ͼ" << endl;
				for (size_t i = 0; i < fiveHandChessSize; i++)
				{
					m_chess.giveImg(chessPosClean[i].row, chessPosClean[i].col, 888);
				}
				// ����
				m_chess.chessDown(&pos4, CHESS_BLACK);
				//�Ծ��������У�ɾ��5��n�������
				chessbook.clearFiveHandChess();
				/*}*/
			}
		}
	}
	else
	{
		cout << "�Ѿ��������֣�" << endl;
	}

	//��������
	if (m_chess.playerFlag == true) {
		std::cout << "�ֵ��˷�����" << std::endl;
	}
	else
	{
		std::cout << "�ֵ�ai������" << std::endl;
		ai.go(&m_chess, &m_chess.lastPos);
		if (m_chess.checkOver()) {
			m_chess.init();
		}
	}
	while (1) {
		man.go(&m_chess, &m_chess.lastPos);
		if (m_chess.checkOver()) {
			m_chess.init();
			continue;
		}

		ai.go(&m_chess, &m_chess.lastPos);
		if (m_chess.checkOver()) {
			m_chess.init();
			continue;
		}
	}
	//û����ʷ��־�Ĭ��Ϊtrue(����)


}


//��ʼ��Ϸ
void ChessGame::play()
{
	chess->init();
	handleMouseClick(*chess , *man , *ai);


	
}


// ����������¼��ĺ���
void ChessGame::handleMouseClick(Chess chess, Man man, AI ai) {
	int imageX1 = 120, imageX2 = 210, imageX3 = 300, imageX4 = 390, imageX5 = 480; // ͼƬ�����Ͻ�X����
	int imageY = 0;   // ͼƬ�����Ͻ�Y����
	int imageWidth = 80;
	int imageHeight = 50;
	// ��ȡ������¼�
	while (true) {
		MOUSEMSG msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN) { // ֻ������������¼������Ը��������޸�
			int mouseX = msg.x;
			int mouseY = msg.y;

			if (isMouseClickWithinImage(mouseX, mouseY, imageX1, imageY, imageWidth, imageHeight)) { 
				functionA(chess, man, ai);
			

			}
			else if (  isMouseClickWithinImage(mouseX, mouseY, imageX2, imageY, imageWidth, imageHeight)) {
				functionB(chess, man, ai);
			}
			else if (isMouseClickWithinImage(mouseX, mouseY, imageX3, imageY, imageWidth, imageHeight)) {

			}
			else if (isMouseClickWithinImage(mouseX, mouseY, imageX4, imageY, imageWidth, imageHeight)) {
				//functionD();
			}
			else if (isMouseClickWithinImage(mouseX, mouseY, imageX5, imageY, imageWidth, imageHeight)) {
				functionE(chess, man, ai);
			}
		}
	}
}

//��ť�����ť�¼�
// �ж�������Ƿ���ͼƬ�����ڵĺ���
bool ChessGame::isMouseClickWithinImage(int mouseX, int mouseY, int imageX, int imageY, int imageWidth, int imageHeight) {
	return (mouseX >= imageX && mouseX <= imageX + imageWidth && mouseY >= imageY && mouseY <= imageY + imageHeight);
}
