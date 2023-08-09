#include "ChessBook.h"

ChessBook::ChessBook() {

}

std::string ChessBook::getCurrentTimeAsString() {
	// 获取当前时间点
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	// 转换为time_t类型
	std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
	// 将time_t转换为结构体tm，用于获取日期和时间信息
	struct std::tm timeInfo;
	localtime_s(&timeInfo, &currentTime); // 使用localtime_s函数
	// 格式化日期时间
	std::ostringstream oss;
	oss << std::put_time(&timeInfo, "%Y-%m-%d %H:%M");
	std::string formattedTime = "[" + oss.str() + " 仲恺农业工程学院]";
	return formattedTime;
}

// 将数字转换为字母
char ChessBook::convertNumberToChar(int number) {
	char result = 'a' + number -1;
	return result;
}

//将历史信息处理成棋谱需要的信息
std::string ChessBook::formatAdjustment(vector<ChessPos> moveHistory) {
	int flag = 0;
	std::ostringstream oss;
	for (const auto& pos : moveHistory) {
		if (flag == 0) {
			oss << ";B(" << convertNumberToChar(pos.col+1) << "," << 15-pos.row << ")";
			flag++;
		}
		else {
			oss << ";W(" << convertNumberToChar(pos.col+1) << "," << 15-pos.row << ")";
			flag--;
		}
	}
	return oss.str();
}

//将信息拼凑放入文件
void ChessBook::exportRecord(/*const std::string& filename, */bool playerFlag, vector<ChessPos> moveHistory) {
	//TODO 将所有信息拼接在一起
	std::string date = getCurrentTimeAsString();//记录的时候的时间
	std::string results;//判断胜方
	//palyerFlag = flase是man方获胜  
	bool chessChoose = loadChessChoose();//ture为ai白
	while (1) {
		std::cout << "本次比赛是否经过换手,是请输入1，否为0" << endl;
		std::string changeHand;
		std::cin >> changeHand;
		if (changeHand == "1") {
			chessChoose = !chessChoose;
			break;
		}
		else if (changeHand == "0") {
			break;
		}
		else {
			std::cout << "输入错误，请重新输入！" << endl;
		}
	}

	if (playerFlag == false) {
		//man方获胜
		if (chessChoose == false) {
			//man黑子
			results = "先手胜";
		}
		else {
			results = "后手胜";
		}
	}
	else
	{
		//ai获胜
		if (chessChoose == false) {
			//ai为白子
			results = "后手胜";
		}
		else {
			results = "先手胜";
		}
	}
	std::cout << "请输入局数的名字: ";
	std::string gameNum;
	std::cin >> gameNum;
	std::cout << "请输入先手方棋队的名字: ";
	std::string blackTeamName;
	std::cin >> blackTeamName;
	std::cout << "请输入后手方棋队的名字: ";
	std::string whiteTeamName;
	std::cin >> whiteTeamName;
	std::string allChessMoveHistory = formatAdjustment(moveHistory);//处理好了标准的棋子信息
	std::string allData = "{[C5][" + blackTeamName + "][" + whiteTeamName + "][" + results + "]" + date + "[2023 CCGC]" + allChessMoveHistory/*标准的棋子信息*/ + "}";
	std::string filename = gameNum + "C5-" + blackTeamName + "vs" + whiteTeamName + "-" + results + ".txt";
	std::ofstream fout(filename);
	if (fout.is_open()) {
		fout << allData;
		std::cout << "棋谱记录成功!" << std::endl;
	}
	else {
		std::cout << "棋谱记录失败!" << std::endl;
	}
	fout.close();
}

// 将moveHistory实时存储到文件中,还有最后一步棋的下棋方(bug太多，先用下面的方法)
void ChessBook::saveMoveHistory(const std::vector<ChessPos>& moveHistory/*, bool playerFlag*/)
{
	std::string filename = "chessData.txt";//文件名暂时写死了，后期有需要再改
	std::ofstream file(filename);

	if (file.is_open()) {
		//file << playerFlag << std::endl;//最后一步棋的下棋方
		for (const auto& pos : moveHistory) {
			file << pos.row << " " << pos.col << std::endl;
		}
		file.close();
		std::cout << "行棋已保存" << std::endl;
	}
	else {
		std::cout << "行棋保存失败" << std::endl;
	}
}

//清除chessdata的数据
void ChessBook::cleanChessData()
{
	std::ofstream file("chessData.txt", std::ios::trunc); // 打开文件并清空内容
	if (file.is_open()) {
		file.close();
		std::cout << "对局临时数据已经清除" << std::endl;
	}
	else {
		std::cout << "无法清除文件" << std::endl;
	}
}

// 从文件中读取moveHistory
std::vector<ChessPos> ChessBook::loadMoveHistory(/*bool& playerFlag*/)
{
	std::string filename = "chessData.txt";//文件名暂时写死了，后期有需要再改
	std::vector<ChessPos> moveHistory;
	//playerFlag = true;
	std::ifstream file(filename);

	if (file.is_open()) {
		//file >> std::boolalpha >> playerFlag; // 从文件中读取最后一手棋的下棋方
		//std::cout << "playerFlag: " << playerFlag << std::endl; // 打印读取到的 playerFlag
		int row, col;
		while (file >> row >> col) {
			ChessPos pos{ row, col };
			moveHistory.push_back(pos);
		}
		file.close();
		std::cout << "行棋历史已经读取" << std::endl;
		//清除下棋历史文件中的数据，避免影响
		//cleanChessData();
	}
	else {
		std::cout << "行棋历史文件读取失败" << std::endl;
	}

	return moveHistory;
}

