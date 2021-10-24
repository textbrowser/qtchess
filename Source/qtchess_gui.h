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

#ifndef _QTCHESS_GUI_
#define _QTCHESS_GUI_

#include <QCloseEvent>
#include <QComboBox>
#include <QHostAddress>
#include <QPointer>
#include <QTimer>

#include "qtchess_gui_opengl.h"
#include "ui_qtchess_helpDialog.h"
#include "ui_qtchess_mainwindow.h"
#include "ui_qtchess_promotionDialog.h"
#include "ui_qtchess_setupDialog.h"

class qtchess_help_dialog: public QDialog
{
  Q_OBJECT

 public:
  qtchess_help_dialog(QWidget *);
  void setup(void);

 private:
  Ui_helpDialog ui;

 private slots:
  void ok_cb(void);
};

class qtchess_setup_dialog: public QDialog
{
  Q_OBJECT

 public:
  qtchess_setup_dialog(QWidget *);
  QHostAddress getListeningAddress(void) const;
  QLineEdit *getAllowedHostField(void) const;
  QLineEdit *getHostField(void) const;
  QLineEdit *getRHostField(void) const;
  QLineEdit *getRScopeIdField(void) const;
  QSpinBox *getPortField(void) const;
  QSpinBox *getRPortField(void) const;

  QString color(void) const
  {
    return ui.color->currentText();
  }

 private:
  Ui_setupDialog ui;

 private slots:
  void close_cb(void);
  void connect_cb(void);
  void slotConnectedToClient(void);
  void slotDisconnect(void);
  void slotDisconnectedFromClient(void);
  void slotListen(void);
  void slotLocal(bool state);
  void slotProtocolChanged(void);
  void slotRemote(bool state);
  void slotSetCaissa(void);
};

class qtchess_promote_dialog: public QDialog
{
  Q_OBJECT

 public:
  qtchess_promote_dialog(QWidget *);

  QComboBox *getMenu(void) const
  {
    return ui.menu;
  }

  void setup(void);

 private:
  Ui_promotionDialog ui;

 private slots:
  void ok_cb(void);
};

class qtchess_gui: public QMainWindow
{
  Q_OBJECT

 public:
  qtchess_gui();
  ~qtchess_gui();

  QString color(void) const
  {
    if(setup_dialog)
      return setup_dialog->color();
    else
      return "";
  }

  openglWid *getGLBoard(void) const
  {
    return glboard;
  }

  qtchess_promote_dialog *getPromoteDialog(void) const;
  qtchess_setup_dialog *getSetupDialog(void) const;
  void addHistoryMove(const struct move_s &, const int);
  void clearHistory(void);
  void init(void);
  void initClocks(void);
  void notifyConnection(const QString &, const quint16);
  void setStatusText(const QString &);
  void showDisconnect(void);
  void showErrorMsg(const char *);
  void showGameOver(const int);
  void startTimers(const int);
  void stopTimers(const int);

 private:
  QPointer<QLabel> statusLabel;
  QPointer<QTimer> opponentt;
  QPointer<QTimer> playert;
  QPointer<openglWid> glboard;
  QPointer<qtchess_help_dialog> help_dialog;
  QPointer<qtchess_promote_dialog> promote_dialog;
  QPointer<qtchess_setup_dialog> setup_dialog;
  Ui_qtchess_mainWindow ui;

  void closeEvent(QCloseEvent *event)
  {
    if(event)
      event->ignore();

    quit();
  }

 private slots:
  void about(void);
  void help(void);
  void newGame(void);
  void quit(void);
  void setup(void);
  void updateOpponent(void);
  void updatePlayer(void);
};

#endif
