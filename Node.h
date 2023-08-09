#pragma once

#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "Chess.h"
class Node
{
public:
	Node();
	Node(Node* parent, ChessPos* lastPos, int chessMap[15][15], int count, int coefficient);//构造方法
	bool fully_expanded();									//判断节点是否完全扩展
	vector<ChessPos> get_legal_pos(int count, int chessmap[15][15], ChessPos lastPos);//添加未选择的落子
	
	//fully_expanded()中调用的添加未选择的落子
	vector<ChessPos> get_legal_pos_for_fully_expanded(int count, int chessmap[15][15], ChessPos lastPos);
	
	string non_terminal(ChessPos pre_pos);					//判断节点是否为终端叶子节点，即该节点对应的格局是否已分出胜负
	string game_over(ChessPos pre_pos);						//判断游戏输赢
	Node* pick_unvisited();									//选择一个未访问的节点并将其添加进当前节点的孩子中
	ChessPos calculateScore(int chessMap[15][15], ChessPos lastPos);
	Node* pick_random();									//随机选择该节点的一个孩子进行扩展
	Node* best_uct(double c_param = 2.81);					//UCT比较过程，选择当前策略下最优的孩子节点

	Node* traverse(Node* node);								//遍历结点，遇到已完全展开的结点，利用best_uct选择最佳的孩子节点继续向下遍历。此函数实现选择+拓展过程
	int rollout(Node* node);								//仿真过程，不断利用pick_random()随机选择结点，直至决出胜负，返回胜负值。
	void backpropagate(Node* node, int result);				//反向传播过程，利用rollout()的结果向上更新节点内容。

	//Node* best_child(Node* node);							//利用每个节点的参数（胜利次数/游戏次数）来判断哪一个孩子收益最大，并返回这个孩子。
	//Node* best_child();							//利用每个节点的参数（胜利次数/游戏次数）来判断哪一个孩子收益最大，并返回这个孩子。
	std::vector<Node*> best_children(int num_children);
	void delete_subtree(Node* node);						//删除游戏树
	//ChessPos monte_carlo_tree_search(int chessMap[15][15], ChessPos* pre_pos, int count, int mcts_times);//实现蒙特卡洛搜索，选出最佳位置
	ChessPos monte_carlo_tree_search(int chessMap[15][15], ChessPos* pre_pos, int mcts_times);
	std::vector<ChessPos> monte_carlo_tree_search(int chessMap[15][15], ChessPos* pre_pos, int count, int mcts_times);
	ChessPos getLastPos();

	ChessPos lastPos;			//最近落子位置
	ChessPos myPos;				//落子位置
	int count;					//第几手



private:

	int chessMap[15][15];		//棋局
	Node* parent;				//父节点 
	vector<Node*> children;		//所有的子节点
	vector<ChessPos> not_visit_pos;//未访问的节点
	int num_visit;				//节点被访问的次数
	int num_of_wins;			//仿真过程胜利分数
	int coefficient;			//系数
	vector<Node*> children2;		//模拟的子节点

};

