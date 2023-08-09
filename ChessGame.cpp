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
// 需要调用的函数A
void functionA(Chess chess, Man man, AI ai) {//先手
	chess.count = 1;

	chess.playerFlag = true;

	//储存信息，用于恢复棋时候判断
	bool chooseFlag = false;//true为ai白
	ChessBook chessbook;
	chessbook.saveChessChoose(chooseFlag);

	//黑棋
	//开局三手 确定第五手打子数量

	chess.playerFlag = true;
	man.colour = CHESS_WHITE;
	ai.colour = CHESS_BLACK;

	//chessMap 先行后列

	//用chessdown函数比较好

	ChessPos pos1(7, 7);
	chess.chessDown(&pos1, CHESS_BLACK);
	chess.count++;
	cout << "chess.count : " << chess.count << endl;

	//原来7 8 
	ChessPos pos2(6, 7);
	chess.chessDown(&pos2, CHESS_WHITE);
	chess.count++;
	cout << "chess.count : " << chess.count << endl;
	ChessPos pos3;




	srand(time(0));//设置随机数种子

	int r = rand();


	//原来都5 6
	cout << "r 的 值 为 :  " << r << endl;
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

	//开局写死
	//输出第五子打点数量 ---> 3


	cout << "第五手打点数量为 3  " << endl;
	chessbook.saveFiveHandNum(3);//保存第五手打点数

	//如果换手 
	cout << "对方是否选择换手 如果是 请输入1 如果不是请输入0  " << endl;




	int flag;
	while (1) {
		cin >> flag;
		cout << "flag : " << flag << endl;
		if (flag == 1 || flag == 0) {
			break;
		}
		cout << "输入错误，请重新输入！" << endl;
	}




	if (flag == 1) {
		//如果换手
		cout << "---换手---" << endl;
		chess.playerFlag = false;
		man.colour = CHESS_BLACK;
		ai.colour = CHESS_WHITE;

		//储存信息，用于恢复棋时候判断
		ChessBook chessbook;
		chessbook.saveChessChoose(!chooseFlag);

		ai.go46(&chess, &chess.lastPos);

		chess.count++;
		cout << "chess.count : " << chess.count << endl;

		//对面给出三个点
		//三个在棋盘给出但是单独记录在棋盘  鼠标点出
		ChessPos AIThink;


		ChessPos chessArr[3];
		for (size_t i = 0; i < 3; i++)
		{
			MOUSEMSG msg;
			while (1) {
				msg = GetMouseMsg();
				if (msg.uMsg == WM_LBUTTONDOWN && chess.clickBoard(msg.x, msg.y, &chessArr[i])) {
					cout << "位置合法" << endl;
					break;
				}

			}
			chess.giveImg(chessArr[i].row, chessArr[i].col, CHESS_CHOOSEBLACK);

		}
		//储存5手n打，恢复棋的时候用
		chessbook.saveFiveHandChess(chessArr, 3);

		// 然后AI给分

		cout << "AI正在挑选位置......  " << endl;
		Sleep(500);
		//AI选择三个点中的一个-->然后给出坐标 然后把他换成实体 然后 把坐标输出给对面 让对面保留
		AIThink = ai.thinkFIVE(&chess, chessArr);

		//清理刚刚的贴图
		for (size_t i = 0; i < 3; i++)
		{
			chess.giveImg(chessArr[i].row, chessArr[i].col, 888);
		}






		cout << "我方AI选择的位置是: " << AIThink.row << "行 " << AIThink.col << "列 " << endl;


		chess.chessMap[AIThink.row][AIThink.col] = CHESS_BLACK;
		int fiveChooseRow = AIThink.row;
		int fiveChooseCol = AIThink.col;
		ChessPos fiveChoose(fiveChooseRow, fiveChooseCol);
		chess.chessDown(&fiveChoose, CHESS_BLACK);

		//对局正常进行，删除5手n打的数据
		chessbook.clearFiveHandChess();

		chess.count++;
		cout << "chess.count : " << chess.count << endl;
		ai.go46(&chess, &chess.lastPos);
	
		//正常开局
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
		//如果不换手

		cout << "---不换手---" << endl;

		chess.count = 3;

		//给出三个打点


		Sleep(500);

		man.go(&chess, &chess.lastPos);

		chess.count++;
		cout << "chess.count : " << chess.count << endl;


		ChessPos  chessPosClean[3];
		//下出打点
		for (size_t i = 0; i < 3; i++)
		{

			ChessPos chessPos = ai.think(&chess);
			chessPosClean[i] = chessPos;
			chess.chessMap[chessPos.row][chessPos.col] = CHESS_CHOOSEBLACK;
			chess.giveImg(chessPos.row, chessPos.col, CHESS_CHOOSEBLACK);
		}

		//储存5手n打，恢复棋的时候用
		chessbook.saveFiveHandChess(chessPosClean, 3);

		//清除数据残余 只剩下贴图


		for (size_t i = 0; i < 3; i++)
		{

			chess.chessMap[chessPosClean[i].row][chessPosClean[i].col] = 0;

		}


		// 点击后 直接获取位置然后chessDown就行了
		//需要鼠标点击确定 然后把那颗的位置确定

		ChessPos pos4;
		MOUSEMSG msg;
		while (1) {
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN && chess.clickBoard(msg.x, msg.y, &pos4)) {
				cout << "位置合法" << endl;
				break;
			}

		}


		//把棋子清除
		cout << "开始清理贴图" << endl;



		for (size_t i = 0; i < 3; i++)
		{

			chess.giveImg(chessPosClean[i].row, chessPosClean[i].col, 888);

		}


		// 落子
		chess.chessDown(&pos4, CHESS_BLACK);

		//对局正常进行，删除5手n打的数据
		chessbook.clearFiveHandChess();

		chess.count++;
		cout << "chess.count : " << chess.count << endl;


		//开始正常对局


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

// 需要调用的函数B
void functionB(Chess chess, Man man, AI ai) {//后手
	chess.playerFlag = false;
	chess.count = 1;
	man.colour = CHESS_BLACK;
	ai.colour = CHESS_WHITE;
	//储存信息，用于恢复棋时候判断
	bool chooseFlag = true;
	ChessBook chessbook;
	chessbook.saveChessChoose(chooseFlag);


	//对面给出三个点--->用鼠标点击的方式下吧 以及 打点数量-->用一个变量存储起来 



	//我们最先是白方


	//对面给出开局三子位置 我们 跟着下出即可

	cout << "请先下出前三子" << endl;

	ChessPos posThree[3];
	for (size_t i = 0; i < 3; i++)
	{
		MOUSEMSG msg;
		while (1) {
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN && chess.clickBoard(msg.x, msg.y, &posThree[i])) {
				cout << "位置合法" << endl;
				break;
			}

		}

		if (i == 1)
		{
			//第二手白 
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


	//AI 选择是否换手  如果对面是黑棋必胜起手-->  花月 浦月 
	cout << "对方给出的打点数量是?" << endl;
	int FiveHandNum;

	cin >> FiveHandNum;
	cout << "对方给出的第五手打点数是 : " << FiveHandNum << " 已经成功记录 " << endl;
	chessbook.saveFiveHandNum(FiveHandNum);//保存第五手打点数

	cout << "AI正在思考换不换手..." << endl;

	// 1 换 0 不换 
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
	//加入的
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
		//换手
		cout << "我方AI选择换手" << endl;
		//我方变黑 第四手由对方下
		man.colour = CHESS_WHITE;
		ai.colour = CHESS_BLACK;

		//储存信息，用于恢复棋时候判断
		ChessBook chessbook;
		chessbook.saveChessChoose(!chooseFlag);

		man.go(&chess, &chess.lastPos);

		chess.count++;
		cout << "chess.count : " << chess.count << endl;

		//给出打点位置 打点数上面已经记录了

		//下出打点

		vector<ChessPos> chessPosClean;
		ChessPos fiveHandChess[10];//恢复棋的文件中的数据
		for (size_t i = 0; i < FiveHandNum; i++)
		{

			ChessPos chessPos = ai.think(&chess);
			fiveHandChess[i] = chessPos;//copy数据到chessArr
			chessPosClean.push_back(chessPos);

			chess.chessMap[chessPos.row][chessPos.col] = CHESS_CHOOSEBLACK;
			chess.giveImg(chessPos.row, chessPos.col, CHESS_CHOOSEBLACK);
		}

		//储存5手n打，恢复棋的时候用
		chessbook.saveFiveHandChess(fiveHandChess, FiveHandNum);

		//清除数据残余 只剩下贴图


		for (size_t i = 0; i < FiveHandNum; i++)
		{

			chess.chessMap[chessPosClean[i].row][chessPosClean[i].col] = 0;

		}


		// 点击后 直接获取位置然后chessDown就行了
		//需要鼠标点击确定 然后把那颗的位置确定

		ChessPos pos4;
		MOUSEMSG msg;
		while (1) {
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN && chess.clickBoard(msg.x, msg.y, &pos4)) {
				cout << "位置合法" << endl;
				break;
			}

		}


		//把棋子清除
		cout << "开始清理贴图" << endl;



		for (size_t i = 0; i < FiveHandNum; i++)
		{

			chess.giveImg(chessPosClean[i].row, chessPosClean[i].col, 888);

		}


		// 落子
		chess.chessDown(&pos4, CHESS_BLACK);

		//对局正常进行，删除5手n打的数据
		chessbook.clearFiveHandChess();

		chess.count++;
		cout << "chess.count : " << chess.count << endl;


		//正常对局 -->第六手 白 对面 

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
		//不换手
		cout << "我方AI选择不换手,继续对局" << endl;
		//第四手
		chess.count = 3;


		ai.go46(&chess, &chess.lastPos);
		chess.count++;
		cout << "chess.count : " << chess.count << endl;
		//选择对面第五手落子  

		//FiveHandNum
		ChessPos AIThink;

		ChessPos chessArr[10];


		ChessPos nullPos;

		//给chessArr初始化
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
					cout << "位置合法" << endl;
					break;
				}

			}
			chess.giveImg(chessArr[i].row, chessArr[i].col, CHESS_CHOOSEBLACK);

		}

		//储存5手n打，恢复棋的时候用
		chessbook.saveFiveHandChess(chessArr, FiveHandNum);

		// 然后AI给分

		cout << "AI正在挑选位置......  " << endl;
		Sleep(500);
		//AI选择三个点中的一个-->然后给出坐标 然后把他换成实体 然后 把坐标输出给对面 让对面保留

		AIThink = ai.thinkFIVELengChange(&chess, chessArr, FiveHandNum);

		//清理刚刚的贴图
		for (size_t i = 0; i < 3; i++)
		{
			chess.giveImg(chessArr[i].row, chessArr[i].col, 888);
		}

		cout << "我方AI选择的位置是: " << AIThink.row << "行 " << AIThink.col << "列 " << endl;


		chess.chessMap[AIThink.row][AIThink.col] = CHESS_BLACK;
		int fiveChooseRow = AIThink.row;
		int fiveChooseCol = AIThink.col;
		ChessPos fiveChoose(fiveChooseRow, fiveChooseCol);
		chess.chessDown(&fiveChoose, CHESS_BLACK);

		//对局正常进行，删除5手n打的数据
		chessbook.clearFiveHandChess();

		chess.count++;
		cout << "chess.count : " << chess.count << endl;

		//正常对局  我方白 对面 黑 

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

