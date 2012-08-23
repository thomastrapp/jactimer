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


#ifndef CLOCKTRAY_H
#define CLOCKTRAY_H

#include <QColor>
#include <QIcon>
#include <QPainter>
#include <QPixmap>
#include <QPoint>
#include <QSystemTrayIcon>

#include <QDebug>

class clockTray : public QSystemTrayIcon
{
  Q_OBJECT
  static const uint iconSize = 22;

public:
  clockTray(QObject * parent = 0);
  ~clockTray();

  void setDelay(uint d);
  uint getDelay() const { return delay; }
  void showFractionFor(uint i);
  void setBackgroundColor(const QColor& col);
  void setForegroundColor(const QColor& col);

public slots:
  void reset();
  void onNewTimerIndex(uint togo);

private:
  uint delay;
  QPixmap pixm;
  QPainter p;
  QColor bgColor;
  QColor fgColor;
};

#endif // CLOCKTRAY_H
