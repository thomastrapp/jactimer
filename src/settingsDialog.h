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


#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <cmath>

#include <QColor>
#include <QColorDialog>
#include <QPalette>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QSharedPointer>
#include <QSettings>
#include <QSystemTrayIcon>

#include "clockTime.h"
#include "aboutJACT.h"

#include "startProcessEventTester.h"
#ifdef WITH_PHONON
  #include <phonon>
  #include "audioEventTester.h"
#endif

namespace Ui {
    class settingsDialog;
}

class settingsDialog : public QDialog
{
  Q_OBJECT
  
public:
  settingsDialog(QSharedPointer<QSettings> s, QWidget *parent = 0);
  ~settingsDialog();

  void showCreateSettings();
  void showEditSettings();

public slots:
  void runAudioFileDialog();
  void runBinaryDialog();

  void onTimeInputChanged();
  void onSliderValueChanged();

  void onFinished(int result);
  void chkPlayAudioFileToggled(bool on);
  void chkExecProgToggled(bool on);
  void startProgClicked();
#ifdef WITH_PHONON
  void playAudioToggled(bool on);
#endif
  void setBackgroundColor();
  void setForegroundColor();

signals:
  void settingsChanged();
  
protected:
  void changeEvent(QEvent *e);

private:
  settingsDialog(const settingsDialog&);
  settingsDialog& operator=(const settingsDialog&);

  void connectSignals();
  void showSettings(bool edit_mode);
  void fillInSettings(bool edit_mode);
  void sync();

  void blockSliders();
  void unblockSliders();
  void blockTimeInput();
  void unblockTimeInput();

  uint getSecondsFromTimeInput() const;
  uint getSecondsFromSliders() const;

  Ui::settingsDialog *ui;
  QSharedPointer<QSettings> settings;

  aboutJACT about;

  startProcessEventTester processTester;
#ifdef WITH_PHONON
  audioEventTester audioTester;
#endif
};

#endif // SETTINGSDIALOG_H
