#ifndef _QTCHESS_VALIDATE_
#define _QTCHESS_VALIDATE_

/*
** -- Local Includes --
**/

#include "qtchess.h"
#include "qtchess_defs.h"

/*
** -- Namespaces --
*/

namespace qtchess_validate
{
  int color(const int);
  int isValidMove(const int, const int, const int, const int, const int);
  bool isKing(const int);
  bool isPawn(const int);
  bool isRook(const int);
  bool isRook1(const int);
  bool isRook2(const int);
  bool isBlack(const int);
  bool isColor(const int, const int);
  bool isEmpty(const int);
  bool isQueen(const int);
  bool isWhite(const int);
  bool isBishop(const int);
  bool isKnight(const int);
  bool isThreatened(const int, const int, const int);
  bool isKingChecked(const struct move_s);
  QString findDeparture(const int, const int, const int, const int,
			const int);
};

#endif
