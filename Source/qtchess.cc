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

#include <QtDebug>

#include "qtchess.h"
#include "qtchess_communications.h"
#include "qtchess_gui.h"
#include "qtchess_validate.h"

extern QPointer<qtchess_communications> comm;
extern QPointer<qtchess_gui> gui;

bool qtchess::is_ready(void) const
{
  if(gui && gui->is_ready())
    return get_turn() == MY_TURN;
  else
    return false;
}

void qtchess::initialize(void)
{
  m_board[0][0] = ROOK1_WHITE;
  m_board[0][7] = ROOK1_BLACK;
  m_board[1][0] = m_board[6][0] = KNIGHT_WHITE;
  m_board[1][7] = m_board[6][7] = KNIGHT_BLACK;
  m_board[2][0] = m_board[5][0] = BISHOP_WHITE;
  m_board[2][7] = m_board[5][7] = BISHOP_BLACK;
  m_board[3][0] = QUEEN_WHITE;
  m_board[3][7] = QUEEN_BLACK;
  m_board[4][0] = KING_WHITE;
  m_board[4][7] = KING_BLACK;
  m_board[7][0] = ROOK2_WHITE;
  m_board[7][7] = ROOK2_BLACK;
  m_game_over = false;
  m_king_has_moved = false;
  m_rook1_has_moved = false;
  m_rook2_has_moved = false;
  m_won_piece = false;

  for(int i = 0; i < NSQUARES; i++)
    {
      m_board[i][1] = PAWN_WHITE;
      m_board[i][6] = PAWN_BLACK;
    }

  for(int i = 2; i < 6; i++)
    for(int j = 0; j < NSQUARES; j++)
      m_board[j][i] = EMPTY_SQUARE;

  m_last_opponent_move.m_pawn2 = m_last_opponent_move.m_promoted = 0;
  m_last_opponent_move.m_piece =
    m_last_opponent_move.m_rook =
    m_last_opponent_move.m_rook_x1 =
    m_last_opponent_move.m_rook_x2 =
    m_last_opponent_move.m_rook_y1 =
    m_last_opponent_move.m_rook_y2 =
    m_last_opponent_move.m_x1 =
    m_last_opponent_move.m_x2 =
    m_last_opponent_move.m_y1 =
    m_last_opponent_move.m_y2 = -1;
  memset(m_last_opponent_move.m_departure,
	 0,
	 sizeof(char) * sizeof(m_last_opponent_move.m_departure));
}

void qtchess::quit(const char *message_text, const int exit_code)
{
  /*
  ** Print the exit message.
  */

  if(message_text != nullptr)
    qDebug() << message_text;

  QApplication::exit(exit_code);
}

