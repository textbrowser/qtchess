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

#include <QtDebug>

#include "qtchess.h"
#include "qtchess_comm.h"
#include "qtchess_gui.h"
#include "qtchess_validate.h"

extern qtchess_comm *comm;
extern qtchess_gui *gui;

bool qtchess::isReady(void)
{
  return comm && comm->isReady() && (getTurn() == MY_TURN);
}

void qtchess::init(void)
{
  board[0][0] = ROOK1_WHITE;
  board[0][7] = ROOK1_BLACK;
  board[1][0] = board[6][0] = KNIGHT_WHITE;
  board[1][7] = board[6][7] = KNIGHT_BLACK;
  board[2][0] = board[5][0] = BISHOP_WHITE;
  board[2][7] = board[5][7] = BISHOP_BLACK;
  board[3][0] = QUEEN_WHITE;
  board[3][7] = QUEEN_BLACK;
  board[4][0] = KING_WHITE;
  board[4][7] = KING_BLACK;
  board[7][0] = ROOK2_WHITE;
  board[7][7] = ROOK2_BLACK;
  game_over = false;
  king_has_moved = false;
  rook1_has_moved = false;
  rook2_has_moved = false;
  wonPiece = false;

  for(int i = 0; i < NSQUARES; i++)
    {
      board[i][1] = PAWN_WHITE;
      board[i][6] = PAWN_BLACK;
    }

  for(int i = 2; i < 6; i++)
    for(int j = 0; j < NSQUARES; j++)
      board[j][i] = EMPTY_SQUARE;

  last_opponent_move.x1 = last_opponent_move.x2 = last_opponent_move.y1 =
    last_opponent_move.y2 = last_opponent_move.r_x1 =
    last_opponent_move.r_x2 = last_opponent_move.r_y1 =
    last_opponent_move.r_y2 = last_opponent_move.piece =
    last_opponent_move.rook = -1;
  last_opponent_move.promoted = last_opponent_move.pawn_2 = 0;
  memset(last_opponent_move.departure, 0,
	 sizeof(last_opponent_move.departure));
}

void qtchess::quit(const char *message_text, const int exit_code)
{
  /*
  ** Print the exit message.
  */

  if(message_text != nullptr)
    qDebug() << message_text;

  if(QApplication::instance())
    QApplication::instance()->exit(exit_code);
  else
    exit(exit_code);
}

void qtchess::updateBoard(const QByteArray &buffer)
{
  QList<QByteArray> list = buffer.simplified().split(' ');

  if(list.size() < 15 + NSQUARES * NSQUARES)
    {
      qDebug() << "qtchess::updateBoard(): list.size() is too small.";
      return;
    }

  struct move_s current_move = {};

  /*
  ** Now for some magic.
  */

  /*
  ** Copy the information into current_move.
  */

  current_move.piece = list.value(8).toInt();
  current_move.r_x1 = qBound(-1, list.value(4).toInt(), NSQUARES - 1);
  current_move.r_x2 = qBound(-1, list.value(5).toInt(), NSQUARES - 1);
  current_move.r_y1 = qBound(-1, list.value(6).toInt(), NSQUARES - 1);
  current_move.r_y2 = qBound(-1, list.value(7).toInt(), NSQUARES - 1);
  current_move.rook = list.value(9).toInt();
  current_move.x1 = qBound(-1, list.value(0).toInt(), NSQUARES - 1);
  current_move.x2 = qBound(-1, list.value(1).toInt(), NSQUARES - 1);
  current_move.y1 = qBound(-1, list.value(2).toInt(), NSQUARES - 1);
  current_move.y2 = qBound(-1, list.value(3).toInt(), NSQUARES - 1);

  if(getMyColor() == BLACK)
    {
      if(!(current_move.piece >= 200 && current_move.piece <= 207))
	current_move.piece = EMPTY_SQUARE;
    }
  else
    {
      if(!(current_move.piece >= 100 && current_move.piece <= 107))
	current_move.piece = EMPTY_SQUARE;
    }

  if(!(current_move.rook == -1 ||
       current_move.rook == ROOK1_BLACK ||
       current_move.rook == ROOK1_WHITE ||
       current_move.rook == ROOK2_BLACK ||
       current_move.rook == ROOK2_WHITE))
    current_move.rook = -1;

  current_move.enpassant = QVariant(list.value(12).toInt()).toBool();
  current_move.isOppKingThreat = QVariant(list.value(13).toInt()).toBool();
  current_move.pawn_2 = QVariant(list.value(11).toInt()).toBool();
  current_move.promoted = QVariant(list.value(10).toInt()).toBool();
  snprintf(current_move.departure,
	   sizeof(current_move.departure),
	   "%s",
	   list.value(14).constData());

  for(int i = 0, x = 15; i < NSQUARES; i++)
    for(int j = 0; j < NSQUARES; j++)
      {
	current_move.board[i][j] = list.value(x).toInt();

	if(!((current_move.board[i][j] >= 100 &&
	      current_move.board[i][j] <= 107) ||
	     (current_move.board[i][j] >= 200 &&
	      current_move.board[i][j] <= 207)))
	  current_move.board[i][j] = EMPTY_SQUARE;

	x += 1;
      }

  list.clear();

  if(current_move.x1 == -1)
    {
      /*
      ** New game?
      */

      init();
      setGameOver(false);
      setTurn(THEIR_TURN);

      if(gui)
	{
	  if(gui->getGLBoard())
	    gui->getGLBoard()->updateGL();

	  gui->clearHistory();
	  gui->showNewGameInfo();
	  gui->initClocks();
	  gui->update();
	}
    }
  else
    {
      if(gui)
	{
	  gui->stopTimers(OPPONENT_TIMER);
	  gui->startTimers(PLAYER_TIMER);
	}

      int color = BLACK;

      if(getMyColor() == BLACK)
	color = WHITE;
      else
	color = BLACK;

      if(current_move.x2 >= 0 && current_move.x2 < NSQUARES &&
	 current_move.y2 >= 0 && current_move.y2 < NSQUARES &&
	 qtchess_validate::isKing(board[current_move.y2][current_move.x2]))
	{
	  game_over = true;
	  setGameOver(game_over);
	}
      else
	setTurn(MY_TURN);

      last_opponent_move = current_move;

      int nonEmptyNow = 0, nonEmptyThen = 0;

      for(int i = 0; i < NSQUARES; i++)
	for(int j = 0; j < NSQUARES; j++)
	  {
	    if(board[i][j] != EMPTY_SQUARE)
	      nonEmptyThen += 1;

	    if(current_move.board[i][j] != EMPTY_SQUARE)
	      nonEmptyNow += 1;
	  }

      if(nonEmptyNow != nonEmptyThen)
	wonPiece = true;
      else
	wonPiece = false;

      for(int i = 0; i < NSQUARES; i++)
	for(int j = 0; j < NSQUARES; j++)
	  board[i][j] = current_move.board[i][j];

      if(gui)
	{
	  gui->addHistoryMove(current_move, color);

	  if(gui->getGLBoard())
	    gui->getGLBoard()->updateGL();

	  gui->update();
	}

      if(gui && game_over)
	{
	  gui->showGameOver(getTurn());
	  gui->stopTimers(PLAYER_TIMER);
	  gui->stopTimers(OPPONENT_TIMER);
	}
    }
}
