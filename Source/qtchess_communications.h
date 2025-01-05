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

#ifndef _QTCHESS_COMMUNICATIONS_
#define _QTCHESS_COMMUNICATIONS_

#include <QPointer>
#include <QProcess>
#include <QTcpServer>
#include <QTcpSocket>

class qtchess_communications: public QObject
{
  Q_OBJECT

 public:
  qtchess_communications(void);
  ~qtchess_communications();
  bool is_connected_remotely(void) const;
  bool is_listening(void) const;
  bool is_ready(void) const;
  qint64 gnuchess_process_id(void) const;
  static QHostAddress preferred_host_address
    (const QAbstractSocket::NetworkLayerProtocol protocol);
  void connect_remotely(void);
  void disconnect_remotely(void);
  void initialize(void);
  void quit(void);
  void send_move(const struct move_s &current_move);
  void set_caissa(const QString &caissa);
  void set_listen(void);
  void start_gnuchess(void);
  void stop_listening(void);

 private:
  QPointer<QTcpSocket> m_client_connection;
  QProcess m_gnuchess;
  QString m_caissa;
  QTcpServer m_listening_socket;
  static const int s_buffer_size = 1024;
  QByteArray digest(const QByteArray &data) const;
  QByteArray xor_arrays(const QByteArray &a, const QByteArray &b) const;
  bool memcmp(const QByteArray &a, const QByteArray &b) const;
  static QByteArray hmac(const QByteArray &data, const QByteArray &k);
  static QByteArray shax(const QByteArray &data);
  void prepare_connection_status(void);

 private slots:
  void slot_accept_connection(void);
  void slot_client_connected(void);
  void slot_client_disconnected(void);
  void slot_disconnected(QAbstractSocket::SocketError error);
  void slot_read_gnuchess_output(void);
  void slot_update_board(void);

 signals:
  void connected_to_client(void);
  void disconnected_from_client(void);
};

#endif
