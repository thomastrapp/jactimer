/*
  This file is part of the Jactimer Project.
  Jactimer is a free and feature-rich countdown timer.
  Visit http://jactimer.org

  Copyright (C) 2010 Thomas Trapp; mailto: jactimer@thomastrapp.com

  Jactimer is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

  See COPYING for details.
*/


#ifndef TIMERWINDOW_H
#define TIMERWINDOW_H

#include <QDebug>

#include <QMainWindow>
#include <QSharedPointer>
#include <QSizeGrip>
#include <QSystemTrayIcon>
#include <QMouseEvent>

#include "clockTray.h"
#include "timeDisplay.h"
#include "timerMenu.h"
#include "configuredTimer.h"
#include "aboutJACT.h"
#include "aboutJACTDialog.h"

class timerMenu;

namespace Ui {
  class timerWindow;
}

class timerWindow : public QMainWindow
{
  Q_OBJECT

public:
  timerWindow(QSharedPointer<configuredTimer> t, QSharedPointer<QSettings> settings, QWidget *parent = 0);
  ~timerWindow();

public slots:
  /* Slots */
  void onNewTimerIndex(uint togo);
  void toggleTimer(bool checked);
  void onTrayIconClicked(QSystemTrayIcon::ActivationReason ar);
  void onTimerFinished();
  void onShowSettings();
  void onSettingsChanged();
  void showMedium();
  void quit();
  void showAboutDialog();

signals:
  /* Signals */
  void showSettings();

protected:
  /* Events */
  void changeEvent(QEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void contextMenuEvent(QContextMenuEvent *event);

private:
  timerWindow(const timerWindow&);
  timerWindow& operator=(const timerWindow&);

  void connectTimer();
  void buildSystemTray();
  void applySettings();
  void applyWindowFlags();

  Ui::timerWindow *ui;
  timeDisplay tDisplay;
  QSizeGrip grip;

  QSharedPointer<configuredTimer> timer;
  QSharedPointer<QSettings> sett;

  clockTray * trayIcon;
  timerMenu * menu;

  QPoint dragPosition;
};

#endif // TIMERWINDOW_H
