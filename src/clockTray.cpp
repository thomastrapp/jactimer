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


#include "clockTray.h"
#include <QDebug>

clockTray::clockTray(QObject * parent)
  : QSystemTrayIcon(parent),
    delay(0),
    pixm(clockTray::iconSize, clockTray::iconSize),
    bgColor(Qt::black),
    fgColor(Qt::white)
{
  reset();
}

clockTray::~clockTray()
{
  qDebug() << "clockTray going down";
}

void clockTray::reset()
{
  if( p.isActive() )
    p.end();

  pixm.fill(Qt::transparent);

  p.begin(&pixm);

  p.setRenderHint(QPainter::Antialiasing);
  p.setPen(Qt::NoPen);
  p.setBrush(QColor(bgColor));

  const int halfIconSize = 0.5 * clockTray::iconSize;
  p.drawEllipse(QPoint(halfIconSize, halfIconSize), halfIconSize, halfIconSize);

  p.setBrush(QColor(fgColor));

  setIcon(pixm);
}

void clockTray::setForegroundColor(const QColor& col)
{
  fgColor = col;
}

void clockTray::setBackgroundColor(const QColor& col)
{
  bgColor = col;
}

void clockTray::setDelay(uint d)
{
  delay = d;
}

void clockTray::showFractionFor(uint i)
{
  Q_ASSERT(delay != 0);

  if( delay != 0 )
  {
    p.drawPie(2, 2, clockTray::iconSize - 4, clockTray::iconSize - 4, 90 * 16, static_cast<int>(delay - i) * -16 * 360 / static_cast<int>(delay));
    setIcon(pixm);
  }
}

void clockTray::onNewTimerIndex(uint togo)
{
  if( togo < 1 )
  {
    reset();
  }
  else
  {
    showFractionFor(togo);
  }
}
