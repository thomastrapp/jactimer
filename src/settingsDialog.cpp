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


#include "settingsDialog.h"
#include "ui_settingsDialog.h"
#include "ui_aboutJACT.h"
#include <QDebug>

settingsDialog::settingsDialog(QSharedPointer<QSettings> s, QWidget * parent)
  : QDialog(parent),
    ui(new Ui::settingsDialog),
    settings(s),
    about(this),
    processTester(this)
#ifdef WITH_PHONON
    , audioTester(this)
#endif
{
  ui->setupUi(this);

  QFont mono_font("Monospace");
  mono_font.setStyleHint(QFont::TypeWriter);

  ui->lbBackgroundColor->setFont(mono_font);
  ui->lbForegroundColor->setFont(mono_font);

  ui->vl_about->addWidget(&about);

  setWindowIcon(QPixmap(":/icons/title_icon.png"));

  connectSignals();
}

settingsDialog::~settingsDialog()
{
  qDebug() << "settingsDialog going down";
  delete ui;
}

void settingsDialog::connectSignals()
{
  // File dialogs
#ifdef WITH_PHONON
  connect(ui->pbBrowseAudioFile, SIGNAL(clicked()),         this, SLOT(runAudioFileDialog()));
#endif
  connect(ui->pbBrowseBin,       SIGNAL(clicked()),         this, SLOT(runBinaryDialog()));

  // Color dialogs
  connect(ui->pbPickBackgroundColor, SIGNAL(clicked()), this, SLOT(setBackgroundColor()));
  connect(ui->pbPickForegroundColor, SIGNAL(clicked()), this, SLOT(setForegroundColor()));

  // Sliders and Spin Boxes need to be in sync
  connect(ui->sbHours,   SIGNAL(valueChanged(int)), this, SLOT(onTimeInputChanged()));
  connect(ui->sbMinutes, SIGNAL(valueChanged(int)), this, SLOT(onTimeInputChanged()));
  connect(ui->sbSeconds, SIGNAL(valueChanged(int)), this, SLOT(onTimeInputChanged()));

  connect(ui->slHours,   SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged()));
  connect(ui->slMinutes, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged()));
  connect(ui->slSeconds, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged()));

  // Interface logic
#ifdef WITH_PHONON
  connect(ui->chkPlayAudioFile, SIGNAL(toggled(bool)), this, SLOT(chkPlayAudioFileToggled(bool)));
#endif
  connect(ui->chkExecProg,      SIGNAL(toggled(bool)), this, SLOT(chkExecProgToggled(bool)));

  // Test buttons
  connect(ui->pbTestProg,  SIGNAL(clicked()),     this, SLOT(startProgClicked()));
#ifdef WITH_PHONON
  connect(ui->pbTestAudio, SIGNAL(toggled(bool)), this, SLOT(playAudioToggled(bool)));
#endif

  connect(this, SIGNAL(finished(int)), this, SLOT(onFinished(int)));
}

void settingsDialog::setBackgroundColor()
{
  QColor color = QColorDialog::getColor(ui->lbBackgroundColor->text(), this);
  if (color.isValid())
  {
    ui->lbBackgroundColor->setText(color.name());
    ui->lbBackgroundColor->setPalette(QPalette(color));
    ui->lbBackgroundColor->setAutoFillBackground(true);
  }
}

void settingsDialog::setForegroundColor()
{
  QColor color = QColorDialog::getColor(ui->lbForegroundColor->text(), this);
  if (color.isValid())
  {
    ui->lbForegroundColor->setText(color.name());
    ui->lbForegroundColor->setPalette(QPalette(color));
    ui->lbForegroundColor->setAutoFillBackground(true);
  }
}

void settingsDialog::onFinished(int result)
{
#ifdef WITH_PHONON
  if( ui->pbTestAudio->isChecked() )
    ui->pbTestAudio->setChecked(false);
#endif

  sync();
  if( result == QDialog::Accepted )
  {
    emit settingsChanged();
  }
}

void settingsDialog::showCreateSettings()
{
  showSettings(false);
}

void settingsDialog::showEditSettings()
{
  showSettings(true);
}

void settingsDialog::showSettings(bool edit_mode)
{
  fillInSettings(edit_mode);
  show();
}

void settingsDialog::runAudioFileDialog()
{
  QString fileName = QFileDialog::getOpenFileName(this,
                                                  tr("Select audio file"),
                                                  ui->leAudioFile->text(),
                                                  tr("Audio (*);;Any file (*)"));

  if( !fileName.isEmpty() )
  {
    QFileInfo audioFileInfo(fileName);

    if( audioFileInfo.isReadable() )
    {
      ui->leAudioFile->setText(audioFileInfo.absoluteFilePath());
    }
    else
    {
      QMessageBox::critical(this, tr("Error"), tr("Selected file is not readable"));
    }
  }
}

