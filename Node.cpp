#pragma once
#include "Node.h"
#include <iostream>
#include <set>


//���췽��
Node::Node(Node* parent, ChessPos* lastPos, int chessMap[15][15], int count, int coefficient) {
	this->parent = parent;
	this->lastPos.col = lastPos->col;
	this->lastPos.row = lastPos->row;
	this->count = count;
	for (size_t i = 0; i < 15; i++) {
		for (size_t j = 0; j < 15; j++) {
			this->chessMap[i][j] = chessMap[i][j];
		}
	}


	this->coefficient = coefficient;
	//this->chessMap = chessMap;
}


Node::Node() {}

//�жϽڵ��Ƿ���ȫչ��
bool Node::fully_expanded() {
	if (not_visit_pos.empty() && children.empty()) {
		not_visit_pos = get_legal_pos_for_fully_expanded(count, chessMap, lastPos);
	}
	return (not_visit_pos.empty() && !children.empty());
}

//�жϽڵ��Ƿ�Ϊ�ն�Ҷ�ӽڵ㣬���ýڵ��Ӧ�ĸ���Ƿ��ѷֳ�ʤ��
std::string Node::non_terminal(ChessPos pre_pos) {
	std::string game_result = game_over(pre_pos);

	return game_result;
}

//ѡ��һ��δ������Ľڵ�
Node* Node::pick_unvisited() {
	int random_index = rand() % not_visit_pos.size();
	ChessPos move_pos = not_visit_pos[random_index];
	chessMap[move_pos.row][move_pos.col] = -1 * chessMap[lastPos.row][lastPos.col];
	not_visit_pos.erase(not_visit_pos.begin() + random_index);
	Node* new_node = new Node(this, &move_pos, chessMap, 1 + this->count, -1 * this->coefficient);
	chessMap[move_pos.row][move_pos.col] = 0;
	children.push_back(new_node);

	return new_node;
}




