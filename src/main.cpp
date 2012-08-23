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


#include <iostream>

#include <QtGui/QApplication>
#include <QTranslator>

#include "timerApp.h"

int main(int argc, char *argv[])
{
  try
  {
    QApplication a(argc, argv);

    a.setOrganizationName("jactimer");
    a.setOrganizationDomain("jactimer.org");
    a.setApplicationName("jactimer");
    a.setQuitOnLastWindowClosed(false);

#ifndef Q_OS_LINUX
    QApplication::addLibraryPath(QCoreApplication::applicationDirPath() + "/plugins");
#endif

    QString locale = QLocale::system().name();

    QTranslator translator;

#ifndef Q_OS_LINUX
    translator.load(QString("timer_") + locale);
#else
    translator.load("timer_" + locale, "/usr/share/jactimer/locale/");
#endif

    a.installTranslator(&translator);

    timerApp timer;

    timer.run();

    return a.exec();
  }
  catch(const std::exception& e)
  {
    std::cout << "Uncaught exception: " << e.what();
  }
  catch(...)
  {
    std::cout << "Uncaught and unknown exception";
  }

  return 1;
}
