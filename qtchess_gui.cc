/*
** -- Local Includes --
*/

#include "qtchess_gui.h"

/*
** -- Qt Includes --
*/

#include <QMenu>
#include <QLabel>
#include <QCloseEvent>
#include <QGridLayout>
#include <QHBoxLayout>

extern qtchess *chess;
extern qtchess_gui *gui;
extern qtchess_comm *comm;
extern QApplication *qapp;

int qtchess_gui::exec(void)
{
  if(qapp)
    return qapp->exec();
  else
    return -1;
}

void qtchess_gui::setStatusText(const QString &str)
{
  statusLabel->setText(tr(str.toAscii()));
}

void qtchess_gui::init(void)
{
  QActionGroup *ag1 = 0;

  ui.setupUi(this);
  setWindowTitle(tr("QtChess"));
  denominator = 4.0 / 3.0;
  ui.menu_Setup->setVisible(false);

  if((statusLabel = new(std::nothrow) QLabel(tr("Status: Ready"))) == 0)
    {
      if(chess)
	chess->quit("Memory allocation failure.", EXIT_FAILURE);
      else
	::exit(-1);
    }

  if((ag1 = new(std::nothrow) QActionGroup(this)) == 0)
    {
      if(chess)
	chess->quit("Memory allocation failure.", EXIT_FAILURE);
      else
	::exit(-1);
    }

  if((action_Large_Size = new(std::nothrow) QAction("&Large Size", this)) == 0)
    {
      if(chess)
	chess->quit("Memory allocation failure.", EXIT_FAILURE);
      else
	::exit(-1);
    }

  if((action_Normal_Size = new(std::nothrow) QAction("&Normal Size",
						     this)) == 0)
    {
      if(chess)
	chess->quit("Memory allocation failure.", EXIT_FAILURE);
      else
	::exit(-1);
    }

  ag1->setExclusive(true);
  ag1->addAction(action_Large_Size);
  ag1->addAction(action_Normal_Size);
  ui.menu_View->addAction(action_Large_Size);
  ui.menu_View->addAction(action_Normal_Size);
  action_Large_Size->setCheckable(true);
  action_Normal_Size->setCheckable(true);
  action_Normal_Size->setChecked(true);
  statusLabel->setMargin(0);
  statusLabel->setFrameStyle(QFrame::NoFrame);
  statusBar()->setSizeGripEnabled(false);
  statusBar()->addWidget(statusLabel, 100);
  connect(ui.action_New_Game, SIGNAL(triggered(void)), this,
	  SLOT(newGame(void)));
  connect(ui.action_Quit, SIGNAL(triggered(void)), this, SLOT(quit(void)));
  connect(ui.action_Connection_Configuration,
	  SIGNAL(triggered(void)), this, SLOT(setup(void)));
  connect(action_Large_Size,
	  SIGNAL(triggered(void)), this, SLOT(slotChangeSize(void)));
  connect(action_Normal_Size,
	  SIGNAL(triggered(void)), this, SLOT(slotChangeSize(void)));
  connect(ui.action_About,
	  SIGNAL(triggered(void)), this, SLOT(about(void)));
  connect(ui.action_Help,
	  SIGNAL(triggered(void)), this, SLOT(help(void)));

  if((playert = new(std::nothrow) QTimer()) != 0)
    {
      connect(playert, SIGNAL(timeout()), SLOT(updatePlayer(void)));
      playert->start(1000);
    }

  if((opponentt = new(std::nothrow) QTimer()) != 0)
    {
      connect(opponentt, SIGNAL(timeout()), SLOT(updateOpponent(void)));
      opponentt->start(1000);
    }

  if((help_dialog = new(std::nothrow) qtchess_help_dialog(this)) == 0)
    {
      if(chess)
	chess->quit("Memory allocation failure.", EXIT_FAILURE);
      else
	::exit(-1);
    }

  if((setup_dialog = new(std::nothrow) qtchess_setup_dialog(this)) == 0)
    {
      if(chess)
	chess->quit("Memory allocation failure.", EXIT_FAILURE);
      else
	::exit(-1);
    }

  if((promote_dialog = new(std::nothrow) qtchess_promote_dialog(this)) == 0)
    {
      if(chess)
	chess->quit("Memory allocation failure.", EXIT_FAILURE);
      else
	::exit(-1);
    }

  if((glboard = new(std::nothrow) openglWid(ui.boardFrame)) == 0)
    {
      if(chess)
	chess->quit("Memory allocation failure.", EXIT_FAILURE);
      else
	::exit(-1);
    }

  /*
  ** Add the OpenGL board.
  */

  if(glboard)
    {
      glboard->rescale(denominator);
      glboard->resize((int) (0.75 * OPEN_GL_DIMENSION),
		      (int) (0.75 * OPEN_GL_DIMENSION));
      ui.boardFrame->setFixedSize(glboard->size());
    }

  ui.history->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
  ui.history->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
  ui.boardFrame->hide();
  ui.boardFrame->show();
  resize(sizeHint());
  ui.boardFrame->setFocus();
  show();
}

