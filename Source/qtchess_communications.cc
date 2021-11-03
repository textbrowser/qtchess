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

#include <QCryptographicHash>

#include "qtchess.h"
#include "qtchess_communications.h"
#include "qtchess_gui.h"

extern QPointer<qtchess> chess;
extern QPointer<qtchess_communications> comm;
extern QPointer<qtchess_gui> gui;
static QByteArray s_eof = "\n";

qtchess_communications::qtchess_communications(void):QObject()
{
  connect(&m_listening_sock,
	  SIGNAL(newConnection(void)),
	  this,
	  SLOT(slot_accept_connection(void)));
}

QByteArray qtchess_communications::digest(const QByteArray &data) const
{
  QByteArray key;

  if(m_client_connection)
    {
      auto a(m_client_connection->localAddress());
      auto b(m_client_connection->peerAddress());

      key = xor_arrays
	(a.toString().toUtf8().toHex(), b.toString().toUtf8().toHex());
      key.append(QByteArray::number(m_client_connection->localPort() ^
				    m_client_connection->peerPort()).toHex());
      key.append(m_caissa.toUtf8().toHex());
    }

  return hmac(data, hmac(key, QByteArray("QtChess").append(0x01)));
}

QByteArray qtchess_communications::
hmac(const QByteArray &data, const QByteArray &k) const
{
  auto key(k);
  static const int s_block_length = 512 / CHAR_BIT;

  if(s_block_length < key.length())
    key = sha1(key);

  if(s_block_length > key.length())
    key.append(QByteArray(s_block_length - key.length(), 0));

  QByteArray left(s_block_length, 0);
  const QByteArray ipad(s_block_length, 0x36);
  const QByteArray opad(s_block_length, 0x5c);

  for(int i = 0; i < s_block_length; i++)
    left[i] = static_cast<char> (key.at(i) ^ opad.at(i));

  QByteArray right(s_block_length, 0);

  for(int i = 0; i < s_block_length; i++)
    right[i] = static_cast<char> (key.at(i) ^ ipad.at(i));

  return sha1(left.append(sha1(right.append(data))));
}

QByteArray qtchess_communications::sha1(const QByteArray &data) const
{
  QCryptographicHash sha1(QCryptographicHash::Sha1);

  sha1.addData(data);
  return sha1.result();
}

QByteArray qtchess_communications::xor_arrays
(const QByteArray &a, const QByteArray &b) const
{
  QByteArray bytes(qMin(a.length(), b.length()), 0);

  for(int i = 0; i < bytes.length(); i++)
    bytes[i] = static_cast<char> (a[i] ^ b[i]);

  return bytes;
}

bool qtchess_communications::is_connected_remotely(void) const
{
  return m_client_connection &&
    m_client_connection->state() == QAbstractSocket::ConnectedState;
}

bool qtchess_communications::is_listening(void) const
{
  return m_listening_sock.isListening();
}

bool qtchess_communications::is_ready(void) const
{
  return is_connected_remotely();
}

bool qtchess_communications::
memcmp(const QByteArray &a, const QByteArray &b) const
{
  auto length = qMax(a.length(), b.length());
  quint64 rc = 0;

  for(int i = 0; i < length; i++)
    rc |= (i < a.length() ? static_cast<quint64> (a.at(i)) : 0ULL) ^
      (i < b.length() ? static_cast<quint64> (b.at(i)) : 0ULL);

  return rc == 0;
}

void qtchess_communications::connect_remotely(void)
{
  QString scope_id("");
  QString str1("");
  QString str2("");
  quint16 remote_port = 0;

  if(gui)
    {
      if(gui->get_setup_dialog() &&
	 gui->get_setup_dialog()->get_remote_host_field())
	str1 = gui->get_setup_dialog()->get_remote_host_field()->
	  text().trimmed();

      if(gui->get_setup_dialog() &&
	 gui->get_setup_dialog()->get_remote_port_field())
	str2 = gui->get_setup_dialog()->get_remote_port_field()->
	  text().trimmed();

      if(gui->get_setup_dialog() &&
	 gui->get_setup_dialog()->get_remote_scope_id_field())
	scope_id = gui->get_setup_dialog()->get_remote_scope_id_field()->
	  text().trimmed();
    }

  remote_port = static_cast<quint16> (str2.toInt());

  QHostAddress address(str1);

  if(!scope_id.isEmpty())
    address.setScopeId(scope_id);

  if(m_client_connection)
    {
      m_client_connection->abort();
      m_client_connection->deleteLater();
    }

  m_client_connection = new QTcpSocket(this);
  connect(m_client_connection,
	  SIGNAL(connected(void)),
	  this,
	  SIGNAL(connected_to_client(void)));
  connect(m_client_connection,
	  SIGNAL(connected(void)),
	  this,
	  SLOT(slot_client_connected(void)));
  connect(m_client_connection,
	  SIGNAL(disconnected(void)),
	  this,
	  SIGNAL(disconnected_from_client(void)));
  connect(m_client_connection,
	  SIGNAL(disconnected(void)),
	  this,
	  SLOT(slot_client_disconnected(void)));
#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
  connect(m_client_connection,
	  SIGNAL(error(QAbstractSocket::SocketError)),
	  m_client_connection,
	  SIGNAL(disconnected(void)));
#else
  connect(m_client_connection,
	  SIGNAL(errorOccurred(QAbstractSocket::SocketError)),
	  m_client_connection,
	  SIGNAL(disconnected(void)));
#endif
  connect(m_client_connection,
	  SIGNAL(readyRead(void)),
	  this,
	  SLOT(slot_update_board(void)));
  m_client_connection->connectToHost(address, remote_port);
}

