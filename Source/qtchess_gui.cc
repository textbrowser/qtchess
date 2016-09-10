/*
** -- Local Includes --
*/

#include "qtchess_gui.h"

/*
** -- Qt Includes --
*/

#include <QCloseEvent>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>

extern qtchess *chess;
extern qtchess_comm *comm;
extern qtchess_gui *gui;

int qtchess_gui::exec(void)
{
  if(QApplication::instance())
    return QApplication::instance()->exec();
  else
    return EXIT_FAILURE;
}

void qtchess_gui::setStatusText(const QString &str)
{
  statusLabel->setText(tr(str.toLatin1()));
}

void qtchess_gui::init(void)
{
  static bool s_initialized = false;

  if(s_initialized)
    return;

  QActionGroup *ag1 = 0;

  ui.setupUi(this);
  setWindowTitle(tr("QtChess"));
  denominator = 1.25;
  ui.menu_Setup->setVisible(false);

  if((statusLabel = new(std::nothrow) QLabel(tr("Status: Ready"))) == 0)
    {
      if(chess)
	chess->quit("Memory allocation failure.", EXIT_FAILURE);
      else
	exit(EXIT_FAILURE);
    }

  if((ag1 = new(std::nothrow) QActionGroup(this)) == 0)
    {
      if(chess)
	chess->quit("Memory allocation failure.", EXIT_FAILURE);
      else
	exit(EXIT_FAILURE);
    }

  if((action_Large_Size = new(std::nothrow) QAction(tr("&Large Size"),
						    this)) == 0)
    {
      if(chess)
	chess->quit("Memory allocation failure.", EXIT_FAILURE);
      else
	exit(EXIT_FAILURE);
    }

  action_Large_Size->setData("L");

  if((action_Miniature_Size = new(std::nothrow) QAction(tr("&Miniature Size"),
							this)) == 0)
    {
      if(chess)
	chess->quit("Memory allocation failure.", EXIT_FAILURE);
      else
	exit(EXIT_FAILURE);
    }

  action_Miniature_Size->setData("M");

  if((action_Normal_Size = new(std::nothrow) QAction(tr("&Normal Size"),
						     this)) == 0)
    {
      if(chess)
	chess->quit("Memory allocation failure.", EXIT_FAILURE);
      else
	exit(EXIT_FAILURE);
    }

  action_Normal_Size->setData("N");
  ag1->setExclusive(true);
  ag1->addAction(action_Large_Size);
  ag1->addAction(action_Miniature_Size);
  ag1->addAction(action_Normal_Size);
  ui.menu_View->addAction(action_Large_Size);
  ui.menu_View->addAction(action_Miniature_Size);
  ui.menu_View->addAction(action_Normal_Size);
  action_Large_Size->setCheckable(true);
  action_Miniature_Size->setCheckable(true);
  action_Miniature_Size->setChecked(true);
  action_Normal_Size->setCheckable(true);
  statusLabel->setMargin(5);
  statusLabel->setFrameShadow(QFrame::Raised);
  statusLabel->setFrameShape(QFrame::NoFrame);
  statusBar()->setSizeGripEnabled(false);
  statusBar()->setStyleSheet("QStatusBar::item {"
			     "border: none; "
			     "}");
  statusBar()->addWidget(statusLabel, 100);
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
  connect(action_Large_Size,
	  SIGNAL(triggered(void)),
	  this,
	  SLOT(slotChangeSize(void)));
  connect(action_Miniature_Size,
	  SIGNAL(triggered(void)),
	  this,
	  SLOT(slotChangeSize(void)));
  connect(action_Normal_Size,
	  SIGNAL(triggered(void)),
	  this,
	  SLOT(slotChangeSize(void)));
  connect(ui.action_About,
	  SIGNAL(triggered(void)),
	  this,
	  SLOT(about(void)));
  connect(ui.action_Help,
	  SIGNAL(triggered(void)),
	  this,
	  SLOT(help(void)));

  if((playert = new(std::nothrow) QTimer(this)) != 0)
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

  if((opponentt = new(std::nothrow) QTimer(this)) != 0)
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

  if((help_dialog = new(std::nothrow) qtchess_help_dialog(this)) == 0)
    {
      if(chess)
	chess->quit("Memory allocation failure.", EXIT_FAILURE);
      else
	exit(EXIT_FAILURE);
    }

  if((setup_dialog = new(std::nothrow) qtchess_setup_dialog(this)) == 0)
    {
      if(chess)
	chess->quit("Memory allocation failure.", EXIT_FAILURE);
      else
	exit(EXIT_FAILURE);
    }

  if((promote_dialog = new(std::nothrow) qtchess_promote_dialog(this)) == 0)
    {
      if(chess)
	chess->quit("Memory allocation failure.", EXIT_FAILURE);
      else
	exit(EXIT_FAILURE);
    }

  if((glboard = new(std::nothrow) openglWid(this)) == 0)
    {
      if(chess)
	chess->quit("Memory allocation failure.", EXIT_FAILURE);
      else
	exit(EXIT_FAILURE);
    }

  if(glboard)
    ui.boardFrame->layout()->addWidget(glboard);

  /*
  ** Add the OpenGL board.
  */

  if(glboard)
    {
      glboard->rescale(denominator);
      glboard->resize((int) (1.0 / denominator * OPEN_GL_DIMENSION),
		      (int) (1.0 / denominator * OPEN_GL_DIMENSION));
      ui.boardFrame->setFixedSize(glboard->size() + QSize(25, 25));
    }

#if QT_VERSION < 0x050000
  ui.history->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
  ui.history->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#else
  ui.history->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  ui.history->verticalHeader()->setSectionResizeMode
    (QHeaderView::ResizeToContents);
#endif
  ui.boardFrame->hide();
  ui.boardFrame->show();
  resize(sizeHint());
  ui.boardFrame->setFocus();
  show();
  s_initialized = true;
}