void qtchess::update_board(const QByteArray &buffer)
{
  auto const list(buffer.simplified().split(' '));

  if(list.size() < 15 + NSQUARES * NSQUARES)
    {
      qDebug() << "qtchess::update_board(): list.size() is too small.";
      return;
    }

  struct move_s current_move = {};

  /*
  ** Now for some magic.
  */

  /*
  ** Copy the information into current_move.
  */

  current_move.m_piece = list.value(8).toInt();
  current_move.m_rook_x1 = qBound(-1, list.value(4).toInt(), NSQUARES - 1);
  current_move.m_rook_x2 = qBound(-1, list.value(5).toInt(), NSQUARES - 1);
  current_move.m_rook_y1 = qBound(-1, list.value(6).toInt(), NSQUARES - 1);
  current_move.m_rook_y2 = qBound(-1, list.value(7).toInt(), NSQUARES - 1);
  current_move.m_rook = list.value(9).toInt();
  current_move.m_x1 = qBound(-1, list.value(0).toInt(), NSQUARES - 1);
  current_move.m_x2 = qBound(-1, list.value(1).toInt(), NSQUARES - 1);
  current_move.m_y1 = qBound(-1, list.value(2).toInt(), NSQUARES - 1);
  current_move.m_y2 = qBound(-1, list.value(3).toInt(), NSQUARES - 1);

  if(get_my_color() == BLACK)
    {
      if(!(current_move.m_piece >= 200 && current_move.m_piece <= 207))
	current_move.m_piece = EMPTY_SQUARE;
    }
  else
    {
      if(!(current_move.m_piece >= 100 && current_move.m_piece <= 107))
	current_move.m_piece = EMPTY_SQUARE;
    }

  if(!(current_move.m_rook == -1 ||
       current_move.m_rook == ROOK1_BLACK ||
       current_move.m_rook == ROOK1_WHITE ||
       current_move.m_rook == ROOK2_BLACK ||
       current_move.m_rook == ROOK2_WHITE))
    current_move.m_rook = -1;

  current_move.m_enpassant = QVariant(list.value(12).toInt()).toBool();
  current_move.m_is_opponent_king_threat =
    QVariant(list.value(13).toInt()).toBool();
  current_move.m_pawn2 = QVariant(list.value(11).toInt()).toBool();
  current_move.m_promoted = QVariant(list.value(10).toInt()).toBool();
  snprintf(current_move.m_departure,
	   sizeof(current_move.m_departure),
	   "%s",
	   list.value(14).constData());

  for(int i = 0, x = 15; i < NSQUARES; i++)
    for(int j = 0; j < NSQUARES; j++)
      {
	current_move.m_board[i][j] = list.value(x).toInt();

	if(!((current_move.m_board[i][j] >= 100 &&
	      current_move.m_board[i][j] <= 107) ||
	     (current_move.m_board[i][j] >= 200 &&
	      current_move.m_board[i][j] <= 207)))
	  current_move.m_board[i][j] = EMPTY_SQUARE;

	x += 1;
      }

  if(current_move.m_x1 == -1)
    {
      /*
      ** New game?
      */

      initialize();
      set_game_over(false);
      set_turn(THEIR_TURN);

      if(gui)
	{
	  gui->clear_history();
	  gui->initialize_clocks();
	  gui->update();
	}
    }
  else
    {
      if(gui)
	{
	  gui->start_timers(PLAYER_TIMER);
	  gui->stop_timers(OPPONENT_TIMER);
	}

      auto color = BLACK;

      if(get_my_color() == BLACK)
	color = WHITE;
      else
	color = BLACK;

      if(current_move.m_x2 >= 0 && current_move.m_x2 < NSQUARES &&
	 current_move.m_y2 >= 0 && current_move.m_y2 < NSQUARES &&
	 qtchess_validate::is_king(m_board[current_move.m_y2]
				          [current_move.m_x2]))
	{
	  m_game_over = true;
	  set_game_over(m_game_over);
	}
      else
	set_turn(MY_TURN);

      m_last_opponent_move = current_move;

      int non_empty_now = 0;
      int non_empty_then = 0;

      for(int i = 0; i < NSQUARES; i++)
	for(int j = 0; j < NSQUARES; j++)
	  {
	    if(current_move.m_board[i][j] != EMPTY_SQUARE)
	      non_empty_now += 1;

	    if(m_board[i][j] != EMPTY_SQUARE)
	      non_empty_then += 1;
	  }

      if(non_empty_now != non_empty_then)
	m_won_piece = true;
      else
	m_won_piece = false;

      for(int i = 0; i < NSQUARES; i++)
	for(int j = 0; j < NSQUARES; j++)
	  m_board[i][j] = current_move.m_board[i][j];

      if(gui)
	{
	  gui->add_history_move(current_move, color);
	  gui->update();
	}

      if(gui && m_game_over)
	{
	  gui->show_game_over(get_turn());
	  gui->stop_timers(OPPONENT_TIMER);
	  gui->stop_timers(PLAYER_TIMER);
	}
    }
}

void qtchess::update_board(const QString &move, const QStringList &state)
{
  if(state.size() == 8)
    {
      if(gui)
	{
	  gui->start_timers(PLAYER_TIMER);
	  gui->stop_timers(OPPONENT_TIMER);
	}

      int board[NSQUARES][NSQUARES];

      for(int i = 0; i < state.size(); i++)
	{
	  auto const row(state.at(i).split(' '));

	  for(int j = 0; j < row.size(); j++)
	    if(row[j] == ".")
	      board[i][j] = EMPTY_SQUARE;
	    else if(row[j] == "B")
	      board[i][j] = BISHOP_WHITE;
	    else if(row[j] == "K")
	      board[i][j] = KING_WHITE;
	    else if(row[j] == "N")
	      board[i][j] = KNIGHT_WHITE;
	    else if(row[j] == "Q")
	      board[i][j] = QUEEN_WHITE;
	    else if(row[j] == "P")
	      board[i][j] = PAWN_WHITE;
	    else if(row[j] == "R")
	      board[i][j] = ROOK_WHITE;
	    else if(row[j] == "b")
	      board[i][j] = BISHOP_BLACK;
	    else if(row[j] == "k")
	      board[i][j] = KING_BLACK;
	    else if(row[j] == "n")
	      board[i][j] = KNIGHT_BLACK;
	    else if(row[j] == "q")
	      board[i][j] = QUEEN_BLACK;
	    else if(row[j] == "p")
	      board[i][j] = PAWN_BLACK;
	    else if(row[j] == "r")
	      board[i][j] = ROOK_BLACK;
	}

      for(int i = 0; i < NSQUARES; i++)
	for(int j = 0; j < NSQUARES; j++)
	  m_board[i][j] = board[i][j];

      set_turn(MY_TURN);

      if(gui)
	{
	  gui->add_history_move
	    (move.mid(move.lastIndexOf(' ') + 1).trimmed(), BLACK);
	  gui->update();
	}
    }
}
