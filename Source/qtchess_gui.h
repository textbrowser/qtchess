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

#include "qtchess_gui_board.h"
#include "ui_qtchess_help.h"
#include "ui_qtchess_mainwindow.h"
#include "ui_qtchess_promotion.h"
#include "ui_qtchess_setupDialog.h"

class qtchess_help: public QDialog
{
  Q_OBJECT

 public:
  qtchess_help(QWidget *parent);
  void setup(void);

 private:
  Ui_qtchess_help m_ui;

 private slots:
  void slot_ok(void);
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

class qtchess_promotion: public QDialog
{
  Q_OBJECT

 public:
  qtchess_promotion(QWidget *parent);

  QComboBox *get_menu(void) const
  {
    return m_ui.menu;
  }

  void setup(void);

 private:
  Ui_qtchess_promotion m_ui;

 private slots:
  void slot_ok(void);
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

  qtchess_gui_board *board(void) const
  {
    return m_board;
  }

  qtchess_promotion *get_promote_dialog(void) const;
  qtchess_setup_dialog *getSetupDialog(void) const;
  void add_history_move(const struct move_s &current_move, const int color);
  void clear_history(void);
  void initialize(void);
  void initialize_clocks(void);
  void notifyConnection(const QString &, const quint16);
  void setStatusText(const QString &);
  void showDisconnect(void);
  void showErrorMsg(const char *);
  void showGameOver(const int);
  void startTimers(const int);
  void stopTimers(const int);

 private:
  QPointer<QLabel> m_status_label;
  QPointer<QTimer> m_opponent_timer;
  QPointer<QTimer> m_player_timer;
  QPointer<qtchess_gui_board> m_board;
  QPointer<qtchess_help> m_help;
  QPointer<qtchess_promotion> m_promotion;
  QPointer<qtchess_setup_dialog> setup_dialog;
  Ui_qtchess_mainwindow m_ui;

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
  void slot_update_opponent(void);
  void slot_update_player(void);
};

#endif
