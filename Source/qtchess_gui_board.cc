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
#include "qtchess_communications.h"
#include "qtchess_gui.h"
#include "qtchess_gui_board.h"
#include "qtchess_validate.h"

extern QPointer<qtchess> chess;
extern QPointer<qtchess_communications> comm;
extern QPointer<qtchess_gui> gui;

qtchess_gui_board::qtchess_gui_board(QObject *parent):QObject(parent)
{
  for(int i = 0; i < NSQUARES; i++)
    for(int j = 0; j < NSQUARES; j++)
      {
	m_labels[i][j] = new qtchess_piece(i, j, this);
	connect(m_labels[i][j],
		SIGNAL(double_clicked(qtchess_piece *)),
		this,
		SLOT(slot_piece_double_clicked(qtchess_piece *)));
	connect(m_labels[i][j],
		SIGNAL(pressed(qtchess_piece *)),
		this,
		SLOT(slot_piece_pressed(qtchess_piece *)));

	QColor color;
	auto font(m_labels[i][j]->font());

	font.setPointSize(30);
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

  m_mouse_pressed = 0;
  initialize();
}

void qtchess_gui_board::add(QFrame *frame)
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

void qtchess_gui_board::highlight_square(const int i, const int j)
{
  if(i >= 0 && i < NSQUARES && j >= 0 && j < NSQUARES)
    m_labels[i][j]->setStyleSheet
      ("QLabel {background-color: orange; border: 1px solid navy;}");
}

void qtchess_gui_board::new_game(void)
{
  m_mouse_pressed = 0;
  m_point_selected.m_x = m_point_selected.m_y = -1;
  paint();
}

void qtchess_gui_board::paint(void)
{
  if(!chess)
    return;

  for(int i = 0; i < NSQUARES; i++)
    for(int j = 0; j < NSQUARES; j++)
      {
	/*
	** Label the pieces.
	*/

	int offset = 0;
	int piece = 0;

	if(!qtchess_validate::is_empty(chess->m_board[i][j]))
	  {
	    if(qtchess_validate::is_white(chess->m_board[i][j]))
	      offset = 0;
	    else
	      offset = 6;
	  }

	if(qtchess_validate::is_bishop(chess->m_board[i][j]))
	  piece = 9815 + offset;
	else if(qtchess_validate::is_king(chess->m_board[i][j]))
	  piece = 9812 + offset;
	else if(qtchess_validate::is_knight(chess->m_board[i][j]))
	  piece = 9816 + offset;
	else if(qtchess_validate::is_pawn(chess->m_board[i][j]))
	  piece = 9817 + offset;
	else if(qtchess_validate::is_queen(chess->m_board[i][j]))
	  piece = 9813 + offset;
	else if(qtchess_validate::is_rook(chess->m_board[i][j]))
	  piece = 9814 + offset;

	if(piece == 0)
	  m_labels[i][j]->clear();
	else
	  m_labels[i][j]->setText(QString("&#%1;").arg(piece));

	QColor color;

	if((i + j) % 2 == 0)
	  color = QColor(255, 87, 51);
	else
	  color = QColor(255, 255, 237);

	m_labels[i][j]->setStyleSheet
	  (QString("QLabel {background-color: %1; border: 1px solid navy;}").
	   arg(color.name()));
      }
}

void qtchess_gui_board::initialize(void)
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

  m_mouse_pressed = 0;
  m_point_selected.m_x = m_point_selected.m_y = -1;
}

