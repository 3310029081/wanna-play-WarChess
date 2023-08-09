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
	Node(Node* parent, ChessPos* lastPos, int chessMap[15][15], int count, int coefficient);//���췽��
	bool fully_expanded();									//�жϽڵ��Ƿ���ȫ��չ
	vector<ChessPos> get_legal_pos(int count, int chessmap[15][15], ChessPos lastPos);//���δѡ�������
	
	//fully_expanded()�е��õ����δѡ�������
	vector<ChessPos> get_legal_pos_for_fully_expanded(int count, int chessmap[15][15], ChessPos lastPos);
	
	string non_terminal(ChessPos pre_pos);					//�жϽڵ��Ƿ�Ϊ�ն�Ҷ�ӽڵ㣬���ýڵ��Ӧ�ĸ���Ƿ��ѷֳ�ʤ��
	string game_over(ChessPos pre_pos);						//�ж���Ϸ��Ӯ
	Node* pick_unvisited();									//ѡ��һ��δ���ʵĽڵ㲢������ӽ���ǰ�ڵ�ĺ�����
	ChessPos calculateScore(int chessMap[15][15], ChessPos lastPos);
	Node* pick_random();									//���ѡ��ýڵ��һ�����ӽ�����չ
	Node* best_uct(double c_param = 2.81);					//UCT�ȽϹ��̣�ѡ��ǰ���������ŵĺ��ӽڵ�

	Node* traverse(Node* node);								//������㣬��������ȫչ���Ľ�㣬����best_uctѡ����ѵĺ��ӽڵ�������±������˺���ʵ��ѡ��+��չ����
	int rollout(Node* node);								//������̣���������pick_random()���ѡ���㣬ֱ������ʤ��������ʤ��ֵ��
	void backpropagate(Node* node, int result);				//���򴫲����̣�����rollout()�Ľ�����ϸ��½ڵ����ݡ�

	//Node* best_child(Node* node);							//����ÿ���ڵ�Ĳ�����ʤ������/��Ϸ���������ж���һ������������󣬲�����������ӡ�
	//Node* best_child();							//����ÿ���ڵ�Ĳ�����ʤ������/��Ϸ���������ж���һ������������󣬲�����������ӡ�
	std::vector<Node*> best_children(int num_children);
	void delete_subtree(Node* node);						//ɾ����Ϸ��
	//ChessPos monte_carlo_tree_search(int chessMap[15][15], ChessPos* pre_pos, int count, int mcts_times);//ʵ�����ؿ���������ѡ�����λ��
	ChessPos monte_carlo_tree_search(int chessMap[15][15], ChessPos* pre_pos, int mcts_times);
	std::vector<ChessPos> monte_carlo_tree_search(int chessMap[15][15], ChessPos* pre_pos, int count, int mcts_times);
	ChessPos getLastPos();

	ChessPos lastPos;			//�������λ��
	ChessPos myPos;				//����λ��
	int count;					//�ڼ���



private:

	int chessMap[15][15];		//���
	Node* parent;				//���ڵ� 
	vector<Node*> children;		//���е��ӽڵ�
	vector<ChessPos> not_visit_pos;//δ���ʵĽڵ�
	int num_visit;				//�ڵ㱻���ʵĴ���
	int num_of_wins;			//�������ʤ������
	int coefficient;			//ϵ��
	vector<Node*> children2;		//ģ����ӽڵ�

};

