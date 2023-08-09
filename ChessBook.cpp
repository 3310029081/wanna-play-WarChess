#include "ChessBook.h"

ChessBook::ChessBook() {

}

std::string ChessBook::getCurrentTimeAsString() {
	// ��ȡ��ǰʱ���
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	// ת��Ϊtime_t����
	std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
	// ��time_tת��Ϊ�ṹ��tm�����ڻ�ȡ���ں�ʱ����Ϣ
	struct std::tm timeInfo;
	localtime_s(&timeInfo, &currentTime); // ʹ��localtime_s����
	// ��ʽ������ʱ��
	std::ostringstream oss;
	oss << std::put_time(&timeInfo, "%Y-%m-%d %H:%M");
	std::string formattedTime = "[" + oss.str() + " ����ũҵ����ѧԺ]";
	return formattedTime;
}

// ������ת��Ϊ��ĸ
char ChessBook::convertNumberToChar(int number) {
	char result = 'a' + number -1;
	return result;
}

//����ʷ��Ϣ�����������Ҫ����Ϣ
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

//����Ϣƴ�շ����ļ�
void ChessBook::exportRecord(/*const std::string& filename, */bool playerFlag, vector<ChessPos> moveHistory) {
	//TODO ��������Ϣƴ����һ��
	std::string date = getCurrentTimeAsString();//��¼��ʱ���ʱ��
	std::string results;//�ж�ʤ��
	//palyerFlag = flase��man����ʤ  
	bool chessChoose = loadChessChoose();//tureΪai��
	while (1) {
		std::cout << "���α����Ƿ񾭹�����,��������1����Ϊ0" << endl;
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
			std::cout << "����������������룡" << endl;
		}
	}

	if (playerFlag == false) {
		//man����ʤ
		if (chessChoose == false) {
			//man����
			results = "����ʤ";
		}
		else {
			results = "����ʤ";
		}
	}
	else
	{
		//ai��ʤ
		if (chessChoose == false) {
			//aiΪ����
			results = "����ʤ";
		}
		else {
			results = "����ʤ";
		}
	}
	std::cout << "���������������: ";
	std::string gameNum;
	std::cin >> gameNum;
	std::cout << "���������ַ���ӵ�����: ";
	std::string blackTeamName;
	std::cin >> blackTeamName;
	std::cout << "��������ַ���ӵ�����: ";
	std::string whiteTeamName;
	std::cin >> whiteTeamName;
	std::string allChessMoveHistory = formatAdjustment(moveHistory);//������˱�׼��������Ϣ
	std::string allData = "{[C5][" + blackTeamName + "][" + whiteTeamName + "][" + results + "]" + date + "[2023 CCGC]" + allChessMoveHistory/*��׼��������Ϣ*/ + "}";
	std::string filename = gameNum + "C5-" + blackTeamName + "vs" + whiteTeamName + "-" + results + ".txt";
	std::ofstream fout(filename);
	if (fout.is_open()) {
		fout << allData;
		std::cout << "���׼�¼�ɹ�!" << std::endl;
	}
	else {
		std::cout << "���׼�¼ʧ��!" << std::endl;
	}
	fout.close();
}

// ��moveHistoryʵʱ�洢���ļ���,�������һ��������巽(bug̫�࣬��������ķ���)
void ChessBook::saveMoveHistory(const std::vector<ChessPos>& moveHistory/*, bool playerFlag*/)
{
	std::string filename = "chessData.txt";//�ļ�����ʱд���ˣ���������Ҫ�ٸ�
	std::ofstream file(filename);

	if (file.is_open()) {
		//file << playerFlag << std::endl;//���һ��������巽
		for (const auto& pos : moveHistory) {
			file << pos.row << " " << pos.col << std::endl;
		}
		file.close();
		std::cout << "�����ѱ���" << std::endl;
	}
	else {
		std::cout << "���屣��ʧ��" << std::endl;
	}
}

//���chessdata������
void ChessBook::cleanChessData()
{
	std::ofstream file("chessData.txt", std::ios::trunc); // ���ļ����������
	if (file.is_open()) {
		file.close();
		std::cout << "�Ծ���ʱ�����Ѿ����" << std::endl;
	}
	else {
		std::cout << "�޷�����ļ�" << std::endl;
	}
}

// ���ļ��ж�ȡmoveHistory
std::vector<ChessPos> ChessBook::loadMoveHistory(/*bool& playerFlag*/)
{
	std::string filename = "chessData.txt";//�ļ�����ʱд���ˣ���������Ҫ�ٸ�
	std::vector<ChessPos> moveHistory;
	//playerFlag = true;
	std::ifstream file(filename);

	if (file.is_open()) {
		//file >> std::boolalpha >> playerFlag; // ���ļ��ж�ȡ���һ��������巽
		//std::cout << "playerFlag: " << playerFlag << std::endl; // ��ӡ��ȡ���� playerFlag
		int row, col;
		while (file >> row >> col) {
			ChessPos pos{ row, col };
			moveHistory.push_back(pos);
		}
		file.close();
		std::cout << "������ʷ�Ѿ���ȡ" << std::endl;
		//���������ʷ�ļ��е����ݣ�����Ӱ��
		//cleanChessData();
	}
	else {
		std::cout << "������ʷ�ļ���ȡʧ��" << std::endl;
	}

	return moveHistory;
}

//�õ�movehistory�ĳ���
int ChessBook::getHistoryChessCount(const std::vector<ChessPos>& HistoryChess)
{
	return HistoryChess.size();
}

