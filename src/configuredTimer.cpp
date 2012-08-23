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


#include "configuredTimer.h"
#include <QDebug>

configuredTimer::configuredTimer(QSharedPointer<QSettings> sett)
  : timedTrigger(sett->value("timer/delay").value<uint>()),
    settings(sett)
{
  buildEvents();
}

configuredTimer::~configuredTimer()
{
  deleteEvents();
  qDebug() << "configuredTimer going down";
}

void configuredTimer::deleteEvents()
{
  while( !ev.empty() )
  {
    delete ev.last();
    ev.pop_back();
  }
}

void configuredTimer::onSettingsChanged()
{
  deleteEvents();
  buildEvents();
  if( getDelay() != settings->value("timer/delay").value<uint>() )
    setDelay(settings->value("timer/delay").value<uint>());
}

void configuredTimer::buildEvents()
{
  if( settings->value("actions/dialog").value<bool>() )
  {
    dialogEvent * de = new dialogEvent(settings->value("timer/message").value<QString>());
    QObject::connect(this, SIGNAL(switchedOn()), de, SLOT(start()));
    ev.push_back(de);
  }

  if( settings->value("actions/execprog").value<bool>() )
  {
    commandString cmd(settings->value("actions/program").value<QString>());

    if( cmd.isValid() )
    {
      QFileInfo proc_path(cmd.getPathToBinary());

      if( proc_path.isExecutable() )
      {
        startProcessEvent * procEvent = new startProcessEvent();

        procEvent->setPathToBinary(proc_path.absoluteFilePath());
        procEvent->setParameters(cmd.getParameters());

        QObject::connect(this, SIGNAL(switchedOn()), procEvent, SLOT(start()));
        ev.push_back(procEvent);
      }
      else
      {
        qDebug() << "File is not executable: " << proc_path.absoluteFilePath();
      }
    }
    else
    {
      qDebug() << "Cannot parse command string: " << cmd.getPathToBinary() << cmd.getParameters();
    }
  }

#ifdef WITH_PHONON
  if( settings->value("actions/audio").value<bool>() )
  {
    QFileInfo audio_path(settings->value("actions/audiofile").value<QString>());

    if( audio_path.isReadable() )
    {
      audioEvent * playAudioEvent = new audioEvent();

      playAudioEvent->setAudioFile(audio_path.absoluteFilePath());

      QObject::connect(this, SIGNAL(switchedOn()),  playAudioEvent, SLOT(start()));
      QObject::connect(this, SIGNAL(switchedOff()), playAudioEvent, SLOT(stop()));
      ev.push_back(playAudioEvent);
    }
    else
    {
      qDebug() << "File is not readable: " << audio_path.absoluteFilePath();
    }
  }
#endif
}
