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
  ui.setupUi(this);

  if(menuBar())
    menuBar()->setNativeMenuBar(true);
}

qtchess_gui::~qtchess_gui()
{
  if(m_board)
    m_board->deleteLater();
}

qtchess_promote_dialog *qtchess_gui::getPromoteDialog(void) const
{
  return promote_dialog;
}

qtchess_setup_dialog *qtchess_gui::getSetupDialog(void) const
{
  return setup_dialog;
}

void qtchess_gui::about(void)
{
  QMessageBox mb(this);
  QPixmap pixmap(":/chess.png");

  mb.setWindowTitle(tr("QtChess: About"));
  mb.setTextFormat(Qt::RichText);
  mb.setText
    (tr("<html>QtChess Version %1.<br>"
	"Copyright (c) 2003 - 2021 X.<br>"
	"Qt version %2."
	"<hr>"
	"Please visit <a href=\"https://textbrowser.github.io/qtchess\">"
	"https://textbrowser.github.io/qtchess</a> for "
	"project information.").
     arg(QTCHESS_VERSION).
     arg(QT_VERSION_STR));
  mb.setStandardButtons(QMessageBox::Ok);
  mb.setIconPixmap
    (pixmap.scaled(256, 256, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
  mb.exec();
}

void qtchess_gui::addHistoryMove(const struct move_s &current_move,
				 const int color)
{
  QTableWidgetItem *item = nullptr;
  char from_position[2];
  char hist[512];
  char insertX[3];
  char to_let[2];

  memset(from_position, 0, sizeof(from_position));
  memset(hist, 0, sizeof(hist));
  memset(insertX, 0, sizeof(insertX));
  memset(to_let, 0, sizeof(to_let));
  from_position[0] = (char) (97 + current_move.m_y1);
  to_let[0] = (char) (97 + current_move.m_y2);

  if(current_move.m_promoted)
    {
      if(chess && chess->was_piece_won())
	snprintf(insertX, sizeof(insertX), "%s%s", from_position, "x");

      if(qtchess_validate::is_bishop(current_move.m_piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%dB", insertX,
		 to_let, current_move.m_x2 + 1);
      else if(qtchess_validate::is_knight(current_move.m_piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%dN", insertX,
		 to_let, current_move.m_x2 + 1);
      else if(qtchess_validate::is_queen(current_move.m_piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%dQ", insertX,
		 to_let, current_move.m_x2 + 1);
      if(qtchess_validate::is_rook(current_move.m_piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%dR", insertX,
		 to_let, current_move.m_x2 + 1);
    }
  else
    {
      if(chess && chess->was_piece_won())
	snprintf(insertX, sizeof(insertX), "%s", "x");

      if(qtchess_validate::is_bishop(current_move.m_piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%s%s%d", "B",
		 current_move.m_departure, insertX,
		 to_let, current_move.m_x2 + 1);
      else if(qtchess_validate::is_king(current_move.m_piece))
	{
	  if(current_move.m_rook_x1 != -1)
	    {
	      if(current_move.m_y2 == 6)
		snprintf(hist, sizeof(hist),
			 "%s%s%s%d 0-0", "K", insertX,
			 to_let, current_move.m_x2 + 1);
	      else
		snprintf(hist, sizeof(hist),
			 "%s%s%s%d 0-0-0", "K", insertX,
			 to_let, current_move.m_x2 + 1);
	    }
	  else
	    snprintf(hist, sizeof(hist),
		     "%s%s%s%d", "K", insertX,
		     to_let, current_move.m_x2 + 1);
	}
      else if(qtchess_validate::is_knight(current_move.m_piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%s%s%d", "N",
		 current_move.m_departure, insertX,
		 to_let, current_move.m_x2 + 1);
      else if(qtchess_validate::is_pawn(current_move.m_piece))
	{
	  if(chess && chess->was_piece_won())
	    snprintf(insertX, sizeof(insertX), "%s%s",
		     from_position, "x");

	  if(current_move.m_enpassant)
	    snprintf(hist, sizeof(hist),
		     "%s%s%d e.p.", insertX,
		     to_let, current_move.m_x2 + 1);
	  else
	    snprintf(hist, sizeof(hist),
		     "%s%s%d", insertX,
		     to_let, current_move.m_x2 + 1);
	}
      else if(qtchess_validate::is_queen(current_move.m_piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%s%s%d", "Q",
		 current_move.m_departure, insertX,
		 to_let, current_move.m_x2 + 1);
      else if(qtchess_validate::is_rook(current_move.m_piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%s%s%d", "R",
		 current_move.m_departure, insertX,
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

      if(ui.history->rowCount() == 0)
	ui.history->setRowCount(1);

      if(ui.history->item(ui.history->rowCount() - 1, 0) != nullptr &&
	 ui.history->item(ui.history->rowCount() - 1, 1) != nullptr)
	ui.history->setRowCount(ui.history->rowCount() + 1);

      if(color == WHITE)
	ui.history->setItem(ui.history->rowCount() - 1, 0, item);
      else if(color == BLACK)
	ui.history->setItem(ui.history->rowCount() - 1, 1, item);
      else
	{
	  delete item;

	  if(ui.history->rowCount() > 0)
	    ui.history->setRowCount(ui.history->rowCount() - 1);
	}

      ui.history->scrollToBottom();
    }
}

void qtchess_gui::clearHistory(void)
{
  ui.history->setRowCount(0);
  ui.history->scrollToTop();
  ui.history->horizontalScrollBar()->setValue(0);
}

void qtchess_gui::help(void)
{
  if(help_dialog)
    help_dialog->setup();
}

void qtchess_gui::initialize(void)
{
  static bool s_initialized = false;

  if(s_initialized)
    return;

  setWindowTitle(tr("QtChess"));

  if(statusBar())
    {
      if((statusLabel = new(std::nothrow)
	  QLabel(tr("Status: Peer Disconnected"))) == nullptr)
	{
	  if(chess)
	    chess->quit("Memory allocation failure.", EXIT_FAILURE);
	  else
	    exit(EXIT_FAILURE);
	}

      statusLabel->setMargin(5);
      statusLabel->setFrameShadow(QFrame::Raised);
      statusLabel->setFrameShape(QFrame::NoFrame);
      statusBar()->setSizeGripEnabled(false);
      statusBar()->setStyleSheet("QStatusBar::item {"
				 "border: none; "
				 "}");
      statusBar()->addWidget(statusLabel, 100);
    }

  connect(ui.action_New_Game,
	  SIGNAL(triggered(void)),
	  this,
	  SLOT(newGame(void)));
  connect(ui.action_Quit,
	  SIGNAL(triggered(void)),
	  this,
	  SLOT(quit(void)));
  connect(ui.action_Connection_Configuration,
	  SIGNAL(triggered(void)),
	  this,
	  SLOT(setup(void)));
  connect(ui.action_About,
	  SIGNAL(triggered(void)),
	  this,
	  SLOT(about(void)));
  connect(ui.action_Help,
	  SIGNAL(triggered(void)),
	  this,
	  SLOT(help(void)));

  if((m_board = new(std::nothrow) qtchess_gui_board(nullptr)) == nullptr)
    {
      if(chess)
	chess->quit("Memory allocation failure.", EXIT_FAILURE);
      else
	exit(EXIT_FAILURE);
    }

  if((playert = new(std::nothrow) QTimer(this)) != nullptr)
    {
      connect(playert,
	      SIGNAL(timeout(void)),
	      SLOT(updatePlayer(void)));
      playert->start(1000);
    }
  else
    {
      if(chess)
	chess->quit("Memory allocation failure.", EXIT_FAILURE);
      else
	exit(EXIT_FAILURE);
    }

  if((opponentt = new(std::nothrow) QTimer(this)) != nullptr)
    {
      connect(opponentt,
	      SIGNAL(timeout(void)),
	      SLOT(updateOpponent(void)));
      opponentt->start(1000);
    }
  else
    {
      if(chess)
	chess->quit("Memory allocation failure.", EXIT_FAILURE);
      else
	exit(EXIT_FAILURE);
    }

  if((help_dialog = new(std::nothrow) qtchess_help_dialog(this)) == nullptr)
    {
      if(chess)
	chess->quit("Memory allocation failure.", EXIT_FAILURE);
      else
	exit(EXIT_FAILURE);
    }

  if((setup_dialog = new(std::nothrow) qtchess_setup_dialog(this)) == nullptr)
    {
      if(chess)
	chess->quit("Memory allocation failure.", EXIT_FAILURE);
      else
	exit(EXIT_FAILURE);
    }

  if((promote_dialog = new(std::nothrow) qtchess_promote_dialog(this)) ==
     nullptr)
    {
      if(chess)
	chess->quit("Memory allocation failure.", EXIT_FAILURE);
      else
	exit(EXIT_FAILURE);
    }

  delete ui.boardFrame->layout();
  ui.boardFrame->setLayout(new QGridLayout());

  if(m_board)
    m_board->add(ui.boardFrame);

#ifndef Q_OS_ANDROID
  resize(800, 600);
#endif
#if QT_VERSION < 0x050000
  ui.history->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
  ui.history->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#else
  ui.history->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  ui.history->verticalHeader()->setSectionResizeMode
    (QHeaderView::ResizeToContents);
#endif
  s_initialized = true;
  show();
}

void qtchess_gui::initClocks(void)
{
  ui.playerClock->setTime(QTime(0, 0, 0));
  ui.opponentClock->setTime(QTime(0, 0, 0));
}

void qtchess_gui::newGame(void)
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

  clearHistory();
  initClocks();

  /*
  ** Careful here. Set this after calling sendMove().
  */

  if(chess)
    chess->set_turn(MY_TURN);
}

void qtchess_gui::notifyConnection(const QString &address,
				   const quint16 port)
{
  setStatusText
    (tr("Status: Peer %1:%2 Connected").arg(address).arg(port));
}

void qtchess_gui::quit(void)
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
    exit(EXIT_FAILURE);
}

void qtchess_gui::setStatusText(const QString &str)
{
  statusLabel->setText(tr(str.toLatin1()));
}

void qtchess_gui::setup(void)
{
  if(setup_dialog)
    {
      if(setup_dialog->getHostField())
	setup_dialog->getHostField()->setFocus();

#ifdef Q_OS_ANDROID
      setup_dialog->showMaximized();
#else
      setup_dialog->resize(setup_dialog->sizeHint());
#endif
      setup_dialog->exec();
    }
}

void qtchess_gui::showDisconnect(void)
{
  stopTimers(PLAYER_TIMER);
  stopTimers(OPPONENT_TIMER);
}

void qtchess_gui::showErrorMsg(const char *message)
{
  if(message)
    QMessageBox::critical(this, tr("QtChess: Error"), tr(message),
			  QMessageBox::Ok,
			  QMessageBox::Default);
  else
    QMessageBox::critical(this, tr("QtChess: Error"),
			  tr("An unknown error occurred."),
			  QMessageBox::Ok,
			  QMessageBox::Default);
}

void qtchess_gui::showGameOver(const int turn)
{
  Q_UNUSED(turn);
  QMessageBox::information
    (this,
     tr("QtChess: Game Over"),
     tr("Game Over. Please enjoy another game."),
     QMessageBox::Ok | QMessageBox::Default);
}

void qtchess_gui::startTimers(const int which)
{
  Q_UNUSED(which);
}

void qtchess_gui::stopTimers(const int which)
{
  Q_UNUSED(which);
}

void qtchess_gui::updateOpponent(void)
{
  static QString stylesheet(ui.opponentClock->styleSheet());

  if(chess &&
     chess->get_turn() == THEIR_TURN &&
     !chess->is_game_over() &&
     comm &&
     comm->is_ready())
    {
      ui.opponentClock->setStyleSheet
	("QWidget {background: rgb(240, 128, 128);}");
      ui.opponentClock->setTime(ui.opponentClock->time().addSecs(1));
    }
  else
    ui.opponentClock->setStyleSheet(stylesheet);
}

void qtchess_gui::updatePlayer(void)
{
  static QString stylesheet(ui.playerClock->styleSheet());

  if(chess &&
     chess->get_turn() == MY_TURN &&
     !chess->is_game_over() &&
     comm &&
     comm->is_ready())
    {
      ui.playerClock->setStyleSheet
	("QWidget {background: rgb(144, 238, 144);}");
      ui.playerClock->setTime(ui.playerClock->time().addSecs(1));
    }
  else
    ui.playerClock->setStyleSheet(stylesheet);
}

qtchess_help_dialog::qtchess_help_dialog(QWidget *parent):QDialog(parent)
{
  ui.setupUi(this);
  connect(ui.ok,
	  SIGNAL(clicked(void)),
	  this,
	  SLOT(ok_cb(void)));
  setMinimumWidth(600);
  setMinimumHeight(300);
  setWindowModality(Qt::NonModal);
  ui.text->append
    (tr("An empty Allowed IP Address value will allow any peer to connect.\n"
	"Moves are prohibited until connections have been established.\n"
	"To move a piece, first click it and then click "
	"the desired destination.\n"
	"To prevent peer connections, please set the "
	"Allowed IP Address to 0.0.0.0 (:: for IPv6).\n"
	"To view a selected piece's valid moves, "
	"double-click the selected piece while the Control key is depressed."
	));
}

void qtchess_help_dialog::ok_cb(void)
{
  hide();
}

void qtchess_help_dialog::setup(void)
{
  if(isVisible())
    hide();

  ui.ok->setFocus();
  show();
}

qtchess_promote_dialog::qtchess_promote_dialog(QWidget *parent):QDialog(parent)
{
  ui.setupUi(this);
  connect(ui.ok,
	  SIGNAL(clicked(void)),
	  this,
	  SLOT(ok_cb(void)));
}

void qtchess_promote_dialog::ok_cb(void)
{
  hide();
}

void qtchess_promote_dialog::setup(void)
{
  ui.menu->setCurrentIndex(0);
  ui.menu->setFocus();
  exec();
}

qtchess_setup_dialog::qtchess_setup_dialog(QWidget *parent):QDialog(parent)
{
  ui.setupUi(this);
  ui.host->setText(QHostAddress(QHostAddress::LocalHost).toString());
  ui.lScopeId->setEnabled(false);
  ui.rScopeId->setEnabled(false);
  ui.rhost->setText(QHostAddress(QHostAddress::LocalHost).toString());

  if(comm)
    {
      connect(comm,
	      SIGNAL(connected_to_client(void)),
	      this,
	      SLOT(slotConnectedToClient(void)));
      connect(comm,
	      SIGNAL(disconnected_from_client(void)),
	      this,
	      SLOT(slotDisconnectedFromClient(void)));
    }

  connect(ui.cancel,
	  SIGNAL(clicked(void)),
	  this,
	  SLOT(close_cb(void)));
  connect(ui.connect,
	  SIGNAL(clicked(void)),
	  this,
	  SLOT(connect_cb(void)));
  connect(ui.disconnect,
	  SIGNAL(clicked(void)),
	  this,
	  SLOT(slotDisconnect(void)));
  connect(ui.listen,
	  SIGNAL(clicked(void)),
	  this,
	  SLOT(slotListen(void)));
  connect(ui.lipv4,
	  SIGNAL(clicked(void)),
	  this,
	  SLOT(slotProtocolChanged(void)));
  connect(ui.lipv6,
	  SIGNAL(clicked(void)),
	  this,
	  SLOT(slotProtocolChanged(void)));
  connect(ui.local,
	  SIGNAL(toggled(bool)),
	  this,
	  SLOT(slotLocal(bool)));
  connect(ui.remote,
	  SIGNAL(toggled(bool)),
	  this,
	  SLOT(slotRemote(bool)));
  connect(ui.ripv4,
	  SIGNAL(clicked(void)),
	  this,
	  SLOT(slotProtocolChanged(void)));
  connect(ui.ripv6,
	  SIGNAL(clicked(void)),
	  this,
	  SLOT(slotProtocolChanged(void)));
  connect(ui.set_caissa,
	  SIGNAL(clicked(void)),
	  this,
	  SLOT(slotSetCaissa(void)));
  ui.remote_gb->setEnabled(false);
}

QHostAddress qtchess_setup_dialog::getListeningAddress(void) const
{
  QHostAddress address;

  if(ui.lipv4->isChecked())
    address.setAddress(ui.host->text().trimmed());
  else
    {
      address.setAddress(ui.host->text().trimmed());
      address.setScopeId(ui.lScopeId->text().trimmed());
    }

  return address;
}

QLineEdit *qtchess_setup_dialog::getAllowedHostField(void) const
{
  return ui.allowedHost;
}

QLineEdit *qtchess_setup_dialog::getHostField(void) const
{
  return ui.host;
}

QLineEdit *qtchess_setup_dialog::getRHostField(void) const
{
  return ui.rhost;
}

QLineEdit *qtchess_setup_dialog::getRScopeIdField(void) const
{
  return ui.rScopeId;
}

QSpinBox *qtchess_setup_dialog::getPortField(void) const
{
  return ui.port;
}

QSpinBox *qtchess_setup_dialog::getRPortField(void) const
{
  return ui.rport;
}

void qtchess_setup_dialog::close_cb(void)
{
  hide();
}

void qtchess_setup_dialog::connect_cb(void)
{
  if(comm)
    {
      if(comm->is_connected_remotely())
	comm->disconnect_remotely();
      else
	comm->connect_remotely();
    }
}

void qtchess_setup_dialog::slotConnectedToClient(void)
{
  if(ui.local->isChecked())
    return;

  ui.connect->setText(tr("&Disconnect"));
  ui.rScopeId->setReadOnly(true);
  ui.ripv4->setEnabled(false);
  ui.ripv6->setEnabled(false);
  ui.rhost->setReadOnly(true);
  ui.rport->setReadOnly(true);
}

void qtchess_setup_dialog::slotDisconnect(void)
{
  if(comm)
    comm->disconnect_remotely();
}

void qtchess_setup_dialog::slotDisconnectedFromClient(void)
{
  if(ui.local->isChecked())
    return;

  ui.connect->setText(tr("&Connect"));
  ui.rScopeId->setReadOnly(false);
  ui.rhost->setReadOnly(false);
  ui.ripv4->setEnabled(true);
  ui.ripv6->setEnabled(true);
  ui.rport->setReadOnly(false);
}

void qtchess_setup_dialog::slotListen(void)
{
  bool state = false;

  if(sender() == ui.listen)
    if(comm)
      {
	if(comm->is_listening())
	  comm->stop_listening();
	else
	  comm->set_listen();

	state = comm->is_listening();
      }

  ui.host->setReadOnly(state);
  ui.lipv4->setEnabled(!state);
  ui.lipv6->setEnabled(!state);
  ui.port->setReadOnly(state);

  if(ui.lipv6->isChecked())
    ui.lScopeId->setReadOnly(state);

  if(state)
    ui.listen->setText(tr("&Stop Listening"));
  else
    ui.listen->setText(tr("&Listen"));
}

void qtchess_setup_dialog::slotLocal(bool state)
{
  if(state)
    {
      if(comm)
	comm->disconnect_remotely();

      slotDisconnectedFromClient();
      ui.local_gb->setEnabled(true);
      ui.remote->blockSignals(true);
      ui.remote->setChecked(false);
      ui.remote->blockSignals(false);
      ui.remote_gb->setEnabled(false);
      ui.scrollArea->verticalScrollBar()->setValue(0);
    }
  else
    {
      ui.local->blockSignals(true);
      ui.local->setChecked(true);
      ui.local->blockSignals(false);
    }
}

void qtchess_setup_dialog::slotProtocolChanged(void)
{
  if(sender() == ui.lipv4)
    {
      ui.host->setText(QHostAddress(QHostAddress::LocalHost).toString());
      ui.allowedHost->setText(ui.host->text());
      ui.lScopeId->clear();
      ui.lScopeId->setEnabled(false);
    }
  else if(sender() == ui.lipv6)
    {
      ui.host->setText(QHostAddress(QHostAddress::LocalHostIPv6).toString());
      ui.allowedHost->setText(ui.host->text());
      ui.lScopeId->setText(QHostAddress(QHostAddress::LocalHostIPv6).
			   scopeId());
      ui.lScopeId->setEnabled(true);
    }
  else if(sender() == ui.ripv4)
    {
      ui.rhost->setText(QHostAddress(QHostAddress::LocalHost).toString());
      ui.rScopeId->clear();
      ui.rScopeId->setEnabled(false);
    }
  else if(sender() == ui.ripv6)
    {
      ui.rhost->setText(QHostAddress(QHostAddress::LocalHostIPv6).toString());
      ui.rScopeId->setText(QHostAddress(QHostAddress::LocalHostIPv6).
			   scopeId());
      ui.rScopeId->setEnabled(true);
    }
}

void qtchess_setup_dialog::slotRemote(bool state)
{
  if(state)
    {
      if(comm)
	{
	  comm->disconnect_remotely();
	  comm->stop_listening();
	}

      slotDisconnectedFromClient();
      slotListen();
      ui.local->blockSignals(true);
      ui.local->setChecked(false);
      ui.local->blockSignals(false);
      ui.local_gb->setEnabled(false);
      ui.remote_gb->setEnabled(true);
      ui.scrollArea->verticalScrollBar()->setValue
	(ui.scrollArea->verticalScrollBar()->maximum());
    }
  else
    {
      ui.remote->blockSignals(true);
      ui.remote->setChecked(true);
      ui.remote->blockSignals(false);
    }
}

void qtchess_setup_dialog::slotSetCaissa(void)
{
  if(comm)
    {
      comm->set_caissa(ui.caissa->text());
      ui.caissa->selectAll();
    }
}
