#ifndef _QTCHESS_DEFS_
#define _QTCHESS_DEFS_

extern "C"
{
#include <stdio.h>
#include <stdlib.h>
}

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
  int x1;
  int x2;
  int y1;
  int y2;

  /*
  ** Used for castling moves.
  */

  int r_x1;
  int r_x2;
  int r_y1;
  int r_y2;
  int rook;
};

#endif
