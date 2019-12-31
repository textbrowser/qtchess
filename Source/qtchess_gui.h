#ifndef _QTCHESS_GUI_
#define _QTCHESS_GUI_

#include "qtchess.h"
#include "qtchess_gui_opengl.h"
#include "ui_helpDialog.h"
#include "ui_mainwindow.h"
#include "ui_promotionDialog.h"
#include "ui_setupDialog.h"

#include <QCloseEvent>
#include <QComboBox>
#include <QFrame>
#include <QHostAddress>
#include <QTextEdit>
#include <QTimeEdit>

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
  qtchess_setup_dialog(void);
  QHostAddress getListeningAddress(void) const;
  QLineEdit *getAllowedHostField(void) const;
  QLineEdit *getHostField(void) const;
  QLineEdit *getPortField(void) const;
  QLineEdit *getRHostField(void) const;
  QLineEdit *getRPortField(void) const;
  QLineEdit *getRScopeIdField(void) const;

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
  void slotLocal(bool state);
  void slotListen(void);
  void slotProtocolChanged(void);
  void slotRemote(bool state);
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
  qtchess_gui(void)
  {
    action_Large_Size = 0;
    action_Miniature_Size = 0;
    action_Normal_Size = 0;
    denominator = 1;
    glboard = 0;
    help_dialog = 0;
    opponentt = 0;
    playert = 0;
    promote_dialog = 0;
    setup_dialog = 0;
    statusLabel = 0;
  }

  ~qtchess_gui()
  {
    if(help_dialog != 0)
      help_dialog->deleteLater();

    if(opponentt != 0)
      opponentt->deleteLater();

    if(playert != 0)
      playert->deleteLater();

    if(promote_dialog != 0)
      promote_dialog->deleteLater();

    if(setup_dialog != 0)
      setup_dialog->deleteLater();
  }

  QString color(void) const
  {
    if(setup_dialog)
      return setup_dialog->color();
    else
      return "";
  }

  Ui_mainWindow getUI(void) const
  {
    return ui;
  }

  int exec(void);

  openglWid *getGLBoard(void) const
  {
    return glboard;
  }

  qtchess_promote_dialog *getPromoteDialog(void) const;
  qtchess_setup_dialog *getSetupDialog(void) const;
  void addHistoryMove(const struct move_s &, const int);
  void clearHistory(void);
  void display(void);
  void init(void);
  void initClocks(void);
  void notifyConnection(const QString &, const quint16);
  void setStatusText(const QString &);
  void showDisconnect(void);
  void showErrorMsg(const char *);
  void showGameOver(const int);
  void showNewGameInfo(void);
  void startTimers(const int);
  void stopTimers(const int);

 private:
  QAction *action_Large_Size;
  QAction *action_Miniature_Size;
  QAction *action_Normal_Size;
  QLabel *statusLabel;
  QTimer *opponentt;
  QTimer *playert;
  Ui_mainWindow ui;
  double denominator;
  openglWid *glboard;
  qtchess_help_dialog *help_dialog;
  qtchess_promote_dialog *promote_dialog;
  qtchess_setup_dialog *setup_dialog;

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
  void slotChangeSize(void);
  void slotShowValidMoves(void);
  void updateOpponent(void);
  void updatePlayer(void);
};

#endif
