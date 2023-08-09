#pragma once
#include "Node.h"
#include <iostream>
#include <set>


//构造方法
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

//判断节点是否完全展开
bool Node::fully_expanded() {
	if (not_visit_pos.empty() && children.empty()) {
		not_visit_pos = get_legal_pos_for_fully_expanded(count, chessMap, lastPos);
	}
	return (not_visit_pos.empty() && !children.empty());
}

//判断节点是否为终端叶子节点，即该节点对应的格局是否已分出胜负
std::string Node::non_terminal(ChessPos pre_pos) {
	std::string game_result = game_over(pre_pos);

	return game_result;
}

//选择一个未浏览过的节点
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
	int personNum = 0; //棋手方（黑棋）多少个连续的棋子
	int aiNum = 0; //AI方（白棋）连续有多少个连续的棋子
	int emptyNum = 0; // 该方向上空白位的个数
	int scoreMap[15][15];//分数集合
	int curcolour = -1 * scoreMap[lastPos.row][lastPos.col];//现在的棋子颜色
	vector<ChessPos> maxPoints;//最好落子
	int bestScore = 0;//最高分数
	// 评分向量数组清零
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			scoreMap[i][j] = 0;
		}
	}

	//对每个点进行计算
	int size = 15;
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			if (chessMap[row][col]) continue;

			for (int y = -1; y <= 0; y++) {        //Y的范围还是-1， 0
				for (int x = -1; x <= 1; x++) {    //X的范围是 -1,0,1
					if (y == 0 && x == 0) continue;
					if (y == 0 && x != 1) continue; //当y=0时，仅允许x=1
					//防止活3计算2次

					personNum = 0;
					aiNum = 0;
					emptyNum = 0;

					// 假设对手在该位置落子，会构成什么棋型
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

					// 反向继续计算
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

	//选一个

	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			// 前提是这个坐标是空的
			if (chessMap[row][col] == 0) {
				ChessPos pos(row, col);

				//TODO xu 暂时注释
				Chess* chess = new Chess(chessMap);
				if (curcolour == CHESS_BLACK && chess->checkForbidden(pos)) {
					// AI是黑棋并且位置违反禁手规则，跳过该位置
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


//随机选择一个子节点
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



//UCT比较过程，选择当前策略下最优的孩子节点
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

//遍历结点，遇到已完全展开的结点，利用best_uct选择最佳的孩子节点继续向下遍历。此函数实现选择+拓展过程
Node* Node::traverse(Node* node) {
	while (node->fully_expanded()) {
		node = node->best_uct(2.50);
	}
	//随机选择一个未访问的、未结束游戏的子节点
	if (node->non_terminal(node->lastPos) == "continue" && node->not_visit_pos.size() != 0) {

		node = node->pick_unvisited();
	}
	return node;
}

//仿真过程，不断利用pick_random()随机选择结点，直至决出胜负，返回胜负值。
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

//反向传播过程，利用rollout()的结果向上更新节点内容。
void Node::backpropagate(Node* node, int result) {
	node->num_visit += 1;
	node->num_of_wins += result;
	if (node->parent) {
		backpropagate(node->parent, result);
	}
}

//利用每个节点的参数（胜利次数/游戏次数）来判断哪一个孩子收益最大，并返回这个孩子。  
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


//添加未选择的落子（fully_expanded()中调用）
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
					// 找到棋子的位置，更新边界
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
		/*参考代码
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

//筛选落子
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
		// valuable_pos 为空，返回原来的位置
		return legal_pos;
	}
	else {
		// valuable_pos 不为空，找到了有价值的位置
		return valuable_pos;
	}
	//return valuable_pos;
}

//fully_expanded()中调用的添加未选择的落子
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
	//				// 找到棋子的位置，更新边界
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
	//	/*参考代码
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

// 游戏结束检查函数
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

	// 检查棋盘是否已满
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

//删除游戏树
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
//	// 创建MCTS树的根节点
//	Node* root = new Node(NULL, pre_pos, chessMap, count, 1);
//
//	// 创建包含所需线程数的ThreadPool，这里设置为3个线程
//	ThreadPool threadPool(3);
//
//	// 使用ThreadPool并行执行MCTS迭代
//	std::vector<std::future<int>> results;
//	for (int i = 0; i < mcts_times; ++i) {
//		results.emplace_back(threadPool.enqueue([&] {
//			Node* leaf = traverse(root);  // 遍历树，选择一个节点进行拓展
//			int simulation_result = rollout(leaf);  // 仿真过程，得到游戏结果
//			backpropagate(leaf, simulation_result);  // 反向传播，更新节点的统计信息
//			return simulation_result;
//			}));
//	}
//
//	// 等待所有任务完成
//	int j = 0;
//	for (auto& result : results) {
//		cout << "正在计算中：" << j++ << endl;
//		result.wait();
//	}
//
//	// 根据MCTS迭代的结果找到最佳走法
//	int best_simulation_result = -1; // 初始化为默认值
//	Node* best_child_node = nullptr;
//	for (auto& result : results) {
//		int simulation_result = result.get();
//		if (simulation_result > best_simulation_result) {
//			best_simulation_result = simulation_result;
//			best_child_node = root->best_child();  // 获取最佳子节点
//		}
//	}
//
//	// 从最佳子节点提取最佳走法
//	ChessPos best_move;
//	if (best_child_node != nullptr) {
//		best_move.col = best_child_node->lastPos.col;
//		best_move.row = best_child_node->lastPos.row;
//	}
//	else {
//		// 处理找不到最佳子节点的情况
//		cout << "没有找到最佳子节点" << endl;
//	}
//
//	// 清理内存并返回最佳走法
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
//	cout << "循环后" << "\n";
//	Node* best_child_node = root->best_child();
//
//	cout << "选最好的" << "\n";
//	ChessPos best_move;
//
//	best_move.col = best_child_node->lastPos.col;
//	best_move.row = best_child_node->lastPos.row;
//
//
//	cout << "得到位置" << "\n";
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
//	cout << "循环后" << "\n";
//
//	std::vector<ChessPos> best_moves;
//	std::vector<Node*> best_child_nodes = root->best_children(5);  // 返回5个最佳子节点
//
//	cout << "选最好的" << "\n";
//
//	for (Node* best_child_node : best_child_nodes) {
//		ChessPos best_move;
//		best_move.col = best_child_node->lastPos.col;
//		best_move.row = best_child_node->lastPos.row;
//		best_moves.push_back(best_move);
//	}
//
//	cout << "得到位置" << "\n";
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




	//最主要的算法


	cout << "=== 循环前=== count1:  " << count1 << endl;
	int key = 300;
	int loop = count1 * key;

	for (int i = 0; i < 80000; ++i) {
		Node* leaf = traverse(root);
		int simulation_result = rollout(leaf);
		backpropagate(leaf, simulation_result);
	}

	cout << "=== 搜索次数=== LOOP :  " << loop << endl;


	std::vector<ChessPos> best_moves;
	std::vector<Node*> best_child_nodes = root->best_children(5);  // 返回5个最佳子节点


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