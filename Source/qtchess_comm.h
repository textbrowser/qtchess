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

#ifndef _QTCHESS_COMM_
#define _QTCHESS_COMM_

#include <QPointer>
#include <QTcpServer>
#include <QTcpSocket>

class qtchess_comm: public QObject
{
  Q_OBJECT

 public:
  qtchess_comm(void);

  ~qtchess_comm()
  {
  }

  QHostAddress peer_address(void) const
  {
    if(m_client_connection)
      return m_client_connection->peerAddress();
    else
      return QHostAddress();
  }

  quint16 peer_port(void) const
  {
    if(m_client_connection)
      return m_client_connection->peerPort();
    else
      return 0;
  }

  bool isConnectedRemotely(void) const;
  bool isListening(void) const;
  bool is_ready(void) const;
  void connectRemotely(void);
  void disconnectRemotely(void);
  void init(void);
  void quit(void);
  void send_move(const struct move_s &current_move);
  void setCaissa(const QString &caissa);
  void setListen(void);
  void stopListening(void);

 private:
  QPointer<QTcpSocket> m_client_connection;
  QString m_caissa;
  QTcpServer m_listening_sock;
  static const int s_buffer_size = 1024;
  QByteArray digest(const QByteArray &data) const;
  QByteArray hmac(const QByteArray &data, const QByteArray &k) const;
  QByteArray sha1(const QByteArray &data) const;
  QByteArray xorArrays(const QByteArray &a, const QByteArray &b) const;
  bool memcmp(const QByteArray &a, const QByteArray &b) const;

 private slots:
  void acceptConnection(void);
  void clientDisconnected(void);
  void slotClientConnected(void);
  void updateBoard(void);

 signals:
  void connectedToClient(void);
  void disconnectedFromClient(void);
};

#endif
