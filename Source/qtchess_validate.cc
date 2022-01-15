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

extern "C"
{
#include <stdio.h>
#include <stdlib.h>
}

#include <QPointer>

#include "qtchess.h"
#include "qtchess_validate.h"

extern QPointer<qtchess> chess;

QString qtchess_validate::find_departure(const int X1,
					 const int Y1,
					 const int X2,
					 const int Y2,
					 const int piece)
{
  /*
  ** This function is primarily used for recording moves.
  */

  if(!chess)
    return "XX";

  char departure[3];
  int fcount = 0;
  int rcount = 0;

  memset(departure, 0, sizeof(departure));

  /*
  ** Same rank? Use the file of the departure square.
  */

  for(int i = 0; i < NSQUARES; i++)
    if(Y1 >= 0 && Y1 < NSQUARES)
      if(chess->m_board[i][Y1] == piece)
	{
	  if(is_valid_move(Y1, i, Y2, X2, piece) != INVALID)
	    {
	      if(Y1 == Y2 && ((X2 < X1 && i > X2) || (X2 > X1 && i < X2)))
		continue;
	      else
		{
		  fcount += 1;
		  break;
		}
	    }
	}

  /*
  ** Same file? Use the rank of the departure square.
  */

  for(int i = 0; i < NSQUARES; i++)
    if(X1 >= 0 && X1 < NSQUARES)
      if(chess->m_board[X1][i] == piece)
	{
	  if(is_valid_move(i, X1, Y2, X2, piece) != INVALID)
	    {
	      if(X1 == X2 && ((Y2 < Y1 && i > Y2) || (Y2 > Y1 && i < Y2)))
		continue;
	      else
		{
		  rcount += 1;
		  break;
		}
	    }
	}

  if(fcount > 0 && rcount > 0)
    return QString::number(static_cast<char> (97 + X1)).
      append(QString::number(Y1 + 1));
  else if(fcount > 0)
    departure[0] = static_cast<char> (97 + X1);
  else if(rcount > 0)
    return QString::number(Y1 + 1);
  else
    departure[0] = static_cast<char> (97 + X1);

  return departure;
}

bool qtchess_validate::is_bishop(const int piece)
{
  if(piece == BISHOP_BLACK || piece == BISHOP_WHITE)
    return true;
  else
    return false;
}

bool qtchess_validate::is_black(const int piece)
{
  switch(piece)
    {
    case BISHOP_BLACK:
    case KING_BLACK:
    case KNIGHT_BLACK:
    case PAWN_BLACK:
    case QUEEN_BLACK:
    case ROOK1_BLACK:
    case ROOK2_BLACK:
    case ROOK_BLACK:
      return true;
    default:
      return false;
    }
}

bool qtchess_validate::is_color(const int piece, const int color)
{
  if(color == BLACK)
    return is_black(piece);
  else
    return is_white(piece);
}

bool qtchess_validate::is_empty(const int piece)
{
  if(piece == EMPTY_SQUARE)
    return true;
  else
    return false;
}

bool qtchess_validate::is_king(const int piece)
{
  if(piece == KING_BLACK || piece == KING_WHITE)
    return true;
  else
    return false;
}

bool qtchess_validate::is_king_checked(const struct move_s &current_move)
{
  auto checked = false;

  if(!chess)
    return checked;

  int I = -1;
  int J = -1;

  for(int i = 0; i < NSQUARES && I == -1 && J == -1; i++)
    for(int j = 0; j < NSQUARES; j++)
      if(chess->get_my_color() == WHITE)
	{
	  if(is_color(chess->m_board[i][j], BLACK) &&
	     is_king(chess->m_board[i][j]))
	    {
	      I = i;
	      J = j;
	      break;
	    }
	}
      else
	{
	  if(is_color(chess->m_board[i][j], WHITE) &&
	     is_king(chess->m_board[i][j]))
	    {
	      I = i;
	      J = j;
	      break;
	    }
	}

  if(I != -1 && J != -1)
    for(int i = 0; i < NSQUARES && !checked; i++)
      for(int j = 0; j < NSQUARES; j++)
	if(is_valid_move(j, i, J, I, chess->m_board[i][j]) != INVALID)
	  {
	    if(i == current_move.m_y2 && j == current_move.m_x2)
	      {
		checked = true;
		break;
	      }
	  }

  return checked;
}

