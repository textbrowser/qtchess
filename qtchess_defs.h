#ifndef _QTCHESS_DEFS_
#define _QTCHESS_DEFS_

/*
** -- System Includes --
*/

extern "C"
{
#include <stdio.h>
#include <stdlib.h>
}

/*
** -- Local Includes --
*/

enum
{
  BLACK = 1,
  WHITE = 2,
  MY_TURN = 3,
  I_AM_FIRST = 4,
  THEIR_TURN = 5,
  THEY_ARE_FIRST = 6,
  PLAYER_TIMER = 7,
  OPPONENT_TIMER = 8
};

enum
{
  KING_BLACK = 100,
  PAWN_BLACK,
  ROOK_BLACK,
  QUEEN_BLACK,
  ROOK1_BLACK,
  ROOK2_BLACK,
  BISHOP_BLACK,
  KNIGHT_BLACK
};

enum
{
  KING_WHITE = 200,
  PAWN_WHITE,
  ROOK_WHITE,
  QUEEN_WHITE,
  ROOK1_WHITE,
  ROOK2_WHITE,
  BISHOP_WHITE,
  KNIGHT_WHITE
};

enum
{
  VALID = 1,
  INVALID = 0,
  NSQUARES = 8,
  EMPTY_SQUARE = 0,
  VALID_CASTLE = 2,
  VALID_PAWN_2 = 3,
  VALID_EN_PASSANT = 4,
  OPEN_GL_DIMENSION = 700
};

struct move_s
{
  int x1;
  int x2;
  int y1;
  int y2;
  int piece;

  /*
  ** Used for castling moves.
  */

  int r_x1;
  int r_x2;
  int r_y1;
  int r_y2;
  int rook;

  /*
  ** For promotions.
  */

  int promoted;

  /*
  ** En passant.
  */

  int enpassant;

  /*
  ** Is the opponent's king threatened?
  */

  int isOppKingThreat;

  /*
  ** Pawn moved two squares?
  */

  int pawn_2;

  /*
  ** Current state of the board.
  */

  int board[NSQUARES][NSQUARES];

  /*
  ** Departure.
  */

  char departure[3];
};

#endif
