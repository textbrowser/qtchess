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

#ifndef _QTCHESS_GUI_OPENGL_
#define _QTCHESS_GUI_OPENGL_

#include <QEvent>
#include <QMouseEvent>
#if QT_VERSION < 0x050400
#include <QtOpenGL>
#else
#include <QOpenGLWidget>
#endif

class point
{
 public:
  double x;
  double y;
};

#if QT_VERSION < 0x050400
class openglWid: public QGLWidget
#else
class openglWid: public QOpenGLWidget
#endif
{
 public:
  openglWid(QWidget *);
  void highlightSquare(const double, const double);
  void initializeGL(void);
  void mousePressEvent(QMouseEvent *);
  void newGame(void);
  void paintGL(void);
  void reinit(void);
  void rescale(const double);
  void resizeGL(int, int);
  void showValidMoves(void);

 private:
  bool showValid;
  double BISHOP_HEIGHT;
  double BISHOP_WIDTH;
  double BISHOP_X_OFFSET;
  double BISHOP_Y_OFFSET;
  double KING_B_HEIGHT;
  double KING_B_WIDTH;
  double KING_BT_HEIGHT;
  double KING_BT_WIDTH;
  double KING_HCROSS_HEIGHT;
  double KING_HCROSS_WIDTH;
  double KING_HEIGHT;
  double KING_VCROSS_HEIGHT;
  double KING_VCROSS_WIDTH;
  double KING_WIDTH;
  double KING_X_OFFSET;
  double KING_Y_OFFSET;
  double KNIGHT_HEIGHT;
  double KNIGHT_WIDTH;
  double KNIGHT_X_OFFSET;
  double KNIGHT_Y_OFFSET;
  double PAWN_HEIGHT;
  double PAWN_WIDTH;
  double PAWN_X_OFFSET;
  double PAWN_Y_OFFSET;
  double QUEEN_B_HEIGHT;
  double QUEEN_B_WIDTH;
  double QUEEN_BT_HEIGHT;
  double QUEEN_BT_WIDTH;
  double QUEEN_HEIGHT;
  double QUEEN_WIDTH;
  double QUEEN_X_OFFSET;
  double QUEEN_Y_OFFSET;
  double ROOK_B_HEIGHT;
  double ROOK_B_WIDTH;
  double ROOK_HEIGHT;
  double ROOK_T_HEIGHT;
  double ROOK_T_WIDTH;
  double ROOK_WIDTH;
  double ROOK_X_OFFSET;
  double ROOK_Y_OFFSET;
  double block_size;
  double denominator;
  double px; // Used for centering the board.
  double py; // Used for centering the board.
  int mouse_pressed;
  point point_pressed;
  point point_selected;
};

#endif
