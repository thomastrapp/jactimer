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


#include "timerApp.h"
#include <QDebug>

timerApp::timerApp()
  : settings(new QSettings),
    settingsWindow(settings),
    window(NULL),
    hasWindow(false)
{
  connect(&settingsWindow, SIGNAL(finished(int)), this, SLOT(onDialogFinished(int)));
}

timerApp::~timerApp()
{
  qDebug() << "timerApp going down";
}

void timerApp::onShowSettings()
{
  settingsWindow.showEditSettings();
}

void timerApp::onDialogFinished(int result)
{
  switch( result )
  {
  case QDialog::Accepted:
    if( !hasWindow )
      create();
    break;
  case QDialog::Rejected:
    if( !hasWindow )
      QApplication::instance()->quit();
    break;
  default:
    Q_ASSERT(false);
  }
}

void timerApp::create()
{
  Q_ASSERT(hasWindow == false);

  QSharedPointer<configuredTimer> timer(new configuredTimer(settings));

  // Timer Window deletes itself if user wishes to create a new timer (timerWindow calls deleteLater())
  window = new timerWindow(timer, settings);
  hasWindow = true;

  connect(window,          SIGNAL(showSettings()),    this,         SLOT(onShowSettings()));
  connect(&settingsWindow, SIGNAL(settingsChanged()), window,       SLOT(onSettingsChanged()));
  connect(&settingsWindow, SIGNAL(settingsChanged()), timer.data(), SLOT(onSettingsChanged()));

  window->show();

  if( settings->value("timer/start", false).value<bool>() )
    timer->start();
}

void timerApp::run()
{
  if( settings->value("timer/showsettings", true).value<bool>() )
    settingsWindow.showCreateSettings();
  else
    create();
}
