#ifndef _QTCHESS_
#define _QTCHESS_

/*
** -- Qt Includes --
*/

#include <QApplication>

/*
** -- Local Includes --
*/

#include "qtchess_gui.h"
#include "qtchess_comm.h"
#include "qtchess_defs.h"
#include "qtchess_validate.h"

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
      int i = 0, j = 0;

      for(i = 0; i < NSQUARES; i++)
	for(j = 0; j < NSQUARES; j++)
	  board[i][j] = 0;

      turn = -1;
      first = -1;
      my_color = -1;
    }

  int getTurn(void)
    {
      return turn;
    }

  int getFirst(void)
    {
      return first;
    }

  int getMyColor(void)
    {
      return my_color;
    }

  bool hasKingMoved(void)
    {
      return king_has_moved;
    }

  bool hasRook1Moved(void)
    {
      return rook1_has_moved;
    }

  bool hasRook2Moved(void)
    {
      return rook2_has_moved;
    }

  bool isGameOver(void)
    {
      return game_over;
    }

  bool wasPieceWon(void)
    {
      return wonPiece;
    }

  struct move_s getLastOpponentMove(void)
    {
      return last_opponent_move;
    }

  void setWonPiece(const bool wonPieceArg)
    {
      wonPiece = wonPieceArg;
    }

  void setLastOpponentMove(const struct move_s move)
    {
      last_opponent_move = move;
    }

  void setGameOver(const bool game_over_arg)
    {
      game_over = game_over_arg;
    }

  void setTurn(const int turn_arg)
    {
      turn = turn_arg;
    }

  void setFirst(const int first_arg)
    {
      first = first_arg;
    }

  void setMyColor(const int my_color_arg)
    {
      my_color = my_color_arg;
    }

  void setKingHasMoved(const bool king_has_moved_arg)
    {
      king_has_moved = king_has_moved_arg;
    }

  void setRook1HasMoved(const bool rook1_has_moved_arg)
    {
      rook1_has_moved = rook1_has_moved_arg;
    }

  void setRook2HasMoved(const bool rook2_has_moved_arg)
    {
      rook2_has_moved = rook2_has_moved_arg;
    }

  bool isReady(void);
  bool isConnected(void);
  void init(void);
  void quit(const char *, const int);
  void updateBoard(const QByteArray &buffer);

 private:

  /*
  ** -- Members --
  */

  int turn;
  int first;
  int my_color;
  bool wonPiece;
  bool game_over;
  bool king_has_moved;
  bool rook1_has_moved;
  bool rook2_has_moved;
  struct move_s last_opponent_move;

  /*
  ** -- Methods
  */
};

#endif