void qtchess_gui::slotShowValidMoves(void)
{
  if(glboard)
    glboard->showValidMoves();
}

void qtchess_gui::slotChangeSize(void)
{
  QAction *action = qobject_cast<QAction *> (sender());
  QString data = "";

  if(action)
    data = action->data().toString();

  if(data == "L")
    {
      if(denominator == 1.0)
	return;

      denominator = 1.0;

      if(glboard)
	{
	  glboard->reinit();
	  glboard->rescale(denominator);
	  glboard->resize(OPEN_GL_DIMENSION, OPEN_GL_DIMENSION);
	  ui.boardFrame->setFixedSize(glboard->size() + QSize(25, 25));
	}

      ui.boardFrame->hide();
      ui.boardFrame->show();
      resize(sizeHint());
      ui.history->scrollToBottom();
      ui.boardFrame->setFocus();
    }
  else if(data == "M")
    {
      if(denominator == 1.25)
	return;

      denominator = 1.25;

      if(glboard)
	{
	  glboard->reinit();
	  glboard->rescale(denominator);
	  glboard->resize((int) (1.0 / denominator * OPEN_GL_DIMENSION),
			  (int) (1.0 / denominator * OPEN_GL_DIMENSION));
	  ui.boardFrame->setFixedSize(glboard->size() + QSize(25, 25));
	}

      ui.boardFrame->hide();
      ui.boardFrame->show();
      resize(sizeHint());
      ui.history->scrollToBottom();
      ui.boardFrame->setFocus();
    }
  else if(data == "N")
    {
      if(denominator == 4.0 / 3.8)
	return;

      denominator = 4.0 / 3.8;

      if(glboard)
	{
	  glboard->reinit();
	  glboard->rescale(denominator);
	  glboard->resize((int) (1.0 / denominator * OPEN_GL_DIMENSION),
			  (int) (1.0 / denominator * OPEN_GL_DIMENSION));
	  ui.boardFrame->setFixedSize(glboard->size() + QSize(25, 25));
	}

      ui.boardFrame->hide();
      ui.boardFrame->show();
      resize(sizeHint());
      ui.history->scrollToBottom();
      ui.boardFrame->setFocus();
    }
}

void qtchess_gui::display(void)
{
}

