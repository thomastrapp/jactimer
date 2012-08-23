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


#include "timerMenu.h"
#include <QDebug>

timerMenu::timerMenu(QSharedPointer<configuredTimer> t, timerWindow *parent)
  : QMenu(parent),
    timer(t),
    playPauseAction(new QAction(QIcon(":/buttons/play.svg"), tr("Run Timer"), this)),
    playPauseChecked(false)
{
  buildMenu(parent);
}

timerMenu::~timerMenu()
{
  qDebug() << "timerMenu going down";
}

void timerMenu::onTimerStarted()
{
  playPauseChecked = true;
  playPauseAction->setIcon(QIcon(":/buttons/pause.svg"));
  playPauseAction->setText(tr("Pause Timer"));
}

void timerMenu::onTimerStopped()
{
  playPauseChecked = false;
  playPauseAction->setIcon(QIcon(":/buttons/play.svg"));
  playPauseAction->setText(tr("Run Timer"));
}

void timerMenu::onTimerReset()
{
  toggleEnablePlayPause(true);
}

void timerMenu::onTimerFinished()
{
  toggleEnablePlayPause(false);
}

void timerMenu::onPlayPauseClicked()
{
  if( playPauseChecked )
  {
    timer.data()->stop();
  }
  else
  {
    timer.data()->start();
  }
}

void timerMenu::toggleEnablePlayPause(bool enable)
{
  playPauseAction->setEnabled(enable);
}

void timerMenu::buildMenu(timerWindow * parent)
{
  QAction * resetAction = new QAction(QIcon(":/buttons/stop.svg"), tr("Stop"), this);
  QAction *  prefAction = new QAction(QIcon(":/buttons/pref.svg"), tr("Preferences"), this);
  QAction *  aboutAction = new QAction(QIcon(":/buttons/about.svg"), tr("About"), this);
  QAction *  quitAction = new QAction(QIcon(":/buttons/quit.svg"), tr("Quit"), this);
  QMenu *    windowMenu = new QMenu(tr("Window"), this);
  windowMenu->setIcon(QIcon(":/buttons/window.svg"));

  resetAction->setShortcut(Qt::Key_S);
  quitAction->setShortcut(QKeySequence::Quit);
  playPauseAction->setShortcut(Qt::Key_Space);
#ifdef Q_WS_MAC
  prefAction->setShortcut(QKeySequence::Preferences);
#else
  prefAction->setShortcut(QKeySequence(tr("Ctrl+O", "Open Options")));
#endif

  // QMenu will take ownership of these actions
  windowMenu->addAction(tr("Maximize"),    parent, SLOT(showMaximized()));
  windowMenu->addAction(tr("Normal"),      parent, SLOT(showNormal()));
  windowMenu->addAction(tr("Medium Size"), parent, SLOT(showMedium()));

  // Let actions be accessible by parent (timerWindow)
  // In this case to enable application wide shortcuts to work
  parent->addAction(prefAction);
  parent->addAction(resetAction);
  parent->addAction(playPauseAction);
  parent->addAction(quitAction);

  addAction(playPauseAction);
  addAction(resetAction);
  addAction(prefAction);
  addMenu(windowMenu);
  addAction(aboutAction);
  addAction(quitAction);

  connect(resetAction,     SIGNAL(triggered()),    timer.data(), SLOT(reset()));
  connect(resetAction,     SIGNAL(triggered()),    timer.data(), SLOT(switchOff()));
  connect(playPauseAction, SIGNAL(triggered()),    this,         SLOT(onPlayPauseClicked()));
  connect(prefAction,      SIGNAL(triggered()),    parent,       SLOT(onShowSettings()));
  connect(aboutAction,     SIGNAL(triggered()),    parent,       SLOT(showAboutDialog()));
  connect(quitAction,      SIGNAL(triggered()),    parent,       SLOT(quit()));
  connect(timer.data(),    SIGNAL(timerStarted()), this,         SLOT(onTimerStarted()));
  connect(timer.data(),    SIGNAL(timerStopped()), this,         SLOT(onTimerStopped()));
  connect(timer.data(),    SIGNAL(timerReset()),   this,         SLOT(onTimerReset()));
  connect(timer.data(),    SIGNAL(switchedOn()),   this,         SLOT(onTimerFinished()));
}
