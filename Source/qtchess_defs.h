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

#ifndef _QTCHESS_DEFS_
#define _QTCHESS_DEFS_

enum
{
  BLACK = 1,
  I_AM_FIRST = 4,
  MY_TURN = 3,
  OPPONENT_TIMER = 8,
  PLAYER_TIMER = 7,
  THEIR_TURN = 5,
  THEY_ARE_FIRST = 6,
  WHITE = 2
};

enum
{
  BISHOP_BLACK = 100,
  KING_BLACK,
  KNIGHT_BLACK,
  PAWN_BLACK,
  QUEEN_BLACK,
  ROOK_BLACK,
  ROOK1_BLACK,
  ROOK2_BLACK
};

enum
{
  BISHOP_WHITE = 200,
  KING_WHITE,
  KNIGHT_WHITE,
  PAWN_WHITE,
  QUEEN_WHITE,
  ROOK_WHITE,
  ROOK1_WHITE,
  ROOK2_WHITE
};

enum
{
  EMPTY_SQUARE = 0,
  INVALID = 0,
  NSQUARES = 8,
  OPEN_GL_DIMENSION = 700,
  VALID = 1,
  VALID_CASTLE = 2,
  VALID_EN_PASSANT = 4,
  VALID_PAWN_2 = 3
};

struct move_s
{
  char departure[3];
  int board[NSQUARES][NSQUARES];
  int enpassant;
  int isOppKingThreat;
  int pawn_2; // Did a pawn move two squares?
  int piece;
  int promoted;
  int r_x1; // Castling.
  int r_x2; // Castling.
  int r_y1; // Castling.
  int r_y2; // Castling.
  int rook; // Castling.
  int x1;
  int x2;
  int y1;
  int y2;
};

#endif
