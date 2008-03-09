/*
** -- Local Includes --
*/

#include "qtchess_comm.h"

extern qtchess *chess;
extern qtchess_gui *gui;
extern qtchess_comm *comm;
extern QApplication *qapp;

void qtchess_comm::updateBoard(void)
{
  char buffer[BUFFER_SIZE];

#ifndef QTCHESS_PLUGIN
  while(clientConnection != NULL && clientConnection->canReadLine())
    {
      QApplication::setOverrideCursor(Qt::WaitCursor);

      if(clientConnection->readLine(buffer, (qint64) sizeof(buffer)) != -1)
	{
	  QApplication::restoreOverrideCursor();
	  chess->updateBoard(buffer);
	}
      else
	QApplication::restoreOverrideCursor();
    }
#else
  (void) buffer;
#endif
}

void qtchess_comm::init(void)
{
#ifndef QTCHESS_PLUGIN
  connected = false;

  if(listening_sock.isListening())
    listening_sock.close();

  setListen();
  gui->getSetupDialog()->getHostField()->setText("127.0.0.1");
#else
  connected = true;
#endif
}

bool qtchess_comm::isSet(void)
{
  if(connected)
    return true;
  else
    return false;
}

bool qtchess_comm::isReady(void)
{
  if(isSet())
    {
#ifndef QTCHESS_PLUGIN
      if(send_sock.isOpen() && !listening_sock.isListening())
	return true;
      else
	return false;
#else
      return true;
#endif
    }
  else
    return false;
}

void qtchess_comm::setListen(void)
{
#ifndef QTCHESS_PLUGIN
  listening_sock.setMaxPendingConnections(1);

  /*
  ** Listen!
  */

  if(!listening_sock.listen())
    chess->quit("listen() failure.", EXIT_FAILURE);

  /*
  ** Save the port number.
  */

  gui->getSetupDialog()->getPortField()->setText
    (QString::number(listening_sock.serverPort()));
  gui->setStatusText
    ("Status: Local Port " + QString::number(listening_sock.serverPort()));
#else
  gui->setStatusText("Status: Ready");
#endif
}

void qtchess_comm::quit(void)
{
#ifndef QTCHESS_PLUGIN
  /*
  ** Terminate all communications.
  */

  if(send_sock.isOpen())
    send_sock.close();

  if(listening_sock.isListening())
    listening_sock.close();
#endif

  setConnected(false);
}

void qtchess_comm::setConnected(const bool connected_arg)
{
  connected = connected_arg;
}

void qtchess_comm::disconnectRemotely(void)
{
#ifndef QTCHESS_PLUGIN
  if(send_sock.isOpen())
    send_sock.close();
#endif

  chess->setTurn(-1);
  chess->setFirst(-1);
  chess->setMyColor(-1);
  setConnected(false);
  gui->showDisconnect(false);
}

void qtchess_comm::connectRemotely(void)
{
#ifndef QTCHESS_PLUGIN
  QString str1 = "", str2 = "";
  quint16 remotePort = 0;

  str1 = gui->getSetupDialog()->getRHostField()->text();
  str1 = str1.trimmed();
  str2 = gui->getSetupDialog()->getRPortField()->text();
  str2 = str2.trimmed();
  remotePort = (quint16) str2.toInt();
  QApplication::setOverrideCursor(Qt::WaitCursor);
  send_sock.connectToHost(str1, remotePort);

  if(!send_sock.waitForConnected(10000))
    {
      if(send_sock.isOpen())
	send_sock.close();

      QApplication::restoreOverrideCursor();
      return;
    }

  QApplication::restoreOverrideCursor();
#endif
  setConnected(true);

  if(chess->getFirst() == -1)
    {
      chess->setTurn(MY_TURN);
      chess->setFirst(I_AM_FIRST);
      chess->setMyColor(WHITE);
    }
}

void qtchess_comm::sendMove(const struct move_s current_move)
{
  int i = 0, j = 0;
  char buffer[BUFFER_SIZE];
  char numstr[32];

#ifndef QTCHESS_PLUGIN
  if(!send_sock.isOpen())
    return;
#endif

  /*
  ** Copy the structure.
  */

  (void) memset(buffer, 0, sizeof(buffer));
  (void) snprintf(buffer, sizeof(buffer),
		  "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %s ",
		  current_move.x1,
		  current_move.x2,
		  current_move.y1,
		  current_move.y2,
		  current_move.r_x1,
		  current_move.r_x2,
		  current_move.r_y1,
		  current_move.r_y2,
		  current_move.piece,
		  current_move.rook,
		  current_move.promoted,
		  current_move.pawn_2,
		  current_move.enpassant,
		  current_move.isOppKingThreat,
		  current_move.departure);

  for(i = 0; i < NSQUARES; i++)
    for(j = 0; j < NSQUARES; j++)
      {
	(void) memset(numstr, 0, sizeof(numstr));
	(void) snprintf(numstr, sizeof(numstr), "%d ",
			current_move.board[i][j]);
	(void) strcat(buffer, numstr);
      }

  /*
  ** Remove the extra space.
  */

  buffer[strlen(buffer) - 1] = '\0';
  (void) strcat(buffer, "\n");

#ifndef QTCHESS_PLUGIN
  QApplication::setOverrideCursor(Qt::WaitCursor);

  if(send_sock.write(buffer, (qint64) strlen(buffer)) == -1)
    {
      QApplication::restoreOverrideCursor();
      gui->showErrorMsg("Move could not be delivered.");
    }
  else
    {
      QApplication::restoreOverrideCursor();
      chess->setTurn(THEIR_TURN);
    }
#else
#endif
}

qtchess_comm::qtchess_comm(void)
{
#ifndef QTCHESS_PLUGIN
  clientConnection = NULL;
  connect(&listening_sock, SIGNAL(newConnection()), this,
	  SLOT(acceptConnection()));
#endif
}

#ifndef QTCHESS_PLUGIN
void qtchess_comm::acceptConnection(void)
{
  if(!listening_sock.hasPendingConnections())
    return;

  if((clientConnection = listening_sock.nextPendingConnection()) == NULL)
    return;

  connect(clientConnection, SIGNAL(readyRead()), this, SLOT(updateBoard()));
  connect(clientConnection, SIGNAL(disconnected()), this,
	  SLOT(clientDisconnected()));
  gui->notifyConnection(clientConnection->peerAddress().toString());
  listening_sock.close();

  if(chess->getFirst() == -1)
    {
      chess->setTurn(THEIR_TURN);
      chess->setFirst(THEY_ARE_FIRST);
      chess->setMyColor(BLACK);
    }
}
#endif

void qtchess_comm::clientDisconnected(void)
{
#ifndef QTCHESS_PLUGIN
  clientConnection->deleteLater();
#endif
  chess->setTurn(-1);
  chess->setFirst(-1);
  chess->setMyColor(-1);
#ifndef QTCHESS_PLUGIN
  setListen();
#endif
  disconnectRemotely();
  gui->showDisconnect(true);
}
