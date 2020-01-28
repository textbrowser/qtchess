#ifndef _QTCHESS_GUI_OPENGL_
#define _QTCHESS_GUI_OPENGL_

#include <QEvent>
#include <QMouseEvent>
#include <QtOpenGL>

class point
{
 public:
  double x;
  double y;
};

class openglWid: public QOpenGLWidget
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
  void renderText(const int x,
		  const int y,
		  const QString &text,
		  const QFont &font);
};

#endif