// 需要调用的函数C
void functionC(){
	// 在这里添加函数D的实际代码
}

// 需要调用的函数D
void functionD() {//信息

	// 在这里添加函数D的实际代码
}

// 需要调用的函数E
void functionE(Chess m_chess, Man man, AI ai) {//恢复

	//恢复上一盘棋
	ChessBook chessbook;
	m_chess.playerFlag = chessbook.setChessByMoveHistory(&m_chess);
	bool Flag = chessbook.loadChessChoose();//true为ai白
	if (Flag == true) {
		std::cout << "ai是白子: " << std::endl;
		man.colour = CHESS_BLACK;
		ai.colour = CHESS_WHITE;
	}
	else {
		std::cout << "ai是黑子: " << std::endl;
		man.colour = CHESS_WHITE;
		ai.colour = CHESS_BLACK;
	}

	//判断是否需要5手n打
	int HistoryChessSize = chessbook.getHistoryChessCount(chessbook.loadMoveHistory());
	int FiveHandNum = chessbook.loadFiveHandNum();//读取已经保存的FiveHandNum
	int fiveHandChessSize = chessbook.getHistoryChessCount(chessbook.loadFiveHandChess());//已经下的打点数
	std::string filename = "fiveHandChessData.txt";
	if (HistoryChessSize < 5) {
		if (HistoryChessSize == 0) {
			//没下过棋！
			cout << "没下过棋!无数据，默认先手开始" << endl;
		}
		//下过棋，需要对棋进行处理
		//换手
		if (HistoryChessSize <= 3) {
			//分情况考虑换手
			if (HistoryChessSize == 1) {
				//继续下棋
				//正常下棋
				if (m_chess.playerFlag == true) {
					//std::cout << "轮到人方下棋" << std::endl;
					man.go(&m_chess, &m_chess.lastPos);
					if (m_chess.checkOver()) {
						m_chess.init();
					}
				}
				else
				{
					//std::cout << "轮到ai方下棋" << std::endl;
					ai.go(&m_chess, &m_chess.lastPos);
					if (m_chess.checkOver()) {
						m_chess.init();
					}
				}
				HistoryChessSize++;
			}
			if (HistoryChessSize == 2) {
				//继续下棋
				//正常下棋
				if (m_chess.playerFlag == true) {
					//std::cout << "轮到人方下棋" << std::endl;
					man.go(&m_chess, &m_chess.lastPos);
					if (m_chess.checkOver()) {
						m_chess.init();
					}
					//考虑换手
					cout << "AI正在思考换不换手..." << endl;
					// 1 换 0 不换 
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
						//换手
						cout << "我方AI选择换手" << endl;
						//我方变黑 第四手由对方下
						man.colour = CHESS_WHITE;
						ai.colour = CHESS_BLACK;
						//储存信息，用于恢复棋时候判断
						ChessBook chessbook;
						chessbook.saveChessChoose(!Flag);
						Flag = !Flag;
					}
				}
				else
				{
					//std::cout << "轮到ai方下棋" << std::endl;
					ai.go(&m_chess, &m_chess.lastPos);
					if (m_chess.checkOver()) {
						m_chess.init();
					}
					//考虑换手
					cout << "对方是否选择换手 如果是 请输入1 如果不是请输入0  " << endl;

					int flag;
					while (1) {
						cin >> flag;
						cout << "flag : " << flag << endl;
						if (flag == 1 || flag == 0) {
							break;
						}
						cout << "输入错误，请重新输入！" << endl;
					}
					if (flag == 1) {
						//如果换手
						cout << "---换手---" << endl;
						m_chess.playerFlag = false;
						man.colour = CHESS_BLACK;
						ai.colour = CHESS_WHITE;
						//储存信息，用于恢复棋时候判断
						ChessBook chessbook;
						chessbook.saveChessChoose(!Flag);
						Flag = !Flag;
					}
					cout << "对方给出的打点数量是?" << endl;
					int FiveHandNum;
					cin >> FiveHandNum;
					cout << "对方给出的第五手打点数是 : " << FiveHandNum << " 已经成功记录 " << endl;
					chessbook.saveFiveHandNum(FiveHandNum);//保存第五手打点数
				}
				HistoryChessSize++;
			}
			if (HistoryChessSize == 3) {
				//继续下棋
				//正常下棋
				if (m_chess.playerFlag == true) {
					//std::cout << "轮到人方下棋" << std::endl;
					man.go(&m_chess, &m_chess.lastPos);
					if (m_chess.checkOver()) {
						m_chess.init();
					}
				}
				else
				{
					//std::cout << "轮到ai方下棋" << std::endl;
					ai.go(&m_chess, &m_chess.lastPos);
					if (m_chess.checkOver()) {
						m_chess.init();
					}
				}
				HistoryChessSize++;
			}
		}
		//5手n打
		FiveHandNum = chessbook.loadFiveHandNum();//更新FiveHandNum
		if (HistoryChessSize == 4 && chessbook.isFileEmpty(filename)) {
			//继续下棋
			if (m_chess.playerFlag == true) {
				std::cout << "轮到人方5手" << std::endl;
				//五手n打
				ChessPos AIThink;
				ChessPos chessArr[10];
				ChessPos nullPos;
				//给chessArr初始化
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
							cout << "位置合法" << endl;
							break;
						}
					}
					m_chess.giveImg(chessArr[i].row, chessArr[i].col, CHESS_CHOOSEBLACK);
				}
				//储存5手n打，恢复棋的时候用
				chessbook.saveFiveHandChess(chessArr, FiveHandNum);
				// 然后AI给分
				cout << "AI正在挑选位置......  " << endl;
				Sleep(500);
				//AI选择三个点中的一个-->然后给出坐标 然后把他换成实体 然后 把坐标输出给对面 让对面保留
				AIThink = ai.thinkFIVELengChange(&m_chess, chessArr, FiveHandNum);
				//清理刚刚的贴图
				for (size_t i = 0; i < FiveHandNum; i++)//FiveHandNum \ 3
				{
					m_chess.giveImg(chessArr[i].row, chessArr[i].col, 888);
				}
				cout << "我方AI选择的位置是: " << AIThink.row << "行 " << AIThink.col << "列 " << endl;
				m_chess.chessMap[AIThink.row][AIThink.col] = CHESS_BLACK;
				int fiveChooseRow = AIThink.row;
				int fiveChooseCol = AIThink.col;
				ChessPos fiveChoose(fiveChooseRow, fiveChooseCol);
				m_chess.chessDown(&fiveChoose, CHESS_BLACK);
				//对局正常进行，删除5手n打的数据
				chessbook.clearFiveHandChess();
			}
			else
			{
				std::cout << "轮到ai方5手" << std::endl;
				//开始5手n打
				ChessPos  chessPosClean[10];
				//下出打点
				for (size_t i = 0; i < FiveHandNum; i++)
				{
					ChessPos chessPos = ai.think(&m_chess);
					chessPosClean[i] = chessPos;
					m_chess.chessMap[chessPos.row][chessPos.col] = CHESS_CHOOSEBLACK;
					m_chess.giveImg(chessPos.row, chessPos.col, CHESS_CHOOSEBLACK);
				}
				//储存5手n打，恢复棋的时候用
				chessbook.saveFiveHandChess(chessPosClean, FiveHandNum);//TODO 3应该换成打点数√
				//清除数据残余 只剩下贴图
				for (size_t i = 0; i < FiveHandNum; i++)//TODO 3应该换成打点数√
				{
					m_chess.chessMap[chessPosClean[i].row][chessPosClean[i].col] = 0;
				}
				// 点击后 直接获取位置然后chessDown就行了
				//需要鼠标点击确定 然后把那颗的位置确定
				ChessPos pos4;
				MOUSEMSG msg;
				while (1) {
					msg = GetMouseMsg();
					if (msg.uMsg == WM_LBUTTONDOWN && m_chess.clickBoard(msg.x, msg.y, &pos4)) {
						cout << "位置合法" << endl;
						break;
					}
				}
				//把棋子清除
				cout << "开始清理贴图" << endl;
				for (size_t i = 0; i < FiveHandNum; i++)//TODO 3应该换成打点数√
				{
					m_chess.giveImg(chessPosClean[i].row, chessPosClean[i].col, 888);
				}
				// 落子
				m_chess.chessDown(&pos4, CHESS_BLACK);
				//对局正常进行，删除5手n打的数据
				chessbook.clearFiveHandChess();
			}
		}
		if (HistoryChessSize == 4 && !chessbook.isFileEmpty(filename)) {
			std::vector<ChessPos> chessPosClean = chessbook.loadFiveHandChess();//已经下完的打点的棋子

			//TODO 判断是否已经下完了打点
			// FiveHandNum

			//正在五手n打中的选择棋子
			if (m_chess.playerFlag == true) {
				//std::cout << "ai选5手" << std::endl;
				/*if (Flag == true) {*/
					//ai白，人黑
				ChessPos AIThink;
				ChessPos chessArr[10];
				ChessPos nullPos;
				//给chessArr初始化
				for (size_t i = 0; i < FiveHandNum; i++)
				{
					chessArr[i] = chessPosClean[i];
				}
				for (size_t i = 0; i < FiveHandNum; i++)
				{
					m_chess.giveImg(chessArr[i].row, chessArr[i].col, CHESS_CHOOSEBLACK);
				}
				// 然后AI给分
				cout << "AI正在挑选位置......  " << endl;
				Sleep(500);
				//AI选择三个点中的一个-->然后给出坐标 然后把他换成实体 然后 把坐标输出给对面 让对面保留
				AIThink = ai.thinkFIVELengChange(&m_chess, chessArr, FiveHandNum);
				//清理刚刚的贴图
				for (size_t i = 0; i < 3; i++)
				{
					m_chess.giveImg(chessArr[i].row, chessArr[i].col, 888);
				}
				cout << "我方AI选择的位置是: " << AIThink.row << "行 " << AIThink.col << "列 " << endl;
				m_chess.chessMap[AIThink.row][AIThink.col] = CHESS_BLACK;
				int fiveChooseRow = AIThink.row;
				int fiveChooseCol = AIThink.col;
				ChessPos fiveChoose(fiveChooseRow, fiveChooseCol);
				m_chess.chessDown(&fiveChoose, CHESS_BLACK);
				//m_chess.playerFlag = !m_chess.playerFlag;//恢复数据
				//对局正常进行，删除5手n打的数据
				chessbook.clearFiveHandChess();
				/*}*/
			}
			else {
				//std::cout << "人选5手" << std::endl;
				//if (Flag == true) {
				//	//ai白，人黑

				//}
				//else {
					//ai黑,人白
					//黑子的五手n打
				for (size_t i = 0; i < FiveHandNum; i++)
				{
					ChessPos chessPos = chessPosClean[i];
					m_chess.chessMap[chessPos.row][chessPos.col] = CHESS_CHOOSEBLACK;
					m_chess.giveImg(chessPos.row, chessPos.col, CHESS_CHOOSEBLACK);
				}
				//清除数据残余 只剩下贴图
				for (size_t i = 0; i < FiveHandNum; i++)
				{
					m_chess.chessMap[chessPosClean[i].row][chessPosClean[i].col] = 0;
				}
				// 点击后 直接获取位置然后chessDown就行了
				//需要鼠标点击确定 然后把那颗的位置确定
				ChessPos pos4;
				MOUSEMSG msg;
				while (1) {
					msg = GetMouseMsg();
					if (msg.uMsg == WM_LBUTTONDOWN && m_chess.clickBoard(msg.x, msg.y, &pos4)) {
						cout << "位置合法" << endl;
						break;
					}
				}
				//把棋子清除
				cout << "开始清理贴图" << endl;
				for (size_t i = 0; i < fiveHandChessSize; i++)
				{
					m_chess.giveImg(chessPosClean[i].row, chessPosClean[i].col, 888);
				}
				// 落子
				m_chess.chessDown(&pos4, CHESS_BLACK);
				//对局正常进行，删除5手n打的数据
				chessbook.clearFiveHandChess();
				/*}*/
			}
		}
	}
	else
	{
		cout << "已经过了五手！" << endl;
	}

	//正常下棋
	if (m_chess.playerFlag == true) {
		std::cout << "轮到人方下棋" << std::endl;
	}
	else
	{
		std::cout << "轮到ai方下棋" << std::endl;
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
	//没有历史棋局就默认为true(先手)


}


//开始游戏
void ChessGame::play()
{
	chess->init();
	handleMouseClick(*chess , *man , *ai);


	
}


// 处理鼠标点击事件的函数
void ChessGame::handleMouseClick(Chess chess, Man man, AI ai) {
	int imageX1 = 120, imageX2 = 210, imageX3 = 300, imageX4 = 390, imageX5 = 480; // 图片的左上角X坐标
	int imageY = 0;   // 图片的左上角Y坐标
	int imageWidth = 80;
	int imageHeight = 50;
	// 获取鼠标点击事件
	while (true) {
		MOUSEMSG msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN) { // 只处理左键按下事件，可以根据需求修改
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

//按钮点击按钮事件
// 判断鼠标点击是否在图片区域内的函数
bool ChessGame::isMouseClickWithinImage(int mouseX, int mouseY, int imageX, int imageY, int imageWidth, int imageHeight) {
	return (mouseX >= imageX && mouseX <= imageX + imageWidth && mouseY >= imageY && mouseY <= imageY + imageHeight);
}