bool qtchess_validate::is_knight(const int piece)
{
  if(piece == KNIGHT_BLACK || piece == KNIGHT_WHITE)
    return true;
  else
    return false;
}

bool qtchess_validate::is_pawn(const int piece)
{
  if(piece == PAWN_BLACK || piece == PAWN_WHITE)
    return true;
  else
    return false;
}

bool qtchess_validate::is_queen(const int piece)
{
  if(piece == QUEEN_BLACK || piece == QUEEN_WHITE)
    return true;
  else
    return false;
}

bool qtchess_validate::is_rook(const int piece)
{
  if(piece == ROOK1_BLACK ||
     piece == ROOK1_WHITE ||
     piece == ROOK2_BLACK ||
     piece == ROOK2_WHITE ||
     piece == ROOK_BLACK ||
     piece == ROOK_WHITE)
    return true;
  else
    return false;
}

bool qtchess_validate::is_rook1(const int piece)
{
  if(piece == ROOK1_BLACK || piece == ROOK1_WHITE)
    return true;
  else
    return false;
}

bool qtchess_validate::is_rook2(const int piece)
{
  if(piece == ROOK2_BLACK || piece == ROOK2_WHITE)
    return true;
  else
    return false;
}

bool qtchess_validate::is_threatened(const int x, const int y, int color)
{
  /*
  ** Determine if the given opponent threatens a certain square.
  */

  if(chess)
    for(int i = 0; i < NSQUARES; i++)
      for(int j = 0; j < NSQUARES; j++)
	if(!is_empty(chess->m_board[i][j]))
	  if(is_color(chess->m_board[i][j], color))
	    if(is_valid_move(j, i, y, x, chess->m_board[i][j]) != INVALID)
	      return true;

  return false;
}

bool qtchess_validate::is_white(const int piece)
{
  switch(piece)
    {
    case BISHOP_WHITE:
    case KING_WHITE:
    case KNIGHT_WHITE:
    case PAWN_WHITE:
    case QUEEN_WHITE:
    case ROOK1_WHITE:
    case ROOK2_WHITE:
    case ROOK_WHITE:
      return true;
    default:
      return false;
    }
}

int qtchess_validate::color(const int piece)
{
  if(is_black(piece))
    return BLACK;
  else
    return WHITE;
}

