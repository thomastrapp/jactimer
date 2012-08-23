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


#include "timerWindow.h"
#include "ui_timerWindow.h"

#include <QDebug>

timerWindow::timerWindow(QSharedPointer<configuredTimer> t, QSharedPointer<QSettings> settings, QWidget *parent)
  : QMainWindow(parent, ( Qt::Window | Qt::CustomizeWindowHint | Qt::FramelessWindowHint )),
    ui(new Ui::timerWindow),
    tDisplay(settings->value("timer/delay").value<uint>(), settings->value("appearance/numdigits").value<uint>(), this),
    grip(this),
    timer(t),
    sett(settings),
    trayIcon((clockTray*)0),
    menu(new timerMenu(timer, this))
{
  ui->setupUi(this);

  tDisplay.setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  tDisplay.setCursor(Qt::OpenHandCursor);
  ui->hlAll->insertWidget(0, &tDisplay);

  ui->vlGrip->addWidget(&grip, 0, Qt::AlignBottom | Qt::AlignRight);

  setWindowTitle(tr("JACT"));
  setWindowIcon(QPixmap(":/icons/title_icon.png"));
  if( sett->value("window/saveposition", false).value<bool>() && sett->value("window/geometry").isValid() )
    restoreGeometry(sett->value("window/geometry").toByteArray());

  connectTimer();

  if( QSystemTrayIcon::isSystemTrayAvailable() && sett->value("window/usetray").value<bool>() )
  {
    buildSystemTray();
  }

  applySettings();
}

timerWindow::~timerWindow()
{
  qDebug() << "timerWindow going down";
  if( !isMaximized() && sett->value("window/saveposition").value<bool>() )
  {
    sett->setValue("window/geometry", saveGeometry());
  }
  delete ui;
}

void timerWindow::onShowSettings()
{
  emit showSettings();
}

void timerWindow::onSettingsChanged()
{
  applySettings();
}

void timerWindow::showAboutDialog()
{
  aboutJACTDialog aboutWindow(this);
  aboutWindow.exec();
}

void timerWindow::applyWindowFlags()
{
  Qt::WindowFlags fl = 0;

  if( sett->value("appearance/hidetaskbar").value<bool>() && sett->value("window/alwaysontop").value<bool>() )
    fl = Qt::Tool;
  else
    fl = Qt::Window;

  if( sett->value("window/alwaysontop").value<bool>() )
    fl |= Qt::WindowStaysOnTopHint;

  if( sett->value("appearance/hidedecorations").value<bool>() )
  {
    fl |= Qt::FramelessWindowHint;
    fl |= Qt::CustomizeWindowHint;
  }

  if( fl != windowFlags() )
  {
    setWindowFlags(fl);
    show();
  }
}

void timerWindow::showMedium()
{
  if( isMaximized() )
    showNormal();
  setGeometry(x(), y(), 400, 150);
}

void timerWindow::applySettings()
{
  applyWindowFlags();

  if( tDisplay.numDigits() != sett->value("appearance/numdigits").value<int>() )
  {
    tDisplay.setNumDigits(sett->value("appearance/numdigits").value<uint>());
    tDisplay.updateGeometry();
  }

  QString customStyleSheet = sett->value("appearance/stylesheet").value<QString>();

  if( !customStyleSheet.isEmpty() && styleSheet() != customStyleSheet )
  {
    setStyleSheet(customStyleSheet);
  }

  if( trayIcon != NULL )
  {
    trayIcon->setDelay(sett->value("timer/delay").value<uint>());
    trayIcon->setForegroundColor(sett->value("appearance/color/foreground").value<QString>());
    trayIcon->setBackgroundColor(sett->value("appearance/color/background").value<QString>());
    trayIcon->reset();
  }
}

void timerWindow::mouseReleaseEvent(QMouseEvent *)
{
  tDisplay.setCursor(Qt::OpenHandCursor);
}

void timerWindow::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
  {
    dragPosition = event->globalPos() - frameGeometry().topLeft();
    event->accept();
    tDisplay.setCursor(Qt::ClosedHandCursor);
  }
}

void timerWindow::mouseMoveEvent(QMouseEvent *event)
{
  if( event->buttons() & Qt::LeftButton )
  {
    move(event->globalPos() - dragPosition);
    event->accept();
  }
}

void timerWindow::onNewTimerIndex(uint togo)
{
  tDisplay.onNewTimerIndex(togo);
}

void timerWindow::quit()
{
  deleteLater();
  QApplication::instance()->quit();
}

void timerWindow::connectTimer()
{
  connect(timer.data(),    SIGNAL(newTimerIndex(uint)), this,         SLOT(onNewTimerIndex(uint)));
  connect(timer.data(),    SIGNAL(switchedOn()),        this,         SLOT(onTimerFinished()));
}

void timerWindow::toggleTimer(bool checked)
{
  if( checked )
  {
    timer->start();
  }
  else
  {
    timer->stop();
  }
}

void timerWindow::onTimerFinished()
{
  // Show system tray message
  if( QSystemTrayIcon::isSystemTrayAvailable() &&
      QSystemTrayIcon::supportsMessages() &&
      sett->value("actions/traymessage").value<bool>() &&
      trayIcon != (clockTray*)0)
  {
    trayIcon->showMessage(tr("Timer finished"), sett->value("timer/message").value<QString>(), QSystemTrayIcon::Information);
  }
  QApplication::alert(this);
}

void timerWindow::onTrayIconClicked(QSystemTrayIcon::ActivationReason ar)
{
  switch( ar )
  {
    // Show window if user activates tray icon
    case QSystemTrayIcon::Unknown:
    case QSystemTrayIcon::DoubleClick:
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::MiddleClick:
    {
      // If window minimized or out of focus
      if( isHidden() )
      {
        // bring to focus
        show();
      }
      else
      {
        hide();
      }
    }
    break;
    // Context Menu
    case QSystemTrayIcon::Context:
    default:
      break;
  }
}

void timerWindow::contextMenuEvent(QContextMenuEvent *event)
{
  menu->exec(event->globalPos());
}

void timerWindow::buildSystemTray()
{
  trayIcon = new clockTray(this);
  trayIcon->setDelay(timer->getDelay());

  if( sett->value("window/animatetray").value<bool>() )
    connect(timer.data(), SIGNAL(newTimerIndex(uint)),
            trayIcon,     SLOT(onNewTimerIndex(uint)));

  // Click on tray icon brings application to foreground/background
  connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
          this,     SLOT(onTrayIconClicked(QSystemTrayIcon::ActivationReason)));

  connect(timer.data(), SIGNAL(timerReset()), trayIcon, SLOT(reset()));

  trayIcon->setContextMenu(menu);
  trayIcon->show();
}

void timerWindow::changeEvent(QEvent *event)
{
  QMainWindow::changeEvent(event);
  switch(event->type())
  {
    case QEvent::LanguageChange:
      ui->retranslateUi(this);
      break;
    default:
      break;
  }
}

