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

#include "qtchess.h"
#include "qtchess_gui_board.h"
#include "ui_qtchess_help.h"
#include "ui_qtchess_mainwindow.h"
#include "ui_qtchess_promotion.h"
#include "ui_qtchess_setup.h"

extern QPointer<qtchess> chess;

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

class qtchess_setup: public QDialog
{
  Q_OBJECT

 public:
  qtchess_setup(QWidget *parent);
  QHostAddress get_listening_address(void) const;
  QLineEdit *get_allowed_host_field(void) const;
  QLineEdit *get_local_host_field(void) const;
  QLineEdit *get_remote_host_field(void) const;
  QLineEdit *get_remote_scope_id_field(void) const;
  QSpinBox *get_local_port_field(void) const;
  QSpinBox *get_remote_port_field(void) const;

  QString color(void) const
  {
    return m_ui.color->currentText();
  }

  QString gnuChessPath(void) const
  {
    return m_ui.gnu_chess->text().trimmed();
  }

  void reset(void);
  void stop(void);

 private:
  Ui_qtchess_setup m_ui;

 private slots:
  void slot_close(void);
  void slot_connect(void);
  void slot_connected_to_client(void);
  void slot_disconnect(void);
  void slot_disconnected_from_client(void);
  void slot_listen(void);
  void slot_local(bool state);
  void slot_protocol_changed(void);
  void slot_remote(bool state);
  void slot_reset(void);
  void slot_set_caissa(void);
};

class qtchess_gui: public QMainWindow
{
  Q_OBJECT

 public:
  qtchess_gui();
  ~qtchess_gui();

  QString color(void) const
  {
    if(m_setup)
      return m_setup->color();
    else
      return "";
  }

  QString gnuChessPath(void) const
  {
    return m_setup ? m_setup->gnuChessPath() : "";
  }

  bool is_ready(void) const;

  qtchess_gui_board *board(void) const
  {
    return m_board;
  }

  qtchess_promotion *get_promote_dialog(void) const;
  qtchess_setup *get_setup_dialog(void) const;
  static QString move_as_history_string(const struct move_s &move);
  void add_history_move(const QString &string, const int color);
  void add_history_move(const struct move_s &current_move, const int color);
  void clear_history(void);
  void initialize(void);
  void initialize_clocks(void);
  void notify_connection(const QString &address, const quint16 port);
  void reset_move(void);
  void set_status_text(const QString &str);
  void show_disconnect(void);
  void show_error_message(const char *message);
  void show_game_over(const int turn);
  void start_timers(const int which);
  void stop_timers(const int which);

  void update(void)
  {
    QMainWindow::update();
    chess ?
      m_ui.action_Undo_GNUChess_Move->setEnabled(!chess->is_new()) : (void) 0;
    m_board ? m_board->paint() : (void) 0;
  }

 private:
  QPointer<QLabel> m_status_label;
  QPointer<QTimer> m_opponent_timer;
  QPointer<QTimer> m_player_timer;
  QPointer<qtchess_gui_board> m_board;
  QPointer<qtchess_help> m_help;
  QPointer<qtchess_promotion> m_promotion;
  QPointer<qtchess_setup> m_setup;
  Ui_qtchess_mainwindow m_ui;
  bool new_game_prompt(void);

  void closeEvent(QCloseEvent *event)
  {
    if(event)
      event->ignore();

    slot_quit();
  }

  void initialize_board(void);

 private slots:
  void slot_about(void);
  void slot_help(void);
  void slot_new_game(void);
  void slot_new_gnuchess_game(void);
  void slot_quit(void);
  void slot_quit_gnuchess(void);
  void slot_setup(void);
  void slot_undo_gnuchess_move(void);
  void slot_update_opponent(void);
  void slot_update_player(void);
};

#endif
