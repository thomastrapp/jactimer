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


#ifndef STARTPROCESSEVENT_H
#define STARTPROCESSEVENT_H

#include <QDebug>
#include <QFileInfo>
#include <QProcess>
#include <QString>
#include <QStringBuilder>
#include <QStringList>

#include "commandString.h"
#include "event.h"

class startProcessEvent : public event
{
  Q_OBJECT

public:
  startProcessEvent();
  ~startProcessEvent();

  void setPathToBinary(const QString& binary);
  void setParameters(const QStringList& params);

public slots:
  void start();
  void stop();

private:
  QString pathToBinary;
  QStringList parameters;
};

#endif // STARTPROCESSEVENT_H
