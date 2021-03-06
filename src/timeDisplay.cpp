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


#include "timeDisplay.h"
#include <QDebug>

timeDisplay::timeDisplay(uint seconds, uint numDigits, QWidget *parent)
  : QLCDNumber(numDigits, parent),
    s(seconds)
{
  setSegmentStyle(Flat);
  onNewTimerIndex(s);
}

timeDisplay::~timeDisplay()
{
  qDebug() << "timeDisplay going down";
}

void timeDisplay::onNewTimerIndex(uint togo)
{
  s = togo;

  const clockTime ct(s);
  const QString ctStr =
    (ct.h < 10 ? "0" : "" ) % QString::number(ct.h) % ":" %
    (ct.m < 10 ? "0" : "" ) % QString::number(ct.m) % ":" %
    (ct.s < 10 ? "0" : "" ) % QString::number(ct.s);

  display(ctStr);
}