//得到movehistory的长度
int ChessBook::getHistoryChessCount(const std::vector<ChessPos>& HistoryChess)
{
	return HistoryChess.size();
}

//将historyMove中的数据摆放棋子
bool ChessBook::setChessByMoveHistory(Chess* m_chess/*std::vector<ChessPos> moveHistory*/)
{
	bool playerFlag = loadChessChoose();
	bool flag = true;
	//读取数据
	std::vector<ChessPos> moveHistory = loadMoveHistory(/*playerFlag*/);
	if (!moveHistory.empty()) {
		std::cout << "正在恢复棋局..." << std::endl;
		//摆放之前的棋子
		for (ChessPos& pos : moveHistory) {
			m_chess->chessRecover(&pos, (flag ? CHESS_BLACK : CHESS_WHITE));
			playerFlag = !playerFlag;
			flag = !flag;
		}
	}
	else
	{
		std::cout << "历史为空！" << std::endl;
	}



	return playerFlag;//返回下棋方
}



// 将一开始选择先后手存储到文件中
void ChessBook::saveChessChoose(bool playerFlag)
{
	std::string filename = "chessChoose.txt";//文件名暂时写死了，后期有需要再改
	std::ofstream file(filename);

	if (file.is_open()) {
		file << std::boolalpha << playerFlag;
		file.close();
		std::cout << "先后手已保存" << std::endl;
	}
	else {
		std::cout << "先后手保存失败" << std::endl;
	}
}
// 从文件中读取先后手
bool ChessBook::loadChessChoose()
{
	std::ifstream inputFile("chessChoose.txt"); // 创建输入文件流对象
	bool playerFlag = true;//文件名暂时写死了，后期有需要再改

	if (inputFile.is_open()) { // 检查文件是否成功打开
		inputFile >> std::boolalpha >> playerFlag; // 从文件中读取 bool 变量
		inputFile.close(); // 关闭文件
		std::cout << "先后手已读取" << std::endl;
	}
	else
	{
		std::cout << "先后手读取失败" << std::endl;
	}
	return playerFlag;
}

//将5手n打的数据写入文件
void ChessBook::saveFiveHandChess(ChessPos chessArr[], int size)
{
	std::ofstream outputFile("fiveHandChessData.txt"); // 创建输出文件流对象
	if (outputFile.is_open()) { // 检查文件是否成功打开
		for (int i = 0; i < size; i++) {
			outputFile << chessArr[i].row << " " << chessArr[i].col << std::endl; // 将每个元素的 row 和 col 写入文件，每行一个数据
		}
		outputFile.close(); // 关闭文件
		std::cout << "储存5手n打的文件数据成功!" << std::endl;
	}
	else {
		std::cout << "储存5手n打的文件数据失败!" << std::endl;
	}
}

//从文件中读取5手n打的信息
std::vector<ChessPos> ChessBook::loadFiveHandChess()
{
	std::ifstream inputFile("fiveHandChessData.txt"); // 创建输入文件流对象
	std::vector<ChessPos> chessData; // 存储读取的棋盘数据

	if (inputFile.is_open()) { // 检查文件是否成功打开
		int row, col;
		while (inputFile >> row >> col) {
			ChessPos pos;
			pos.row = row;
			pos.col = col;
			chessData.push_back(pos); // 将读取的数据添加到向量中
		}
		inputFile.close(); // 关闭文件
		std::cout << "读取5手n打的文件数据成功!" << std::endl;
	}
	else {
		std::cout << "打开5手n打的文件数据失败!" << std::endl;
	}

	return chessData;
}

//清除5手n打文件中的数据
void ChessBook::clearFiveHandChess()
{
	std::ofstream outputFile("fiveHandChessData.txt", std::ios::trunc); // 创建输出文件流对象，并设置打开文件的模式为 trunc

	if (outputFile.is_open()) { // 检查文件是否成功打开
		outputFile.close(); // 关闭文件
		std::cout << "对局正常，清除5手n打的文件数据成功!" << std::endl;
	}
	else {
		std::cout << "清除5手n打的文件数据失败!" << std::endl;
	}
}

//判断文件是否为空
bool ChessBook::isFileEmpty(const std::string& filename)
{
	std::ifstream file(filename);
	return file.peek() == std::ifstream::traits_type::eof();
}

//将FiveHandNum的数据写入文件
void ChessBook::saveFiveHandNum(int num)
{
	std::ofstream file("FiveHandNum.txt");
	if (file.is_open()) {
		file << num;
		file.close();
		std::cout << "FiveHandNum已经保存" << std::endl;
	}
	else {
		std::cout << "FiveHandNum保存失败，无法打开文件FiveHandNum.txt" << std::endl;
	}
}

//读取FiveHandNum的数据
int ChessBook::loadFiveHandNum()
{
	int num = 3;//默认值为3
	std::ifstream file("FiveHandNum.txt");
	if (file.is_open()) {
		file >> num;
		file.close();
		std::cout << "FiveHandNum获取成功" << std::endl;
	}
	else {
		std::cout << "FiveHandNum获取失败，无法打开文件FiveHandNum.txt" << std::endl;
	}
	return num;
}