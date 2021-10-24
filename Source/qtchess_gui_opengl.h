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

#ifndef _QTCHESS_GUI_OPENGL_
#define _QTCHESS_GUI_OPENGL_

#include <QLabel>
#include <QMouseEvent>
#include <QPointer>

#include "qtchess_defs.h"

class point
{
 public:
  point(void)
  {
    x = y = -1;
  }

  int x;
  int y;
};

class qtchess_piece: public QLabel
{
  Q_OBJECT

 public:
  qtchess_piece(const int i, const int j, QObject *parent):QLabel(nullptr)
  {
    Q_UNUSED(parent);
    m_i = i;
    m_j = j;
  };

  int i() const
  {
    return m_i;
  }

  int j() const
  {
    return m_j;
  }

  void resizeEvent(QResizeEvent *event)
  {
    Q_UNUSED(event);

    auto font(this->font());
    auto p = font.pointSizeF();

    p = qMin(size().height(), size().width()) / 2.5;
    font.setPointSizeF(p);
    setFont(font);
  }

 private:
  int m_i;
  int m_j;

  void mouseDoubleClickEvent(QMouseEvent *event)
  {
    if(!event)
      return;

    if(event->button() == Qt::LeftButton)
      emit doubleClicked(this);

    QLabel::mouseDoubleClickEvent(event);
  }

  void mousePressEvent(QMouseEvent *event)
  {
    if(!event)
      return;

    if(event->button() == Qt::LeftButton)
      emit pressed(this);

    QLabel::mousePressEvent(event);
  }

 signals:
  void doubleClicked(qtchess_piece *piece);
  void pressed(qtchess_piece *piece);
};

class openglWid: public QObject
{
  Q_OBJECT

 public:
  openglWid(QObject *);
  void add(QFrame *frame);
  void newGame(void);
  void reinit(void);

 private:
  QPointer<qtchess_piece> m_labels[NSQUARES][NSQUARES];
  int m_mouse_pressed;
  point m_point_pressed;
  point m_point_selected;
  void highlightSquare(const int i, const int j);
  void paint(void);

 private slots:
  void slotPieceDoubleClicked(qtchess_piece *piece);
  void slotPiecePressed(qtchess_piece *piece);
};

#endif
