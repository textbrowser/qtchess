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

#include "qtchess.h"
#include "qtchess_comm.h"
#include "qtchess_gui.h"
#include "qtchess_gui_opengl.h"
#include "qtchess_validate.h"

extern QPointer<qtchess> chess;
extern QPointer<qtchess_comm> comm;
extern QPointer<qtchess_gui> gui;

openglWid::openglWid(QObject *parent):QObject(parent)
{
  for(int i = 0; i < NSQUARES; i++)
    for(int j = 0; j < NSQUARES; j++)
      {
	m_labels[i][j] = new qtchess_piece(i, j, this);
	connect(m_labels[i][j],
		SIGNAL(doubleClicked(qtchess_piece *)),
		this,
		SLOT(slotPieceDoubleClicked(qtchess_piece *)));
	connect(m_labels[i][j],
		SIGNAL(pressed(qtchess_piece *)),
		this,
		SLOT(slotPiecePressed(qtchess_piece *)));

	QColor color;
	auto font(m_labels[i][j]->font());

	font.setPointSize(25);
	m_labels[i][j]->setAlignment(Qt::AlignCenter);
	m_labels[i][j]->setContentsMargins(0, 0, 0, 0);
	m_labels[i][j]->setFont(font);
	m_labels[i][j]->setSizePolicy
	  (QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

	if((i + j) % 2 == 0)
	  color = QColor(255, 87, 51);
	else
	  color = QColor(255, 255, 237);

	m_labels[i][j]->setStyleSheet
	  (QString("QLabel {background-color: %1; border: 1px solid navy;}").
	   arg(color.name()));
	m_labels[i][j]->setTextFormat(Qt::RichText);
      }

  mouse_pressed = 0;
  showValid = false;
  reinit();
  point_pressed.x = point_pressed.y = -1;
}

void openglWid::add(QFrame *frame)
{
  if(!frame)
    return;

  auto layout = qobject_cast<QGridLayout *> (frame->layout());

  if(!layout || layout->count() > 0)
    return;

  layout->setSpacing(0);

  for(int i = 0; i < NSQUARES; i++)
    for(int j = 0; j < NSQUARES; j++)
      layout->addWidget(m_labels[i][j], i, j);

  paint();
}

void openglWid::highlightSquare(const int i, const int j)
{
  if(i >= 0 && i < NSQUARES && j >= 0 && j < NSQUARES)
    m_labels[i][j]->setStyleSheet
      ("QLabel {background-color: orange; border: 1px solid navy;}");
}

void openglWid::newGame(void)
{
  mouse_pressed = 0;
  point_pressed.x = point_pressed.y = -1;
  point_selected.x = point_selected.y = -1;
  showValid = false;
  paint();
}

void openglWid::paint(void)
{
  QFont font;
  int I = 0, J = 0, found = 0;
  struct move_s current_move = {};

  for(int i = 0; i < NSQUARES; i++)
    for(int j = 0; j < NSQUARES; j++)
      {
	if(mouse_pressed)
	  if(i == point_pressed.x && j == point_pressed.y)
	    {
	      I = i;
	      J = j;
	      found = 1;
	    }

	/*
	** Highlight the selected piece's valid moves.
	*/

	if(showValid)
	  if(chess &&
	     qtchess_validate::
	     areValidCoordinates(point_selected.x,
				 point_selected.y) &&
	     qtchess_validate::isValidMove
	     (point_selected.y,
	      point_selected.x,
	      j, i,
	      chess->board
	      [point_selected.x]
	      [point_selected.y]) != INVALID)
	    {
	      bool isValid = true;

	      if(qtchess_validate::isKing
		 (chess->board
		  [point_selected.x][point_selected.y]))
		{
		  int piece1 = chess->board[i][j];
		  int piece2 = chess->board
		    [point_selected.x][point_selected.y];

		  chess->board[i][j] = piece2;
		  chess->board
		    [point_selected.x][point_selected.y] =
		    EMPTY_SQUARE;
		  isValid = !qtchess_validate::isThreatened
		    (i, j, qtchess_validate::color(piece2) == BLACK ?
		     WHITE : BLACK);
		  chess->board[i][j] = piece1;
		  chess->board
		    [point_selected.x][point_selected.y] =
		    piece2;
		}

	      if(isValid)
		{
		}
	    }

	/*
	** Label the pieces.
	*/

	int offset = 0;
	int piece = 0;

	if(chess && !qtchess_validate::isEmpty(chess->board[i][j]))
	  {
	    if(qtchess_validate::isWhite(chess->board[i][j]))
	      offset = 0;
	    else
	      offset = 6;
	  }

	if(chess && qtchess_validate::isBishop(chess->board[i][j]))
	  piece = 9815 + offset;
	else if(chess && qtchess_validate::isKing(chess->board[i][j]))
	  piece = 9812 + offset;
	else if(chess && qtchess_validate::isKnight(chess->board[i][j]))
	  piece = 9816 + offset;
	else if(chess && qtchess_validate::isPawn(chess->board[i][j]))
	  piece = 9817 + offset;
	else if(chess && qtchess_validate::isQueen(chess->board[i][j]))
	  piece = 9813 + offset;
	else if(chess && qtchess_validate::isRook(chess->board[i][j]))
	  piece = 9814 + offset;

	if(i >= 0 && i < NSQUARES && j >= 0 && j < NSQUARES && piece > 0)
	  m_labels[j][i]->setText(QString("&#%1;").arg(piece));
      }

  /*
  ** Add the coordinate labels.
  */

  font.setBold(false);
  font.setStyleStrategy(QFont::StyleStrategy(QFont::PreferAntialias));

  /*
  ** Highlight the selected square.
  */

  bool isValid = true;

  if(qtchess_validate::areValidCoordinates(point_selected.x,
					   point_selected.y) &&
     qtchess_validate::isKing
     (chess->board
      [point_selected.x][point_selected.y]))
    {
      int piece1 = chess->board[I][J];
      int piece2 = chess->board
	[point_selected.x][point_selected.y];

      chess->board[I][J] = piece2;
      chess->board
	[point_selected.x][point_selected.y] =
	EMPTY_SQUARE;
      isValid = !qtchess_validate::isThreatened
	(I, J, qtchess_validate::color(piece2) == BLACK ?
	 WHITE : BLACK);
      chess->board[I][J] = piece1;
      chess->board
	[point_selected.x][point_selected.y] =
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
	    }
	  else if(chess->getMyColor() == BLACK &&
		  !qtchess_validate::isBlack(chess->board[I][J]))
	    {
	      mouse_pressed = 0;
	      point_selected.x = -1;
	      point_selected.y = -1;
	    }
	  else
	    {
	      point_selected.x = I;
	      point_selected.y = J;
	    }
	}
      else if(mouse_pressed == 2 &&
	      qFuzzyCompare(point_selected.x, static_cast<double> (I)) &&
	      qFuzzyCompare(point_selected.y, static_cast<double> (J)))
	{
	  mouse_pressed = 1;
	}
      else if(mouse_pressed == 2 && isValid &&
	      qtchess_validate::areValidCoordinates(point_selected.x,
						    point_selected.y) &&
	      (rc = qtchess_validate::isValidMove
	       (point_selected.y,
		point_selected.x,
		J, I,
		chess->board
		[point_selected.x]
		[point_selected.y])) != INVALID)
	{
	  if(qtchess_validate::isRook1(chess->board
				       [point_selected.x]
				       [point_selected.y]))
	    chess->setRook1HasMoved(true);
	  else if(qtchess_validate::isRook2(chess->board
					    [point_selected.x]
					    [point_selected.y]))
	    chess->setRook2HasMoved(true);
	  else if(qtchess_validate::isKing(chess->board
					   [point_selected.x]
					   [point_selected.y]))
	    chess->setKingHasMoved(true);

	  bool game_over = false;

	  if(qtchess_validate::isKing(chess->board[I][J]))
	    game_over = true;

	  mouse_pressed = 0;
	  showValid = false;
	  current_move.x1 = point_selected.y;
	  current_move.y1 = point_selected.x;
	  current_move.x2 = J;
	  current_move.y2 = I;
	  current_move.piece =
	    chess->board[point_selected.x][point_selected.y];
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
	     (chess->board[point_selected.x][point_selected.y]) &&
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
		      board[point_selected.x][point_selected.y]))
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
		      board[point_selected.x][point_selected.y]))
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
		      board[point_selected.x][point_selected.y]))
		    chess->board[I][J] = current_move.piece = QUEEN_BLACK;
		  else
		    chess->board[I][J] = current_move.piece = QUEEN_WHITE;
		}
	      else
		{
		  if(qtchess_validate::isBlack
		     (chess->
		      board[point_selected.x][point_selected.y]))
		    chess->board[I][J] = current_move.piece = ROOK_BLACK;
		  else
		    chess->board[I][J] = current_move.piece = ROOK_WHITE;
		}
	    }
	  else
	    chess->board[I][J] = chess->
	      board[point_selected.x][point_selected.y];

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

	  chess->board[point_selected.x][point_selected.y] =
	    EMPTY_SQUARE;

	  int origPiece = chess->board[I][J];

	  chess->board[I][J] = EMPTY_SQUARE;
	  snprintf
	    (current_move.departure, sizeof(current_move.departure), "%s",
	     qtchess_validate::findDeparture
	     (point_selected.x, point_selected.y,
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
	    }
	  else
	    {
	      mouse_pressed = 0;
	      point_selected.x = -1;
	      point_selected.y = -1;
	    }
	}

      highlightSquare(I, J);
    }
  else
    mouse_pressed = 0;
}

