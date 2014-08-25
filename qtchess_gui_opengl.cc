/*
** -- Qt Includes --
*/

#include <QMouseEvent>

/*
** -- Local Includes --
*/

#include "qtchess.h"

extern QApplication *qapp;
extern qtchess *chess;
extern qtchess_comm *comm;
extern qtchess_gui *gui;

void openglWid::newGame(void)
{
  mouse_pressed = 0;
  showValid = false;
  point_pressed.x = point_pressed.y = -1;
  point_selected.x = point_selected.y = -1;
}

void openglWid::paintGL(void)
{
  QFont font;
  bool game_over = false;
  double X = 0, Y = 0, x = 0, y = 0;
  int I = 0, J = 0, found = 0, i = 0, j = 0, rc = 0;
  int nonEmptyNow = 0, nonEmptyThen = 0;
  int oldBoard[NSQUARES][NSQUARES];
  int origPiece = 0;
  struct move_s current_move;

  /*
  ** All the drawing occurs here.
  */

  glLineWidth(2.0);
  glPointSize(2.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen.

  /*
  ** First an empty board.
  */

  for(i = 0; i < NSQUARES; i++)
    for(j = 0; j < NSQUARES; j++)
      {
	if((i + j) % 2 != 0)
	  glColor3f((GLfloat) 0.8, (GLfloat) 0.7, (GLfloat) 0.5);
	else
	  glColor3f((GLfloat) 0.5, (GLfloat) 0.5, (GLfloat) 0.6);

	x = px + i * block_size;
	y = py + j * block_size;
	glRectd((double) x,
		(double) y,
		px + (i + 1) * block_size,
		py + (j + 1) * block_size);

	if(mouse_pressed)
	  if(point_pressed.x >= (px + i * block_size) &&
	     point_pressed.x <= (px + (i + 1) * block_size) &&
	     point_pressed.y >= (py + j * block_size) &&
	     point_pressed.y <= (py + (j + 1) * block_size))
	    {
	      found = 1;
	      X = px + i * block_size;
	      Y = py + j * block_size;
	      I = i;
	      J = j;
	    }

	/*
	** Highlight the selected piece's valid moves.
	*/

	if(showValid)
	  if(chess &&
	     qtchess_validate::areValidCoordinates(point_selected.x,
						   point_selected.y) &&
	     qtchess_validate::isValidMove
	     ((int) point_selected.y,
	      (int) point_selected.x,
	      j, i,
	      chess->board
	      [(int) point_selected.x]
	      [(int) point_selected.y]) != INVALID)
	    {
	      bool isValid = true;

	      if(qtchess_validate::isKing
		 (chess->board
		  [(int) point_selected.x][(int) point_selected.y]))
		{
		  int piece1 = chess->board[i][j];
		  int piece2 = chess->board
		    [(int) point_selected.x][(int) point_selected.y];

		  chess->board[i][j] = piece2;
		  chess->board
		    [(int) point_selected.x][(int) point_selected.y] =
		    EMPTY_SQUARE;
		  isValid = !qtchess_validate::isThreatened
		    (i, j, qtchess_validate::color(piece2) == BLACK ?
		     WHITE : BLACK);
		  chess->board[i][j] = piece1;
		  chess->board
		    [(int) point_selected.x][(int) point_selected.y] =
		    piece2;
		}

	      if(isValid)
		{
		  glColor3f((GLfloat) 1.2, (GLfloat) 2.05, (GLfloat) 0.75);
		  glRectd((double) x + 1,
			  (double) y + 1,
			  px + (i + 1) * block_size - 1,
			  py + (j + 1) * block_size - 1);
		}
	    }

	if((i + j) % 2 != 0)
	  glColor3f((GLfloat) 0.8, (GLfloat) 0.7, (GLfloat) 0.5);
	else
	  glColor3f((GLfloat) 0.5, (GLfloat) 0.5, (GLfloat) 0.6);

	/*
	** Draw the pieces.
	*/

	if(chess && !qtchess_validate::isEmpty(chess->board[i][j]))
	  {
	    if(qtchess_validate::isWhite(chess->board[i][j]))
	      glColor3f((GLfloat) 0.96, (GLfloat) 0.96, (GLfloat) 0.86);
	    else
	      glColor3f((GLfloat) 0.65, (GLfloat) 0.16, (GLfloat) 0.16);
	  }

	if(chess && qtchess_validate::isBishop(chess->board[i][j]))
	  {
	    glBegin(GL_POLYGON);
	    glVertex2d(x + BISHOP_X_OFFSET,
		       y + BISHOP_Y_OFFSET);
	    glVertex2d(x + BISHOP_X_OFFSET + BISHOP_WIDTH,
		       y + BISHOP_Y_OFFSET);
	    glVertex2d(x + BISHOP_X_OFFSET + BISHOP_WIDTH / 2 +
		       8 / denominator,
		       y + BISHOP_Y_OFFSET + BISHOP_HEIGHT -
		       20 / denominator);
	    glVertex2d(x + BISHOP_X_OFFSET + BISHOP_WIDTH / 2 -
		       8 / denominator,
		       y + BISHOP_Y_OFFSET + BISHOP_HEIGHT -
		       20 / denominator);
	    glEnd();
	    glBegin(GL_POLYGON);
	    glVertex2d(x + BISHOP_X_OFFSET + BISHOP_WIDTH / 2 +
		       8 / denominator,
		       y + BISHOP_Y_OFFSET + BISHOP_HEIGHT -
		       20 / denominator);
	    glVertex2d(x + BISHOP_X_OFFSET + BISHOP_WIDTH / 2 +
		       18 / denominator,
		       y + BISHOP_Y_OFFSET + BISHOP_HEIGHT -
		       10 / denominator);
	    glVertex2d(x + BISHOP_X_OFFSET + BISHOP_WIDTH / 2,
		       y + BISHOP_Y_OFFSET + BISHOP_HEIGHT +
		       2 / denominator);
	    glVertex2d(x + BISHOP_X_OFFSET + BISHOP_WIDTH / 2 -
		       18 / denominator,
		       y + BISHOP_Y_OFFSET + BISHOP_HEIGHT -
		       10 / denominator);
	    glVertex2d(x + BISHOP_X_OFFSET + BISHOP_WIDTH / 2 -
		       8 / denominator,
		       y + BISHOP_Y_OFFSET + BISHOP_HEIGHT -
		       20 / denominator);
	    glEnd();
	    glColor3f(0.0, 0.0, 0.0);
	    glBegin(GL_LINE_LOOP);
	    glVertex2d(x + BISHOP_X_OFFSET,
		       y + BISHOP_Y_OFFSET);
	    glVertex2d(x + BISHOP_X_OFFSET + BISHOP_WIDTH,
		       y + BISHOP_Y_OFFSET);
	    glVertex2d(x + BISHOP_X_OFFSET + BISHOP_WIDTH / 2 +
		       8 / denominator,
		       y + BISHOP_Y_OFFSET + BISHOP_HEIGHT -
		       20 / denominator);
	    glVertex2d(x + BISHOP_X_OFFSET + BISHOP_WIDTH / 2 -
		       8 / denominator,
		       y + BISHOP_Y_OFFSET + BISHOP_HEIGHT -
		       20 / denominator);
	    glEnd();
	    glBegin(GL_LINE_LOOP);
	    glVertex2d(x + BISHOP_X_OFFSET + BISHOP_WIDTH / 2 +
		       8 / denominator,
		       y + BISHOP_Y_OFFSET + BISHOP_HEIGHT -
		       20 / denominator);
	    glVertex2d(x + BISHOP_X_OFFSET + BISHOP_WIDTH / 2 +
		       18 / denominator,
		       y + BISHOP_Y_OFFSET + BISHOP_HEIGHT -
		       10 / denominator);
	    glVertex2d(x + BISHOP_X_OFFSET + BISHOP_WIDTH / 2,
		       y + BISHOP_Y_OFFSET + BISHOP_HEIGHT +
		       2 / denominator);
	    glVertex2d(x + BISHOP_X_OFFSET + BISHOP_WIDTH / 2 -
		       18 / denominator,
		       y + BISHOP_Y_OFFSET + BISHOP_HEIGHT -
		       10 / denominator);
	    glVertex2d(x + BISHOP_X_OFFSET + BISHOP_WIDTH / 2 -
		       8 / denominator,
		       y + BISHOP_Y_OFFSET + BISHOP_HEIGHT -
		       20 / denominator);
	    glEnd();
	  }
	else if(chess && qtchess_validate::isKing(chess->board[i][j]))
	  {
	    glRectd(x + KING_X_OFFSET, y + KING_Y_OFFSET,
		    x + KING_X_OFFSET + KING_B_WIDTH,
		    y + KING_Y_OFFSET + KING_B_HEIGHT);
	    glRectd(x + KING_X_OFFSET + 5 / denominator,
		    y + KING_Y_OFFSET + KING_B_HEIGHT,
		    x + KING_X_OFFSET + 5 / denominator + KING_BT_WIDTH,
		    y + KING_Y_OFFSET + KING_B_HEIGHT + KING_BT_HEIGHT);
	    glRectd(x + KING_X_OFFSET + 8 / denominator,
		    y + KING_Y_OFFSET + KING_B_HEIGHT + KING_BT_HEIGHT,
		    x + KING_X_OFFSET + 8 / denominator + KING_WIDTH,
		    y + KING_Y_OFFSET + KING_B_HEIGHT + KING_BT_HEIGHT +
		    KING_HEIGHT);
	    glRectd(x + KING_X_OFFSET + 14 / denominator,
		    y + KING_Y_OFFSET + KING_B_HEIGHT +
		    KING_BT_HEIGHT + KING_HEIGHT,
		    x + KING_X_OFFSET + 14 / denominator + KING_VCROSS_WIDTH,
		    y + KING_Y_OFFSET + KING_B_HEIGHT +
		    KING_BT_HEIGHT + KING_HEIGHT + KING_VCROSS_HEIGHT);
	    glRectd(x + KING_X_OFFSET + 8 / denominator,
		    y + KING_Y_OFFSET + KING_B_HEIGHT +
		    KING_BT_HEIGHT + KING_HEIGHT + 5 / denominator,
		    x + KING_X_OFFSET + 8 / denominator + KING_HCROSS_WIDTH,
		    y + KING_Y_OFFSET + KING_B_HEIGHT +
		    KING_BT_HEIGHT + KING_HEIGHT + KING_HCROSS_HEIGHT +
		    5 / denominator);
	    glColor3f(0.0, 0.0, 0.0);
	    glBegin(GL_LINE_LOOP);
	    glVertex2d(x + KING_X_OFFSET, y + KING_Y_OFFSET);
	    glVertex2d(x + KING_X_OFFSET + KING_B_WIDTH,
		       y + KING_Y_OFFSET);
	    glVertex2d(x + KING_X_OFFSET + KING_B_WIDTH,
		       y + KING_Y_OFFSET + KING_B_HEIGHT);
	    glVertex2d(x + KING_X_OFFSET,
		       y + KING_Y_OFFSET + KING_B_HEIGHT);
	    glEnd();
	    glBegin(GL_LINE_LOOP);
	    glVertex2d(x + KING_X_OFFSET + 5 / denominator,
		       y + KING_Y_OFFSET + KING_B_HEIGHT);
	    glVertex2d(x + KING_X_OFFSET + 5 / denominator +
		       KING_BT_WIDTH,
		       y + KING_Y_OFFSET + KING_B_HEIGHT);
	    glVertex2d(x + KING_X_OFFSET + 5 / denominator +
		       KING_BT_WIDTH,
		       y + KING_Y_OFFSET + KING_B_HEIGHT +
		       KING_BT_HEIGHT);
	    glVertex2d(x + KING_X_OFFSET + 5 / denominator,
		       y + KING_Y_OFFSET + KING_B_HEIGHT +
		       KING_BT_HEIGHT);
	    glEnd();
	    glBegin(GL_LINE_LOOP);
	    glVertex2d(x + KING_X_OFFSET + 8 / denominator,
		       y + KING_Y_OFFSET + KING_B_HEIGHT +
		       KING_BT_HEIGHT);
	    glVertex2d(x + KING_X_OFFSET + 8 / denominator +
		       KING_WIDTH,
		       y + KING_Y_OFFSET + KING_B_HEIGHT +
		       KING_BT_HEIGHT);
	    glVertex2d(x + KING_X_OFFSET + 8 / denominator +
		       KING_WIDTH,
		       y + KING_Y_OFFSET + KING_B_HEIGHT +
		       KING_BT_HEIGHT + KING_HEIGHT);
	    glVertex2d(x + KING_X_OFFSET + 8 / denominator,
		       y + KING_Y_OFFSET + KING_B_HEIGHT +
		       KING_BT_HEIGHT + KING_HEIGHT);
	    glEnd();
	    glBegin(GL_LINE_LOOP);
	    glVertex2d(x + KING_X_OFFSET + KING_VCROSS_WIDTH +
		       14 / denominator,
		       y + KING_Y_OFFSET + KING_B_HEIGHT +
		       KING_BT_HEIGHT + KING_HEIGHT);
	    glVertex2d(x + KING_X_OFFSET + KING_VCROSS_WIDTH +
		       14 / denominator,
		       y + KING_Y_OFFSET + KING_B_HEIGHT +
		       KING_BT_HEIGHT + KING_HEIGHT + 5 / denominator);
	    glVertex2d(x + KING_X_OFFSET + KING_VCROSS_WIDTH +
		       20 / denominator,
		       y + KING_Y_OFFSET + KING_B_HEIGHT +
		       KING_BT_HEIGHT + KING_HEIGHT + 5 / denominator);
	    glVertex2d(x + KING_X_OFFSET + KING_VCROSS_WIDTH +
		       20 / denominator,
		       y + KING_Y_OFFSET + KING_B_HEIGHT +
		       KING_BT_HEIGHT + KING_HEIGHT + KING_HCROSS_HEIGHT +
		       5 / denominator);
	    glVertex2d(x + KING_X_OFFSET + KING_VCROSS_WIDTH +
		       14 / denominator,
		       y + KING_Y_OFFSET + KING_B_HEIGHT +
		       KING_BT_HEIGHT + KING_HEIGHT + 5 / denominator +
		       KING_HCROSS_HEIGHT);
	    glVertex2d(x + KING_X_OFFSET + KING_VCROSS_WIDTH +
		       14 / denominator,
		       y + KING_Y_OFFSET + KING_B_HEIGHT +
		       KING_BT_HEIGHT + KING_HEIGHT + 5 / denominator +
		       KING_HCROSS_HEIGHT + 5 / denominator);
	    glVertex2d(x + KING_X_OFFSET + 14 / denominator,
		       y + KING_Y_OFFSET + KING_B_HEIGHT +
		       KING_BT_HEIGHT + KING_HEIGHT + 5 / denominator +
		       KING_HCROSS_HEIGHT + 5 / denominator);
	    glVertex2d(x + KING_X_OFFSET + 14 / denominator,
		       y + KING_Y_OFFSET + KING_B_HEIGHT +
		       KING_BT_HEIGHT + KING_HEIGHT + 5 / denominator +
		       KING_HCROSS_HEIGHT);
	    glVertex2d(x + KING_X_OFFSET + 8 / denominator,
		       y + KING_Y_OFFSET + KING_B_HEIGHT +
		       KING_BT_HEIGHT + KING_HEIGHT + 5 / denominator +
		       KING_HCROSS_HEIGHT);
	    glVertex2d(x + KING_X_OFFSET + 8 / denominator,
		       y + KING_Y_OFFSET + KING_B_HEIGHT +
		       KING_BT_HEIGHT + KING_HEIGHT + 5 / denominator);
	    glVertex2d(x + KING_X_OFFSET + 14 / denominator,
		       y + KING_Y_OFFSET + KING_B_HEIGHT +
		       KING_BT_HEIGHT + KING_HEIGHT + 5 / denominator);
	    glVertex2d(x + KING_X_OFFSET + 14 / denominator,
		       y + KING_Y_OFFSET + KING_B_HEIGHT +
		       KING_BT_HEIGHT + KING_HEIGHT);
	    glEnd();
	  }
	else if(chess && qtchess_validate::isQueen(chess->board[i][j]))
	  {
	    glRectd(x + QUEEN_X_OFFSET, y + QUEEN_Y_OFFSET,
		    x + QUEEN_X_OFFSET + QUEEN_B_WIDTH,
		    y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT);
	    glRectd(x + QUEEN_X_OFFSET + 5 / denominator,
		    y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT,
		    x + QUEEN_X_OFFSET + 5 / denominator + QUEEN_BT_WIDTH,
		    y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT + QUEEN_BT_HEIGHT);
	    glRectd(x + QUEEN_X_OFFSET + 8 / denominator,
		    y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT + QUEEN_BT_HEIGHT,
		    x + QUEEN_X_OFFSET + 8 / denominator + QUEEN_WIDTH,
		    y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT + QUEEN_BT_HEIGHT +
		    QUEEN_HEIGHT);
	    glBegin(GL_POLYGON);
	    glVertex2d(x + QUEEN_X_OFFSET + 8 / denominator +
		       QUEEN_WIDTH / 2,
		       y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT +
		       QUEEN_BT_HEIGHT + QUEEN_HEIGHT);
	    glVertex2d(x + QUEEN_X_OFFSET + 8 / denominator +
		       QUEEN_WIDTH,
		       y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT +
		       QUEEN_BT_HEIGHT + QUEEN_HEIGHT);
	    glVertex2d(x + QUEEN_X_OFFSET + 8 / denominator +
		       QUEEN_WIDTH + 10 / denominator,
		       y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT +
		       QUEEN_BT_HEIGHT + QUEEN_HEIGHT + 20 / denominator);
	    glVertex2d(x + QUEEN_X_OFFSET + 8 / denominator +
		       QUEEN_WIDTH / 2,
		       y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT +
		       QUEEN_BT_HEIGHT + QUEEN_HEIGHT + 5 / denominator);
	    glEnd();
	    glBegin(GL_POLYGON);
	    glVertex2d(x + QUEEN_X_OFFSET + 8 / denominator +
		       QUEEN_WIDTH / 2,
		       y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT +
		       QUEEN_BT_HEIGHT + QUEEN_HEIGHT);
	    glVertex2d(x + QUEEN_X_OFFSET + 8 / denominator +
		       QUEEN_WIDTH / 2,
		       y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT +
		       QUEEN_BT_HEIGHT + QUEEN_HEIGHT + 5 / denominator);
	    glVertex2d(x + QUEEN_X_OFFSET - 2 / denominator,
		       y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT +
		       QUEEN_BT_HEIGHT + QUEEN_HEIGHT + 20 / denominator);
	    glVertex2d(x + QUEEN_X_OFFSET + 8 / denominator,
		       y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT +
		       QUEEN_BT_HEIGHT + QUEEN_HEIGHT);
	    glEnd();
	    glBegin(GL_POLYGON);
	    glVertex2d(x + QUEEN_X_OFFSET + 8 / denominator +
		       QUEEN_WIDTH / 2,
		       y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT +
		       QUEEN_BT_HEIGHT + QUEEN_HEIGHT + 5 / denominator);
	    glVertex2d(x + QUEEN_X_OFFSET + 8 / denominator +
		       3 * QUEEN_WIDTH / 4,
		       y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT +
		       QUEEN_BT_HEIGHT + QUEEN_HEIGHT + 7 / denominator);
	    glVertex2d(x + QUEEN_X_OFFSET + 8 / denominator +
		       QUEEN_WIDTH / 2,
		       y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT +
		       QUEEN_BT_HEIGHT + QUEEN_HEIGHT + 20 / denominator);
	    glVertex2d(x + QUEEN_X_OFFSET + 8 / denominator +
		       QUEEN_WIDTH / 4,
		       y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT +
		       QUEEN_BT_HEIGHT + QUEEN_HEIGHT + 7 / denominator);
	    glEnd();
	    glColor3f(0.0, 0.0, 0.0);
	    glBegin(GL_LINE_LOOP);
	    glVertex2d(x + QUEEN_X_OFFSET, y + QUEEN_Y_OFFSET);
	    glVertex2d(x + QUEEN_X_OFFSET + QUEEN_B_WIDTH,
		       y + QUEEN_Y_OFFSET);
	    glVertex2d(x + QUEEN_X_OFFSET + QUEEN_B_WIDTH,
		       y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT);
	    glVertex2d(x + QUEEN_X_OFFSET,
		       y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT);
	    glEnd();
	    glBegin(GL_LINE_LOOP);
	    glVertex2d(x + QUEEN_X_OFFSET + 5 / denominator,
		       y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT);
	    glVertex2d(x + QUEEN_X_OFFSET + 5 / denominator +
		       QUEEN_BT_WIDTH,
		       y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT);
	    glVertex2d(x + QUEEN_X_OFFSET + 5 / denominator +
		       QUEEN_BT_WIDTH,
		       y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT +
		       QUEEN_BT_HEIGHT);
	    glVertex2d(x + QUEEN_X_OFFSET + 5 / denominator,
		       y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT +
		       QUEEN_BT_HEIGHT);
	    glEnd();
	    glBegin(GL_LINE_LOOP);
	    glVertex2d(x + QUEEN_X_OFFSET + 8 / denominator,
		       y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT +
		       QUEEN_BT_HEIGHT);
	    glVertex2d(x + QUEEN_X_OFFSET + 8 / denominator +
		       QUEEN_WIDTH,
		       y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT +
		       QUEEN_BT_HEIGHT);
	    glVertex2d(x + QUEEN_X_OFFSET + 8 / denominator +
		       QUEEN_WIDTH,
		       y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT +
		       QUEEN_BT_HEIGHT + QUEEN_HEIGHT);
	    glVertex2d(x + QUEEN_X_OFFSET + 8 / denominator,
		       y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT +
		       QUEEN_BT_HEIGHT + QUEEN_HEIGHT);
	    glEnd();
	    glBegin(GL_LINE_LOOP);
	    glVertex2d(x + QUEEN_X_OFFSET + 8 / denominator +
		       QUEEN_WIDTH,
		       y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT +
		       QUEEN_BT_HEIGHT + QUEEN_HEIGHT);
	    glVertex2d(x + QUEEN_X_OFFSET + 8 / denominator +
		       QUEEN_WIDTH + 10 / denominator,
		       y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT +
		       QUEEN_BT_HEIGHT + QUEEN_HEIGHT + 20 / denominator);
	    glVertex2d(x + QUEEN_X_OFFSET + 8 / denominator +
		       3 * QUEEN_WIDTH / 4,
		       y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT +
		       QUEEN_BT_HEIGHT + QUEEN_HEIGHT + 8 / denominator);
	    glVertex2d(x + QUEEN_X_OFFSET + 8 / denominator +
		       QUEEN_WIDTH / 2,
		       y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT +
		       QUEEN_BT_HEIGHT + QUEEN_HEIGHT + 20 / denominator);
	    glVertex2d(x + QUEEN_X_OFFSET + 8 / denominator +
		       QUEEN_WIDTH / 4,
		       y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT +
		       QUEEN_BT_HEIGHT + QUEEN_HEIGHT + 8 / denominator);
	    glVertex2d(x + QUEEN_X_OFFSET - 2 / denominator,
		       y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT +
		       QUEEN_BT_HEIGHT + QUEEN_HEIGHT + 20 / denominator);
	    glVertex2d(x + QUEEN_X_OFFSET + 8 / denominator,
		       y + QUEEN_Y_OFFSET + QUEEN_B_HEIGHT +
		       QUEEN_BT_HEIGHT + QUEEN_HEIGHT);
	    glEnd();
	  }
	else if(chess && qtchess_validate::isKnight(chess->board[i][j]))
	  {
	    glBegin(GL_POLYGON);
	    glVertex2d(x + KNIGHT_X_OFFSET, y + KNIGHT_Y_OFFSET);
	    glVertex2d(x + KNIGHT_X_OFFSET + KNIGHT_WIDTH + 2 / denominator,
		       y + KNIGHT_Y_OFFSET);
	    glVertex2d(x + KNIGHT_X_OFFSET + KNIGHT_WIDTH / 2,
		       y + KNIGHT_Y_OFFSET + KNIGHT_HEIGHT - 23 / denominator);
 	    glVertex2d(x + KNIGHT_X_OFFSET + KNIGHT_WIDTH / 2 +
		       1 / denominator,
 		       y + KNIGHT_Y_OFFSET + KNIGHT_HEIGHT - 5 / denominator);
	    glVertex2d(x + KNIGHT_X_OFFSET + KNIGHT_WIDTH / 2 -
		       4 / denominator,
		       y + KNIGHT_Y_OFFSET + KNIGHT_HEIGHT);
	    glVertex2d(x + KNIGHT_X_OFFSET + KNIGHT_WIDTH / 2 -
		       4 / denominator,
		       y + KNIGHT_Y_OFFSET + KNIGHT_HEIGHT);
	    glVertex2d(x + KNIGHT_X_OFFSET, y + KNIGHT_Y_OFFSET +
		       42 / denominator);
	    glEnd();
	    glBegin(GL_POLYGON);
	    glVertex2d(x + KNIGHT_X_OFFSET + KNIGHT_WIDTH / 2,
		       y + KNIGHT_Y_OFFSET + KNIGHT_HEIGHT - 23 / denominator);
	    glVertex2d(x + KNIGHT_X_OFFSET + KNIGHT_WIDTH + 5 / denominator,
		       y + KNIGHT_Y_OFFSET + 15 / denominator);
 	    glVertex2d(x + KNIGHT_X_OFFSET + KNIGHT_WIDTH + 5 / denominator,
 		       y + KNIGHT_Y_OFFSET + 25 / denominator);
 	    glVertex2d(x + KNIGHT_X_OFFSET + KNIGHT_WIDTH / 2 +
		       15 / denominator,
 		       y + KNIGHT_Y_OFFSET + KNIGHT_HEIGHT - 10 / denominator);
 	    glVertex2d(x + KNIGHT_X_OFFSET + KNIGHT_WIDTH / 2 +
		       1 / denominator,
 		       y + KNIGHT_Y_OFFSET + KNIGHT_HEIGHT - 5 / denominator);
	    glEnd();
	    glColor3f(0.0, 0.0, 0.0);
	    glBegin(GL_LINE_LOOP);
	    glVertex2d(x + KNIGHT_X_OFFSET, y + KNIGHT_Y_OFFSET);
	    glVertex2d(x + KNIGHT_X_OFFSET + KNIGHT_WIDTH + 2 / denominator,
		       y + KNIGHT_Y_OFFSET);
	    glVertex2d(x + KNIGHT_X_OFFSET + KNIGHT_WIDTH / 2,
		       y + KNIGHT_Y_OFFSET + KNIGHT_HEIGHT - 23 / denominator);
	    glVertex2d(x + KNIGHT_X_OFFSET + KNIGHT_WIDTH + 5 / denominator,
		       y + KNIGHT_Y_OFFSET + 15 / denominator);
 	    glVertex2d(x + KNIGHT_X_OFFSET + KNIGHT_WIDTH + 5 / denominator,
 		       y + KNIGHT_Y_OFFSET + 25 / denominator);
 	    glVertex2d(x + KNIGHT_X_OFFSET + KNIGHT_WIDTH / 2 +
		       15 / denominator,
 		       y + KNIGHT_Y_OFFSET + KNIGHT_HEIGHT - 10 / denominator);
 	    glVertex2d(x + KNIGHT_X_OFFSET + KNIGHT_WIDTH / 2 +
		       1 / denominator,
 		       y + KNIGHT_Y_OFFSET + KNIGHT_HEIGHT - 5 / denominator);
	    glVertex2d(x + KNIGHT_X_OFFSET + KNIGHT_WIDTH / 2 -
		       4 / denominator,
		       y + KNIGHT_Y_OFFSET + KNIGHT_HEIGHT);
	    glVertex2d(x + KNIGHT_X_OFFSET + KNIGHT_WIDTH / 2 -
		       4 / denominator,
		       y + KNIGHT_Y_OFFSET + KNIGHT_HEIGHT);
	    glVertex2d(x + KNIGHT_X_OFFSET, y + KNIGHT_Y_OFFSET +
		       42 / denominator);
	    glEnd();
	  }
	else if(chess && qtchess_validate::isRook(chess->board[i][j]))
	  {
	    glRectd(x + ROOK_X_OFFSET,
		    y + ROOK_Y_OFFSET,
		    x + ROOK_X_OFFSET + ROOK_B_WIDTH,
		    y + ROOK_Y_OFFSET + ROOK_B_HEIGHT);
	    glRectd(x + ROOK_X_OFFSET + (ROOK_B_WIDTH - ROOK_WIDTH) / 2,
		    y + ROOK_Y_OFFSET + ROOK_B_HEIGHT,
		    x + ROOK_X_OFFSET + (ROOK_B_WIDTH - ROOK_WIDTH) / 2 +
		    ROOK_WIDTH,
		    y + ROOK_Y_OFFSET + ROOK_B_HEIGHT + ROOK_HEIGHT);
	    glRectd(x + ROOK_X_OFFSET + 5 / denominator,
		    y + ROOK_Y_OFFSET + ROOK_B_HEIGHT + ROOK_HEIGHT,
		    x + ROOK_X_OFFSET + 5 / denominator + ROOK_T_WIDTH,
		    y + ROOK_Y_OFFSET + ROOK_B_HEIGHT + ROOK_HEIGHT +
		    ROOK_T_HEIGHT);
	    glColor3f(0.0, 0.0, 0.0);
	    glBegin(GL_LINE_LOOP);
	    glVertex2d(x + ROOK_X_OFFSET, y + ROOK_Y_OFFSET);
	    glVertex2d(x + ROOK_X_OFFSET + ROOK_B_WIDTH, y + ROOK_Y_OFFSET);
	    glVertex2d(x + ROOK_X_OFFSET + ROOK_B_WIDTH,
		       y + ROOK_Y_OFFSET + ROOK_B_HEIGHT);
	    glVertex2d(x + ROOK_X_OFFSET,
		       y + ROOK_Y_OFFSET + ROOK_B_HEIGHT);
	    glEnd();
	    glBegin(GL_LINE_LOOP);
	    glVertex2d(x + ROOK_X_OFFSET + 10 / denominator + ROOK_WIDTH,
		       y + ROOK_Y_OFFSET + ROOK_B_HEIGHT);
	    glVertex2d(x + ROOK_X_OFFSET + 10 / denominator + ROOK_WIDTH,
		       y + ROOK_Y_OFFSET + ROOK_B_HEIGHT + ROOK_HEIGHT);
	    glVertex2d(x + ROOK_X_OFFSET + 10 / denominator,
		       y + ROOK_Y_OFFSET + ROOK_B_HEIGHT + ROOK_HEIGHT);
	    glVertex2d(x + ROOK_X_OFFSET + 10 / denominator,
		       y + ROOK_Y_OFFSET + ROOK_B_HEIGHT);
	    glEnd();
	    glBegin(GL_LINE_LOOP);
	    glVertex2d(x + ROOK_X_OFFSET + 5 / denominator,
		       y + ROOK_Y_OFFSET + ROOK_B_HEIGHT + ROOK_HEIGHT);
	    glVertex2d(x + ROOK_X_OFFSET + 5 / denominator + ROOK_T_WIDTH,
		       y + ROOK_Y_OFFSET + ROOK_B_HEIGHT + ROOK_HEIGHT);
	    glVertex2d(x + ROOK_X_OFFSET + 5 / denominator + ROOK_T_WIDTH,
		       y + ROOK_Y_OFFSET + ROOK_B_HEIGHT + ROOK_HEIGHT +
		       ROOK_T_HEIGHT);
	    glVertex2d(x + ROOK_X_OFFSET + 5 / denominator,
		       y + ROOK_Y_OFFSET + ROOK_B_HEIGHT + ROOK_HEIGHT +
		       ROOK_T_HEIGHT);
	    glEnd();
	  }
	else if(chess && qtchess_validate::isPawn(chess->board[i][j]))
	  {
	    glRectd(x + PAWN_X_OFFSET,
		    y + PAWN_Y_OFFSET,
		    x + PAWN_X_OFFSET + PAWN_WIDTH,
		    y + PAWN_Y_OFFSET + PAWN_HEIGHT);
	    glBegin(GL_POLYGON);
	    glVertex2d(x + PAWN_X_OFFSET + PAWN_WIDTH,
		       y + PAWN_Y_OFFSET + PAWN_HEIGHT);
	    glVertex2d(x + PAWN_X_OFFSET + PAWN_WIDTH + 10 / denominator,
		       y + PAWN_Y_OFFSET + PAWN_HEIGHT + 20 / denominator);
	    glVertex2d(x + PAWN_X_OFFSET + PAWN_WIDTH / 2,
		       y + PAWN_Y_OFFSET + PAWN_HEIGHT + 27 / denominator);
	    glVertex2d(x + PAWN_X_OFFSET - 10 / denominator,
		       y + PAWN_Y_OFFSET + PAWN_HEIGHT + 20 / denominator);
	    glVertex2d(x + PAWN_X_OFFSET, y + PAWN_Y_OFFSET + PAWN_HEIGHT);
	    glEnd();
	    glColor3f(0.0, 0.0, 0.0);
	    glBegin(GL_LINE_LOOP);
	    glVertex2d(x + PAWN_X_OFFSET, y + PAWN_Y_OFFSET);
	    glVertex2d(x + PAWN_X_OFFSET + PAWN_WIDTH, y + PAWN_Y_OFFSET);
	    glVertex2d(x + PAWN_X_OFFSET + PAWN_WIDTH,
		       y + PAWN_Y_OFFSET + PAWN_HEIGHT);
	    glVertex2d(x + PAWN_X_OFFSET, y + PAWN_Y_OFFSET + PAWN_HEIGHT);
	    glEnd();
	    glBegin(GL_LINE_LOOP);
	    glVertex2d(x + PAWN_X_OFFSET + PAWN_WIDTH,
		       y + PAWN_Y_OFFSET + PAWN_HEIGHT);
	    glVertex2d(x + PAWN_X_OFFSET + PAWN_WIDTH + 10 / denominator,
		       y + PAWN_Y_OFFSET + PAWN_HEIGHT + 20 / denominator);
	    glVertex2d(x + PAWN_X_OFFSET + PAWN_WIDTH / 2,
		       y + PAWN_Y_OFFSET + PAWN_HEIGHT + 27 / denominator);
	    glVertex2d(x + PAWN_X_OFFSET - 10 / denominator,
		       y + PAWN_Y_OFFSET + PAWN_HEIGHT + 20 / denominator);
	    glVertex2d(x + PAWN_X_OFFSET, y + PAWN_Y_OFFSET + PAWN_HEIGHT);
	    glEnd();
	  }
      }

  /*
  ** Add the coordinate labels.
  */

  glColor3f(0.0, 0.0, 0.0);
  font.setBold(false);

  for(int m = 1; m <= 8; m++)
    {
      renderText((int) (px + m * block_size - block_size / 2 - 5),
		 (int) (py - 5),
		 QString((char) 96 + m), font);
      renderText((int) (px - 15),
		 (int) (py + m * block_size - block_size / 2 + 5),
		 QString::number(9 - m), font);
    }

  /*
  ** Now add some black borders to the board.
  */

  glBegin(GL_LINES);

  for(i = 0; i < NSQUARES + 1; i++)
    {
      /*
      ** Vertical lines.
      */

      glVertex2d(px + i * block_size, py);
      glVertex2d(px + i * block_size, py + 8 * block_size);

      /*
      ** Horizontal lines.
      */

      glVertex2d(px, py + i * block_size);
      glVertex2d(px + 8 * block_size, py + i * block_size);
    }

  glEnd();
  glFlush();

  /*
  ** Highlight the selected square.
  */

  bool isValid = true;

  if(qtchess_validate::areValidCoordinates((int) point_selected.x,
					   (int) point_selected.y) &&
     qtchess_validate::isKing
     (chess->board
      [(int) point_selected.x][(int) point_selected.y]))
    {
      int piece1 = chess->board[I][J];
      int piece2 = chess->board
	[(int) point_selected.x][(int) point_selected.y];

      chess->board[I][J] = piece2;
      chess->board
	[(int) point_selected.x][(int) point_selected.y] =
	EMPTY_SQUARE;
      isValid = !qtchess_validate::isThreatened
	(I, J, qtchess_validate::color(piece2) == BLACK ?
	 WHITE : BLACK);
      chess->board[I][J] = piece1;
      chess->board
	[(int) point_selected.x][(int) point_selected.y] =
	piece2;
    }

#ifdef _DEBUG_
  if(chess && mouse_pressed && found)
#else
  if(chess && mouse_pressed && found && chess->isReady())
#endif
    {
      /*
      ** Set the last square's border to its original color (black).
      */

      if(mouse_pressed == 1 && qtchess_validate::isEmpty(chess->board[I][J]))
	{
	  mouse_pressed = 0;
	  return;
	}
      else if(mouse_pressed == 1)
	{
	  if(chess->getMyColor() == WHITE &&
	     !qtchess_validate::isWhite(chess->board[I][J]))
	    {
	      mouse_pressed = 0;
	      point_selected.x = -1;
	      point_selected.y = -1;
	      glColor3f(1.0, 0.0, 0.0);
	    }
	  else if(chess->getMyColor() == BLACK &&
		  !qtchess_validate::isBlack(chess->board[I][J]))
	    {
	      mouse_pressed = 0;
	      point_selected.x = -1;
	      point_selected.y = -1;
	      glColor3f(1.0, 0.0, 0.0);
	    }
	  else
	    {
	      point_selected.x = I;
	      point_selected.y = J;
	      glColor3f(0.0, 1.0, 0.0);
	    }
	}
      else if(mouse_pressed == 2 && point_selected.x == I &&
	      point_selected.y == J)
	{
	  mouse_pressed = 1;
	  glColor3f(0.0, 1.0, 0.0);
	}
      else if(mouse_pressed == 2 && isValid &&
	      qtchess_validate::areValidCoordinates((int) point_selected.x,
						    (int) point_selected.y) &&
	      (rc = qtchess_validate::isValidMove
	       ((int) point_selected.y,
		(int) point_selected.x,
		J, I,
		chess->board
		[(int) point_selected.x]
		[(int) point_selected.y])) != INVALID)
	{
	  if(qtchess_validate::isRook1(chess->board
				       [(int) point_selected.x]
				       [(int) point_selected.y]))
	    chess->setRook1HasMoved(true);
	  else if(qtchess_validate::isRook2(chess->board
					    [(int) point_selected.x]
					    [(int) point_selected.y]))
	    chess->setRook2HasMoved(true);
	  else if(qtchess_validate::isKing(chess->board
					   [(int) point_selected.x]
					   [(int) point_selected.y]))
	    chess->setKingHasMoved(true);

	  if(qtchess_validate::isKing(chess->board[I][J]))
	    game_over = true;

	  mouse_pressed = 0;
	  showValid = false;
	  glColor3f(0.0, 1.0, 0.0);
	  current_move.x1 = (int) point_selected.y;
	  current_move.y1 = (int) point_selected.x;
	  current_move.x2 = J;
	  current_move.y2 = I;
	  current_move.piece =
	    chess->board[(int) point_selected.x][(int) point_selected.y];
	  current_move.r_x1 = current_move.r_x2 = current_move.r_y1 =
	    current_move.r_y2 = -1;
	  current_move.promoted = 0;
	  current_move.enpassant = 0;
	  current_move.isOppKingThreat = 0;
	  current_move.pawn_2 = 0;
	  current_move.rook = -1;

	  for(i = 0; i < NSQUARES; i++)
	    for(j = 0; j < NSQUARES; j++)
	      oldBoard[i][j] = chess->board[i][j];

	  /*
	  ** Piece promotion?
	  */

	  if(qtchess_validate::isPawn
	     (chess->board[(int) point_selected.x][(int) point_selected.y]) &&
	     (J == 0 || J == 7) &&
	     !qtchess_validate::isKing(chess->board[I][J]))
	    {
	      current_move.promoted = 1;

	      if(gui && gui->getPromoteDialog())
		gui->getPromoteDialog()->setup();

	      if(gui && gui->getPromoteDialog() &&
		 gui->getPromoteDialog()->getMenu() &&
		 gui->getPromoteDialog()->getMenu()->currentIndex() == 0)
		{
		  if(qtchess_validate::isBlack
		     (chess->
		      board[(int) point_selected.x][(int) point_selected.y]))
		    chess->board[I][J] = current_move.piece = BISHOP_BLACK;
		  else
		    chess->board[I][J] = current_move.piece = BISHOP_WHITE;
		}
	      else if(gui && gui->getPromoteDialog() &&
		      gui->getPromoteDialog()->getMenu() &&
		      gui->getPromoteDialog()->getMenu()->currentIndex() == 1)
		{
		  if(qtchess_validate::isBlack
		     (chess->
		      board[(int) point_selected.x][(int) point_selected.y]))
		    chess->board[I][J] = current_move.piece = KNIGHT_BLACK;
		  else
		    chess->board[I][J] = current_move.piece = KNIGHT_WHITE;
		}
	      else if(gui && gui->getPromoteDialog() &&
		      gui->getPromoteDialog()->getMenu() &&
		      gui->getPromoteDialog()->getMenu()->currentIndex() == 2)
		{
		  if(qtchess_validate::isBlack
		     (chess->
		      board[(int) point_selected.x][(int) point_selected.y]))
		    chess->board[I][J] = current_move.piece = QUEEN_BLACK;
		  else
		    chess->board[I][J] = current_move.piece = QUEEN_WHITE;
		}
	      else
		{
		  if(qtchess_validate::isBlack
		     (chess->
		      board[(int) point_selected.x][(int) point_selected.y]))
		    chess->board[I][J] = current_move.piece = ROOK_BLACK;
		  else
		    chess->board[I][J] = current_move.piece = ROOK_WHITE;
		}
	    }
	  else
	    chess->board[I][J] = chess->
	      board[(int) point_selected.x][(int) point_selected.y];

	  if(rc == VALID_CASTLE)
	    {
	      if(I == 2 && J == 7) // Rook #1 Black
		{
		  chess->board[3][7] = ROOK1_BLACK;
		  chess->board[0][7] = EMPTY_SQUARE;
		  current_move.r_x1 = 7;
		  current_move.r_y1 = 3;
		  current_move.r_x2 = 7;
		  current_move.r_y2 = 0;
		  current_move.rook = ROOK1_BLACK;
		}
	      else if(I == 6 && J == 7) // Rook #2 Black
		{
		  chess->board[5][7] = ROOK2_BLACK;
		  chess->board[7][7] = EMPTY_SQUARE;
		  current_move.r_x1 = 7;
		  current_move.r_y1 = 5;
		  current_move.r_x2 = 7;
		  current_move.r_y2 = 7;
		  current_move.rook = ROOK2_BLACK;
		}
	      else if(I == 2 && J == 0) // Rook #1 White
		{
		  chess->board[3][0] = ROOK1_WHITE;
		  chess->board[0][0] = EMPTY_SQUARE;
		  current_move.r_x1 = 0;
		  current_move.r_y1 = 3;
		  current_move.r_x2 = 0;
		  current_move.r_y2 = 0;
		  current_move.rook = ROOK1_WHITE;
		}
	      else if(I == 6 && J == 0) // Rook #2 White
		{
		  chess->board[5][0] = ROOK2_WHITE;
		  chess->board[7][0] = EMPTY_SQUARE;
		  current_move.r_x1 = 0;
		  current_move.r_y1 = 5;
		  current_move.r_x2 = 0;
		  current_move.r_y2 = 7;
		  current_move.rook = ROOK2_WHITE;
		}
	    }
	  else if(rc == VALID_PAWN_2)
	    current_move.pawn_2 = 1;
	  else if(rc == VALID_EN_PASSANT)
	    {
	      current_move.enpassant = 1;

	      if(point_selected.y < J)
		chess->board[I][J - 1] = EMPTY_SQUARE;
	      else
		chess->board[I][J + 1] = EMPTY_SQUARE;
	    }

	  chess->board[(int) point_selected.x][(int) point_selected.y] =
	    EMPTY_SQUARE;
	  origPiece = chess->board[I][J];
	  chess->board[I][J] = EMPTY_SQUARE;
	  snprintf
	    (current_move.departure, sizeof(current_move.departure), "%s",
	     qtchess_validate::findDeparture
	     ((int) point_selected.x, (int) point_selected.y,
	      I, J, origPiece).toLatin1().constData());
	  chess->board[I][J] = origPiece;

	  for(i = 0; i < NSQUARES; i++)
	    for(j = 0; j < NSQUARES; j++)
	      current_move.board[i][j] = chess->board[i][j];

	  for(i = 0; i < NSQUARES; i++)
	    for(j = 0; j < NSQUARES; j++)
	      {
		if(chess->board[i][j] != EMPTY_SQUARE)
		  nonEmptyThen += 1;

		if(oldBoard[i][j] != EMPTY_SQUARE)
		  nonEmptyNow += 1;
	      }

	  if(nonEmptyNow != nonEmptyThen)
	    chess->setWonPiece(true);
	  else
	    chess->setWonPiece(false);

	  if(qtchess_validate::isKingChecked(current_move))
	    current_move.isOppKingThreat = 1;
	  else
	    current_move.isOppKingThreat = 0;

	  if(gui)
	    gui->addHistoryMove
	      (current_move,
	       qtchess_validate::isWhite(current_move.piece) ? WHITE : BLACK);

	  /*
	  ** Send the move.
	  */

	  if(comm)
	    comm->sendMove(current_move);

	  updateGL();
	  update();
	  chess->setGameOver(game_over);

	  if(game_over)
	    if(gui)
	      gui->showGameOver(chess->getTurn());

	  return;
	}
      else
	{
	  mouse_pressed = 0;
	  point_selected.x = -1;
	  point_selected.y = -1;
	  glColor3f(1.0, 0.0, 0.0);
	}

      highlightSquare(X, Y);
    }
  else
    mouse_pressed = 0;

  glFlush();
}

void openglWid::initializeGL(void)
{
  /*
  ** Initiaze OpenGL.
  */

  /*
  ** Set a white background color.
  */

  if(gui)
    gui->getUI().boardFrame->palette();

  glClearColor(1.0, 1.0, 1.0, 1.0);
  glShadeModel(GL_FLAT);
}

void openglWid::resizeGL(int w, int h)
{
  px = (w - 8 * block_size) / 2;
  py = (h - 8 * block_size) / 2;
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, w, 0.0, h, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
}

void openglWid::mousePressEvent(QMouseEvent *e)
{
  if(e && e->type() == QEvent::MouseButtonDblClick)
    showValidMoves();
  else
    {
      showValid = false;
      mouse_pressed += 1;

      if(e)
	{
	  point_pressed.x = e->x();
	  point_pressed.y = height() - e->y();
	}

      updateGL();
    }
}

void openglWid::rescale(const double denominatorArg)
{
  mouse_pressed = 0;

  if(denominatorArg != 0.0)
    {
      denominator = denominatorArg;
      px /= denominator;
      py /= denominator;
      block_size = block_size / denominator;
      PAWN_WIDTH /= denominator;
      PAWN_HEIGHT /= denominator;
      PAWN_X_OFFSET /= denominator;
      PAWN_Y_OFFSET /= denominator;
      BISHOP_WIDTH /= denominator;
      BISHOP_HEIGHT /= denominator;
      BISHOP_X_OFFSET /= denominator;
      BISHOP_Y_OFFSET /= denominator;
      KING_X_OFFSET /= denominator;
      KING_Y_OFFSET /= denominator;
      KING_B_WIDTH /= denominator;
      KING_B_HEIGHT /= denominator;
      KING_BT_WIDTH /= denominator;
      KING_BT_HEIGHT /= denominator;
      KING_WIDTH /= denominator;
      KING_HEIGHT /= denominator;
      KING_HCROSS_WIDTH /= denominator;
      KING_HCROSS_HEIGHT /= denominator;
      KING_VCROSS_WIDTH /= denominator;
      KING_VCROSS_HEIGHT /= denominator;
      KNIGHT_WIDTH /= denominator;
      KNIGHT_HEIGHT /= denominator;
      KNIGHT_X_OFFSET /= denominator;
      KNIGHT_Y_OFFSET /= denominator;
      ROOK_WIDTH /= denominator;
      ROOK_HEIGHT /= denominator;
      ROOK_B_WIDTH /= denominator;
      ROOK_B_HEIGHT /= denominator;
      ROOK_T_WIDTH /= denominator;
      ROOK_T_HEIGHT /= denominator;
      ROOK_X_OFFSET /= denominator;
      ROOK_Y_OFFSET /= denominator;
      QUEEN_X_OFFSET /= denominator;
      QUEEN_Y_OFFSET /= denominator;
      QUEEN_B_WIDTH /= denominator;
      QUEEN_B_HEIGHT /= denominator;
      QUEEN_BT_WIDTH /= denominator;
      QUEEN_BT_HEIGHT /= denominator;
      QUEEN_WIDTH /= denominator;
      QUEEN_HEIGHT /= denominator;
    }

  point_pressed.x = point_pressed.y = -1;
  point_selected.x = point_selected.y = -1;
  updateGL();
  update();
}

void openglWid::reinit(void)
{
  showValid = false;
  PAWN_WIDTH = 18;
  PAWN_HEIGHT = 24;
  PAWN_X_OFFSET = 30;
  PAWN_Y_OFFSET = 5;
  BISHOP_WIDTH = 42;
  BISHOP_HEIGHT = 68;
  BISHOP_X_OFFSET = 20;
  BISHOP_Y_OFFSET = 5;
  KING_X_OFFSET = 20;
  KING_Y_OFFSET = 5;
  KING_B_WIDTH = 42;
  KING_B_HEIGHT = 10;
  KING_BT_WIDTH = 32;
  KING_BT_HEIGHT = 5;
  KING_WIDTH = 26;
  KING_HEIGHT = 30;
  KING_HCROSS_WIDTH = 26;
  KING_HCROSS_HEIGHT = 10;
  KING_VCROSS_WIDTH = 14;
  KING_VCROSS_HEIGHT = 20;
  KNIGHT_WIDTH = 42;
  KNIGHT_HEIGHT = 64;
  KNIGHT_X_OFFSET = 20;
  KNIGHT_Y_OFFSET = 5;
  ROOK_WIDTH = 22;
  ROOK_HEIGHT = 32;
  ROOK_B_WIDTH = 42;
  ROOK_B_HEIGHT = 10;
  ROOK_T_WIDTH = 32;
  ROOK_T_HEIGHT = 20;
  ROOK_X_OFFSET = 20;
  ROOK_Y_OFFSET = 5;
  QUEEN_X_OFFSET = 20;
  QUEEN_Y_OFFSET = 5;
  QUEEN_B_WIDTH = 42;
  QUEEN_B_HEIGHT = 10;
  QUEEN_BT_WIDTH = 32;
  QUEEN_BT_HEIGHT = 5;
  QUEEN_WIDTH = 26;
  QUEEN_HEIGHT = 30;
  mouse_pressed = 0;
  point_pressed.x = point_pressed.y = -1;
  point_selected.x = point_selected.y = -1;
  denominator = 1.0;
  px = py = 4; // Reasonable default value.
  block_size = 80; // Good default value.
}

openglWid::openglWid(QWidget *parent):QGLWidget(parent)
{
  showValid = false;
  mouse_pressed = 0;
  reinit();
  point_pressed.x = point_pressed.y = -1;
  setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
}

void openglWid::showValidMoves(void)
{
  if(!chess || !comm)
    return;

  if(chess->getTurn() == MY_TURN && !chess->isGameOver() &&
     comm->isReady())
    {
      showValid = true;
      updateGL();
    }
}

void openglWid::highlightSquare(const double x, const double y)
{
  glLineWidth(2.0);
  glBegin(GL_LINES);

  /*
  ** Horizontal lines.
  */

  glVertex2d(x + 2, y + 2);
  glVertex2d(x + block_size - 2, y + 2);
  glVertex2d(x + 2, y + block_size - 2);
  glVertex2d(x + block_size - 2, y + block_size - 2);

  /*
  ** Vertical lines.
  */

  glVertex2d(x + 2 , y + 2);
  glVertex2d(x + 2, y + block_size - 2);
  glVertex2d(x + block_size - 2, y + 2);
  glVertex2d(x + block_size - 2, y + block_size - 2);
  glEnd();
  glFlush();
}
