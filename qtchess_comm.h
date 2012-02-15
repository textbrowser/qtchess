#ifndef _QTCHESS_COMM_
#define _QTCHESS_COMM_

#define BUFFER_SIZE 1024

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

  /*
  ** -- Members --
  */

  /*
  ** -- Methods --
  */

  qtchess_comm(void);

  ~qtchess_comm()
    {
#ifdef _DEBUG_
      fprintf(stderr, "~qtchess_comm()\n");
#endif
    }

  bool isSet(void) const;
  bool isReady(void) const;
  bool isListening(void) const;
  bool isConnectedRemotely(void) const;
  void init(void);
  void quit(void);
  void sendMove(const struct move_s);
  void setListen(void);
  void setConnected(const bool);
  void stopListening(void);
  void connectRemotely(void);
  void disconnectRemotely(void);

 private:

  /*
  ** -- Members --
  */

  bool connected;
  QTcpServer listening_sock;
  QTcpSocket send_sock;
  QPointer<QTcpSocket> clientConnection;

  /*
  ** -- Methods --
  */

 private slots:

  void updateBoard(void);
  void acceptConnection(void);
  void clientDisconnected(void);
  void slotClientConnected(void);

 signals:
  void connectedToClient(void);
  void disconnectedFromClient(void);
};

#endif
