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

#include <QByteArray>

#include "qtchess_defs.h"

#define QTCHESS_VERSION "2020.02.15"

static struct move_s emptyMoveStruct;

class qtchess
{
 public:
  /*
  ** -- Members --
  */

  int board[NSQUARES][NSQUARES];

  /*
  ** -- Methods --
  */

  qtchess(void)
  {
    for(int i = 0; i < NSQUARES; i++)
      for(int j = 0; j < NSQUARES; j++)
	board[i][j] = 0;

    first = -1;
    game_over = false;
    king_has_moved = false;
    last_opponent_move = emptyMoveStruct;
    my_color = -1;
    rook1_has_moved = false;
    rook2_has_moved = false;
    turn = -1;
    wonPiece = false;
  }

  bool hasKingMoved(void) const
  {
    return king_has_moved;
  }

  bool hasRook1Moved(void) const
  {
    return rook1_has_moved;
  }

  bool hasRook2Moved(void) const
  {
    return rook2_has_moved;
  }

  bool isGameOver(void) const
  {
    return game_over;
  }

  bool wasPieceWon(void) const
  {
    return wonPiece;
  }

  int getFirst(void) const
  {
    return first;
  }

  int getMyColor(void) const
  {
    return my_color;
  }

  int getTurn(void) const
  {
    return turn;
  }

  struct move_s getLastOpponentMove(void) const
  {
    return last_opponent_move;
  }

  void setFirst(const int first_arg)
  {
    first = first_arg;
  }

  void setGameOver(const bool game_over_arg)
  {
    game_over = game_over_arg;
  }

  void setKingHasMoved(const bool king_has_moved_arg)
  {
    king_has_moved = king_has_moved_arg;
  }

  void setLastOpponentMove(const struct move_s &move)
  {
    last_opponent_move = move;
  }

  void setMyColor(const int my_color_arg)
  {
    my_color = my_color_arg;
  }

  void setRook1HasMoved(const bool rook1_has_moved_arg)
  {
    rook1_has_moved = rook1_has_moved_arg;
  }

  void setRook2HasMoved(const bool rook2_has_moved_arg)
  {
    rook2_has_moved = rook2_has_moved_arg;
  }

  void setTurn(const int turn_arg)
  {
    turn = turn_arg;
  }

  void setWonPiece(const bool wonPieceArg)
  {
    wonPiece = wonPieceArg;
  }

  bool isReady(void);
  void init(void);
  void quit(const char *, const int);
  void updateBoard(const QByteArray &buffer);

 private:
  /*
  ** -- Members --
  */

  bool game_over;
  bool king_has_moved;
  bool rook1_has_moved;
  bool rook2_has_moved;
  bool wonPiece;
  int first;
  int my_color;
  int turn;
  struct move_s last_opponent_move;

  /*
  ** -- Methods
  */
};

#endif