void qtchess_communications::disconnect_remotely(void)
{
  if(m_client_connection)
    m_client_connection->deleteLater();

  if(chess)
    {
      chess->set_first(-1);
      chess->set_my_color(-1);
      chess->set_turn(-1);
    }

  if(gui)
    gui->show_disconnect();
}

void qtchess_communications::initialize(void)
{
  if(m_listening_sock.isListening())
    m_listening_sock.close();

  if(gui && gui->get_setup_dialog() &&
     gui->get_setup_dialog()->get_allowed_host_field())
    gui->get_setup_dialog()->get_allowed_host_field()->setText
      (QHostAddress(QHostAddress::LocalHost).toString());

  if(gui && gui->get_setup_dialog() &&
     gui->get_setup_dialog()->get_local_host_field())
    gui->get_setup_dialog()->get_local_host_field()->setText
      (QHostAddress(QHostAddress::LocalHost).toString());
}

void qtchess_communications::quit(void)
{
  /*
  ** Terminate all communications.
  */

  if(m_client_connection)
    m_client_connection->deleteLater();

  m_listening_sock.close();
}

void qtchess_communications::send_move(const struct move_s &current_move)
{
  if(!m_client_connection)
    return;
  else if(m_client_connection->state() != QAbstractSocket::ConnectedState)
    return;

  QByteArray buffer;

  /*
  ** Copy the structure.
  */

  buffer.append(QByteArray::number(current_move.m_x1));
  buffer.append(" ");
  buffer.append(QByteArray::number(current_move.m_x2));
  buffer.append(" ");
  buffer.append(QByteArray::number(current_move.m_y1));
  buffer.append(" ");
  buffer.append(QByteArray::number(current_move.m_y2));
  buffer.append(" ");
  buffer.append(QByteArray::number(current_move.m_rook_x1));
  buffer.append(" ");
  buffer.append(QByteArray::number(current_move.m_rook_x2));
  buffer.append(" ");
  buffer.append(QByteArray::number(current_move.m_rook_y1));
  buffer.append(" ");
  buffer.append(QByteArray::number(current_move.m_rook_y2));
  buffer.append(" ");
  buffer.append(QByteArray::number(current_move.m_piece));
  buffer.append(" ");
  buffer.append(QByteArray::number(current_move.m_rook));
  buffer.append(" ");
  buffer.append(QByteArray::number(current_move.m_promoted));
  buffer.append(" ");
  buffer.append(QByteArray::number(current_move.m_pawn2));
  buffer.append(" ");
  buffer.append(QByteArray::number(current_move.m_enpassant));
  buffer.append(" ");
  buffer.append(QByteArray::number(current_move.m_is_opponent_king_threat));
  buffer.append(" ");
  buffer.append(current_move.m_departure);
  buffer.append(" ");

  for(int i = 0; i < NSQUARES; i++)
    for(int j = 0; j < NSQUARES; j++)
      {
	buffer.append(QByteArray::number(current_move.m_board[i][j]));
	buffer.append(" ");
      }

  buffer.append(digest(buffer).toHex());
  buffer.append(s_eof);

  QApplication::setOverrideCursor(Qt::WaitCursor);

  if(m_client_connection->write(buffer.constData(), buffer.length()) !=
     static_cast<qint64> (buffer.length()))
    {
      QApplication::restoreOverrideCursor();

      if(gui)
	gui->show_error_message("Move could not be delivered.");
    }
  else
    {
      QApplication::restoreOverrideCursor();

      if(chess)
	chess->set_turn(THEIR_TURN);
    }
}

void qtchess_communications::set_caissa(const QString &caissa)
{
  m_caissa = caissa;
}