void openglWid::reinit(void)
{
  for(int i = 0; i < NSQUARES; i++)
    for(int j = 0; j < NSQUARES; j++)
      {
	QColor color;

	if((i + j) % 2 == 0)
	  color = QColor(255, 87, 51);
	else
	  color = QColor(255, 255, 237);

	m_labels[i][j]->clear();
  	m_labels[i][j]->setStyleSheet
	  (QString("QLabel {background-color: %1; border: 1px solid navy;}").
	   arg(color.name()));
      }

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
  mouse_pressed = 0;
  point_pressed.x = point_pressed.y = -1;
  point_selected.x = point_selected.y = -1;
  showValid = false;
}

void openglWid::showValidMoves(void)
{
  if(!chess || !comm)
    return;

#ifdef QTCHESS_DEBUG
  showValid = true;
#else
  if(chess->getTurn() == MY_TURN && !chess->isGameOver() && comm->isReady())
    showValid = true;
#endif

  paint();
}

void openglWid::slotPieceDoubleClicked(qtchess_piece *piece)
{
  if(!piece)
    return;

  showValidMoves();
}

void openglWid::slotPiecePressed(qtchess_piece *piece)
{
#ifdef QTCHESS_DEBUG
  if(!piece)
    return;
#else
  if(!chess || !chess->isGameOver() || !piece)
    return;
#endif

  mouse_pressed = 1;

  for(int i = 0; i < NSQUARES; i++)
    for(int j = 0; j < NSQUARES; j++)
      {
	QColor color;

	if((i + j) % 2 == 0)
	  color = QColor(255, 87, 51);
	else
	  color = QColor(255, 255, 237);

	if(m_labels[i][j] == piece)
	  m_labels[i][j]->setStyleSheet
	    ("QLabel {background-color: orange; border: 1px solid navy;}");
	else
	  m_labels[i][j]->setStyleSheet
	    (QString("QLabel {background-color: %1; border: 1px solid navy;}").
	     arg(color.name()));
      }
}
