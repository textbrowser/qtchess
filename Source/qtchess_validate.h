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

#ifndef _QTCHESS_VALIDATE_
#define _QTCHESS_VALIDATE_

#include <QString>

namespace qtchess_validate
{
  QString find_departure
    (const int X1, const int Y1, const int X2 , const int Y2, const int piece);
  bool is_bishop(const int piece);
  bool is_black(const int piece);
  bool is_color(const int piece, const int color);
  bool is_empty(const int piece);
  bool is_king(const int piece);
  bool is_king_checked(const int a, const int b, const int x, const int y);
  bool is_king_checked(const struct move_s &current_move);
  bool is_king_checked_implementation(const int board[NSQUARES][NSQUARES],
				      const int color,
				      const int x,
				      const int y);
  bool is_king_checked_implementation(const int board[NSQUARES][NSQUARES],
				      const int color,
				      const struct move_s &current_move);
  bool is_knight(const int piece);
  bool is_pawn(const int piece);
  bool is_queen(const int piece);
  bool is_rook(const int piece);
  bool is_rook1(const int piece);
  bool is_rook2(const int piece);
  bool is_threatened(const int x, const int y, const int color);
  bool is_threatened_implementation(const int board[NSQUARES][NSQUARES],
				    const int x,
				    const int y,
				    const int color);
  bool is_white(const int piece);
  int color(const int piece);
  int is_valid_move(const int row_from,
		    const int col_from,
		    const int row_to,
		    const int col_to,
		    const int piece);
}

#endif
