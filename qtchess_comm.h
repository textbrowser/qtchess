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

#ifndef QTCHESS_PLUGIN
#include <QPointer>
#include <QTcpServer>
#include <QTcpSocket>
#endif

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

  bool isSet(void);
  bool isReady(void);
  void init(void);
  void quit(void);
  void sendMove(const struct move_s);
  void setListen(void);
  void setConnected(const bool);
  void connectRemotely(void);
  void disconnectRemotely(void);

 private:

  /*
  ** -- Members --
  */

  bool connected;
#ifndef QTCHESS_PLUGIN
  QTcpServer listening_sock;
  QTcpSocket send_sock;
  QPointer<QTcpSocket> clientConnection;
#endif

  /*
  ** -- Methods --
  */

 private slots:

  void updateBoard(void);
#ifndef QTCHESS_PLUGIN
  void acceptConnection(void);
#endif
  void clientDisconnected(void);
  void slotClientConnected(void);

#ifndef QTCHESS_PLUGIN
 signals:
  void connectedToClient(void);
#endif
};

#endif
