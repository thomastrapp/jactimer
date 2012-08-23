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


#include "startProcessEventTester.h"

startProcessEventTester::startProcessEventTester(QWidget * parent)
  : parentWindow(parent)
{
}

bool startProcessEventTester::startProcess(const QString& prog)
{
  if( prog.isEmpty() )
  {
    QMessageBox::information(parentWindow, tr("Select a file"), tr("Please select an executable"));
    return false;
  }
  else
  {
    commandString cmd(prog);
    if( cmd.isValid() )
    {
      QFileInfo prog_info(cmd.getPathToBinary());

      if( prog_info.isExecutable() )
      {
        setPathToBinary(prog_info.absoluteFilePath());
        setParameters(cmd.getParameters());
        start();
        return true;
      }
      else
      {
        QMessageBox::critical(parentWindow, tr("Error"), tr("Selected file is not executable: %1").arg(prog_info.absoluteFilePath()));
        return false;
      }
    }
    else
    {
      QMessageBox::critical(parentWindow, tr("Error"), tr("Cannot parse command: %1").arg(prog));
      return false;
    }
  }
}
