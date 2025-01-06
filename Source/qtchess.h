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

#ifndef _QTCHESS_
#define _QTCHESS_

#include <QObject>

#include "qtchess_definitions.h"

#define QTCHESS_VERSION "2025.01.15"

class qtchess: public QObject
{
  Q_OBJECT

 public:
  qtchess(void):QObject()
  {
    for(int i = 0; i < NSQUARES; i++)
      for(int j = 0; j < NSQUARES; j++)
	m_board[i][j] = 0;

    m_first = -1;
    m_game_over = false;
    m_king_has_moved = false;
    m_last_opponent_move = {};
    m_my_color = -1;
    m_rook1_has_moved = false;
    m_rook2_has_moved = false;
    m_turn = -1;
    m_won_piece = false;
  }

  int m_board[NSQUARES][NSQUARES];

  bool has_king_moved(void) const
  {
    return m_king_has_moved;
  }

  bool has_rook1_moved(void) const
  {
    return m_rook1_has_moved;
  }

  bool has_rook2_moved(void) const
  {
    return m_rook2_has_moved;
  }

  bool is_game_over(void) const
  {
    return m_game_over;
  }

  bool was_piece_won(void) const
  {
    return m_won_piece;
  }

  int get_first(void) const
  {
    return m_first;
  }

  int get_my_color(void) const
  {
    return m_my_color;
  }

  int get_turn(void) const
  {
    return m_turn;
  }

  struct move_s get_last_opponent_move(void) const
  {
    return m_last_opponent_move;
  }

  void set_first(const int first)
  {
    m_first = first;
  }

  void set_game_over(const bool game_over)
  {
    m_game_over = game_over;
  }

  void set_king_has_moved(const bool king_has_moved)
  {
    m_king_has_moved = king_has_moved;
  }

  void set_last_opponent_move(const struct move_s &move)
  {
    m_last_opponent_move = move;
  }

  void set_my_color(const int my_color)
  {
    m_my_color = my_color;
  }

  void set_rook1_has_moved(const bool rook1_has_moved)
  {
    m_rook1_has_moved = rook1_has_moved;
  }

  void set_rook2_has_moved(const bool rook2_has_moved)
  {
    m_rook2_has_moved = rook2_has_moved;
  }

  void set_turn(const int turn)
  {
    m_turn = turn;
  }

  void set_won_piece(const bool won_piece)
  {
    m_won_piece = won_piece;
  }

  bool is_ready(void) const;
  void initialize(void);
  void quit(const char *message_text, const int exit_code);
  void update_board(const QByteArray &buffer);
  void update_board(const QString &move, const QStringList &state);

 private:
  bool m_game_over;
  bool m_king_has_moved;
  bool m_rook1_has_moved;
  bool m_rook2_has_moved;
  bool m_won_piece;
  int m_first;
  int m_my_color;
  int m_turn;
  struct move_s m_last_opponent_move;
};

#endif