ChessPos Node::calculateScore(int chessMap[15][15], ChessPos lastPos)
{
	int personNum = 0; //���ַ������壩���ٸ�����������
	int aiNum = 0; //AI�������壩�����ж��ٸ�����������
	int emptyNum = 0; // �÷����Ͽհ�λ�ĸ���
	int scoreMap[15][15];//��������
	int curcolour = -1 * scoreMap[lastPos.row][lastPos.col];//���ڵ�������ɫ
	vector<ChessPos> maxPoints;//�������
	int bestScore = 0;//��߷���
	// ����������������
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			scoreMap[i][j] = 0;
		}
	}

	//��ÿ������м���
	int size = 15;
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			if (chessMap[row][col]) continue;

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
							chessMap[curRow][curCol] == -1 * curcolour) {
							personNum++;
						}
						else if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							chessMap[curRow][curCol] == 0) {
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
							chessMap[curRow][curCol] == -1 * curcolour) {
							personNum++;
						}
						else if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							chessMap[curRow][curCol] == 0) {
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
							chessMap[curRow][curCol] == curcolour) {
							aiNum++;
						}
						else if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							chessMap[curRow][curCol] == 0) {
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
							chessMap[curRow][curCol] == curcolour) {
							aiNum++;
						}
						else if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							chessMap[curRow][curCol] == 0) {
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

	//ѡһ��

	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			// ǰ������������ǿյ�
			if (chessMap[row][col] == 0) {
				ChessPos pos(row, col);

				//TODO xu ��ʱע��
				Chess* chess = new Chess(chessMap);
				if (curcolour == CHESS_BLACK && chess->checkForbidden(pos)) {
					// AI�Ǻ��岢��λ��Υ�����ֹ���������λ��
					continue;
				}

				if (scoreMap[row][col] > bestScore) {
					bestScore = scoreMap[row][col];
					maxPoints.clear();
					maxPoints.push_back(pos);
				}
				else if (scoreMap[row][col] == bestScore) {
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


//���ѡ��һ���ӽڵ�
Node* Node::pick_random() {

	std::vector<ChessPos> possible_moves = get_legal_pos(count , chessMap , lastPos);
	int random_index = rand() % possible_moves.size();
	ChessPos move_pos = possible_moves[random_index];

	//ChessPos move_pos = calculateScore(chessMap, lastPos);

	chessMap[move_pos.row][move_pos.col] = -1 * chessMap[lastPos.row][lastPos.col];
	Node* new_node = new Node(this, &move_pos, chessMap, 1 + this->count, -1 * this->coefficient);
	this->children2.push_back(new_node);
	chessMap[move_pos.row][move_pos.col] = 0;
	return new_node;
}



//UCT�ȽϹ��̣�ѡ��ǰ���������ŵĺ��ӽڵ�
Node* Node::best_uct(double c_param) {
	std::vector<double> uct_of_children;
	for (Node* child : children) {
		double uct;
		if (child->num_visit == 0)
		{
			uct = c_param * std::sqrt(std::log(num_visit) / child->num_visit);
		}
		else {
			uct = (static_cast<double>(child->num_of_wins) / child->num_visit)
				+ c_param * std::sqrt(std::log(num_visit) / child->num_visit);
		}
		uct_of_children.push_back(uct);
	}
	auto best_index = std::distance(uct_of_children.begin(), std::max_element(uct_of_children.begin(), uct_of_children.end()));
	return children[best_index];
}

//������㣬��������ȫչ���Ľ�㣬����best_uctѡ����ѵĺ��ӽڵ�������±������˺���ʵ��ѡ��+��չ����
Node* Node::traverse(Node* node) {
	while (node->fully_expanded()) {
		node = node->best_uct(2.50);
	}
	//���ѡ��һ��δ���ʵġ�δ������Ϸ���ӽڵ�
	if (node->non_terminal(node->lastPos) == "continue" && node->not_visit_pos.size() != 0) {

		node = node->pick_unvisited();
	}
	return node;
}

//������̣���������pick_random()���ѡ���㣬ֱ������ʤ��������ʤ��ֵ��
int Node::rollout(Node* node) {
	while (true) {
		std::string game_result = node->non_terminal(node->lastPos);
		if (game_result == "continue") {
			node = node->pick_random();
		}
		else {
			break;
		}
	}

	if (node->non_terminal(node->lastPos) == "win" && node->coefficient == 1) {
		return -1;
	}
	else if (node->non_terminal(node->lastPos) == "win") {
		return 1;
	}
	else {
		return 0;
	}
}

//���򴫲����̣�����rollout()�Ľ�����ϸ��½ڵ����ݡ�
void Node::backpropagate(Node* node, int result) {
	node->num_visit += 1;
	node->num_of_wins += result;
	if (node->parent) {
		backpropagate(node->parent, result);
	}
}

//����ÿ���ڵ�Ĳ�����ʤ������/��Ϸ���������ж���һ������������󣬲�����������ӡ�  
//Node* Node::best_child() {
//
//	cout << "000" << "\n";
//	if (children.empty()) {
//		return nullptr;
//	}
//
//	cout << "111" << "\n";
//	std::vector<int> visit_num_of_children;
//	for (Node* child : children) {
//		visit_num_of_children.push_back(child->num_visit);
//	}
//
//	cout << "222" << "\n";
//
//	using distance_type = typename std::iterator_traits<std::vector<int>::iterator>::difference_type;
//	auto best_index = static_cast<distance_type>(std::distance(visit_num_of_children.begin(),
//		std::max_element(visit_num_of_children.begin(), visit_num_of_children.end())));
//
//	cout << "333" << "\n";
//
//	if (best_index < 0 || best_index >= children.size()) {
//		return nullptr;
//	}
//
//	return children[best_index];
//}
std::vector<Node*> Node::best_children(int num_children) {
	std::vector<Node*> best_children;

	if (children.empty()) {
		return best_children;
	}

	std::vector<int> visit_num_of_children;
	for (Node* child : children) {
		visit_num_of_children.push_back(child->num_visit);
	}

	using distance_type = typename std::iterator_traits<std::vector<int>::iterator>::difference_type;
	std::set<Node*> marked_nodes;

	for (int i = 0; i < num_children; ++i) {
		auto best_index = static_cast<distance_type>(std::distance(visit_num_of_children.begin(),
			std::max_element(visit_num_of_children.begin(), visit_num_of_children.end())));

		if (best_index >= 0 && best_index < visit_num_of_children.size()) {
			Node* best_child = children[best_index];
			if (marked_nodes.find(best_child) == marked_nodes.end()) {
				best_children.push_back(best_child);
				marked_nodes.insert(best_child);
			}
			visit_num_of_children[best_index] = std::numeric_limits<int>::lowest(); // Reset visit count to avoid reselection
		}
		else {
			break;
		}
	}

	return best_children;
}


//���δѡ������ӣ�fully_expanded()�е��ã�
vector<ChessPos> Node::get_legal_pos(int count , int chessmap[15][15] , ChessPos lastPos)	 {
	vector<ChessPos> legal_pos;
	if (count <= 35) {
		int x = lastPos.col;
		int y = lastPos.row;
	//	cout << "count : " << count << endl;
		for (int row = (0 < (y - 5) ? (y - 5) : 0); row <= (14 < (y + 5) ? 14 : (y + 5)); row++) {
	//		cout << "\n row : "<< row;
			for (int col = (0 < (x - 5) ? (x - 5) : 0); col <= (14 < (x + 5) ? 14 : (x + 5)); col++) {
				if (chessMap[row][col] == 0) {
					legal_pos.push_back(ChessPos(row, col));
				}
	//			cout << chessMap[row][col];
			}
		}
	}
	else if (count <= 101)
	{
		int bottom = -1;
		int top = 15;
		int left = 15;
		int right = -1;

		for (int i = 0; i < 15; ++i) {
			for (int j = 0; j < 15; ++j) {
				if (chessmap[i][j] != 0) {
					// �ҵ����ӵ�λ�ã����±߽�
					if (i < top)
						top = i;
					if (i > bottom)
						bottom = i;
					if (j < left)
						left = j;
					if (j > right)
						right = j;
				}
			}
		}
		for (int row = (0 < (top - 2) ? (top - 2) : 0); row <= (14 < (bottom + 2) ? 15 : (bottom + 2)); row++) {
			for (int col = (0 < (left - 2) ? (left - 2) : 0); col <= (14 < (right + 2) ? 15 : (right + 2)); col++) {
				if (chessMap[row][col] == 0) {
					legal_pos.push_back(ChessPos(row, col));
				}
			}
		}
		/*�ο�����
		for (int row = top; row < bottom; row++) {
			for (int col = left; col < right; col++) {
				if (chessMap[row][col] == 0) {
					legal_pos.push_back(ChessPos(row, col));
				}
			}
		}*/
	}else {
		for (int row = 0; row < 15; row++) {
			for (int col = 0; col < 15; col++) {
				if (chessMap[row][col] == 0) {
					legal_pos.push_back(ChessPos(row, col));
				}
			}
		}
	}

	return legal_pos;
}

//ɸѡ����
vector<ChessPos> get_valuable_positions(const vector<ChessPos>& legal_pos, int chessmap[15][15]) {
	vector<ChessPos> valuable_pos;

	for (const auto& pos : legal_pos) {
		int x = pos.col;
		int y = pos.row;
		bool has_value = false;

		for (int row = (0 < (y - 2) ? (y - 2) : 0); row <= (14 < (y + 2) ? 14 : (y + 2)); row++) {
			for (int col = (0 < (x - 2) ? (x - 2) : 0); col <= (14 < (x + 2) ? 14 : (x + 2)); col++) {
				if (chessmap[row][col] != 0) {
					has_value = true;
					break;
				}
			}
			if (!has_value) {
				break;
			}
		}

		if (has_value) {
			valuable_pos.push_back(pos);
		}
	}
	if (valuable_pos.empty()) {
		// valuable_pos Ϊ�գ�����ԭ����λ��
		return legal_pos;
	}
	else {
		// valuable_pos ��Ϊ�գ��ҵ����м�ֵ��λ��
		return valuable_pos;
	}
	//return valuable_pos;
}

//fully_expanded()�е��õ����δѡ�������
vector<ChessPos> Node::get_legal_pos_for_fully_expanded(int count, int chessmap[15][15], ChessPos lastPos) {
	vector<ChessPos> legal_pos;
	//if (count <= 35) {
	//	int x = lastPos.col;
	//	int y = lastPos.row;
	//	//	cout << "count : " << count << endl;
	//	for (int row = (0 < (y - 5) ? (y - 5) : 0); row <= (14 < (y + 5) ? 14 : (y + 5)); row++) {
	//		//		cout << "\n row : "<< row;
	//		for (int col = (0 < (x - 5) ? (x - 5) : 0); col <= (14 < (x + 5) ? 14 : (x + 5)); col++) {
	//			if (chessMap[row][col] == 0) {
	//				legal_pos.push_back(ChessPos(row, col));
	//			}
	//			//			cout << chessMap[row][col];
	//		}
	//	}
	//}
	//else if (count <= 101)
	//{
	//	int bottom = -1;
	//	int top = 15;
	//	int left = 15;
	//	int right = -1;

	//	for (int i = 0; i < 15; ++i) {
	//		for (int j = 0; j < 15; ++j) {
	//			if (chessmap[i][j] != 0) {
	//				// �ҵ����ӵ�λ�ã����±߽�
	//				if (i < top)
	//					top = i;
	//				if (i > bottom)
	//					bottom = i;
	//				if (j < left)
	//					left = j;
	//				if (j > right)
	//					right = j;
	//			}
	//		}
	//	}
	//	for (int row = (0 < (top - 2) ? (top - 2) : 0); row <= (14 < (bottom + 2) ? 15 : (bottom + 2)); row++) {
	//		for (int col = (0 < (left - 2) ? (left - 2) : 0); col <= (14 < (right + 2) ? 15 : (right + 2)); col++) {
	//			if (chessMap[row][col] == 0) {
	//				legal_pos.push_back(ChessPos(row, col));
	//			}
	//		}
	//	}
	//	/*�ο�����
	//	for (int row = top; row < bottom; row++) {
	//		for (int col = left; col < right; col++) {
	//			if (chessMap[row][col] == 0) {
	//				legal_pos.push_back(ChessPos(row, col));
	//			}
	//		}
	//	}*/
	//}
	//else {
		for (int row = 0; row < 15; row++) {
			for (int col = 0; col < 15; col++) {
				if (chessMap[row][col] == 0) {
					legal_pos.push_back(ChessPos(row, col));
				}
			}
		}
	//}
	return get_valuable_positions(legal_pos, chessMap);
	//return legal_pos;
}

// ��Ϸ������麯��
string Node::game_over(ChessPos pre_pos) {

	int cur_colour = chessMap[pre_pos.row][pre_pos.col];
	int dx[] = { 1, 0, 1, 1 };
	int dy[] = { 0, 1, 1, -1 };

	for (int i = 0; i < 4; i++) {
		int count = 1;
		for (int j = 1; j <= 4; j++) {
			int x = pre_pos.row + dx[i] * j;
			int y = pre_pos.col + dy[i] * j;
			if (x < 0 || x >= 15 || y < 0 || y >= 15) break;
			if (chessMap[x][y] == cur_colour) count++;
			else break;
		}
		for (int j = 1; j <= 4; j++) {
			int x = pre_pos.row - dx[i] * j;
			int y = pre_pos.col - dy[i] * j;
			if (x < 0 || x >= 15 || y < 0 || y >= 15) break;
			if (chessMap[x][y] == cur_colour) count++;
			else break;
		}
		if (count >= 5) {
			return "win";
		}
	}

	// ��������Ƿ�����
	bool is_full = true;
	for (int row = 0; row < 15; row++) {
		for (int col = 0; col < 15; col++) {
			if (chessMap[row][col] == 0) {
				is_full = false;
				break;
			}
		}
		if (!is_full) break;
	}

	if (is_full) {
		return "draw";
	}

	return "continue";
}

//ɾ����Ϸ��
void Node::delete_subtree(Node* node) {
	if (node == nullptr) {
		return;
	}

	for (Node* child : node->children) {
		delete_subtree(child);
	}
	for (Node* child : node->children2) {
		delete_subtree(child);
	}

	delete node;
}



















































































//ChessPos Node::monte_carlo_tree_search(int chessMap[15][15], ChessPos * pre_pos, int count, int mcts_times) {
//	// ����MCTS���ĸ��ڵ�
//	Node* root = new Node(NULL, pre_pos, chessMap, count, 1);
//
//	// �������������߳�����ThreadPool����������Ϊ3���߳�
//	ThreadPool threadPool(3);
//
//	// ʹ��ThreadPool����ִ��MCTS����
//	std::vector<std::future<int>> results;
//	for (int i = 0; i < mcts_times; ++i) {
//		results.emplace_back(threadPool.enqueue([&] {
//			Node* leaf = traverse(root);  // ��������ѡ��һ���ڵ������չ
//			int simulation_result = rollout(leaf);  // ������̣��õ���Ϸ���
//			backpropagate(leaf, simulation_result);  // ���򴫲������½ڵ��ͳ����Ϣ
//			return simulation_result;
//			}));
//	}
//
//	// �ȴ������������
//	int j = 0;
//	for (auto& result : results) {
//		cout << "���ڼ����У�" << j++ << endl;
//		result.wait();
//	}
//
//	// ����MCTS�����Ľ���ҵ�����߷�
//	int best_simulation_result = -1; // ��ʼ��ΪĬ��ֵ
//	Node* best_child_node = nullptr;
//	for (auto& result : results) {
//		int simulation_result = result.get();
//		if (simulation_result > best_simulation_result) {
//			best_simulation_result = simulation_result;
//			best_child_node = root->best_child();  // ��ȡ����ӽڵ�
//		}
//	}
//
//	// ������ӽڵ���ȡ����߷�
//	ChessPos best_move;
//	if (best_child_node != nullptr) {
//		best_move.col = best_child_node->lastPos.col;
//		best_move.row = best_child_node->lastPos.row;
//	}
//	else {
//		// �����Ҳ�������ӽڵ�����
//		cout << "û���ҵ�����ӽڵ�" << endl;
//	}
//
//	// �����ڴ沢��������߷�
//	delete_subtree(root);
//	return best_move;
//}










//ChessPos Node::monte_carlo_tree_search(int chessMap[15][15], ChessPos* pre_pos,int count,  int mcts_times) {
//
//	Node* root = new Node(NULL, pre_pos,  chessMap, count, 1);
//	for (int i = 0; i < mcts_times; ++i) {
//
//		Node* leaf = traverse(root);
//
//		int simulation_result = rollout(leaf);
//		//cout <<non_terminal(lastPos) << "8888888888888\n";
//		backpropagate(leaf, simulation_result);
//
//
//		cout << i << "\n";
//	}
//
//	cout << "ѭ����" << "\n";
//	Node* best_child_node = root->best_child();
//
//	cout << "ѡ��õ�" << "\n";
//	ChessPos best_move;
//
//	best_move.col = best_child_node->lastPos.col;
//	best_move.row = best_child_node->lastPos.row;
//
//
//	cout << "�õ�λ��" << "\n";
//	delete_subtree(root);
//	root = nullptr;
//	return best_move;
//}



//std::vector<ChessPos> Node::monte_carlo_tree_search(int chessMap[15][15], ChessPos* pre_pos, int count, int mcts_times) {
//	Node* root = new Node(NULL, pre_pos, chessMap, count, 1);
//
//	for (int i = 0; i < mcts_times; ++i) {
//		Node* leaf = traverse(root);
//		int simulation_result = rollout(leaf);
//		backpropagate(leaf, simulation_result);
//		cout << i << "\n";
//	}
//
//	cout << "ѭ����" << "\n";
//
//	std::vector<ChessPos> best_moves;
//	std::vector<Node*> best_child_nodes = root->best_children(5);  // ����5������ӽڵ�
//
//	cout << "ѡ��õ�" << "\n";
//
//	for (Node* best_child_node : best_child_nodes) {
//		ChessPos best_move;
//		best_move.col = best_child_node->lastPos.col;
//		best_move.row = best_child_node->lastPos.row;
//		best_moves.push_back(best_move);
//	}
//
//	cout << "�õ�λ��" << "\n";
//	delete_subtree(root);
//	root = nullptr;
//	return best_moves;
//}
std::vector<ChessPos> Node::monte_carlo_tree_search(int chessMap[15][15], ChessPos* pre_pos, int count, int mcts_times) {
	Node* root = new Node(NULL, pre_pos, chessMap, count, 1);
	

	//bool processed[15][15] = { false };

	//int count1 = 0;

	//for (int x = 0; x < 15; x++) {
	//	for (int y = 0; y < 15; y++) {
	//		if (!processed[x][y]) {
	//			processed[x][y] = false;
	//			for (int row = (0 < (y - 2) ? (y - 2) : 0); row <= (14 < (y + 2) ? 14 : (y + 2)); row++) {
	//				for (int col = (0 < (x - 2) ? (x - 2) : 0); col <= (14 < (x + 2) ? 14 : (x + 2)); col++) {
	//					if (chessMap[row][col]!= 0) {

	//						processed[x][y] = true;
	//					}
	//				}
	//			}
	//			if (processed[x][y] = false) {

	//				count1++;
	//			}
	//		}
	//	}
	//}



	int processed[15][15] = { 0 };

	int count1 = 0;

	for (int x = 0; x < 15; x++) {
		for (int y = 0; y < 15; y++) {
			if (processed[x][y] == 0) {

				for (int row = (0 < (y - 2) ? (y - 2) : 0); row <= (14 < (y + 2) ? 14 : (y + 2)); row++) {
					for (int col = (0 < (x - 2) ? (x - 2) : 0); col <= (14 < (x + 2) ? 14 : (x + 2)); col++) {
						if (chessMap[row][col] != 0) {

							processed[x][y] = 1;

						}
					}
				}
				if (processed[x][y] == 1) {

					count1++;
				}
			}
		}
	}




	//����Ҫ���㷨


	cout << "=== ѭ��ǰ=== count1:  " << count1 << endl;
	int key = 300;
	int loop = count1 * key;

	for (int i = 0; i < 80000; ++i) {
		Node* leaf = traverse(root);
		int simulation_result = rollout(leaf);
		backpropagate(leaf, simulation_result);
	}

	cout << "=== ��������=== LOOP :  " << loop << endl;


	std::vector<ChessPos> best_moves;
	std::vector<Node*> best_child_nodes = root->best_children(5);  // ����5������ӽڵ�


	for (Node* best_child_node : best_child_nodes) {
		ChessPos best_move;
		best_move.col = best_child_node->lastPos.col;
		best_move.row = best_child_node->lastPos.row;
		best_moves.push_back(best_move);
	}

	delete_subtree(root);
	root = nullptr;
	return best_moves;
}