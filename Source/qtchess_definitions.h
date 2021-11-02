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

#ifndef _QTCHESS_DEFINITIONS_
#define _QTCHESS_DEFINITIONS_

enum BlackPieces
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

enum BoardInformation
{
  EMPTY_SQUARE = 0,
  INVALID = 0,
  NSQUARES = 8,
  VALID = 1,
  VALID_CASTLE = 2,
  VALID_EN_PASSANT = 4,
  VALID_PAWN2 = 3
};

enum StateInformation
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

enum WhitePieces
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

struct move_s
{
  char m_departure[3];
  int m_board[NSQUARES][NSQUARES];
  int m_enpassant;
  int m_is_opponent_king_threat;
  int m_pawn2; // Did a pawn move two squares?
  int m_piece;
  int m_promoted;
  int m_rook; // Castling.
  int m_rook_x1; // Castling.
  int m_rook_x2; // Castling.
  int m_rook_y1; // Castling.
  int m_rook_y2; // Castling.
  int m_x1;
  int m_x2;
  int m_y1;
  int m_y2;
};

#endif
