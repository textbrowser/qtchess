#include "qtchess.h"
#include "qtchess_comm.h"
#include "qtchess_gui.h"

extern qtchess *chess;
extern qtchess_comm *comm;
extern qtchess_gui *gui;

qtchess_comm::qtchess_comm(void):QObject()
{
  connected = false;
  connect(&listening_sock,
	  SIGNAL(newConnection(void)),
	  this,
	  SLOT(acceptConnection(void)));
}

bool qtchess_comm::isConnectedRemotely(void) const
{
  return m_clientConnection &&
    m_clientConnection->state() == QAbstractSocket::ConnectedState;
}

bool qtchess_comm::isListening(void) const
{
  return listening_sock.isListening();
}

bool qtchess_comm::isReady(void) const
{
  if(isSet())
    {
      if(m_clientConnection &&
	 m_clientConnection->state() == QAbstractSocket::ConnectedState)
	return true;
      else
	return false;
    }
  else
    return false;
}

bool qtchess_comm::isSet(void) const
{
  if(connected)
    return true;
  else
    return false;
}

void qtchess_comm::acceptConnection(void)
{
  QTcpSocket *socket = listening_sock.nextPendingConnection();

  if(!socket)
    return;
  else if(m_clientConnection)
    {
      socket->abort();
      socket->deleteLater();
      return;
    }
  else
    m_clientConnection = socket;

  m_clientConnection->setParent(this);

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

      if(QHostAddress(str) != m_clientConnection->peerAddress())
	{
	  m_clientConnection->abort();
	  m_clientConnection->deleteLater();
	  return;
	}
    }

  setConnected(true);
  emit connectedToClient();
  connect(m_clientConnection,
	  SIGNAL(disconnected(void)),
	  this,
	  SLOT(clientDisconnected(void)));
  connect(m_clientConnection,
	  SIGNAL(error(QAbstractSocket::SocketError)),
	  m_clientConnection,
	  SIGNAL(disconnected(void)));
  connect(m_clientConnection,
	  SIGNAL(readyRead(void)),
	  this,
	  SLOT(updateBoard(void)));

  if(gui)
    gui->notifyConnection(m_clientConnection->peerAddress().toString(),
			  m_clientConnection->peerPort());

  if(chess && chess->getFirst() == -1)
    {
      chess->setFirst(THEY_ARE_FIRST);
      chess->setMyColor(BLACK);
      chess->setTurn(THEIR_TURN);
    }
}

void qtchess_comm::clientDisconnected(void)
{
  QTcpSocket *socket = qobject_cast<QTcpSocket *> (sender());

  if(m_clientConnection && m_clientConnection == socket)
    m_clientConnection->deleteLater();
  else if(socket)
    socket->deleteLater();

  if(chess)
    {
      chess->setFirst(-1);
      chess->setMyColor(-1);
      chess->setTurn(-1);
    }

  if(gui)
    gui->setStatusText(tr("Status: Ready"));

  emit disconnectedFromClient();
  setConnected(false);
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

  if(m_clientConnection)
    {
      m_clientConnection->abort();
      m_clientConnection->deleteLater();
    }

  m_clientConnection = new QTcpSocket(this);
  connect(m_clientConnection,
	  SIGNAL(connected(void)),
	  this,
	  SIGNAL(connectedToClient(void)));
  connect(m_clientConnection,
	  SIGNAL(connected(void)),
	  this,
	  SLOT(slotClientConnected(void)));
  connect(m_clientConnection,
	  SIGNAL(disconnected(void)),
	  this,
	  SLOT(clientDisconnected(void)));
  connect(m_clientConnection,
	  SIGNAL(disconnected(void)),
	  this,
	  SIGNAL(disconnectedFromClient(void)));
  connect(m_clientConnection,
	  SIGNAL(error(QAbstractSocket::SocketError)),
	  m_clientConnection,
	  SIGNAL(disconnected(void)));
  connect(m_clientConnection,
	  SIGNAL(readyRead(void)),
	  this,
	  SLOT(updateBoard(void)));
  m_clientConnection->connectToHost(address, remotePort);
}

void qtchess_comm::disconnectRemotely(void)
{
  if(m_clientConnection)
    m_clientConnection->deleteLater();

  if(chess)
    {
      chess->setFirst(-1);
      chess->setMyColor(-1);
      chess->setTurn(-1);
    }

  setConnected(false);

  if(gui)
    gui->showDisconnect();
}

void qtchess_comm::init(void)
{
  connected = false;

  if(listening_sock.isListening())
    listening_sock.close();

  if(gui && gui->getSetupDialog() && gui->getSetupDialog()->getHostField())
    gui->getSetupDialog()->getHostField()->setText
      (QHostAddress(QHostAddress::LocalHost).toString());

  if(gui && gui->getSetupDialog() &&
     gui->getSetupDialog()->getAllowedHostField())
    gui->getSetupDialog()->getAllowedHostField()->setText
      (QHostAddress(QHostAddress::LocalHost).toString());
}

void qtchess_comm::quit(void)
{
  /*
  ** Terminate all communications.
  */

  if(m_clientConnection)
    m_clientConnection->deleteLater();

  listening_sock.close();
  setConnected(false);
}

void qtchess_comm::sendMove(const struct move_s &current_move)
{
  if(!m_clientConnection)
    return;
  else if(m_clientConnection->state() != QAbstractSocket::ConnectedState)
    return;

  QByteArray buffer;

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

  for(int i = 0; i < NSQUARES; i++)
    for(int j = 0; j < NSQUARES; j++)
      {
	buffer.append(QByteArray::number(current_move.board[i][j]));
	buffer.append(" ");
      }

  buffer.append("\n");

  QApplication::setOverrideCursor(Qt::WaitCursor);

  if(m_clientConnection->write(buffer.constData(),
			       buffer.length()) != (qint64) buffer.length())
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

void qtchess_comm::setConnected(const bool connected_arg)
{
  connected = connected_arg;
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
	gui->getSetupDialog()->getPortField()->setValue
	  (static_cast<int> (listening_sock.serverPort()));
    }
}

void qtchess_comm::slotClientConnected(void)
{
  if(!gui)
    {
      clientDisconnected();
      return;
    }

  setConnected(true);

  if(chess && chess->getFirst() == -1)
    {
      chess->setTurn(MY_TURN);
      chess->setFirst(I_AM_FIRST);

      if(gui->color() == tr("Beige"))
	chess->setMyColor(WHITE);
      else if(gui->color() == tr("Crimson"))
	chess->setMyColor(BLACK);
      else
	{
	  clientDisconnected();
	  return;
	}
    }

  if(m_clientConnection)
    gui->notifyConnection(m_clientConnection->peerAddress().toString(),
			  m_clientConnection->peerPort());
}

void qtchess_comm::stopListening(void)
{
  listening_sock.close();
}

void qtchess_comm::updateBoard(void)
{
  int ntries = 1;

  while(m_clientConnection && m_clientConnection->canReadLine() && ntries <= 5)
    {
      QApplication::setOverrideCursor(Qt::WaitCursor);

      QByteArray buffer(s_buffer_size, 0);

      if(m_clientConnection->readLine(buffer.data(), buffer.length()) != -1)
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

  if(m_clientConnection)
    m_clientConnection->readAll();
}
