  #include "Man.h"

void Man::init(Chess* chess)
{
	this->chess = chess;
}

void Man::go(Chess* m_chess, ChessPos* pos) {
	// 等待棋士有效落子
	MOUSEMSG msg;
	while (1) {
		msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN) {
			int mouseX = msg.x;
			int mouseY = msg.y;
			if (m_chess->clickBoard(mouseX, mouseY, pos)) {
				break;
			}
			else if (isMouseClickWithinImage(mouseX, mouseY, 300, 0, 80, 50)) {
				m_chess->undoMove();
				msg = GetMouseMsg();
				if (msg.uMsg == WM_LBUTTONDOWN) {
					int mouseX = msg.x;
					int mouseY = msg.y;
					if (m_chess->clickBoard(mouseX, mouseY, pos)) {
						break;
					}

				}
			}
		}
	}
	// 落子
	m_chess->chessDown(pos, colour);
}
bool Man::isMouseClickWithinImage(int mouseX, int mouseY, int imageX, int imageY, int imageWidth, int imageHeight) {
	return (mouseX >= imageX && mouseX <= imageX + imageWidth && mouseY >= imageY && mouseY <= imageY + imageHeight);
}