void settingsDialog::runBinaryDialog()
{
  QString fileName = QFileDialog::getOpenFileName(this,
                                                  tr("Select executable file"),
                                                  ui->leExecProg->text(),
                                                  tr("Executable files (*);;Any file (*)"));

  if( !fileName.isEmpty() )
  {
    QFileInfo bin_info(fileName);

    if( bin_info.isExecutable() )
    {
      ui->leExecProg->setText("\"" + bin_info.absoluteFilePath() + "\"");
    }
    else
    {
      QMessageBox::critical(this, tr("Error"), tr("Selected file is not executable"));
    }
  }
}

void settingsDialog::blockSliders()
{
  ui->slSeconds->blockSignals(true);
  ui->slMinutes->blockSignals(true);
  ui->slHours->blockSignals(true);
}

void settingsDialog::unblockSliders()
{
  ui->slSeconds->blockSignals(false);
  ui->slMinutes->blockSignals(false);
  ui->slHours->blockSignals(false);
}

void settingsDialog::blockTimeInput()
{
  ui->sbSeconds->blockSignals(true);
  ui->sbMinutes->blockSignals(true);
  ui->sbHours->blockSignals(true);
}

void settingsDialog::unblockTimeInput()
{
  ui->sbSeconds->blockSignals(false);
  ui->sbMinutes->blockSignals(false);
  ui->sbHours->blockSignals(false);
}

void settingsDialog::onSliderValueChanged()
{
  const clockTime ct(getSecondsFromSliders());

  // Temporarily block signals on QSpinBoxes
  // to avoid emission of valueChanged()
  // while updating QSpinboxes
  blockTimeInput();

  ui->sbSeconds->setValue(ct.s);
  ui->sbMinutes->setValue(ct.m);
  ui->sbHours->setValue(ct.h);

  unblockTimeInput();
}

void settingsDialog::onTimeInputChanged()
{
  const clockTime ct(getSecondsFromTimeInput());

  // Temporarily block signals on sliders
  // to avoid emission of valueChanged()
  blockSliders();

  ui->slSeconds->setValue(ct.s);
  ui->slMinutes->setValue(ct.m);
  ui->slHours->setValue(ct.h);

  unblockSliders();
}

uint settingsDialog::getSecondsFromTimeInput() const
{
  return ui->sbHours->value()   * 3600
       + ui->sbMinutes->value() * 60
       + ui->sbSeconds->value();
}

uint settingsDialog::getSecondsFromSliders() const
{
  return ui->slHours->value()   * 3600
       + ui->slMinutes->value() * 60
       + ui->slSeconds->value();
}

void settingsDialog::changeEvent(QEvent *e)
{
  QDialog::changeEvent(e);
  switch( e->type() )
  {
    case QEvent::LanguageChange:
      ui->retranslateUi(this);
      break;
    default:
      break;
  }
}

void settingsDialog::chkExecProgToggled(bool on)
{
  ui->leExecProg->setEnabled(on);
  ui->pbBrowseBin->setEnabled(on);
  ui->pbTestProg->setEnabled(on);
}

void settingsDialog::startProgClicked()
{
  processTester.startProcess(ui->leExecProg->text());
}

void settingsDialog::chkPlayAudioFileToggled(bool on)
{
  ui->leAudioFile->setEnabled(on);
  ui->pbBrowseAudioFile->setEnabled(on);

  ui->pbTestAudio->setEnabled(on);

  if( ui->pbTestAudio->isChecked() )
    ui->pbTestAudio->setChecked(false);
}

#ifdef WITH_PHONON
void settingsDialog::playAudioToggled(bool on)
{
  if( on )
  {
    ui->pbTestAudio->setText(tr("Playing.."));
    if( !audioTester.play(ui->leAudioFile->text()) )
    {
      playAudioToggled(false);
      ui->pbTestAudio->setChecked(false);
    }
  }
  else
  {
    ui->pbTestAudio->setText(tr("Test"));
    audioTester.stop();
  }
}
#endif

