/*
** -- Qt Includes --
*/

/*
** -- Local Includes --
*/

#include "qtchess_comm.h"

extern QApplication *qapp;
extern qtchess *chess;
extern qtchess_comm *comm;
extern qtchess_gui *gui;

void qtchess_comm::updateBoard(void)
{
  int ntries = 1;

  while(clientConnection && clientConnection->canReadLine() && ntries <= 5)
    {
      QApplication::setOverrideCursor(Qt::WaitCursor);

      QByteArray buffer(BUFFER_SIZE, 0);

      if(clientConnection->
	 readLine(buffer.data(), (qint64) buffer.length()) != -1)
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

  if(gui && gui->getSetupDialog() && gui->getSetupDialog()->getHostField())
    gui->getSetupDialog()->getHostField()->setText
      (QHostAddress(QHostAddress::LocalHost).toString());

  if(gui && gui->getSetupDialog() && gui->getSetupDialog()->getPortField())
    gui->getSetupDialog()->getPortField()->setText("50000");

  if(gui && gui->getSetupDialog() &&
     gui->getSetupDialog()->getAllowedHostField())
    gui->getSetupDialog()->getAllowedHostField()->setText
      (QHostAddress(QHostAddress::LocalHost).toString());
}

bool qtchess_comm::isSet(void) const
{
  if(connected)
    return true;
  else
    return false;
}

bool qtchess_comm::isReady(void) const
{
  if(isSet())
    {
      if(clientConnection &&
	 clientConnection->state() == QAbstractSocket::ConnectedState &&
	 send_sock.state() == QAbstractSocket::ConnectedState)
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

  QHostAddress address(QHostAddress::Any);
  quint16 port = 0;

  if(gui && gui->getSetupDialog())
    {
      address = gui->getSetupDialog()->getListeningAddress();
      port = gui->getSetupDialog()->getPortField()->text().toUShort();
    }

  listening_sock.listen(address, port);

  /*
  ** Save the port number.
  */

  if(gui && gui->getSetupDialog() && gui->getSetupDialog()->getPortField())
    {
      if(listening_sock.isListening())
	gui->getSetupDialog()->getPortField()->setText
	  (QString::number(listening_sock.serverPort()));
    }
}

void qtchess_comm::quit(void)
{
  /*
  ** Terminate all communications.
  */

  send_sock.close();
  listening_sock.close();
  setConnected(false);
}

void qtchess_comm::setConnected(const bool connected_arg)
{
  connected = connected_arg;
}

void qtchess_comm::disconnectRemotely(void)
{
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
  QString scopeId = "", str1 = "", str2 = "";
  quint16 remotePort = 0;

  if(gui)
    {
      if(gui->getSetupDialog() && gui->getSetupDialog()->getRHostField())
	str1 = gui->getSetupDialog()->getRHostField()->text().trimmed();

      if(gui->getSetupDialog() && gui->getSetupDialog()->getRPortField())
	str2 = gui->getSetupDialog()->getRPortField()->text().trimmed();

      if(gui->getSetupDialog() && gui->getSetupDialog()->getRScopeIdField())
	scopeId = gui->getSetupDialog()->getRScopeIdField()->text().trimmed();
    }

  remotePort = (quint16) str2.toInt();

  QHostAddress address(str1);

  if(!scopeId.isEmpty())
    address.setScopeId(scopeId);

  send_sock.abort();
  send_sock.connectToHost(address, remotePort);
}

void qtchess_comm::sendMove(const struct move_s current_move)
{
  if(send_sock.state() != QAbstractSocket::ConnectedState)
    return;

  QByteArray buffer;
  int i = 0, j = 0;

  /*
  ** Copy the structure.
  */

  buffer.append(QByteArray::number(current_move.x1));
  buffer.append(" ");
  buffer.append(QByteArray::number(current_move.x2));
  buffer.append(" ");
  buffer.append(QByteArray::number(current_move.y1));
  buffer.append(" ");
  buffer.append(QByteArray::number(current_move.y2));
  buffer.append(" ");
  buffer.append(QByteArray::number(current_move.r_x1));
  buffer.append(" ");
  buffer.append(QByteArray::number(current_move.r_x2));
  buffer.append(" ");
  buffer.append(QByteArray::number(current_move.r_y1));
  buffer.append(" ");
  buffer.append(QByteArray::number(current_move.r_y2));
  buffer.append(" ");
  buffer.append(QByteArray::number(current_move.piece));
  buffer.append(" ");
  buffer.append(QByteArray::number(current_move.rook));
  buffer.append(" ");
  buffer.append(QByteArray::number(current_move.promoted));
  buffer.append(" ");
  buffer.append(QByteArray::number(current_move.pawn_2));
  buffer.append(" ");
  buffer.append(QByteArray::number(current_move.enpassant));
  buffer.append(" ");
  buffer.append(QByteArray::number(current_move.isOppKingThreat));
  buffer.append(" ");
  buffer.append(current_move.departure);
  buffer.append(" ");

  for(i = 0; i < NSQUARES; i++)
    for(j = 0; j < NSQUARES; j++)
      {
	buffer.append(QByteArray::number(current_move.board[i][j]));
	buffer.append(" ");
      }

  buffer.append("\n");

  QApplication::setOverrideCursor(Qt::WaitCursor);

  if(send_sock.write(buffer.constData(), (qint64) buffer.length()) == -1)
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
  connect(&send_sock,
	  SIGNAL(disconnected(void)),
	  this,
	  SIGNAL(disconnectedFromClient(void)));
}

void qtchess_comm::acceptConnection(void)
{
  if(!listening_sock.hasPendingConnections())
    return;

  QTcpSocket *socket = listening_sock.nextPendingConnection();

  if(!socket)
    return;
  else if(clientConnection)
    {
      socket->abort();
      socket->deleteLater();
      return;
    }
  else
    clientConnection = socket;

  /*
  ** Acceptable peer?
  */

  if(gui && gui->getSetupDialog() &&
     gui->getSetupDialog()->getAllowedHostField() &&
     !gui->getSetupDialog()->getAllowedHostField()->text().
     trimmed().isEmpty())
    {
      QString str(gui->getSetupDialog()->
		  getAllowedHostField()->text().trimmed());

      if(QHostAddress(str) != clientConnection->peerAddress())
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

  if(gui)
    gui->notifyConnection(clientConnection->peerAddress().toString());

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

  if(clientConnection == socket)
    if(clientConnection)
      {
	clientConnection->deleteLater();

	if(gui)
	  gui->setStatusText(tr("Status: Ready"));
      }

  if(chess)
    {
      chess->setTurn(-1);
      chess->setFirst(-1);
      chess->setMyColor(-1);
    }

  setConnected(false);
}

bool qtchess_comm::isListening(void) const
{
  return listening_sock.isListening();
}

void qtchess_comm::stopListening(void)
{
  listening_sock.close();
}

bool qtchess_comm::isConnectedRemotely(void) const
{
  return send_sock.state() == QAbstractSocket::ConnectedState;
}
