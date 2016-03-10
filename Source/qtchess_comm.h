#ifndef _QTCHESS_COMM_
#define _QTCHESS_COMM_

/*
** -- System Includes --
*/

extern "C"
{
#include <stdio.h>
}

/*
** -- Qt Includes --
*/

#include <QPointer>
#include <QTcpServer>
#include <QTcpSocket>

/*
** -- Local Includes --
*/

#include "qtchess.h"
#include "qtchess_defs.h"

class qtchess_comm: public QObject
{
  Q_OBJECT

 public:
  static const int s_buffer_size = 1024;

  /*
  ** -- Members --
  */

  /*
  ** -- Methods --
  */

  qtchess_comm(void);

  ~qtchess_comm()
  {
  }

  QHostAddress peerAddress(void) const
  {
    if(m_clientConnection)
      return m_clientConnection->peerAddress();
    else
      return QHostAddress();
  }

  quint16 peerPort(void) const
  {
    if(m_clientConnection)
      return m_clientConnection->peerPort();
    else
      return 0;
  }

  bool isConnectedRemotely(void) const;
  bool isListening(void) const;
  bool isReady(void) const;
  bool isSet(void) const;
  void connectRemotely(void);
  void disconnectRemotely(void);
  void init(void);
  void quit(void);
  void sendMove(const struct move_s);
  void setConnected(const bool);
  void setListen(void);
  void stopListening(void);

 private:
  /*
  ** -- Members --
  */

  QPointer<QTcpSocket> m_clientConnection;
  QTcpServer listening_sock;
  bool connected;

  /*
  ** -- Methods --
  */

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
