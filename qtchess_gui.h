#ifndef _QTCHESS_GUI_
#define _QTCHESS_GUI_

/*
** -- C Includes --
*/

/*
** -- C++ Includes --
*/

/*
** -- Local Includes --
*/

#include "qtchess.h"
#include "ui_helpDialog.h"
#ifndef QTCHESS_PLUGIN
#include "ui_setupDialog.h"
#endif
#include "ui_mainwindow.h"
#include "ui_promotionDialog.h"
#include "qtchess_gui_opengl.h"

/*
** -- Qt Includes --
*/

#include <QFrame>
#include <QComboBox>
#include <QTextEdit>
#include <QTimeEdit>
#include <QCloseEvent>

/*
** -- Class(es) --
*/

class qtchess_help_dialog: public QDialog
{
  Q_OBJECT

 public:

  /*
  ** -- Members --
  */

  /*
  ** -- Methods --
  */

  void setup(void);
  qtchess_help_dialog(QWidget *);

 private:

  /*
  ** -- Members --
  */

  Ui_helpDialog ui;

  /*
  ** -- Methods --
  */

  /*
  ** -- Slots --
  */

 private slots:

  void ok_cb(void);
};

#ifndef QTCHESS_PLUGIN
class qtchess_setup_dialog: public QDialog
{
  Q_OBJECT

 public:

  /*
  ** -- Members --
  */

  /*
  ** -- Methods --
  */

  qtchess_setup_dialog(void);
  void disconnectedState(void);
  QLineEdit *getHostField(void);
  QLineEdit *getPortField(void);
  QLineEdit *getRHostField(void);
  QLineEdit *getRPortField(void);
  qtchess_setup_dialog(QWidget *);

 private:

  /*
  ** -- Members --
  */

  Ui_setupDialog ui;

  /*
  ** -- Methods --
  */

  /*
  ** -- Slots --
  */

 private slots:

  void ok_cb(void);
  void close_cb(void);
  void disconnect_cb(void);
};
#endif

class qtchess_promote_dialog: public QDialog
{
  Q_OBJECT

 public:

  /*
  ** -- Members --
  */

  /*
  ** -- Methods --
  */

  void setup(void);
  qtchess_promote_dialog(QWidget *);

  QComboBox *getMenu(void)
    {
      return ui.menu;
    }

 private:

  /*
  ** -- Members --
  */

  Ui_promotionDialog ui;

  /*
  ** -- Methods --
  */

  /*
  ** -- Slots --
  */

 private slots:

  void ok_cb(void);
};

class qtchess_gui: public QMainWindow
{
  Q_OBJECT

 public:

  /*
  ** -- Members --
  */

  /*
  ** -- Methods --
  */

  qtchess_gui(void)
    {
      help_dialog = 0;
#ifndef QTCHESS_PLUGIN
      setup_dialog = 0;
#endif
      promote_dialog = 0;
    }

  ~qtchess_gui()
    {
      if(help_dialog != 0)
	delete help_dialog;

#ifndef QTCHESS_PLUGIN
      if(setup_dialog != 0)
	delete setup_dialog;
#endif

      if(promote_dialog != 0)
	delete promote_dialog;

      if(playert != 0)
	delete playert;

      if(opponentt != 0)
	delete opponentt;

#ifdef _DEBUG_
      (void) fprintf(stderr, "~qtchess_gui()\n");
#endif
    }

  openglWid *getGLBoard(void)
    {
      return glboard;
    }

  Ui_mainWindow getUI(void)
    {
      return ui;
    }

  int exec(void);
  void init(void);
  void display(void);
  void closeEvent(QCloseEvent *);
  void initClocks(void);
  void stopTimers(const int);
  void startTimers(const int);
  void clearHistory(void);
  void showErrorMsg(const char *);
  void showGameOver(const int);
  void setStatusText(const QString &);
  void addHistoryMove(const struct move_s, const int);
  void showDisconnect(const bool);
  void showNewGameInfo(void);
  void notifyConnection(const QString &);
#ifndef QTCHESS_PLUGIN
  qtchess_setup_dialog *getSetupDialog(void);
#endif
  qtchess_promote_dialog *getPromoteDialog(void);

 private:

  /*
  ** -- Members --
  */

  double denominator;
  QLabel *statusLabel;
  QTimer *playert;
  QTimer *opponentt;
  QAction *action_Large_Size;
  QAction *action_Normal_Size;
  openglWid *glboard;
  Ui_mainWindow ui;
  qtchess_help_dialog *help_dialog;
#ifndef QTCHESS_PLUGIN
  qtchess_setup_dialog *setup_dialog;
#endif
  qtchess_promote_dialog *promote_dialog;

  /*
  ** -- Methods --
  */

  /*
  ** -- Slots --
  */

 private slots:

  void help(void);
  void quit(void);
  void about(void);
#ifndef QTCHESS_PLUGIN
  void setup(void);
#endif
  void newGame(void);
  void updatePlayer(void);
  void slotChangeSize(void);
  void updateOpponent(void);
  void slotShowValidMoves(void);
};

#endif
