/*
** Copyright (c) 2003 - present, Alexis Megas.
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
** 3. The name of the author may not be used to endorse or promote products
**    derived from QtChess without specific prior written permission.
**
** QTCHESS IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
** IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
** OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
** IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
** INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
** NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
** QTCHESS, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <QMouseEvent>

#ifdef Q_PROCESSOR_ARM
#include <GL/gl.h>
#endif

#include "qtchess.h"
#include "qtchess_comm.h"
#include "qtchess_gui.h"
#include "qtchess_gui_opengl.h"
#include "qtchess_validate.h"

extern qtchess *chess;
extern qtchess_comm *comm;
extern qtchess_gui *gui;

openglWid::openglWid(QWidget *parent):QGLWidget(parent)
{
  mouse_pressed = 0;
  showValid = false;
  reinit();
  point_pressed.x = point_pressed.y = -1;
  setFormat(QGLFormat(QGL::DepthBuffer | QGL::DoubleBuffer));
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

void openglWid::initializeGL(void)
{
  /*
  ** Initialize OpenGL.
  */

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(1.0, 1.0, 1.0, 1.0); // White background.
  glEnable(GL_BLEND);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POINT_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
  glShadeModel(GL_FLAT);
}

void openglWid::mousePressEvent(QMouseEvent *e)
{
  if(e && e->type() == QEvent::MouseButtonDblClick)
    showValidMoves();
  else
    {
      mouse_pressed += 1;
      showValid = false;

      if(e)
	{
	  point_pressed.x = e->x();
	  point_pressed.y = height() - e->y();
	}

      updateGL();
    }
}

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
  double X = 0.0, Y = 0.0;
  int I = 0, J = 0, found = 0;
  struct move_s current_move = {};

  /*
  ** All the drawing occurs here.
  */

  glLineWidth(1.0);
  glPointSize(2.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen.

  /*
  ** First an empty board.
  */

  int auto_screen_scale_factor =
    qgetenv("QT_AUTO_SCREEN_SCALE_FACTOR").toInt();

  for(int i = 0; i < NSQUARES; i++)
    for(int j = 0; j < NSQUARES; j++)
      {
	if((i + j) % 2 != 0)
	  glColor3f((GLfloat) 0.8, (GLfloat) 0.7, (GLfloat) 0.5);
	else
	  glColor3f((GLfloat) 0.5, (GLfloat) 0.5, (GLfloat) 0.6);

	int x = static_cast<int> (px + i * block_size);
	int y = static_cast<int> (py + j * block_size);

	glRectd((double) x,
		(double) y,
		px + (i + 1) * block_size,
		py + (j + 1) * block_size);

	if(auto_screen_scale_factor == 0)
	  {
	    if(mouse_pressed)
	      if(point_pressed.x >= (px + i * block_size) &&
		 point_pressed.x <= (px + (i + 1) * block_size) &&
		 point_pressed.y >= (py + j * block_size) &&
		 point_pressed.y <= (py + (j + 1) * block_size))
		{
		  I = i;
		  J = j;
		  X = px + i * block_size;
		  Y = py + j * block_size;
		  found = 1;
		}
	  }
	else
	  {
	    if(mouse_pressed)
	      {
		int x1 = static_cast<int> (px + i * block_size / 2);
		int x2 = static_cast<int> (px + (i + 1) * block_size / 2);
		int y1 = static_cast<int> (py + j * block_size / 2);
		int y2 = static_cast<int> (py + (j + 1) * block_size / 2);

		if(point_pressed.x >= x1 && point_pressed.x <= x2 &&
		   point_pressed.y >= y1 && point_pressed.y <= y2)
		  {
		    I = i;
		    J = j;
		    X = px + i * block_size;
		    Y = py + j * block_size;
		    found = 1;
		  }
	      }
	  }

	/*
	** Highlight the selected piece's valid moves.
	*/

	if(showValid)
	  if(chess &&
	     qtchess_validate::
	     areValidCoordinates((int) point_selected.x,
				 (int) point_selected.y) &&
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
  font.setStyleStrategy
    (QFont::StyleStrategy(QFont::OpenGLCompatible | QFont::PreferAntialias));

  for(int m = 1; m <= NSQUARES; m++)
    if(auto_screen_scale_factor == 0)
      {
	renderText((int) (px + m * block_size - block_size / 2 - 5),
		   (int) (py - 5),
		   QString((char) 96 + m), font);
	renderText((int) (px - 15),
		   (int) (py + m * block_size - block_size / 2 + 5),
		   QString::number(9 - m), font);
      }
    else
      {
	renderText((int) (px + m * block_size / 2 - block_size / 2 + 20),
		   (int) (py - 20),
		   QString((char) 96 + m), font);
	renderText((int) (px - 25),
		   (int) (py + m * block_size / 2 - block_size / 2 + 30),
		   QString::number(9 - m), font);
      }

  /*
  ** Now add some black borders to the board.
  */

  glBegin(GL_LINES);

  for(int i = 0; i < NSQUARES + 1; i++)
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

#ifdef QTCHESS_DEBUG
  if(chess && mouse_pressed && found)
#else
  if(chess && mouse_pressed && found && chess->isReady())
#endif
    {
      /*
      ** Set the last square's border to its original color (black).
      */

      int rc = 0;

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
      else if(mouse_pressed == 2 &&
	      qFuzzyCompare(point_selected.x, static_cast<double> (I)) &&
	      qFuzzyCompare(point_selected.y, static_cast<double> (J)))
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

	  bool game_over = false;

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

	  int oldBoard[NSQUARES][NSQUARES];

	  for(int i = 0; i < NSQUARES; i++)
	    for(int j = 0; j < NSQUARES; j++)
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

	  int origPiece = chess->board[I][J];

	  chess->board[I][J] = EMPTY_SQUARE;
	  snprintf
	    (current_move.departure, sizeof(current_move.departure), "%s",
	     qtchess_validate::findDeparture
	     ((int) point_selected.x, (int) point_selected.y,
	      I, J, origPiece).toLatin1().constData());
	  chess->board[I][J] = origPiece;

	  for(int i = 0; i < NSQUARES; i++)
	    for(int j = 0; j < NSQUARES; j++)
	      current_move.board[i][j] = chess->board[i][j];

	  int nonEmptyNow = 0, nonEmptyThen = 0;

	  for(int i = 0; i < NSQUARES; i++)
	    for(int j = 0; j < NSQUARES; j++)
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
	  if((chess->getMyColor() == BLACK &&
	      qtchess_validate::isBlack(chess->board[I][J])) ||
	     (chess->getMyColor() == WHITE &&
	      qtchess_validate::isWhite(chess->board[I][J])))
	    {
	      /*
	      ** Allow selection of another similar piece.
	      */

	      mouse_pressed = 1;
	      point_selected.x = I;
	      point_selected.y = J;
	      glColor3f(0.0, 1.0, 0.0);
	    }
	  else
	    {
	      mouse_pressed = 0;
	      point_selected.x = -1;
	      point_selected.y = -1;
	      glColor3f(1.0, 0.0, 0.0);
	    }
	}

      highlightSquare(X, Y);
    }
  else
    mouse_pressed = 0;

  glFlush();
}

void openglWid::reinit(void)
{
  BISHOP_HEIGHT = 68;
  BISHOP_WIDTH = 42;
  BISHOP_X_OFFSET = 20;
  BISHOP_Y_OFFSET = 5;
  KING_BT_HEIGHT = 5;
  KING_BT_WIDTH = 32;
  KING_B_HEIGHT = 10;
  KING_B_WIDTH = 42;
  KING_HCROSS_HEIGHT = 10;
  KING_HCROSS_WIDTH = 26;
  KING_HEIGHT = 30;
  KING_VCROSS_HEIGHT = 20;
  KING_VCROSS_WIDTH = 14;
  KING_WIDTH = 26;
  KING_X_OFFSET = 20;
  KING_Y_OFFSET = 5;
  KNIGHT_HEIGHT = 64;
  KNIGHT_WIDTH = 42;
  KNIGHT_X_OFFSET = 20;
  KNIGHT_Y_OFFSET = 5;
  PAWN_HEIGHT = 24;
  PAWN_WIDTH = 18;
  PAWN_X_OFFSET = 30;
  PAWN_Y_OFFSET = 5;
  QUEEN_BT_HEIGHT = 5;
  QUEEN_BT_WIDTH = 32;
  QUEEN_B_HEIGHT = 10;
  QUEEN_B_WIDTH = 42;
  QUEEN_HEIGHT = 30;
  QUEEN_WIDTH = 26;
  QUEEN_X_OFFSET = 20;
  QUEEN_Y_OFFSET = 5;
  ROOK_B_HEIGHT = 10;
  ROOK_B_WIDTH = 42;
  ROOK_HEIGHT = 32;
  ROOK_T_HEIGHT = 20;
  ROOK_T_WIDTH = 32;
  ROOK_WIDTH = 22;
  ROOK_X_OFFSET = 20;
  ROOK_Y_OFFSET = 5;
  block_size = 80; // Good default value.
  denominator = 1.0;
  mouse_pressed = 0;
  point_pressed.x = point_pressed.y = -1;
  point_selected.x = point_selected.y = -1;
  px = py = 4; // Reasonable default value.
  showValid = false;
}

void openglWid::rescale(const double denominatorArg)
{
  denominator = denominatorArg;

  if(denominator <= 0.0)
    denominator = 1.0;

  BISHOP_HEIGHT /= denominator;
  BISHOP_WIDTH /= denominator;
  BISHOP_X_OFFSET /= denominator;
  BISHOP_Y_OFFSET /= denominator;
  KING_BT_HEIGHT /= denominator;
  KING_BT_WIDTH /= denominator;
  KING_B_HEIGHT /= denominator;
  KING_B_WIDTH /= denominator;
  KING_HCROSS_HEIGHT /= denominator;
  KING_HCROSS_WIDTH /= denominator;
  KING_HEIGHT /= denominator;
  KING_VCROSS_HEIGHT /= denominator;
  KING_VCROSS_WIDTH /= denominator;
  KING_WIDTH /= denominator;
  KING_X_OFFSET /= denominator;
  KING_Y_OFFSET /= denominator;
  KNIGHT_HEIGHT /= denominator;
  KNIGHT_WIDTH /= denominator;
  KNIGHT_X_OFFSET /= denominator;
  KNIGHT_Y_OFFSET /= denominator;
  PAWN_HEIGHT /= denominator;
  PAWN_WIDTH /= denominator;
  PAWN_X_OFFSET /= denominator;
  PAWN_Y_OFFSET /= denominator;
  QUEEN_BT_HEIGHT /= denominator;
  QUEEN_BT_WIDTH /= denominator;
  QUEEN_B_HEIGHT /= denominator;
  QUEEN_B_WIDTH /= denominator;
  QUEEN_HEIGHT /= denominator;
  QUEEN_WIDTH /= denominator;
  QUEEN_X_OFFSET /= denominator;
  QUEEN_Y_OFFSET /= denominator;
  ROOK_B_HEIGHT /= denominator;
  ROOK_B_WIDTH /= denominator;
  ROOK_HEIGHT /= denominator;
  ROOK_T_HEIGHT /= denominator;
  ROOK_T_WIDTH /= denominator;
  ROOK_WIDTH /= denominator;
  ROOK_X_OFFSET /= denominator;
  ROOK_Y_OFFSET /= denominator;
  block_size /= denominator;
  mouse_pressed = 0;
  point_pressed.x = point_pressed.y = -1;
  point_selected.x = point_selected.y = -1;
  px /= denominator;
  py /= denominator;
  updateGL();
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

void openglWid::showValidMoves(void)
{
  if(!chess || !comm)
    return;

  if(chess->getTurn() == MY_TURN && !chess->isGameOver() && comm->isReady())
    {
      showValid = true;
      updateGL();
    }
}
