/*
** -- Qt Includes --
*/

#include <QtDebug>

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
  int ntries = 1;
  char buffer[BUFFER_SIZE];

  while(clientConnection != 0 && clientConnection->canReadLine() &&
	ntries <= 5)
    {
      QApplication::setOverrideCursor(Qt::WaitCursor);

      if(clientConnection->readLine(buffer, (qint64) sizeof(buffer)) != -1)
	{
	  QApplication::restoreOverrideCursor();

	  if(chess)
	    chess->updateBoard(buffer);

	  break;
	}
      else
	QApplication::restoreOverrideCursor();

      ntries += 1;
    }
}

void qtchess_comm::init(void)
{
  connected = false;

  if(listening_sock.isListening())
    listening_sock.close();

  setListen();

  if(gui && gui->getSetupDialog() && gui->getSetupDialog()->getHostField())
    gui->getSetupDialog()->getHostField()->setText
      (QHostAddress(QHostAddress::LocalHost).toString());

  if(gui && gui->getSetupDialog() &&
     gui->getSetupDialog()->getAllowedHostField())
    gui->getSetupDialog()->getAllowedHostField()->setText("0.0.0.0");
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
      if(send_sock.isOpen() && !listening_sock.isListening())
	return true;
      else
	return false;
    }
  else
    return false;
}

void qtchess_comm::setListen(void)
{
  if(listening_sock.isListening())
    return;

  listening_sock.setMaxPendingConnections(1);

  /*
  ** Listen!
  */

  if(!listening_sock.listen())
    if(chess)
      chess->quit("listen() failure.", EXIT_FAILURE);

  /*
  ** Save the port number.
  */

  if(gui && gui->getSetupDialog() && gui->getSetupDialog()->getPortField())
    gui->getSetupDialog()->getPortField()->setText
      (QString::number(listening_sock.serverPort()));

  if(gui)
    gui->setStatusText
      ("Status: Local Port " + QString::number(listening_sock.serverPort()));
}

void qtchess_comm::quit(void)
{
  /*
  ** Terminate all communications.
  */

  if(send_sock.isOpen())
    send_sock.close();

  if(listening_sock.isListening())
    listening_sock.close();

  setConnected(false);
}

void qtchess_comm::setConnected(const bool connected_arg)
{
  connected = connected_arg;
}

void qtchess_comm::disconnectRemotely(void)
{
  if(send_sock.isOpen())
    send_sock.close();

  if(chess)
    {
      chess->setTurn(-1);
      chess->setFirst(-1);
      chess->setMyColor(-1);
    }

  setConnected(false);

  if(gui)
    gui->showDisconnect();
}

void qtchess_comm::connectRemotely(void)
{
  QString str1 = "", str2 = "";
  quint16 remotePort = 0;

  if(gui)
    {
      if(gui->getSetupDialog() && gui->getSetupDialog()->getRHostField())
	str1 = gui->getSetupDialog()->getRHostField()->text().trimmed();

      if(gui->getSetupDialog() && gui->getSetupDialog()->getRPortField())
	str2 = gui->getSetupDialog()->getRPortField()->text().trimmed();
    }

  remotePort = (quint16) str2.toInt();
  send_sock.abort();
  send_sock.connectToHost(str1, remotePort);
}

void qtchess_comm::sendMove(const struct move_s current_move)
{
  int i = 0, j = 0;
  char buffer[BUFFER_SIZE];
  char numstr[32];

  if(!send_sock.isOpen())
    return;

  /*
  ** Copy the structure.
  */

  memset(buffer, 0, sizeof(buffer));
  snprintf(buffer, sizeof(buffer),
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
	memset(numstr, 0, sizeof(numstr));
	snprintf(numstr, sizeof(numstr), "%d ",
		 current_move.board[i][j]);
	strcat(buffer, numstr);
      }

  /*
  ** Remove the extra space.
  */

  buffer[strlen(buffer) - 1] = '\0';
  strcat(buffer, "\n");

  QApplication::setOverrideCursor(Qt::WaitCursor);

  if(send_sock.write(buffer, (qint64) strlen(buffer)) == -1)
    {
      QApplication::restoreOverrideCursor();

      if(gui)
	gui->showErrorMsg("Move could not be delivered.");
    }
  else
    {
      QApplication::restoreOverrideCursor();

      if(chess)
	chess->setTurn(THEIR_TURN);
    }
}

qtchess_comm::qtchess_comm(void)
{
  clientConnection = 0;
  connect(&listening_sock, SIGNAL(newConnection(void)), this,
	  SLOT(acceptConnection(void)));
  connect(&send_sock,
	  SIGNAL(connected(void)),
	  this,
	  SIGNAL(connectedToClient(void)));
  connect(&send_sock,
	  SIGNAL(connected(void)),
	  this,
	  SLOT(slotClientConnected(void)));
  connect(&send_sock,
	  SIGNAL(disconnected(void)),
	  this,
	  SLOT(clientDisconnected(void)));
}

void qtchess_comm::acceptConnection(void)
{
  if(!listening_sock.hasPendingConnections())
    return;

  if((clientConnection = listening_sock.nextPendingConnection()) == 0)
    return;

  /*
  ** Acceptable peer?
  */

  if(gui && gui->getSetupDialog() &&
     gui->getSetupDialog()->getAllowedHostField() &&
     !gui->getSetupDialog()->getAllowedHostField()->text().trimmed().isEmpty())
    {
      QString str(gui->getSetupDialog()->
		  getAllowedHostField()->text().trimmed());

      if(clientConnection &&
	 QHostAddress(str) != clientConnection->peerAddress())
	{
	  clientConnection->abort();
	  clientConnection->deleteLater();
	  return;
	}
    }

  setConnected(true);
  connect(clientConnection, SIGNAL(readyRead(void)), this,
	  SLOT(updateBoard(void)));
  connect(clientConnection, SIGNAL(disconnected(void)), this,
	  SLOT(clientDisconnected(void)));

  if(gui && clientConnection)
    gui->notifyConnection(clientConnection->peerAddress().toString());

  listening_sock.close();

  if(chess && chess->getFirst() == -1)
    {
      chess->setTurn(THEIR_TURN);
      chess->setFirst(THEY_ARE_FIRST);
      chess->setMyColor(BLACK);
    }
}

void qtchess_comm::slotClientConnected(void)
{
  setConnected(true);

  if(chess && chess->getFirst() == -1)
    {
      chess->setTurn(MY_TURN);
      chess->setFirst(I_AM_FIRST);
      chess->setMyColor(WHITE);
    }
}

void qtchess_comm::clientDisconnected(void)
{
  QTcpSocket *socket = qobject_cast<QTcpSocket *> (sender());

  if(socket)
    if(socket != &send_sock && clientConnection)
      clientConnection->deleteLater();

  if(chess)
    {
      chess->setTurn(-1);
      chess->setFirst(-1);
      chess->setMyColor(-1);
    }

  setListen();
  disconnectRemotely();
}