void qtchess_gui_board::slot_piece_double_clicked(qtchess_piece *piece)
{
  if(!(QApplication::keyboardModifiers() & Qt::ControlModifier))
    return;

  if(!chess || !piece)
    return;

  auto x = piece->i();
  auto y = piece->j();

  if(x < 0 || x > NSQUARES || y < 0 || y > NSQUARES)
    return;

  if(chess->m_board[x][y] == EMPTY_SQUARE)
    return;

  for(int i = 0; i < NSQUARES; i++)
    for(int j = 0; j < NSQUARES; j++)
      {
	QColor color;

	if((i + j) % 2 == 0)
	  color = QColor(255, 87, 51);
	else
	  color = QColor(255, 255, 237);

	m_labels[i][j]->setStyleSheet
	  (QString("QLabel {background-color: %1; border: 1px solid navy;}").
	   arg(color.name()));

	/*
	** Highlight the selected piece's valid moves.
	*/

	if(qtchess_validate::
	   is_valid_move(y, x, j, i, chess->m_board[x][y]) != INVALID)
	  m_labels[i][j]->setStyleSheet
	    ("QLabel {background-color: orange; border: 1px solid navy;}");
      }

  m_mouse_pressed = 1;
  m_point_selected.m_x = x;
  m_point_selected.m_y = y;
  piece->setStyleSheet
    ("QLabel {background-color: orange; border: 1px solid navy;}");
}

