#include <iostream>
#include "ChessGame.h"

int main(void) {
	//��������
	Chess chess(15, 96, 76, 39);
	Man man;
	AI ai;
	ChessGame game(&man, &ai, &chess);

	game.play();

	return 0;
}
