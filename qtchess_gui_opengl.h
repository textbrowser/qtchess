#ifndef _QTCHESS_GUI_OPENGL_
#define _QTCHESS_GUI_OPENGL_

/*
** -- Qt Includes --
*/

#include <QEvent>
#include <QtOpenGL>
#include <QMouseEvent>

/*
** -- Class(es) --
*/

class point
{
 public:

  /*
  ** -- Members --
  */

  double x;
  double y;

  /*
  ** -- Methods --
  */
};

class openglWid: public QGLWidget
{
 public:

  /*
  ** -- Members --
  */

  /*
  ** -- Methods --
  */

  openglWid(QWidget *);
  void reinit(void);
  void newGame(void);
  void paintGL(void);
  void rescale(const double);
  void resizeGL(int, int);
  void initializeGL(void);
  void showValidMoves(void);
  void highlightSquare(const double, const double);
  void mousePressEvent(QMouseEvent *);

 private:

  /*
  ** -- Members --
  */

  int mouse_pressed;
  bool showValid;
  point point_pressed;
  point point_selected;
  double denominator;
  double PAWN_WIDTH;
  double PAWN_HEIGHT;
  double PAWN_X_OFFSET;
  double PAWN_Y_OFFSET;
  double BISHOP_WIDTH;
  double BISHOP_HEIGHT;
  double BISHOP_X_OFFSET;
  double BISHOP_Y_OFFSET;
  double KING_X_OFFSET;
  double KING_Y_OFFSET;
  double KING_B_WIDTH;
  double KING_B_HEIGHT;
  double KING_BT_WIDTH;
  double KING_BT_HEIGHT;
  double KING_WIDTH;
  double KING_HEIGHT;
  double KING_HCROSS_WIDTH;
  double KING_HCROSS_HEIGHT;
  double KING_VCROSS_WIDTH;
  double KING_VCROSS_HEIGHT;
  double KNIGHT_WIDTH;
  double KNIGHT_HEIGHT;
  double KNIGHT_X_OFFSET;
  double KNIGHT_Y_OFFSET;
  double ROOK_WIDTH;
  double ROOK_HEIGHT;
  double ROOK_B_WIDTH;
  double ROOK_B_HEIGHT;
  double ROOK_T_WIDTH;
  double ROOK_T_HEIGHT;
  double ROOK_X_OFFSET;
  double ROOK_Y_OFFSET;
  double QUEEN_X_OFFSET;
  double QUEEN_Y_OFFSET;
  double QUEEN_B_WIDTH;
  double QUEEN_B_HEIGHT;
  double QUEEN_BT_WIDTH;
  double QUEEN_BT_HEIGHT;
  double QUEEN_WIDTH;
  double QUEEN_HEIGHT;
  double px; // Used for centering the board.
  double py; // Used for centering the board.
  double block_size;

  /*
  ** -- Methods --
  */
};

#endif