void qtchess_gui::slotShowValidMoves(void)
{
  if(glboard)
    glboard->showValidMoves();
}

void qtchess_gui::slotChangeSize(void)
{
  QAction *action = qobject_cast<QAction *>(sender());
  QString name = "";

  name = action->text();

  if(name.contains(tr("Large")))
    {
      if(denominator == 1.0)
	return;

      denominator = 1.0;

      if(glboard)
	{
	  glboard->reinit();
	  glboard->rescale(denominator);
	  glboard->resize(OPEN_GL_DIMENSION, OPEN_GL_DIMENSION);
	  ui.boardFrame->setFixedSize(glboard->size());
	}

      ui.boardFrame->hide();
      ui.boardFrame->show();
      resize(sizeHint());
      ui.history->scrollToBottom();
      ui.boardFrame->setFocus();
    }
  else if(name.contains(tr("Normal")))
    {
      if(denominator == 4.0 / 3.0)
	return;

      denominator = 4.0 / 3.0;

      if(glboard)
	{
	  glboard->reinit();
	  glboard->rescale(denominator);
	  glboard->resize((int) (0.75 * OPEN_GL_DIMENSION),
			  (int) (0.75 * OPEN_GL_DIMENSION));
	  ui.boardFrame->setFixedSize(glboard->size());
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

  mb.setWindowTitle(tr("QtChess: About"));
  mb.setTextFormat(Qt::RichText);
  mb.setText("<html>QtChess Version 3.16.<br>"
	     "Copyright (c) 2003, 2004, 2006, 2007, 2008, 2009, 2011, 2012 "
	     "Miss Bombastic."
	     "<hr>"
	     "Please visit <a href=\"http://qtchess.sourceforge.net\">"
	     "http://qtchess.sourceforge.net</a> for "
	     "project information.<br>"
	     "For release notes, please visit "
	     "<a href=\"http://qtchess.sourceforge.net/release_news.html\">"
	     "http://qtchess.sourceforge.net/release_news.html</a>.");
  mb.setStandardButtons(QMessageBox::Ok);
  mb.setIconPixmap(QPixmap("./chess.png"));
  mb.exec();
}

void qtchess_gui::showErrorMsg(const char *message)
{
  QMessageBox::critical(this, tr("QtChess: Error"), tr(message),
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
    ::exit(-1);
}

void qtchess_gui::closeEvent(QCloseEvent *e)
{
  Q_UNUSED(e);

  if(chess)
    chess->quit(0, EXIT_SUCCESS);
  else
    ::exit(-1);
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
	current_move.board[i][j] = -1;

  current_move.x1 = current_move.x2 = current_move.y1 =
    current_move.y2 = current_move.r_x1 =
    current_move.r_x2 = current_move.r_y1 =
    current_move.r_y2 = current_move.piece =
    current_move.rook = -1;
  current_move.promoted = current_move.pawn_2 = 0;

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
      if(setup_dialog->getRHostField())
	setup_dialog->getRHostField()->setFocus();

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

void qtchess_gui::notifyConnection(const QString &address)
{
  statusLabel->setText(tr("Status: Host ") +
		       address + tr(" Connected"));
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
  if(setup_dialog)
    setup_dialog->disconnectedState();

  stopTimers(PLAYER_TIMER);
  stopTimers(OPPONENT_TIMER);
}

void qtchess_gui::showNewGameInfo(void)
{
}

void qtchess_setup_dialog::ok_cb(void)
{
  QString str1 = "", str2 = "";

  str1 = ui.rhost->text().trimmed();
  str2 = ui.rport->text().trimmed();

  if(str1.length() == 0 || str2.length() == 0)
    return;

  if(comm)
    comm->connectRemotely();
}

void qtchess_setup_dialog::close_cb(void)
{
  hide();
}

void qtchess_setup_dialog::disconnect_cb(void)
{
  if(comm && comm->isSet())
    {
      comm->disconnectRemotely();

      if(!comm->isSet())
	disconnectedState();
    }
}

void qtchess_setup_dialog::disconnectedState(void)
{
  ui.ok->setText(tr("&Connect"));
  ui.rhost->setReadOnly(false);
  ui.rport->setReadOnly(false);
  ui.rhost->setEnabled(true);
  ui.rport->setEnabled(true);
}

qtchess_setup_dialog::qtchess_setup_dialog(QWidget *parent):
  QDialog(parent)
{
  ui.setupUi(this);
  ui.lScopeId->setEnabled(false);
  ui.rScopeId->setEnabled(false);
  connect(ui.cancel, SIGNAL(clicked(void)), this, SLOT(close_cb(void)));
  connect(ui.ok, SIGNAL(clicked(void)), this, SLOT(ok_cb(void)));
  connect(ui.ipv4, SIGNAL(clicked(void)), this,
	  SLOT(slotProtocolChanged(void)));
  connect(ui.ipv6, SIGNAL(clicked(void)), this,
	  SLOT(slotProtocolChanged(void)));
  connect(ui.listen, SIGNAL(clicked(void)), this,
	  SLOT(slotListen(void)));
  connect(comm, SIGNAL(connectedToClient(void)),
	  this, SLOT(slotConnectedToClient(void)));
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
  if(comm)
    {
      if(comm->isListening())
	comm->stopListening();
      else
	comm->setListen();

      bool state = comm->isListening();

      ui.host->setReadOnly(state);
      ui.ipv4->setEnabled(!state);
      ui.ipv6->setEnabled(!state);

      if(ui.ipv6->isChecked())
	ui.lScopeId->setReadOnly(state);

      if(state)
	ui.listen->setText(tr("&Stop Listening"));
      else
	{
	  ui.port->clear();
	  ui.listen->setText(tr("&Listen"));

	  if(gui)
	    gui->setStatusText(tr("Status: Ready"));
	}
    }
}

void qtchess_setup_dialog::slotProtocolChanged(void)
{
  if(sender() == ui.ipv4)
    {
      ui.host->setText(QHostAddress(QHostAddress::LocalHost).toString());
      ui.allowedHost->setText("0.0.0.0");
      ui.lScopeId->clear();
      ui.lScopeId->setEnabled(false);
      ui.rScopeId->clear();
      ui.rScopeId->setEnabled(false);
    }
  else
    {
      ui.host->setText(QHostAddress(QHostAddress::LocalHostIPv6).toString());
      ui.lScopeId->setText
	(QHostAddress(QHostAddress::LocalHostIPv6).scopeId());
      ui.allowedHost->setText("::");
      ui.lScopeId->setEnabled(true);
      ui.rScopeId->setEnabled(true);
    }
}

void qtchess_setup_dialog::slotConnectedToClient(void)
{
  ui.ok->setText(tr("&Disconnect"));
  ui.rhost->setReadOnly(true);
  ui.rport->setReadOnly(true);
  ui.rhost->setEnabled(true);
  ui.rport->setEnabled(true);
}

QHostAddress qtchess_setup_dialog::getListeningAddress(void) const
{
  QHostAddress address;

  if(ui.ipv4->isChecked())
    address.setAddress(ui.host->text().trimmed());
  else
    {
      address.setAddress(ui.host->text().trimmed());
      address.setScopeId(ui.lScopeId->text().trimmed());
    }

  return address;
}

void qtchess_gui::clearHistory(void)
{
  QStringList list;

  list.append("Beige");
  list.append("Crimson");
  ui.history->clear();
  ui.history->setRowCount(0);
  ui.history->setColumnCount(0);
  ui.history->scrollToTop();
  ui.history->horizontalScrollBar()->setValue(0);
  ui.history->setColumnCount(list.size());
  ui.history->setHorizontalHeaderLabels(list);
  list.clear();
}

void qtchess_gui::addHistoryMove(const struct move_s current_move,
				 const int color)
{
  char hist[512];
  char to_let[2];
  char insertX[3];
  char from_position[2];
  QTableWidgetItem *item = 0;

  memset(hist, 0, sizeof(hist));
  memset(to_let, 0, sizeof(to_let));
  memset(insertX, 0, sizeof(insertX));
  memset(from_position, 0, sizeof(from_position));
  to_let[0] = (char) (97 + current_move.y2);
  from_position[0] = (char) (97 + current_move.y1);

#ifdef _DEBUG_
  fprintf(stderr, "from_position = %s, to_let = %s\n",
	  from_position, to_let);
#endif

  if(current_move.promoted)
    {
      if(chess && chess->wasPieceWon())
	snprintf(insertX, sizeof(insertX), "%s%s", from_position, "x");

      if(qtchess_validate::isRook(current_move.piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%dR", insertX,
		 to_let, current_move.x2 + 1);
      else if(qtchess_validate::isBishop(current_move.piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%dB", insertX,
		 to_let, current_move.x2 + 1);
      else if(qtchess_validate::isQueen(current_move.piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%dQ", insertX,
		 to_let, current_move.x2 + 1);
      else if(qtchess_validate::isKnight(current_move.piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%dN", insertX,
		 to_let, current_move.x2 + 1);
    }
  else
    {
      if(chess && chess->wasPieceWon())
	snprintf(insertX, sizeof(insertX), "%s", "x");

      if(qtchess_validate::isKing(current_move.piece))
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
      else if(qtchess_validate::isRook(current_move.piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%s%s%d", "R",
		 current_move.departure, insertX,
		 to_let, current_move.x2 + 1);
      else if(qtchess_validate::isBishop(current_move.piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%s%s%d", "B",
		 current_move.departure, insertX,
		 to_let, current_move.x2 + 1);
      else if(qtchess_validate::isQueen(current_move.piece))
	snprintf(hist, sizeof(hist),
		 "%s%s%s%s%d", "Q",
		 current_move.departure, insertX,
		 to_let, current_move.x2 + 1);
      else
	snprintf(hist, sizeof(hist),
		 "%s%s%s%s%d", "N",
		 current_move.departure, insertX,
		 to_let, current_move.x2 + 1);
    }

  /*
  ** Is the King in check?
  */

  if(current_move.isOppKingThreat)
    strncat(hist, "+", sizeof(hist) - 1);

  if((item = new(std::nothrow) QTableWidgetItem(tr(hist))) != 0)
    {
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
  connect(ui.ok, SIGNAL(clicked(void)), this, SLOT(ok_cb(void)));
}

void qtchess_help_dialog::ok_cb(void)
{
  hide();
}

void qtchess_help_dialog::setup(void)
{
  ui.ok->setFocus();
  exec();
}

qtchess_help_dialog::qtchess_help_dialog(QWidget *parent):
  QDialog(parent)
{
  ui.setupUi(this);
  connect(ui.ok, SIGNAL(clicked(void)), this, SLOT(ok_cb(void)));
  setMinimumWidth(600);
  setMinimumHeight(300);
  ui.text->append(tr("Please do not connect to the same process.\n"
		     "Beige always moves first.\n"
		     "To move a piece, first click it and then click "
		     "the desired destination.\n"
		     "To view a selected piece's valid moves, "
		     "double-click the selected piece.\n"
		     "Moves are prohibited until connections have been "
		     "established.\n"
		     "An empty Allowed IP Address value will allow any "
		     "peer to connect.\n"
		     "To prevent peer connections, please set the "
		     "Allowed IP Address to 0.0.0.0."
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
  if(comm && chess &&
     comm->isReady() && chess->getTurn() == MY_TURN && !chess->isGameOver())
    ui.playerClock->setTime(ui.playerClock->time().addSecs(1));
}

void qtchess_gui::updateOpponent(void)
{
  if(comm && chess &&
     comm->isReady() && chess->getTurn() == THEIR_TURN && !chess->isGameOver())
    ui.opponentClock->setTime(ui.opponentClock->time().addSecs(1));
}

void qtchess_gui::initClocks(void)
{
  ui.playerClock->setTime(QTime(0, 0, 0));
  ui.opponentClock->setTime(QTime(0, 0, 0));
}