void settingsDialog::fillInSettings(bool edit_mode)
{
  const uint seconds = settings->value("timer/delay", 10U).value<uint>();
  const clockTime ct(seconds);

  // Time input: spinboxes
  ui->sbHours->setValue(ct.h);
  ui->sbMinutes->setValue(ct.m);
  ui->sbSeconds->setValue(ct.s);

  // Time input: sliders
  ui->slHours->setValue(ct.h);
  ui->slMinutes->setValue(ct.m);
  ui->slSeconds->setValue(ct.s);

  ui->chkAlwaysOnTop->setChecked(   settings->value("window/alwaysontop", true).value<bool>());
  ui->chkUseTray->setChecked(       settings->value("window/usetray", true).value<bool>());

  if( QSystemTrayIcon::isSystemTrayAvailable() )
  {
    ui->chkAnimateTray->setChecked(   settings->value("window/animateTray", true).value<bool>());
  }
  else
  {
    ui->chkAnimateTray->setChecked(false);
    ui->chkAnimateTray->setEnabled(false);
  }

  ui->chkWindowSizePos->setChecked( settings->value("window/saveposition", true).value<bool>());

  ui->chkStartTimer->setChecked(   settings->value("timer/start", false).value<bool>());
  ui->chkShowSettings->setChecked( settings->value("timer/showsettings", true).value<bool>());

  ui->chkShowDialog->setChecked(      settings->value("actions/dialog", true).value<bool>());
  ui->chkShowTrayMessage->setChecked( settings->value("actions/traymessage", true).value<bool>());
  ui->chkExecProg->setChecked(        settings->value("actions/execprog", false).value<bool>());
  chkExecProgToggled(                 settings->value("actions/execprog", false).value<bool>());
  ui->leExecProg->setText(            settings->value("actions/program", "").value<QString>());

#ifdef WITH_PHONON
  ui->chkPlayAudioFile->setChecked( settings->value("actions/audio", false).value<bool>());
  chkPlayAudioFileToggled(          settings->value("actions/audio", false).value<bool>());
  ui->leAudioFile->setText(         settings->value("actions/audiofile", "").value<QString>());
#else
  ui->chkPlayAudioFile->setChecked(false);
  chkPlayAudioFileToggled(false);
  ui->chkPlayAudioFile->setEnabled(false);
  ui->pbTestAudio->setEnabled(false);
#endif

  ui->txtMessage->setPlainText(settings->value("timer/message", tr("Your timer has finished")).value<QString>());

  const QColor bg_color(settings->value("appearance/color/background", "black").value<QString>());
  ui->lbBackgroundColor->setText(bg_color.name());
  ui->lbBackgroundColor->setPalette(QPalette(bg_color));
  ui->lbBackgroundColor->setAutoFillBackground(true);

  const QColor color(settings->value("appearance/color/foreground", "white").value<QString>());
  ui->lbForegroundColor->setText(color.name());
  ui->lbForegroundColor->setPalette(QPalette(color));
  ui->lbForegroundColor->setAutoFillBackground(true);

  ui->chkHideDecorations->setChecked(settings->value("appearance/hidedecorations", true).value<bool>());
  ui->chkHideTaskbar->setChecked(settings->value("appearance/hidetaskbar", true).value<bool>());

  if( edit_mode )
  {
    if( QSystemTrayIcon::isSystemTrayAvailable() )
    {
      ui->gBoxSystemTray->setTitle(tr("System Tray (requires restart)"));
    }
    else
    {
      ui->gBoxSystemTray->setTitle(tr("System Tray (N/A)"));
    }
    this->setWindowTitle(tr("Edit preferences"));
  }
  else
  {
    if( QSystemTrayIcon::isSystemTrayAvailable() )
    {
      ui->gBoxSystemTray->setTitle(tr("System Tray"));
    }
    else
    {
      ui->gBoxSystemTray->setTitle(tr("System Tray (N/A)"));
    }
    this->setWindowTitle(tr("Create new timer"));
  }
}

void settingsDialog::sync()
{
  if( ui->chkPlayAudioFile->isChecked() && ui->leAudioFile->text().isEmpty() )
    ui->chkPlayAudioFile->setChecked(false);

  if( ui->chkExecProg->isChecked() && ui->leExecProg->text().isEmpty() )
    ui->chkExecProg->setChecked(false);

  settings->setValue("timer/delay",        getSecondsFromTimeInput());
  settings->setValue("timer/message",      ui->txtMessage->toPlainText());
  settings->setValue("timer/start",        ui->chkStartTimer->isChecked());
  settings->setValue("timer/showsettings", ui->chkShowSettings->isChecked());

  settings->setValue("actions/dialog",      ui->chkShowDialog->isChecked());
  settings->setValue("actions/traymessage", ui->chkShowTrayMessage->isChecked());
  settings->setValue("actions/audio",       ui->chkPlayAudioFile->isChecked());
  settings->setValue("actions/execprog",    ui->chkExecProg->isChecked());
  settings->setValue("actions/program",     ui->leExecProg->text());
  settings->setValue("actions/audiofile",   ui->leAudioFile->text());

  settings->setValue("window/alwaysontop",  ui->chkAlwaysOnTop->isChecked());
  settings->setValue("window/usetray",      ui->chkUseTray->isChecked());
  settings->setValue("window/saveposition", ui->chkWindowSizePos->isChecked());
  settings->setValue("window/animatetray",  ui->chkAnimateTray->isChecked());

  settings->setValue("appearance/color/background", ui->lbBackgroundColor->text());
  settings->setValue("appearance/color/foreground", ui->lbForegroundColor->text());

  settings->setValue("appearance/hidetaskbar",     ui->chkHideTaskbar->isChecked());
  settings->setValue("appearance/hidedecorations", ui->chkHideDecorations->isChecked());

  settings->setValue("appearance/stylesheet",
  "QMainWindow { "
    "background-color: " + settings->value("appearance/color/background", "black").value<QString>() + ";"
  "} "
  "QLCDNumber { "
    "border:none;"
    "color: " + settings->value("appearance/color/foreground", "white").value<QString>() + ";"
  "}");

  clockTime ct(settings->value("timer/delay").value<uint>());
  settings->setValue("appearance/numdigits", (ct.h > 0 ? 8U : (ct.m > 0 ? 5U : 2U)));

  settings->sync();
}
