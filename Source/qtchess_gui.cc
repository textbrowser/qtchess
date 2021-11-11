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

#include "qtchess.h"
#include "qtchess_communications.h"
#include "qtchess_definitions.h"
#include "qtchess_gui.h"
#include "qtchess_validate.h"

#include <QActionGroup>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QScrollBar>
#include <QStatusBar>

extern QPointer<qtchess> chess;
extern QPointer<qtchess_communications> comm;

qtchess_gui::qtchess_gui(void):QMainWindow()
{
  m_ui.setupUi(this);
  m_ui.side->setVisible(false);
  m_ui.splitter->setStretchFactor(0, 1);
  m_ui.splitter->setStretchFactor(1, 0);

  if(menuBar())
    menuBar()->setNativeMenuBar(true);
}

qtchess_gui::~qtchess_gui()
{
  if(m_board)
    m_board->deleteLater();
}

qtchess_promotion *qtchess_gui::get_promote_dialog(void) const
{
  return m_promotion;
}

qtchess_setup *qtchess_gui::get_setup_dialog(void) const
{
  return m_setup;;
}

void qtchess_gui::add_history_move(const struct move_s &current_move,
				   const int color)
{
  QTableWidgetItem *item = nullptr;
  char from_position[2];
  char hist[512];
  char insert_x[3];
  char to_let[2];

  from_position[0] = static_cast<char> (97 + current_move.m_y1);
  memset(from_position, 0, sizeof(from_position));
  memset(hist, 0, sizeof(hist));
  memset(insert_x, 0, sizeof(insert_x));
  memset(to_let, 0, sizeof(to_let));
  to_let[0] = static_cast<char> (97 + current_move.m_y2);

  if(current_move.m_promoted)
    {
      if(chess && chess->was_piece_won())
	snprintf(insert_x, sizeof(insert_x), "%s%s", from_position, "x");

      if(qtchess_validate::is_bishop(current_move.m_piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%dB", insert_x,
		 to_let, current_move.m_x2 + 1);
      else if(qtchess_validate::is_knight(current_move.m_piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%dN", insert_x,
		 to_let, current_move.m_x2 + 1);
      else if(qtchess_validate::is_queen(current_move.m_piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%dQ", insert_x,
		 to_let, current_move.m_x2 + 1);
      if(qtchess_validate::is_rook(current_move.m_piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%dR", insert_x,
		 to_let, current_move.m_x2 + 1);
    }
  else
    {
      if(chess && chess->was_piece_won())
	snprintf(insert_x, sizeof(insert_x), "%s", "x");

      if(qtchess_validate::is_bishop(current_move.m_piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%s%s%d", "B",
		 current_move.m_departure, insert_x,
		 to_let, current_move.m_x2 + 1);
      else if(qtchess_validate::is_king(current_move.m_piece))
	{
	  if(current_move.m_rook_x1 != -1)
	    {
	      if(current_move.m_y2 == 6)
		snprintf(hist, sizeof(hist),
			 "%s%s%s%d 0-0", "K", insert_x,
			 to_let, current_move.m_x2 + 1);
	      else
		snprintf(hist, sizeof(hist),
			 "%s%s%s%d 0-0-0", "K", insert_x,
			 to_let, current_move.m_x2 + 1);
	    }
	  else
	    snprintf(hist, sizeof(hist),
		     "%s%s%s%d", "K", insert_x,
		     to_let, current_move.m_x2 + 1);
	}
      else if(qtchess_validate::is_knight(current_move.m_piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%s%s%d", "N",
		 current_move.m_departure, insert_x,
		 to_let, current_move.m_x2 + 1);
      else if(qtchess_validate::is_pawn(current_move.m_piece))
	{
	  if(chess && chess->was_piece_won())
	    snprintf(insert_x, sizeof(insert_x), "%s%s",
		     from_position, "x");

	  if(current_move.m_enpassant)
	    snprintf(hist, sizeof(hist),
		     "%s%s%d e.p.", insert_x,
		     to_let, current_move.m_x2 + 1);
	  else
	    snprintf(hist, sizeof(hist),
		     "%s%s%d", insert_x,
		     to_let, current_move.m_x2 + 1);
	}
      else if(qtchess_validate::is_queen(current_move.m_piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%s%s%d", "Q",
		 current_move.m_departure, insert_x,
		 to_let, current_move.m_x2 + 1);
      else if(qtchess_validate::is_rook(current_move.m_piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%s%s%d", "R",
		 current_move.m_departure, insert_x,
		 to_let, current_move.m_x2 + 1);
      else
	snprintf(hist, sizeof(hist), "%s", "-----");
    }

  /*
  ** Is the king in check?
  */

  if(current_move.m_is_opponent_king_threat)
    strncat(hist, "+", sizeof(hist) - strnlen(hist, sizeof(hist) - 1) - 1);

  if((item = new(std::nothrow) QTableWidgetItem(tr(hist))) != nullptr)
    {
      item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

      if(m_ui.history->rowCount() == 0)
	m_ui.history->setRowCount(1);

      if(m_ui.history->item(m_ui.history->rowCount() - 1, 0) != nullptr &&
	 m_ui.history->item(m_ui.history->rowCount() - 1, 1) != nullptr)
	m_ui.history->setRowCount(m_ui.history->rowCount() + 1);

      if(color == WHITE)
	m_ui.history->setItem(m_ui.history->rowCount() - 1, 0, item);
      else if(color == BLACK)
	m_ui.history->setItem(m_ui.history->rowCount() - 1, 1, item);
      else
	{
	  delete item;

	  if(m_ui.history->rowCount() > 0)
	    m_ui.history->setRowCount(m_ui.history->rowCount() - 1);
	}

      m_ui.history->scrollToBottom();
    }
}

void qtchess_gui::clear_history(void)
{
  m_ui.history->setRowCount(0);
  m_ui.history->scrollToTop();
  m_ui.history->horizontalScrollBar()->setValue(0);
}

void qtchess_gui::initialize(void)
{
  static bool s_initialized = false;

  if(s_initialized)
    return;

  setWindowTitle(tr("QtChess"));

  if(statusBar())
    {
      if((m_status_label = new(std::nothrow)
	  QLabel(tr("Status: Peer Disconnected"))) == nullptr)
	{
	  if(chess)
	    chess->quit("Memory allocation failure.", EXIT_FAILURE);
	  else
	    {
	      QApplication::exit(EXIT_FAILURE);
	      return;
	    }
	}

      m_status_label->setFrameShadow(QFrame::Raised);
      m_status_label->setFrameShape(QFrame::NoFrame);
      m_status_label->setMargin(5);
      statusBar()->addWidget(m_status_label, 100);
      statusBar()->setSizeGripEnabled(false);
      statusBar()->setStyleSheet("QStatusBar::item {"
				 "border: none; "
				 "}");
    }

  connect(m_ui.action_About,
	  SIGNAL(triggered(void)),
	  this,
	  SLOT(slot_about(void)));
  connect(m_ui.action_Connection_Configuration,
	  SIGNAL(triggered(void)),
	  this,
	  SLOT(slot_setup(void)));
  connect(m_ui.action_Help,
	  SIGNAL(triggered(void)),
	  this,
	  SLOT(slot_help(void)));
  connect(m_ui.action_New_Game,
	  SIGNAL(triggered(void)),
	  this,
	  SLOT(slot_new_game(void)));
  connect(m_ui.action_Quit,
	  SIGNAL(triggered(void)),
	  this,
	  SLOT(slot_quit(void)));

  if((m_board = new(std::nothrow) qtchess_gui_board(nullptr)) == nullptr)
    {
      if(chess)
	chess->quit("Memory allocation failure.", EXIT_FAILURE);
      else
	{
	  QApplication::exit(EXIT_FAILURE);
	  return;
	}
    }

  if((m_help = new(std::nothrow) qtchess_help(this)) == nullptr)
    {
      if(chess)
	chess->quit("Memory allocation failure.", EXIT_FAILURE);
      else
	{
	  QApplication::exit(EXIT_FAILURE);
	  return;
	}
    }

  if((m_opponent_timer = new(std::nothrow) QTimer(this)) != nullptr)
    {
      connect(m_opponent_timer,
	      SIGNAL(timeout(void)),
	      SLOT(slot_update_opponent(void)));
      m_opponent_timer->start(1000);
    }
  else
    {
      if(chess)
	chess->quit("Memory allocation failure.", EXIT_FAILURE);
      else
	{
	  QApplication::exit(EXIT_FAILURE);
	  return;
	}
    }

  if((m_player_timer = new(std::nothrow) QTimer(this)) != nullptr)
    {
      connect(m_player_timer,
	      SIGNAL(timeout(void)),
	      SLOT(slot_update_player(void)));
      m_player_timer->start(1000);
    }
  else
    {
      if(chess)
	chess->quit("Memory allocation failure.", EXIT_FAILURE);
      else
	{
	  QApplication::exit(EXIT_FAILURE);
	  return;
	}
    }

  if((m_promotion = new(std::nothrow) qtchess_promotion(this)) ==
     nullptr)
    {
      if(chess)
	chess->quit("Memory allocation failure.", EXIT_FAILURE);
      else
	{
	  QApplication::exit(EXIT_FAILURE);
	  return;
	}
    }

  if((m_setup = new(std::nothrow) qtchess_setup(this)) == nullptr)
    {
      if(chess)
	chess->quit("Memory allocation failure.", EXIT_FAILURE);
      else
	{
	  QApplication::exit(EXIT_FAILURE);
	  return;
	}
    }

  delete m_ui.board_frame->layout();
  m_ui.board_frame->setFocus();
  m_ui.board_frame->setLayout(new QGridLayout());

  if(m_board)
    m_board->add(m_ui.board_frame);

#ifndef Q_OS_ANDROID
  resize(800, 600);
#endif
#if QT_VERSION < 0x050000
  m_ui.history->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
  m_ui.history->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#else
  m_ui.history->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  m_ui.history->verticalHeader()->setSectionResizeMode
    (QHeaderView::ResizeToContents);
#endif
  s_initialized = true;
  show();
}

void qtchess_gui::initialize_clocks(void)
{
  m_ui.opponent_clock->setTime(QTime(0, 0, 0));
  m_ui.player_clock->setTime(QTime(0, 0, 0));
}

void qtchess_gui::notify_connection(const QString &address,
				    const quint16 port)
{
  if(chess)
    {
      if(chess->get_my_color() == WHITE)
	m_ui.side->setText(tr("You Play As Left"));
      else
	m_ui.side->setText(tr("You Play As Right"));

      m_ui.side->setVisible(true);
    }
  else
    m_ui.side->setVisible(false);

  set_status_text
    (tr("Status: Peer %1:%2 Connected").arg(address).arg(port));
}

void qtchess_gui::slot_new_game(void)
{
  if(comm && comm->is_connected_remotely())
    {
      QMessageBox mb(this);

      mb.setIcon(QMessageBox::Question);
      mb.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
      mb.setText
	(tr("Are you sure that you wish to initiate a new game? "
	    "Your board and your opponent's board will be reset."));
      mb.setWindowModality(Qt::ApplicationModal);
      mb.setWindowTitle(tr("QtChess: Confirmation"));

      if(mb.exec() != QMessageBox::Yes)
	return;
    }

  struct move_s current_move = {};

  if(chess)
    chess->initialize();

  if(m_board)
    m_board->new_game();

  for(int i = 0; i < NSQUARES; i++)
    for(int j = 0; j < NSQUARES; j++)
      if(chess)
	current_move.m_board[i][j] = chess->m_board[i][j];
      else
	current_move.m_board[i][j] = EMPTY_SQUARE;

  current_move.m_enpassant =
    current_move.m_pawn2 =
    current_move.m_promoted = 0;
  current_move.m_is_opponent_king_threat = 0;
  current_move.m_piece =
    current_move.m_rook =
    current_move.m_rook_x1 =
    current_move.m_rook_x2 =
    current_move.m_rook_y1 =
    current_move.m_rook_y2 =
    current_move.m_x1 =
    current_move.m_x2 =
    current_move.m_y1 =
    current_move.m_y2 = -1;
  memset(current_move.m_departure, 0, sizeof(current_move.m_departure));
  current_move.m_departure[0] = '0';

  if(comm)
    comm->send_move(current_move);

  clear_history();
  initialize_clocks();

  if(chess)
    {
      /*
      ** Careful here. Set this after calling send_move().
      */

      chess->set_turn(MY_TURN);

      if(chess->get_my_color() == WHITE)
	m_ui.side->setText(tr("You Play As Left"));
      else
	m_ui.side->setText(tr("You Play As Right"));

      m_ui.side->setVisible(true);
    }
}

void qtchess_gui::slot_quit(void)
{
  if(chess)
    {
      if(!chess->is_game_over() && comm && comm->is_connected_remotely())
	{
	  QMessageBox mb(this);

	  mb.setIcon(QMessageBox::Question);
	  mb.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
	  mb.setText
	    (tr("A game is in progress. Are you sure that you wish to quit?"));
	  mb.setWindowModality(Qt::ApplicationModal);
	  mb.setWindowTitle(tr("QtChess: Confirmation"));

	  if(mb.exec() != QMessageBox::Yes)
	    return;
	}

      chess->quit(nullptr, EXIT_SUCCESS);
    }
  else
    QApplication::exit(EXIT_FAILURE);
}

void qtchess_gui::set_status_text(const QString &str)
{
  m_status_label->setText(str);
}

void qtchess_gui::show_disconnect(void)
{
  m_ui.side->setVisible(false);
  stop_timers(OPPONENT_TIMER);
  stop_timers(PLAYER_TIMER);
}

void qtchess_gui::show_error_message(const char *message)
{
  if(message)
    QMessageBox::critical(this,
			  tr("QtChess: Error"),
			  tr(message),
			  QMessageBox::Ok,
			  QMessageBox::Default);
  else
    QMessageBox::critical(this,
			  tr("QtChess: Error"),
			  tr("An unknown error occurred."),
			  QMessageBox::Ok,
			  QMessageBox::Default);
}

void qtchess_gui::show_game_over(const int turn)
{
  Q_UNUSED(turn);
  QMessageBox::information
    (this,
     tr("QtChess: Game Over"),
     tr("Game Over. Please enjoy another game."),
     QMessageBox::Ok | QMessageBox::Default);
}

void qtchess_gui::slot_about(void)
{
  QMessageBox mb(this);
  QPixmap pixmap(":/chess.png");

  mb.setIconPixmap
    (pixmap.scaled(256, 256, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
  mb.setStandardButtons(QMessageBox::Ok);
  mb.setText
    (tr("<html>QtChess Version %1.<br>"
	"Copyright (c) 2003 - 2022, X.<br>"
	"Qt version %2."
	"<hr>"
	"Please visit <a href=\"https://textbrowser.github.io/qtchess\">"
	"https://textbrowser.github.io/qtchess</a> for "
	"project information.").
     arg(QTCHESS_VERSION).
     arg(QT_VERSION_STR));
  mb.setTextFormat(Qt::RichText);
  mb.setWindowTitle(tr("QtChess: About"));
  mb.exec();
}

void qtchess_gui::slot_help(void)
{
  if(m_help)
    m_help->setup();
}

void qtchess_gui::slot_setup(void)
{
  if(m_setup)
    {
      if(m_setup->get_local_host_field())
	m_setup->get_local_host_field()->setFocus();

#ifdef Q_OS_ANDROID
      m_setup->showMaximized();
#else
      m_setup->resize
	(qMax(m_setup->sizeHint().width(), size().width()) - 100,
	 m_setup->sizeHint().height());
#endif
      m_setup->exec();
    }
}

void qtchess_gui::slot_update_opponent(void)
{
  static QString stylesheet(m_ui.opponent_clock->styleSheet());

  if(chess &&
     chess->get_turn() == THEIR_TURN &&
     !chess->is_game_over() &&
     comm &&
     comm->is_ready())
    {
      m_ui.opponent_clock->setStyleSheet
	("QWidget {background: rgb(240, 128, 128);}");
      m_ui.opponent_clock->setTime(m_ui.opponent_clock->time().addSecs(1));
    }
  else
    m_ui.opponent_clock->setStyleSheet(stylesheet);
}

void qtchess_gui::slot_update_player(void)
{
  static QString stylesheet(m_ui.player_clock->styleSheet());

  if(chess &&
     chess->get_turn() == MY_TURN &&
     !chess->is_game_over() &&
     comm &&
     comm->is_ready())
    {
      m_ui.player_clock->setStyleSheet
	("QWidget {background: rgb(144, 238, 144);}");
      m_ui.player_clock->setTime(m_ui.player_clock->time().addSecs(1));
    }
  else
    m_ui.player_clock->setStyleSheet(stylesheet);
}

void qtchess_gui::start_timers(const int which)
{
  Q_UNUSED(which);
}

void qtchess_gui::stop_timers(const int which)
{
  Q_UNUSED(which);
}

qtchess_help::qtchess_help(QWidget *parent):QDialog(parent)
{
  m_ui.setupUi(this);
  connect(m_ui.ok,
	  SIGNAL(clicked(void)),
	  this,
	  SLOT(slot_ok(void)));
  m_ui.text->append
    (tr("<html>"
	"<ul>"
	"<li>An empty Allowed IP Address value will allow any peer to "
	"connect.</li>"
	"<li>Moves are prohibited until connections have been "
	"established.</li>"
	"<li>To move a piece, first click it and then click "
	"the desired destination.</li>"
	"<li>To prevent peer connections, please set the "
	"Allowed IP Address to 0.0.0.0 (:: for IPv6).</li>"
	"<li>To view a selected piece's valid moves, "
	"double-click the selected piece while the Control key is "
	"depressed.</li>"
	"</ul>"
	"</html>"));
#ifdef Q_OS_ANDROID
  auto font(m_ui.text->font());

  font.setPointSize(20);
  m_ui.text->setFont(font);
#else
  resize(600, 200);
#endif
  setWindowModality(Qt::NonModal);
}

void qtchess_help::slot_ok(void)
{
  hide();
}

void qtchess_help::setup(void)
{
  if(isVisible())
    hide();

  m_ui.ok->setFocus();
#ifdef Q_OS_ANDROID
  showMaximized();
#else
  show();
#endif
}

qtchess_promotion::qtchess_promotion(QWidget *parent):QDialog(parent)
{
  m_ui.setupUi(this);
  connect(m_ui.ok,
	  SIGNAL(clicked(void)),
	  this,
	  SLOT(slot_ok(void)));
}

void qtchess_promotion::slot_ok(void)
{
  hide();
}

void qtchess_promotion::setup(void)
{
  m_ui.menu->setCurrentIndex(0);
  m_ui.menu->setFocus();
  exec();
}

qtchess_setup::qtchess_setup(QWidget *parent):QDialog(parent)
{
  m_ui.setupUi(this);
  m_ui.local_host->setText
    (qtchess_communications::
     preferred_host_address(QAbstractSocket::IPv4Protocol).toString());
  m_ui.local_scope_id->setEnabled(false);
  m_ui.remote_host->setText(QHostAddress(QHostAddress::LocalHost).toString());
  m_ui.remote_scope_id->setEnabled(false);

  if(comm)
    {
      connect(comm,
	      SIGNAL(connected_to_client(void)),
	      this,
	      SLOT(slot_connected_to_client(void)));
      connect(comm,
	      SIGNAL(disconnected_from_client(void)),
	      this,
	      SLOT(slot_disconnected_from_client(void)));
    }

  connect(m_ui.cancel,
	  SIGNAL(clicked(void)),
	  this,
	  SLOT(slot_close(void)));
  connect(m_ui.connect,
	  SIGNAL(clicked(void)),
	  this,
	  SLOT(slot_connect(void)));
  connect(m_ui.disconnect,
	  SIGNAL(clicked(void)),
	  this,
	  SLOT(slot_disconnect(void)));
  connect(m_ui.listen,
	  SIGNAL(clicked(void)),
	  this,
	  SLOT(slot_listen(void)));
  connect(m_ui.local,
	  SIGNAL(toggled(bool)),
	  this,
	  SLOT(slot_local(bool)));
  connect(m_ui.local_ipv4,
	  SIGNAL(clicked(void)),
	  this,
	  SLOT(slot_protocol_changed(void)));
  connect(m_ui.local_ipv6,
	  SIGNAL(clicked(void)),
	  this,
	  SLOT(slot_protocol_changed(void)));
  connect(m_ui.remote,
	  SIGNAL(toggled(bool)),
	  this,
	  SLOT(slot_remote(bool)));
  connect(m_ui.remote_ipv4,
	  SIGNAL(clicked(void)),
	  this,
	  SLOT(slot_protocol_changed(void)));
  connect(m_ui.remote_ipv6,
	  SIGNAL(clicked(void)),
	  this,
	  SLOT(slot_protocol_changed(void)));
  connect(m_ui.set_caissa,
	  SIGNAL(clicked(void)),
	  this,
	  SLOT(slot_set_caissa(void)));
  m_ui.remote_gb->setEnabled(false);
}

QHostAddress qtchess_setup::get_listening_address(void) const
{
  QHostAddress address;

  if(m_ui.local_ipv4->isChecked())
    address.setAddress(m_ui.local_host->text().trimmed());
  else
    {
      address.setAddress(m_ui.local_host->text().trimmed());
      address.setScopeId(m_ui.local_scope_id->text().trimmed());
    }

  return address;
}

QLineEdit *qtchess_setup::get_allowed_host_field(void) const
{
  return m_ui.allowed_host;
}

QLineEdit *qtchess_setup::get_local_host_field(void) const
{
  return m_ui.local_host;
}

QLineEdit *qtchess_setup::get_remote_host_field(void) const
{
  return m_ui.remote_host;
}

QLineEdit *qtchess_setup::get_remote_scope_id_field(void) const
{
  return m_ui.remote_scope_id;
}

QSpinBox *qtchess_setup::get_local_port_field(void) const
{
  return m_ui.local_port;
}

QSpinBox *qtchess_setup::get_remote_port_field(void) const
{
  return m_ui.remote_port;
}

void qtchess_setup::slot_close(void)
{
  hide();
}

void qtchess_setup::slot_connect(void)
{
  if(comm)
    {
      if(comm->is_connected_remotely())
	comm->disconnect_remotely();
      else
	comm->connect_remotely();
    }
}

void qtchess_setup::slot_connected_to_client(void)
{
  if(m_ui.local->isChecked())
    return;

  m_ui.connect->setText(tr("&Disconnect"));
  m_ui.remote_host->setReadOnly(true);
  m_ui.remote_ipv4->setEnabled(false);
  m_ui.remote_ipv6->setEnabled(false);
  m_ui.remote_port->setReadOnly(true);
  m_ui.remote_scope_id->setReadOnly(true);
}

void qtchess_setup::slot_disconnect(void)
{
  if(comm)
    comm->disconnect_remotely();
}

void qtchess_setup::slot_disconnected_from_client(void)
{
  if(m_ui.local->isChecked())
    return;

  m_ui.connect->setText(tr("&Connect"));
  m_ui.remote_host->setReadOnly(false);
  m_ui.remote_ipv4->setEnabled(true);
  m_ui.remote_ipv6->setEnabled(true);
  m_ui.remote_port->setReadOnly(false);
  m_ui.remote_scope_id->setReadOnly(false);
}

void qtchess_setup::slot_listen(void)
{
  auto state = false;

  if(sender() == m_ui.listen)
    if(comm)
      {
	if(comm->is_listening())
	  comm->stop_listening();
	else
	  comm->set_listen();

	state = comm->is_listening();
      }

  m_ui.local_host->setReadOnly(state);
  m_ui.local_ipv4->setEnabled(!state);
  m_ui.local_ipv6->setEnabled(!state);
  m_ui.local_port->setReadOnly(state);

  if(m_ui.local_ipv6->isChecked())
    m_ui.local_scope_id->setReadOnly(state);

  if(state)
    m_ui.listen->setText(tr("&Stop Listening"));
  else
    m_ui.listen->setText(tr("&Listen"));
}

void qtchess_setup::slot_local(bool state)
{
  if(state)
    {
      if(comm)
	comm->disconnect_remotely();

      slot_disconnected_from_client();
      m_ui.local_gb->setEnabled(true);
      m_ui.remote->blockSignals(true);
      m_ui.remote->setChecked(false);
      m_ui.remote->blockSignals(false);
      m_ui.remote_gb->setEnabled(false);
      m_ui.scroll_area->verticalScrollBar()->setValue(0);
    }
  else
    {
      m_ui.local->blockSignals(true);
      m_ui.local->setChecked(true);
      m_ui.local->blockSignals(false);
    }
}

void qtchess_setup::slot_protocol_changed(void)
{
  if(sender() == m_ui.local_ipv4)
    {
      auto preferred_host_address
	(qtchess_communications::
	 preferred_host_address(QAbstractSocket::IPv4Protocol));

      m_ui.allowed_host->clear();
      m_ui.local_host->setText(preferred_host_address.toString());
      m_ui.local_scope_id->clear();
      m_ui.local_scope_id->setEnabled(false);
    }
  else if(sender() == m_ui.local_ipv6)
    {
      auto preferred_host_address
	(qtchess_communications::
	 preferred_host_address(QAbstractSocket::IPv6Protocol));

      m_ui.allowed_host->clear();
      m_ui.local_host->setText(preferred_host_address.toString());
      m_ui.local_scope_id->setEnabled(true);
      m_ui.local_scope_id->setText(preferred_host_address.scopeId());
    }
  else if(sender() == m_ui.remote_ipv4)
    {
      m_ui.remote_host->setText
	(QHostAddress(QHostAddress::LocalHost).toString());
      m_ui.remote_scope_id->clear();
      m_ui.remote_scope_id->setEnabled(false);
    }
  else if(sender() == m_ui.remote_ipv6)
    {
      m_ui.remote_host->setText
	(QHostAddress(QHostAddress::LocalHostIPv6).toString());
      m_ui.remote_scope_id->setText
	(QHostAddress(QHostAddress::LocalHostIPv6).scopeId());
      m_ui.remote_scope_id->setEnabled(true);
    }
}

void qtchess_setup::slot_remote(bool state)
{
  if(state)
    {
      if(comm)
	{
	  comm->disconnect_remotely();
	  comm->stop_listening();
	}

      slot_disconnected_from_client();
      slot_listen();
      m_ui.local->blockSignals(true);
      m_ui.local->setChecked(false);
      m_ui.local->blockSignals(false);
      m_ui.local_gb->setEnabled(false);
      m_ui.remote_gb->setEnabled(true);
      m_ui.scroll_area->verticalScrollBar()->setValue
	(m_ui.scroll_area->verticalScrollBar()->maximum());
    }
  else
    {
      m_ui.remote->blockSignals(true);
      m_ui.remote->setChecked(true);
      m_ui.remote->blockSignals(false);
    }
}

void qtchess_setup::slot_set_caissa(void)
{
  if(comm)
    {
      comm->set_caissa(m_ui.caissa->text());
      m_ui.caissa->selectAll();
    }
}