//��historyMove�е����ݰڷ�����
bool ChessBook::setChessByMoveHistory(Chess* m_chess/*std::vector<ChessPos> moveHistory*/)
{
	bool playerFlag = loadChessChoose();
	bool flag = true;
	//��ȡ����
	std::vector<ChessPos> moveHistory = loadMoveHistory(/*playerFlag*/);
	if (!moveHistory.empty()) {
		std::cout << "���ڻָ����..." << std::endl;
		//�ڷ�֮ǰ������
		for (ChessPos& pos : moveHistory) {
			m_chess->chessRecover(&pos, (flag ? CHESS_BLACK : CHESS_WHITE));
			playerFlag = !playerFlag;
			flag = !flag;
		}
	}
	else
	{
		std::cout << "��ʷΪ�գ�" << std::endl;
	}



	return playerFlag;//�������巽
}



// ��һ��ʼѡ���Ⱥ��ִ洢���ļ���
void ChessBook::saveChessChoose(bool playerFlag)
{
	std::string filename = "chessChoose.txt";//�ļ�����ʱд���ˣ���������Ҫ�ٸ�
	std::ofstream file(filename);

	if (file.is_open()) {
		file << std::boolalpha << playerFlag;
		file.close();
		std::cout << "�Ⱥ����ѱ���" << std::endl;
	}
	else {
		std::cout << "�Ⱥ��ֱ���ʧ��" << std::endl;
	}
}
// ���ļ��ж�ȡ�Ⱥ���
bool ChessBook::loadChessChoose()
{
	std::ifstream inputFile("chessChoose.txt"); // ���������ļ�������
	bool playerFlag = true;//�ļ�����ʱд���ˣ���������Ҫ�ٸ�

	if (inputFile.is_open()) { // ����ļ��Ƿ�ɹ���
		inputFile >> std::boolalpha >> playerFlag; // ���ļ��ж�ȡ bool ����
		inputFile.close(); // �ر��ļ�
		std::cout << "�Ⱥ����Ѷ�ȡ" << std::endl;
	}
	else
	{
		std::cout << "�Ⱥ��ֶ�ȡʧ��" << std::endl;
	}
	return playerFlag;
}

//��5��n�������д���ļ�
void ChessBook::saveFiveHandChess(ChessPos chessArr[], int size)
{
	std::ofstream outputFile("fiveHandChessData.txt"); // ��������ļ�������
	if (outputFile.is_open()) { // ����ļ��Ƿ�ɹ���
		for (int i = 0; i < size; i++) {
			outputFile << chessArr[i].row << " " << chessArr[i].col << std::endl; // ��ÿ��Ԫ�ص� row �� col д���ļ���ÿ��һ������
		}
		outputFile.close(); // �ر��ļ�
		std::cout << "����5��n����ļ����ݳɹ�!" << std::endl;
	}
	else {
		std::cout << "����5��n����ļ�����ʧ��!" << std::endl;
	}
}

//���ļ��ж�ȡ5��n�����Ϣ
std::vector<ChessPos> ChessBook::loadFiveHandChess()
{
	std::ifstream inputFile("fiveHandChessData.txt"); // ���������ļ�������
	std::vector<ChessPos> chessData; // �洢��ȡ����������

	if (inputFile.is_open()) { // ����ļ��Ƿ�ɹ���
		int row, col;
		while (inputFile >> row >> col) {
			ChessPos pos;
			pos.row = row;
			pos.col = col;
			chessData.push_back(pos); // ����ȡ��������ӵ�������
		}
		inputFile.close(); // �ر��ļ�
		std::cout << "��ȡ5��n����ļ����ݳɹ�!" << std::endl;
	}
	else {
		std::cout << "��5��n����ļ�����ʧ��!" << std::endl;
	}

	return chessData;
}

//���5��n���ļ��е�����
void ChessBook::clearFiveHandChess()
{
	std::ofstream outputFile("fiveHandChessData.txt", std::ios::trunc); // ��������ļ������󣬲����ô��ļ���ģʽΪ trunc

	if (outputFile.is_open()) { // ����ļ��Ƿ�ɹ���
		outputFile.close(); // �ر��ļ�
		std::cout << "�Ծ����������5��n����ļ����ݳɹ�!" << std::endl;
	}
	else {
		std::cout << "���5��n����ļ�����ʧ��!" << std::endl;
	}
}

//�ж��ļ��Ƿ�Ϊ��
bool ChessBook::isFileEmpty(const std::string& filename)
{
	std::ifstream file(filename);
	return file.peek() == std::ifstream::traits_type::eof();
}

//��FiveHandNum������д���ļ�
void ChessBook::saveFiveHandNum(int num)
{
	std::ofstream file("FiveHandNum.txt");
	if (file.is_open()) {
		file << num;
		file.close();
		std::cout << "FiveHandNum�Ѿ�����" << std::endl;
	}
	else {
		std::cout << "FiveHandNum����ʧ�ܣ��޷����ļ�FiveHandNum.txt" << std::endl;
	}
}

//��ȡFiveHandNum������
int ChessBook::loadFiveHandNum()
{
	int num = 3;//Ĭ��ֵΪ3
	std::ifstream file("FiveHandNum.txt");
	if (file.is_open()) {
		file >> num;
		file.close();
		std::cout << "FiveHandNum��ȡ�ɹ�" << std::endl;
	}
	else {
		std::cout << "FiveHandNum��ȡʧ�ܣ��޷����ļ�FiveHandNum.txt" << std::endl;
	}
	return num;
}