void qtchess_gui_board::slot_piece_pressed(qtchess_piece *piece)
{
  if(QApplication::keyboardModifiers() & Qt::ControlModifier)
    return;

#ifdef QTCHESS_DEBUG
  if(!chess || !gui || !piece)
    return;
#else
  if(!chess ||
     chess->get_turn() != MY_TURN ||
     chess->is_game_over() ||
     !chess->is_ready() ||
     !gui ||
     !piece)
    return;
#endif

  auto x = piece->i();
  auto y = piece->j();

  if(m_mouse_pressed == 1)
    goto move_label;

  if(chess->m_board[x][y] == EMPTY_SQUARE)
    {
      m_mouse_pressed = 0;
      return;
    }
  else if(m_point_selected.m_x == x &&
	  m_point_selected.m_y == y)
    {
      m_mouse_pressed = 0;
      return;
    }

  for(int i = 0; i < NSQUARES; i++)
    for(int j = 0; j < NSQUARES; j++)
      {
	QColor color;

	if((i + j) % 2 == 0)
	  color = QColor(255, 87, 51);
	else
	  color = QColor(255, 255, 237);

	if(m_labels[i][j] == piece)
	  {
	    m_labels[i][j]->setStyleSheet
	      ("QLabel {background-color: orange; border: 1px solid navy;}");
	    m_mouse_pressed = 1;
	    m_point_selected.m_x = i;
	    m_point_selected.m_y = j;
	  }
	else
	  m_labels[i][j]->setStyleSheet
	    (QString("QLabel {background-color: %1; border: 1px solid navy;}").
	     arg(color.name()));
      }

  return;

 move_label:

  int rc = 0;

  if(chess->get_my_color() == BLACK &&
     !qtchess_validate::is_black(chess->m_board[x][y]))
    {
      highlight_square(x, y);
      m_mouse_pressed = 0;
      m_point_selected.m_x = -1;
      m_point_selected.m_y = -1;
    }
  else if(chess->get_my_color() == WHITE &&
	  !qtchess_validate::is_white(chess->m_board[x][y]))
    {
      highlight_square(x, y);
      m_mouse_pressed = 0;
      m_point_selected.m_x = -1;
      m_point_selected.m_y = -1;
    }
  else if((rc = qtchess_validate::is_valid_move
	   (m_point_selected.m_y,
	    m_point_selected.m_x,
	    y, x,
	    chess->m_board[m_point_selected.m_x][m_point_selected.m_y])) !=
	  INVALID)
    {
      if(qtchess_validate::
	 is_king(chess->m_board[m_point_selected.m_x]
		 [m_point_selected.m_y]))
	chess->set_king_has_moved(true);
      else if(qtchess_validate::
	      is_rook1(chess->m_board[m_point_selected.m_x]
		                     [m_point_selected.m_y]))
	chess->set_rook1_has_moved(true);
      else if(qtchess_validate::
	      is_rook2(chess->m_board[m_point_selected.m_x]
		                     [m_point_selected.m_y]))
	chess->set_rook2_has_moved(true);

      auto game_over = false;

      if(qtchess_validate::is_king(chess->m_board[x][y]))
	game_over = true;

      struct move_s current_move = {};

      current_move.m_enpassant = 0;
      current_move.m_is_opponent_king_threat = 0;
      current_move.m_pawn2 = 0;
      current_move.m_piece =
	chess->m_board[m_point_selected.m_x][m_point_selected.m_y];
      current_move.m_promoted = 0;
      current_move.m_rook_x1 =
	current_move.m_rook_x2 =
	current_move.m_rook_y1 =
	current_move.m_rook_y2 = -1;
      current_move.m_rook = -1;
      current_move.m_x1 = m_point_selected.m_y;
      current_move.m_x2 = y;
      current_move.m_y1 = m_point_selected.m_x;
      current_move.m_y2 = x;
      m_mouse_pressed = 0;

      int old_board[NSQUARES][NSQUARES];

      for(int i = 0; i < NSQUARES; i++)
	for(int j = 0; j < NSQUARES; j++)
	  old_board[i][j] = chess->m_board[i][j];

      /*
      ** Piece promotion?
      */

      if(qtchess_validate::is_pawn
	 (chess->m_board[m_point_selected.m_x][m_point_selected.m_y]) &&
	 (y == 0 || y == 7) &&
	 !qtchess_validate::is_king(chess->m_board[x][y]))
	{
	  current_move.m_promoted = 1;

	  if(gui->get_promote_dialog())
	    gui->get_promote_dialog()->setup();

	  if(gui->get_promote_dialog() &&
	     gui->get_promote_dialog()->get_menu() &&
	     gui->get_promote_dialog()->get_menu()->currentIndex() == 0)
	    {
	      if(qtchess_validate::
		 is_black(chess->m_board[m_point_selected.m_x]
			                [m_point_selected.m_y]))
		chess->m_board[x][y] = current_move.m_piece = BISHOP_BLACK;
	      else
		chess->m_board[x][y] = current_move.m_piece = BISHOP_WHITE;
	    }
	  else if(gui->get_promote_dialog() &&
		  gui->get_promote_dialog()->get_menu() &&
		  gui->get_promote_dialog()->get_menu()->currentIndex() == 1)
	    {
	      if(qtchess_validate::
		 is_black(chess->m_board[m_point_selected.m_x]
			                [m_point_selected.m_y]))
		chess->m_board[x][y] = current_move.m_piece = KNIGHT_BLACK;
	      else
		chess->m_board[x][y] = current_move.m_piece = KNIGHT_WHITE;
	    }
	  else if(gui->get_promote_dialog() &&
		  gui->get_promote_dialog()->get_menu() &&
		  gui->get_promote_dialog()->get_menu()->currentIndex() == 2)
	    {
	      if(qtchess_validate::
		 is_black(chess->m_board[m_point_selected.m_x]
			                [m_point_selected.m_y]))
		chess->m_board[x][y] = current_move.m_piece = QUEEN_BLACK;
	      else
		chess->m_board[x][y] = current_move.m_piece = QUEEN_WHITE;
	    }
	  else
	    {
	      if(qtchess_validate::
		 is_black(chess->m_board[m_point_selected.m_x]
			                [m_point_selected.m_y]))
		chess->m_board[x][y] = current_move.m_piece = ROOK_BLACK;
	      else
		chess->m_board[x][y] = current_move.m_piece = ROOK_WHITE;
	    }
	}
      else
	chess->m_board[x][y] = chess->
	  m_board[m_point_selected.m_x][m_point_selected.m_y];

      if(rc == VALID_CASTLE)
	{
	  if(x == 2 && y == 7) // Rook #1 Black
	    {
	      chess->m_board[0][7] = EMPTY_SQUARE;
	      chess->m_board[3][7] = ROOK1_BLACK;
	      current_move.m_rook = ROOK1_BLACK;
	      current_move.m_rook_x1 = 7;
	      current_move.m_rook_x2 = 7;
	      current_move.m_rook_y1 = 3;
	      current_move.m_rook_y2 = 0;
	    }
	  else if(x == 6 && y == 7) // Rook #2 Black
	    {
	      chess->m_board[5][7] = ROOK2_BLACK;
	      chess->m_board[7][7] = EMPTY_SQUARE;
	      current_move.m_rook = ROOK2_BLACK;
	      current_move.m_rook_x1 = 7;
	      current_move.m_rook_x2 = 7;
	      current_move.m_rook_y1 = 5;
	      current_move.m_rook_y2 = 7;
	    }
	  else if(x == 2 && y == 0) // Rook #1 White
	    {
	      chess->m_board[0][0] = EMPTY_SQUARE;
	      chess->m_board[3][0] = ROOK1_WHITE;
	      current_move.m_rook = ROOK1_WHITE;
	      current_move.m_rook_x1 = 0;
	      current_move.m_rook_x2 = 0;
	      current_move.m_rook_y1 = 3;
	      current_move.m_rook_y2 = 0;
	    }
	  else if(x == 6 && y == 0) // Rook #2 White
	    {
	      chess->m_board[5][0] = ROOK2_WHITE;
	      chess->m_board[7][0] = EMPTY_SQUARE;
	      current_move.m_rook = ROOK2_WHITE;
	      current_move.m_rook_x1 = 0;
	      current_move.m_rook_x2 = 0;
	      current_move.m_rook_y1 = 5;
	      current_move.m_rook_y2 = 7;
	    }
	}
      else if(rc == VALID_PAWN2)
	current_move.m_pawn2 = 1;
      else if(rc == VALID_EN_PASSANT)
	{
	  current_move.m_enpassant = 1;

	  if(m_point_selected.m_y < y)
	    chess->m_board[x][y - 1] = EMPTY_SQUARE;
	  else
	    chess->m_board[x][y + 1] = EMPTY_SQUARE;
	}

      chess->m_board[m_point_selected.m_x][m_point_selected.m_y] =
	EMPTY_SQUARE;

      auto original_piece = chess->m_board[x][y];

      chess->m_board[x][y] = EMPTY_SQUARE;
      snprintf
	(current_move.m_departure, sizeof(current_move.m_departure), "%s",
	 qtchess_validate::find_departure
	 (m_point_selected.m_x, m_point_selected.m_y,
	  x, y, original_piece).toLatin1().constData());
      chess->m_board[x][y] = original_piece;

      for(int i = 0; i < NSQUARES; i++)
	for(int j = 0; j < NSQUARES; j++)
	  current_move.m_board[i][j] = chess->m_board[i][j];

      int non_empty_now = 0;
      int non_empty_then = 0;

      for(int i = 0; i < NSQUARES; i++)
	for(int j = 0; j < NSQUARES; j++)
	  {
	    if(chess->m_board[i][j] != EMPTY_SQUARE)
	      non_empty_then += 1;

	    if(old_board[i][j] != EMPTY_SQUARE)
	      non_empty_now += 1;
	  }

      if(non_empty_now != non_empty_then)
	chess->set_won_piece(true);
      else
	chess->set_won_piece(false);

      if(qtchess_validate::is_king_checked(current_move))
	current_move.m_is_opponent_king_threat = 1;
      else
	current_move.m_is_opponent_king_threat = 0;

      gui->addHistoryMove
	(current_move,
	 qtchess_validate::is_black(current_move.m_piece) ? BLACK : WHITE);

      /*
      ** Send the move.
      */

      if(comm)
	comm->send_move(current_move);

      chess->set_game_over(game_over);

      if(game_over)
	gui->showGameOver(chess->get_turn());

      paint();
      return;
    }
  else
    {
      if((chess->get_my_color() == BLACK &&
	  qtchess_validate::is_black(chess->m_board[x][y])) ||
	 (chess->get_my_color() == WHITE &&
	  qtchess_validate::is_white(chess->m_board[x][y])))
	{
	  /*
	  ** Allow selection of another similar piece.
	  */

	  m_mouse_pressed = 1;
	  m_point_selected.m_x = x;
	  m_point_selected.m_y = y;
	}
      else
	{
	  m_mouse_pressed = 0;
	  m_point_selected.m_x = -1;
	  m_point_selected.m_y = -1;
	}
    }

  paint();
}