void qtchess_communications::set_listen(void)
{
  if(m_listening_sock.isListening())
    return;

  m_listening_sock.setMaxPendingConnections(1);

  /*
  ** Listen!
  */

  QHostAddress address(QHostAddress::Any);
  quint16 port = 0;

  if(gui && gui->get_setup_dialog() &&
     gui->get_setup_dialog()->get_local_port_field())
    {
      address = gui->get_setup_dialog()->get_listening_address();
      port = gui->get_setup_dialog()->get_local_port_field()->text().toUShort();
    }

  m_listening_sock.listen(address, port);

  /*
  ** Save the port number.
  */

  if(gui && gui->get_setup_dialog() &&
     gui->get_setup_dialog()->get_local_port_field() &&
     m_listening_sock.isListening())
    gui->get_setup_dialog()->get_local_port_field()->setValue
      (static_cast<int> (m_listening_sock.serverPort()));
}

void qtchess_communications::slot_accept_connection(void)
{
  auto socket = m_listening_sock.nextPendingConnection();

  if(!socket)
    return;
  else if(m_client_connection)
    {
      socket->abort();
      socket->deleteLater();
      return;
    }
  else
    m_client_connection = socket;

  m_client_connection->setParent(this);

  /*
  ** Acceptable peer?
  */

  if(gui && gui->get_setup_dialog() &&
     gui->get_setup_dialog()->get_allowed_host_field() &&
     !gui->get_setup_dialog()->get_allowed_host_field()->text().
     trimmed().isEmpty())
    {
      auto str
	(gui->get_setup_dialog()->get_allowed_host_field()->text().trimmed());

      if(QHostAddress(str) != m_client_connection->peerAddress())
	{
	  m_client_connection->abort();
	  m_client_connection->deleteLater();
	  return;
	}
    }

  emit connected_to_client();
  connect(m_client_connection,
	  SIGNAL(disconnected(void)),
	  this,
	  SLOT(slot_client_disconnected(void)));
#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
  connect(m_client_connection,
	  SIGNAL(error(QAbstractSocket::SocketError)),
	  m_client_connection,
	  SIGNAL(disconnected(void)));
#else
  connect(m_client_connection,
	  SIGNAL(errorOccurred(QAbstractSocket::SocketError)),
	  m_client_connection,
	  SIGNAL(disconnected(void)));
#endif
  connect(m_client_connection,
	  SIGNAL(readyRead(void)),
	  this,
	  SLOT(slot_update_board(void)));

  if(gui)
    gui->notify_connection(m_client_connection->peerAddress().toString(),
			   m_client_connection->peerPort());

  if(chess && chess->get_first() == -1)
    {
      chess->set_first(THEY_ARE_FIRST);
      chess->set_my_color(BLACK);
      chess->set_turn(THEIR_TURN);
    }
}

void qtchess_communications::slot_client_connected(void)
{
  if(!gui)
    {
      slot_client_disconnected();
      return;
    }

  if(chess && chess->get_first() == -1)
    {
      chess->set_first(I_AM_FIRST);
      chess->set_turn(MY_TURN);

      if(gui->color() == tr("Beige"))
	chess->set_my_color(WHITE);
      else if(gui->color() == tr("Crimson"))
	chess->set_my_color(BLACK);
      else
	{
	  slot_client_disconnected();
	  return;
	}
    }

  if(m_client_connection)
    gui->notify_connection(m_client_connection->peerAddress().toString(),
			   m_client_connection->peerPort());
}

void qtchess_communications::slot_client_disconnected(void)
{
  auto socket = qobject_cast<QTcpSocket *> (sender());

  if(m_client_connection && m_client_connection == socket)
    m_client_connection->deleteLater();
  else if(socket)
    socket->deleteLater();

  if(chess)
    {
      chess->set_first(-1);
      chess->set_my_color(-1);
      chess->set_turn(-1);
    }

  if(gui)
    gui->set_status_text(tr("Status: Peer Disconnected"));

  emit disconnected_from_client();
}

void qtchess_communications::slot_update_board(void)
{
  int ntries = 1;
  static const int s_sha1_output_size = 40;

  while(m_client_connection &&
	m_client_connection->canReadLine() &&
	ntries <= 5)
    {
      QApplication::setOverrideCursor(Qt::WaitCursor);

      QByteArray buffer(s_buffer_size, 0);

      if(m_client_connection->readLine(buffer.data(), buffer.length()) != -1)
	{
	  QApplication::restoreOverrideCursor();

	  if(chess)
	    {
	      buffer = buffer.mid(0, buffer.indexOf(s_eof));

	      auto d
		(QByteArray::fromHex(buffer.mid(buffer.length() -
						s_sha1_output_size)));

	      buffer = buffer.mid(0, buffer.length() - s_sha1_output_size);

	      if(memcmp(d, digest(buffer)))
		chess->update_board(buffer);
	    }

	  break;
	}
      else
	QApplication::restoreOverrideCursor();

      ntries += 1;
    }

  if(m_client_connection)
    m_client_connection->readAll();
}

void qtchess_communications::stop_listening(void)
{
  m_listening_sock.close();
}