int qtchess_validate::is_valid_move(const int row_from,
				    const int col_from,
				    const int row_to,
				    const int col_to,
				    const int piece)
{
  int rc = INVALID;

  if(!chess)
    return rc;

  /*
  ** 7
  ** 6
  ** 5
  ** 4
  ** 3
  ** 2
  ** 1
  ** 0 1 2 3 4 5 6 7
  **/

  /*
  ** Due to the configuration of the display, columns and rows
  ** need to be swapped.
  */

  switch(piece)
    {
    case BISHOP_BLACK:
    case BISHOP_WHITE:
      {
	if(abs(row_from - row_to) == 1 && abs(col_from - col_to) == 1)
	  {
	    if(is_black(piece))
	      {
		if(col_to >= 0 && col_to < NSQUARES &&
		   row_to >= 0 && row_to < NSQUARES &&
		   (is_empty(chess->m_board[col_to][row_to]) ||
		    is_white(chess->m_board[col_to][row_to])))
		  rc = VALID;
	      }
	    else if(col_to >= 0 && col_to < NSQUARES &&
		    row_to >= 0 && row_to < NSQUARES &&
		    (is_empty(chess->m_board[col_to][row_to]) ||
		     is_black(chess->m_board[col_to][row_to])))
	      rc = VALID;
	  }
	else if(abs(row_from - row_to) == 0 || abs(col_from - col_to) == 0)
	  break;
	else if(abs(row_from - row_to) == abs(col_from - col_to))
	  {
	    if(col_from < col_to)
	      {
		if(row_from < row_to)
		  {
		    for(int i = row_from + 1, j = col_from + 1; i < row_to;
			i++, j++)
		      if(i >= 0 && i < NSQUARES &&
			 j >= 0 && j < NSQUARES &&
			 !is_empty(chess->m_board[j][i]))
			goto done_label;

		    if(is_black(piece))
		      {
			if(col_to >= 0 && col_to < NSQUARES &&
			   row_to >= 0 && row_to < NSQUARES &&
			   (is_empty(chess->m_board[col_to][row_to]) ||
			    is_white(chess->m_board[col_to][row_to])))
			  rc = VALID;
		      }
		    else if(col_to >= 0 && col_to < NSQUARES &&
			    row_to >= 0 && row_to < NSQUARES &&
			    (is_empty(chess->m_board[col_to][row_to]) ||
			     is_black(chess->m_board[col_to][row_to])))
		      rc = VALID;
		  }
		else
		  {
		    for(int i = row_from - 1, j = col_from + 1; i > row_to; i--,
			  j++)
		      if(i >= 0 && i < NSQUARES &&
			 j >= 0 && j < NSQUARES &&
			 !is_empty(chess->m_board[j][i]))
			goto done_label;

		    if(is_black(piece))
		      {
			if(col_to >= 0 && col_to < NSQUARES &&
			   row_to >= 0 && row_to < NSQUARES &&
			   (is_empty(chess->m_board[col_to][row_to]) ||
			    is_white(chess->m_board[col_to][row_to])))
			  rc = VALID;
		      }
		    else if(col_to >= 0 && col_to < NSQUARES &&
			    row_to >= 0 && row_to < NSQUARES &&
			    (is_empty(chess->m_board[col_to][row_to]) ||
			     is_black(chess->m_board[col_to][row_to])))
		      rc = VALID;
		  }
	      }
	    else
	      {
		if(row_from < row_to)
		  {
		    for(int i = row_from + 1, j = col_from - 1; i < row_to;
			i++, j--)
		      if(i >= 0 && i < NSQUARES &&
			 j >= 0 && j < NSQUARES &&
			 !is_empty(chess->m_board[j][i]))
			goto done_label;

		    if(is_black(piece))
		      {
			if(col_to >= 0 && col_to < NSQUARES &&
			   row_to >= 0 && row_to < NSQUARES &&
			   (is_empty(chess->m_board[col_to][row_to]) ||
			    is_white(chess->m_board[col_to][row_to])))
			  rc = VALID;
		      }
		    else if(col_to >= 0 && col_to < NSQUARES &&
			    row_to >= 0 && row_to < NSQUARES &&
			    (is_empty(chess->m_board[col_to][row_to]) ||
			     is_black(chess->m_board[col_to][row_to])))
		      rc = VALID;
		  }
		else
		  {
		    for(int i = row_from - 1, j = col_from - 1; i > row_to;
			i--, j--)
		      if(i >= 0 && i < NSQUARES &&
			 j >= 0 && j < NSQUARES &&
			 !is_empty(chess->m_board[j][i]))
			goto done_label;

		    if(is_black(piece))
		      {
			if(col_to >= 0 && col_to < NSQUARES &&
			   row_to >= 0 && row_to < NSQUARES &&
			   (is_empty(chess->m_board[col_to][row_to]) ||
			    is_white(chess->m_board[col_to][row_to])))
			  rc = VALID;
		      }
		    else if(col_to >= 0 && col_to < NSQUARES &&
			    row_to >= 0 && row_to < NSQUARES &&
			    (is_empty(chess->m_board[col_to][row_to]) ||
			     is_black(chess->m_board[col_to][row_to])))
		      rc = VALID;
		  }
	      }
	  }

	break;
      }
    case KING_BLACK:
      {
	if(abs(row_from - row_to) == 0 && abs(col_from - col_to) == 1 &&
	   col_to >= 0 && col_to < NSQUARES &&
	   row_to >= 0 && row_to < NSQUARES &&
	   (is_white(chess->m_board[col_to][row_to]) ||
	    is_empty(chess->m_board[col_to][row_to])))
	  rc = VALID;

	if(abs(row_from - row_to) == 1 && abs(col_from - col_to) == 0 &&
	   col_to >= 0 && col_to < NSQUARES &&
	   row_to >= 0 && row_to < NSQUARES &&
	   (is_white(chess->m_board[col_to][row_to]) ||
	    is_empty(chess->m_board[col_to][row_to])))
	  rc = VALID;

	if(abs(row_from - row_to) == 1 && abs(col_from - col_to) == 1 &&
	   col_to >= 0 && col_to < NSQUARES &&
	   row_to >= 0 && row_to < NSQUARES &&
	   (is_white(chess->m_board[col_to][row_to]) ||
	    is_empty(chess->m_board[col_to][row_to])))
	  rc = VALID;

	/*
	** Castling
	*/

	auto king_has_moved = chess->has_king_moved();

	if(!king_has_moved && (col_to == 2 || col_to == 6) && row_to == 7)
	  {
	    /*
	    ** King moves two squares towards the rook.
	    */

	    if(col_to < col_from) // Rook #1
	      {
		if(!chess->has_rook1_moved())
		  {
		    /*
		    ** First two squares left of the king
		    ** must not be threatened.
		    */

		    if(is_empty(chess->m_board[1][7]) &&
		       is_empty(chess->m_board[2][7]) &&
		       is_empty(chess->m_board[3][7]))
		      /*
		      ** Now make sure these squares are
		      ** not threatened.
		      */

		      if(!is_threatened(1, 7, WHITE) &&
			 !is_threatened(2, 7, WHITE))
			rc = VALID;
		  }
	      }
	    else // Rook #2
	      {
		if(!chess->has_rook2_moved())
		  {
		    /*
		    ** First two squares right of the king
		    ** must not be threatened.
		    */

		    if(is_empty(chess->m_board[5][7]) &&
		       is_empty(chess->m_board[6][7]))
		      /*
		      ** Now make sure these squares are
		      ** not threatened.
		      */

		      if(!is_threatened(5, 7, WHITE) &&
			 !is_threatened(6, 7, WHITE))
			rc = VALID;
		  }
	      }

	    if(rc == VALID)
	      rc = VALID_CASTLE;
	  }

	break;
      }
    case KING_WHITE:
      {
	if(abs(row_from - row_to) == 0 && abs(col_from - col_to) == 1 &&
	   col_to >= 0 && col_to < NSQUARES &&
	   row_to >= 0 && row_to < NSQUARES &&
	   (is_black(chess->m_board[col_to][row_to]) ||
	    is_empty(chess->m_board[col_to][row_to])))
	  rc = VALID;

	if(abs(row_from - row_to) == 1 && abs(col_from - col_to) == 0 &&
	   col_to >= 0 && col_to < NSQUARES &&
	   row_to >= 0 && row_to < NSQUARES &&
	   (is_black(chess->m_board[col_to][row_to]) ||
	    is_empty(chess->m_board[col_to][row_to])))
	  rc = VALID;

	if(abs(row_from - row_to) == 1 && abs(col_from - col_to) == 1 &&
	   col_to >= 0 && col_to < NSQUARES &&
	   row_to >= 0 && row_to < NSQUARES &&
	   (is_black(chess->m_board[col_to][row_to]) ||
	    is_empty(chess->m_board[col_to][row_to])))
	  rc = VALID;

	/*
	** Castling
	*/

	auto king_has_moved = chess->has_king_moved();

	if(!king_has_moved && (col_to == 2 || col_to == 6) && row_to == 0)
	  {
	    /*
	    ** King moves two squares towards the rook.
	    */

	    if(col_to < col_from) // Rook #1
	      {
		if(!chess->has_rook1_moved())
		  {
		    /*
		    ** First two squares left of the king
		    ** must not be threatened.
		    */

		    if(is_empty(chess->m_board[1][0]) &&
		       is_empty(chess->m_board[2][0]) &&
		       is_empty(chess->m_board[3][0]))
		      /*
		      ** Now make sure these squares are
		      ** not threatened.
		      */

		      if(!is_threatened(1, 0, BLACK) &&
			 !is_threatened(2, 0, BLACK))
			rc = VALID;
		  }
	      }
	    else // Rook #2
	      {
		if(!chess->has_rook2_moved())
		  {
		    /*
		    ** First two squares right of the king
		    ** must not be threatened.
		    */

		    if(is_empty(chess->m_board[5][0]) &&
		       is_empty(chess->m_board[6][0]))
		      /*
		      ** Now make sure these squares are
		      ** not threatened.
		      */

		      if(!is_threatened(5, 0, BLACK) &&
			 !is_threatened(6, 0, BLACK))
			rc = VALID;
		  }
	      }

	    if(rc == VALID)
	      rc = VALID_CASTLE;
	  }

	break;
      }
    case KNIGHT_BLACK:
      {
	if(col_to >= 0 && col_to < NSQUARES &&
	   row_to >= 0 && row_to < NSQUARES &&
	   abs(row_from - row_to) == 1 && abs(col_from - col_to) == 2 &&
	   (is_white(chess->m_board[col_to][row_to]) ||
	    is_empty(chess->m_board[col_to][row_to])))
	  rc = VALID;

	if(col_to >= 0 && col_to < NSQUARES &&
	   row_to >= 0 && row_to < NSQUARES &&
	   abs(row_from - row_to) == 2 && abs(col_from - col_to) == 1 &&
	   (is_white(chess->m_board[col_to][row_to]) ||
	    is_empty(chess->m_board[col_to][row_to])))
	  rc = VALID;

	break;
      }
    case KNIGHT_WHITE:
      {
	if(col_to >= 0 && col_to < NSQUARES &&
	   row_to >= 0 && row_to < NSQUARES &&
	   abs(row_from - row_to) == 1 && abs(col_from - col_to) == 2 &&
	   (is_black(chess->m_board[col_to][row_to]) ||
	    is_empty(chess->m_board[col_to][row_to])))
	  rc = VALID;

	if(col_to >= 0 && col_to < NSQUARES &&
	   row_to >= 0 && row_to < NSQUARES &&
	   abs(row_from - row_to) == 2 && abs(col_from - col_to) == 1 &&
	   (is_black(chess->m_board[col_to][row_to]) ||
	    is_empty(chess->m_board[col_to][row_to])))
	  rc = VALID;

	break;
      }
    case PAWN_BLACK:
      {
	/*
	** Going backwards is illegal.
	*/

	if(row_from < row_to)
	  break;

	/*
	** Moving forward is valid if and only if the square is
	** empty and the pawn is advanced by one square.
	*/

	if(col_from == col_to &&
	   col_to >= 0 && col_to < NSQUARES &&
	   row_to >= 0 && row_to < NSQUARES &&
	   is_empty(chess->m_board[col_to][row_to]) &&
	   row_from - 1 == row_to)
	  {
	    rc = VALID;
	    break;
	  }

	/*
	** Moving forward two squares is valid if and only if
	** the first two squares are empty and row_from has
	** a value of six.
	*/

	if(col_from == col_to &&
	   col_to >= 0 && col_to < NSQUARES &&
	   is_empty(chess->m_board[col_to][5]) &&
	   is_empty(chess->m_board[col_to][4]) && row_from == 6 && row_to == 4)
	  {
	    rc = VALID_PAWN2;
	    break;
	  }

	/*
	** Making a diagonal move to an empty square is
	** illegal, unless we're performing en passant.
	*/

	if(col_from != col_to &&
	   col_to >= 0 && col_to < NSQUARES &&
	   row_to >= 0 && row_to < NSQUARES &&
	   chess->m_board[col_to][row_to] != EMPTY_SQUARE &&
	   row_from - 1 == row_to &&
	   !is_black(chess->m_board[col_to][row_to]) &&
	   abs(col_from - col_to) == 1)
	  {
	    rc = VALID;
	    break;
	  }

	/*
	** We can only take a white piece via a diagonal move. This
	** move may only be across one square.
	*/

	if(col_from != col_to &&
	   col_to >= 0 && col_to < NSQUARES &&
	   row_to >= 0 && row_to < NSQUARES &&
	   chess->m_board[col_to][row_to] != EMPTY_SQUARE &&
	   is_white(chess->m_board[col_to][row_to]) && row_from - 1 == row_to &&
	   abs(col_from - col_to) == 1)
	  {
	    rc = VALID;
	    break;
	  }

	/*
	** En Passant
	*/

	auto move = chess->get_last_opponent_move();

	if(move.m_pawn2)
	  {
	    if(row_from == 3 && row_to == 2 && col_to == move.m_y1)
	      rc = VALID_EN_PASSANT;
	  }

	break;
      }
    case PAWN_WHITE:
      {
	/*
	** Going backwards is illegal.
	*/

	if(row_from > row_to)
	  break;

	/*
	** Moving forward is valid if and only if the square is
	** empty and the pawn is advanced by one square.
	*/

	if(col_from == col_to &&
	   col_to >= 0 && col_to < NSQUARES &&
	   row_to >= 0 && row_to < NSQUARES &&
	   is_empty(chess->m_board[col_to][row_to]) &&
	   row_from + 1 == row_to)
	  {
	    rc = VALID;
	    break;
	  }

	/*
	** Moving forward two squares is valid if and only if
	** the first two squares are empty and row_from has
	** a value of one.
	*/

	if(col_from == col_to &&
	   col_to >= 0 && col_to < NSQUARES &&
	   is_empty(chess->m_board[col_to][2]) &&
	   is_empty(chess->m_board[col_to][3]) && row_from == 1 && row_to == 3)
	  {
	    rc = VALID_PAWN2;
	    break;
	  }

	/*
	** Making a diagonal move to an empty square is
	** illegal, unless we're performing en passant.
	*/

	if(col_from != col_to &&
	   col_to >= 0 && col_to < NSQUARES &&
	   row_to >= 0 && row_to < NSQUARES &&
	   chess->m_board[col_to][row_to] != EMPTY_SQUARE &&
	   row_from + 1 == row_to &&
	   !is_white(chess->m_board[col_to][row_to]) &&
	   abs(col_from - col_to) == 1)
	  {
	    rc = VALID;
	    break;
	  }

	/*
	** We can only take a black piece via a diagonal move. This
	** move may only be across one square.
	*/

	if(col_from != col_to &&
	   col_to >= 0 && col_to < NSQUARES &&
	   row_to >= 0 && row_to < NSQUARES &&
	   chess->m_board[col_to][row_to] != EMPTY_SQUARE &&
	   is_black(chess->m_board[col_to][row_to]) && row_from + 1 == row_to &&
	   abs(col_from - col_to) == 1)
	  {
	    rc = VALID;
	    break;
	  }

	/*
	** En Passant
	*/

	auto move = chess->get_last_opponent_move();

	if(move.m_pawn2)
	  {
	    if(row_from == 4 && row_to == 5 && col_to == move.m_y1)
	      rc = VALID_EN_PASSANT;
	  }

	break;
      }
    case QUEEN_BLACK:
      {
	if(is_valid_move(row_from, col_from, row_to, col_to,
			 ROOK_BLACK) != INVALID ||
	   is_valid_move(row_from, col_from, row_to, col_to,
			 BISHOP_BLACK) != INVALID)
	  rc = VALID;

	break;
      }
    case QUEEN_WHITE:
      {
	if(is_valid_move(row_from, col_from, row_to, col_to,
			 ROOK_WHITE) != INVALID ||
	   is_valid_move(row_from, col_from, row_to, col_to,
			 BISHOP_WHITE) != INVALID)
	  rc = VALID;

	break;
      }
    case ROOK1_BLACK:
    case ROOK1_WHITE:
    case ROOK2_BLACK:
    case ROOK2_WHITE:
    case ROOK_BLACK:
    case ROOK_WHITE:
      {
	/*
	** Horizontal Moves
	*/

	if(row_from == row_to)
	  {
	    if(col_from + 1 == col_to)
	      {
		if(is_black(piece))
		  {
		    if(col_to >= 0 && col_to < NSQUARES &&
		       row_to >= 0 && row_to < NSQUARES &&
		       (is_white(chess->m_board[col_to][row_to]) ||
			is_empty(chess->m_board[col_to][row_to])))
		      rc = VALID;
		  }
		else if(col_to >= 0 && col_to < NSQUARES &&
			row_to >= 0 && row_to < NSQUARES &&
			(is_black(chess->m_board[col_to][row_to]) ||
			 is_empty(chess->m_board[col_to][row_to])))
		  rc = VALID;
	      }
	    else if(col_from - 1 == col_to)
	      {
		if(is_black(piece))
		  {
		    if(col_to >= 0 && col_to < NSQUARES &&
		       row_to >= 0 && row_to < NSQUARES &&
		       (is_white(chess->m_board[col_to][row_to]) ||
			is_empty(chess->m_board[col_to][row_to])))
		      rc = VALID;
		  }
		else if(col_to >= 0 && col_to < NSQUARES &&
			row_to >= 0 && row_to < NSQUARES &&
			(is_black(chess->m_board[col_to][row_to]) ||
			 is_empty(chess->m_board[col_to][row_to])))
		  rc = VALID;
	      }
	    else if(col_from < col_to)
	      {
		for(int i = col_from + 1; i < col_to; i++)
		  if(i >= 0 && i < NSQUARES &&
		     row_to >= 0 && row_to < NSQUARES &&
		     !is_empty(chess->m_board[i][row_to]))
		    goto done_label;

		if(col_to >= 0 && col_to < NSQUARES &&
		   row_to >= 0 && row_to < NSQUARES &&
		   is_empty(chess->m_board[col_to][row_to]))
		  rc = VALID;
		else if(is_black(piece))
		  {
		    if(col_to >= 0 && col_to < NSQUARES &&
		       row_to >= 0 && row_to < NSQUARES &&
		       !is_white(chess->m_board[col_to][row_to]))
		      break;
		  }
		else if(col_to >= 0 && col_to < NSQUARES &&
			row_to >= 0 && row_to < NSQUARES &&
			!is_black(chess->m_board[col_to][row_to]))
		  break;

		rc = VALID;
	      }
	    else
	      {
		for(int i = col_from - 1; i > col_to; i--)
		  if(i >= 0 && i < NSQUARES &&
		     row_to >= 0 && row_to < NSQUARES &&
		     !is_empty(chess->m_board[i][row_to]))
		    goto done_label;

		if(col_to >= 0 && col_to < NSQUARES &&
		   row_to >= 0 && row_to < NSQUARES &&
		   is_empty(chess->m_board[col_to][row_to]))
		  rc = VALID;
		else if(is_black(piece))
		  {
		    if(col_to >= 0 && col_to < NSQUARES &&
		       row_to >= 0 && row_to < NSQUARES &&
		       !is_white(chess->m_board[col_to][row_to]))
		      break;
		  }
		else if(col_to >= 0 && col_to < NSQUARES &&
			row_to >= 0 && row_to < NSQUARES &&
			!is_black(chess->m_board[col_to][row_to]))
		  break;

		rc = VALID;
	      }
	  }

	/*
	** Vertical Moves
	*/

	if(col_from == col_to)
	  {
	    if(row_from + 1 == row_to)
	      {
		if(is_black(piece))
		  {
		    if(col_to >= 0 && col_to < NSQUARES &&
		       row_to >= 0 && row_to < NSQUARES &&
		       (is_white(chess->m_board[col_to][row_to]) ||
			is_empty(chess->m_board[col_to][row_to])))
		      rc = VALID;
		  }
		else if(col_to >= 0 && col_to < NSQUARES &&
			row_to >= 0 && row_to < NSQUARES &&
			(is_black(chess->m_board[col_to][row_to]) ||
			 is_empty(chess->m_board[col_to][row_to])))
		  rc = VALID;
	      }
	    else if(row_from - 1 == row_to)
	      {
		if(is_black(piece))
		  {
		    if(col_to >= 0 && col_to < NSQUARES &&
		       row_to >= 0 && row_to < NSQUARES &&
		       (is_white(chess->m_board[col_to][row_to]) ||
			is_empty(chess->m_board[col_to][row_to])))
		      rc = VALID;
		  }
		else if(col_to >= 0 && col_to < NSQUARES &&
			row_to >= 0 && row_to < NSQUARES &&
			(is_black(chess->m_board[col_to][row_to]) ||
			 is_empty(chess->m_board[col_to][row_to])))
		  rc = VALID;
	      }
	    else if(row_from < row_to)
	      {
		for(int i = row_from + 1; i < row_to; i++)
		  if(i >= 0 && i < NSQUARES &&
		     col_to >= 0 && col_to < NSQUARES &&
		     !is_empty(chess->m_board[col_to][i]))
		    goto done_label;

		if(col_to >= 0 && col_to < NSQUARES &&
		   row_to >= 0 && row_to < NSQUARES &&
		   is_empty(chess->m_board[col_to][row_to]))
		  rc = VALID;
		else if(is_black(piece))
		  {
		    if(col_to >= 0 && col_to < NSQUARES &&
		       row_to >= 0 && row_to < NSQUARES &&
		       !is_white(chess->m_board[col_to][row_to]))
		      break;
		  }
		else if(col_to >= 0 && col_to < NSQUARES &&
			row_to >= 0 && row_to < NSQUARES &&
			!is_black(chess->m_board[col_to][row_to]))
		  break;

		rc = VALID;
	      }
	    else
	      {
		for(int i = row_from - 1; i > row_to; i--)
		  if(i >= 0 && i < NSQUARES &&
		     col_to >= 0 && col_to < NSQUARES &&
		     !is_empty(chess->m_board[col_to][i]))
		    goto done_label;

		if(col_to >= 0 && col_to < NSQUARES &&
		   row_to >= 0 && row_to < NSQUARES &&
		   is_empty(chess->m_board[col_to][row_to]))
		  rc = VALID;
		else if(is_black(piece))
		  {
		    if(col_to >= 0 && col_to < NSQUARES &&
		       row_to >= 0 && row_to < NSQUARES &&
		       !is_white(chess->m_board[col_to][row_to]))
		      break;
		  }
		else if(col_to >= 0 && col_to < NSQUARES &&
			row_to >= 0 && row_to < NSQUARES &&
			!is_black(chess->m_board[col_to][row_to]))
		  break;

		rc = VALID;
	      }
	  }

	break;
      }
    default:
      break;
    }

 done_label:
  return rc;
}