void qtchess_gui::about(void)
{
  QMessageBox mb(this);
  QPixmap pixmap(":/chess.png");

  mb.setWindowTitle(tr("QtChess: About"));
  mb.setTextFormat(Qt::RichText);
  mb.setText
    (tr("<html>QtChess Version %1.<br>"
	"Copyright (c) 2003 - 2016 Guess Who?<br>"
	"Qt version %2."
	"<hr>"
	"Please visit <a href=\"http://qtchess.sourceforge.net\">"
	"http://qtchess.sourceforge.net</a> for "
	"project information.<br>"
	"For release notes, please visit "
	"<a href=\"http://qtchess.sourceforge.net/release_news.html\">"
	"http://qtchess.sourceforge.net/release_news.html</a>.<br>").
     arg(QTCHESS_VERSION).
     arg(QT_VERSION_STR));
  mb.setStandardButtons(QMessageBox::Ok);
  mb.setIconPixmap
    (pixmap.scaled(256, 256, Qt::IgnoreAspectRatio,
		   Qt::SmoothTransformation));
  mb.exec();
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

void qtchess_gui::help(void)
{
  if(help_dialog)
    help_dialog->setup();
}

void qtchess_gui::quit(void)
{
  if(chess)
    chess->quit(0, EXIT_SUCCESS);
  else
    exit(EXIT_FAILURE);
}

void qtchess_gui::closeEvent(QCloseEvent *e)
{
  Q_UNUSED(e);

  if(chess)
    chess->quit(0, EXIT_SUCCESS);
  else
    exit(EXIT_FAILURE);
}

void qtchess_gui::newGame(void)
{
  int i = 0, j = 0;
  struct move_s current_move;

  if(chess && chess->getFirst() == -1)
    {
      showErrorMsg("Please start a game first.");
      return;
    }
  else if(chess && chess->getFirst() != I_AM_FIRST)
    {
      showErrorMsg("Only the first player may initiate a new game.");
      return;
    }

  if(chess)
    chess->init();

  if(gui && gui->getGLBoard())
    {
      gui->getGLBoard()->newGame();
      gui->getGLBoard()->updateGL();
    }

  for(i = 0; i < NSQUARES; i++)
    for(j = 0; j < NSQUARES; j++)
      if(chess)
	current_move.board[i][j] = chess->board[i][j];
      else
	current_move.board[i][j] = EMPTY_SQUARE;

  current_move.enpassant =
    current_move.pawn_2 =
    current_move.promoted = 0;
  current_move.isOppKingThreat = 0;
  current_move.piece =
    current_move.rook =
    current_move.r_x1 =
    current_move.r_x2 =
    current_move.r_y1 =
    current_move.r_y2 =
    current_move.x1 =
    current_move.x2 =
    current_move.y1 =
    current_move.y2 = -1;
  memset(current_move.departure, 0, sizeof(current_move.departure));

  if(comm)
    comm->sendMove(current_move);

  clearHistory();
  initClocks();

  /*
  ** Careful here. Set this after calling sendMove().
  */

  if(chess)
    chess->setTurn(MY_TURN);
}

void qtchess_gui::setup(void)
{
  if(setup_dialog)
    {
      if(setup_dialog->getHostField())
	setup_dialog->getHostField()->setFocus();

      setup_dialog->exec();
    }
}

qtchess_setup_dialog *qtchess_gui::getSetupDialog(void) const
{
  return setup_dialog;
}

qtchess_promote_dialog *qtchess_gui::getPromoteDialog(void) const
{
  return promote_dialog;
}

void qtchess_gui::notifyConnection(const QString &address,
				   const quint16 port)
{
  setStatusText
    (tr("Status: Host %1:%2 Connected").arg(address).arg(port));
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

void qtchess_gui::showDisconnect(void)
{
  stopTimers(PLAYER_TIMER);
  stopTimers(OPPONENT_TIMER);
}

void qtchess_gui::showNewGameInfo(void)
{
}

void qtchess_setup_dialog::connect_cb(void)
{
  if(comm)
    {
      if(comm->isConnectedRemotely())
	comm->disconnectRemotely();
      else
	comm->connectRemotely();
    }
}

void qtchess_setup_dialog::close_cb(void)
{
  hide();
}

qtchess_setup_dialog::qtchess_setup_dialog(QWidget *parent):
  QDialog(parent)
{
  ui.setupUi(this);
  ui.lScopeId->setEnabled(false);
  ui.rScopeId->setEnabled(false);
  ui.rhost->setText(QHostAddress(QHostAddress::LocalHost).toString());
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
  connect(ui.listen,
	  SIGNAL(clicked(void)),
	  this,
	  SLOT(slotListen(void)));
  connect(comm,
	  SIGNAL(connectedToClient(void)),
	  this,
	  SLOT(slotConnectedToClient(void)));
  connect(comm,
	  SIGNAL(disconnectedFromClient(void)),
	  this,
	  SLOT(slotDisconnectedFromClient(void)));
  ui.remote_gb->setEnabled(false);
}

QLineEdit *qtchess_setup_dialog::getHostField(void) const
{
  return ui.host;
}

QLineEdit *qtchess_setup_dialog::getPortField(void) const
{
  return ui.port;
}

QLineEdit *qtchess_setup_dialog::getRHostField(void) const
{
  return ui.rhost;
}

QLineEdit *qtchess_setup_dialog::getRPortField(void) const
{
  return ui.rport;
}

QLineEdit *qtchess_setup_dialog::getRScopeIdField(void) const
{
  return ui.rScopeId;
}

QLineEdit *qtchess_setup_dialog::getAllowedHostField(void) const
{
  return ui.allowedHost;
}

void qtchess_setup_dialog::slotListen(void)
{
  bool state = false;

  if(sender() == ui.listen)
    if(comm)
      {
	if(comm->isListening())
	  comm->stopListening();
	else
	  comm->setListen();

	state = comm->isListening();
      }

  ui.host->setReadOnly(state);
  ui.port->setReadOnly(state);
  ui.lipv4->setEnabled(!state);
  ui.lipv6->setEnabled(!state);

  if(ui.lipv6->isChecked())
    ui.lScopeId->setReadOnly(state);

  if(state)
    ui.listen->setText(tr("&Stop Listening"));
  else
    ui.listen->setText(tr("&Listen"));
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

void qtchess_setup_dialog::slotLocal(bool state)
{
  if(state)
    {
      if(comm)
	comm->disconnectRemotely();

      slotDisconnectedFromClient();
      ui.local_gb->setEnabled(true);
      ui.remote->blockSignals(true);
      ui.remote->setChecked(false);
      ui.remote->blockSignals(false);
      ui.remote_gb->setEnabled(false);
    }
  else
    {
      ui.local->blockSignals(true);
      ui.local->setChecked(true);
      ui.local->blockSignals(false);
    }
}

void qtchess_setup_dialog::slotRemote(bool state)
{
  if(state)
    {
      if(comm)
	{
	  comm->disconnectRemotely();
	  comm->stopListening();
	}

      slotDisconnectedFromClient();
      slotListen();
      ui.local->blockSignals(true);
      ui.local->setChecked(false);
      ui.local->blockSignals(false);
      ui.local_gb->setEnabled(false);
      ui.remote_gb->setEnabled(true);
    }
  else
    {
      ui.remote->blockSignals(true);
      ui.remote->setChecked(true);
      ui.remote->blockSignals(false);
    }
}

void qtchess_setup_dialog::slotDisconnect(void)
{
  if(comm)
    comm->disconnectRemotely();
}

void qtchess_gui::clearHistory(void)
{
  ui.history->clearContents();
  ui.history->setRowCount(0);
  ui.history->scrollToTop();
  ui.history->horizontalScrollBar()->setValue(0);
}

void qtchess_gui::addHistoryMove(const struct move_s &current_move,
				 const int color)
{
  QTableWidgetItem *item = 0;
  char from_position[2];
  char hist[512];
  char insertX[3];
  char to_let[2];

  memset(from_position, 0, sizeof(from_position));
  memset(hist, 0, sizeof(hist));
  memset(insertX, 0, sizeof(insertX));
  memset(to_let, 0, sizeof(to_let));
  from_position[0] = (char) (97 + current_move.y1);
  to_let[0] = (char) (97 + current_move.y2);

  if(current_move.promoted)
    {
      if(chess && chess->wasPieceWon())
	snprintf(insertX, sizeof(insertX), "%s%s", from_position, "x");

      if(qtchess_validate::isBishop(current_move.piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%dB", insertX,
		 to_let, current_move.x2 + 1);
      else if(qtchess_validate::isKnight(current_move.piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%dN", insertX,
		 to_let, current_move.x2 + 1);
      else if(qtchess_validate::isQueen(current_move.piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%dQ", insertX,
		 to_let, current_move.x2 + 1);
      if(qtchess_validate::isRook(current_move.piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%dR", insertX,
		 to_let, current_move.x2 + 1);
    }
  else
    {
      if(chess && chess->wasPieceWon())
	snprintf(insertX, sizeof(insertX), "%s", "x");

      if(qtchess_validate::isBishop(current_move.piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%s%s%d", "B",
		 current_move.departure, insertX,
		 to_let, current_move.x2 + 1);
      else if(qtchess_validate::isKing(current_move.piece))
	{
	  if(current_move.r_x1 != -1)
	    {
	      if(current_move.y2 == 6)
		snprintf(hist, sizeof(hist),
			 "%s%s%s%d 0-0", "K", insertX,
			 to_let, current_move.x2 + 1);
	      else
		snprintf(hist, sizeof(hist),
			 "%s%s%s%d 0-0-0", "K", insertX,
			 to_let, current_move.x2 + 1);
	    }
	  else
	    snprintf(hist, sizeof(hist),
		     "%s%s%s%d", "K", insertX,
		     to_let, current_move.x2 + 1);
	}
      else if(qtchess_validate::isKnight(current_move.piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%s%s%d", "N",
		 current_move.departure, insertX,
		 to_let, current_move.x2 + 1);
      else if(qtchess_validate::isPawn(current_move.piece))
	{
	  if(chess && chess->wasPieceWon())
	    snprintf(insertX, sizeof(insertX), "%s%s",
		     from_position, "x");

	  if(current_move.enpassant)
	    snprintf(hist, sizeof(hist),
		     "%s%s%d e.p.", insertX,
		     to_let, current_move.x2 + 1);
	  else
	    snprintf(hist, sizeof(hist),
		     "%s%s%d", insertX,
		     to_let, current_move.x2 + 1);
	}
      else if(qtchess_validate::isQueen(current_move.piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%s%s%d", "Q",
		 current_move.departure, insertX,
		 to_let, current_move.x2 + 1);
      else if(qtchess_validate::isRook(current_move.piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%s%s%d", "R",
		 current_move.departure, insertX,
		 to_let, current_move.x2 + 1);
      else
	snprintf(hist, sizeof(hist), "%s", "-----");
    }

  /*
  ** Is the king in check?
  */

  if(current_move.isOppKingThreat)
    strncat(hist, "+", sizeof(hist) - strnlen(hist, sizeof(hist) - 1) - 1);

  if((item = new(std::nothrow) QTableWidgetItem(tr(hist))) != 0)
    {
      item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

      if(ui.history->rowCount() == 0)
	ui.history->setRowCount(1);

      if(ui.history->item(ui.history->rowCount() - 1, 0) != 0 &&
	 ui.history->item(ui.history->rowCount() - 1, 1) != 0)
	ui.history->setRowCount(ui.history->rowCount() + 1);

      if(color == WHITE)
	ui.history->setItem(ui.history->rowCount() - 1, 0, item);
      else if(color == BLACK)
	ui.history->setItem(ui.history->rowCount() - 1, 1, item);

      ui.history->scrollToBottom();
    }
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

qtchess_promote_dialog::qtchess_promote_dialog(QWidget *parent):
  QDialog(parent)
{
  ui.setupUi(this);
  connect(ui.ok,
	  SIGNAL(clicked(void)),
	  this,
	  SLOT(ok_cb(void)));
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

qtchess_help_dialog::qtchess_help_dialog(QWidget *parent):
  QDialog(parent)
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
	"Beige always moves first, unless both parties are clients.\n"
	"Moves are prohibited until connections have been established.\n"
	"To move a piece, first click it and then click "
	"the desired destination.\n"
	"To prevent peer connections, please set the "
	"Allowed IP Address to 0.0.0.0 (:: for IPv6).\n"
	"To view a selected piece's valid moves, "
	"double-click the selected piece."
	));
}

void qtchess_gui::startTimers(const int which)
{
  Q_UNUSED(which);
}

void qtchess_gui::stopTimers(const int which)
{
  Q_UNUSED(which);
}

void qtchess_gui::updatePlayer(void)
{
  static QString stylesheet
    (ui.playerClock->styleSheet());

  if(comm && chess &&
     comm->isReady() && chess->getTurn() == MY_TURN &&
     !chess->isGameOver())
    {
      ui.playerClock->setStyleSheet
	("QWidget {background: rgb(144, 238, 144);}");
      ui.playerClock->setTime(ui.playerClock->time().addSecs(1));
    }
  else
    ui.playerClock->setStyleSheet(stylesheet);
}

void qtchess_gui::updateOpponent(void)
{
  static QString stylesheet
    (ui.opponentClock->styleSheet());

  if(comm && chess &&
     comm->isReady() && chess->getTurn() == THEIR_TURN &&
     !chess->isGameOver())
    {
      ui.opponentClock->setStyleSheet
	("QWidget {background: rgb(240, 128, 128);}");
      ui.opponentClock->setTime(ui.opponentClock->time().addSecs(1));
    }
  else
    ui.opponentClock->setStyleSheet(stylesheet);
}

void qtchess_gui::initClocks(void)
{
  ui.playerClock->setTime(QTime(0, 0, 0));
  ui.opponentClock->setTime(QTime(0, 0, 0));
}
