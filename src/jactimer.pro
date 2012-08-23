# -------------------------------------------------
# Project created by QtCreator 2010-03-01T15:30:36
# -------------------------------------------------

CONFIG += with_phonon \
    release
CONFIG(debug, debug|release):TARGET = jactimer_debug
else { 
    TARGET = jactimer
    DEFINES += QT_NO_DEBUG_OUTPUT
}

SOURCES += main.cpp \
    settingsDialog.cpp \
    timedTrigger.cpp \
    dialogEvent.cpp \
    timeDisplay.cpp \
    event.cpp \
    timerMenu.cpp \
    clockTray.cpp \
    configuredTimer.cpp \
    signalingTimer.cpp \
    timerApp.cpp \
    timerWindow.cpp \
    startProcessEvent.cpp \
    commandString.cpp \
    startProcessEventTester.cpp \
    aboutJACT.cpp \
    aboutJACTDialog.cpp
HEADERS += timedTrigger.h \
    settingsDialog.h \
    event.h \
    dialogEvent.h \
    clockTime.h \
    timeDisplay.h \
    events.h \
    timerMenu.h \
    clockTray.h \
    timerApp.h \
    configuredTimer.h \
    signalingTimer.h \
    timerWindow.h \
    startProcessEvent.h \
    commandString.h \
    startProcessEventTester.h \
    aboutJACT.h \
    aboutJACTDialog.h
FORMS += settingsDialog.ui \
    timerWindow.ui \
    aboutJACT.ui \
    aboutJACTDialog.ui
RESOURCES += buttons.qrc \
    icons.qrc
TRANSLATIONS = translations/timer_de.ts \
  translations/timer_de_DE.ts
win32 {
  RC_FILE = timer.rc
}

with_phonon { 
    QT += phonon
    DEFINES += WITH_PHONON
    SOURCES += audioEvent.cpp \
        audioEventTester.cpp
    HEADERS += audioEvent.h \
        audioEventTester.h
}
OTHER_FILES += timer.rc \
    jact_icon.ico \
    freedesktop/jactimer.xpm \
    freedesktop/jactimer.png \
    freedesktop/jactimer.desktop

target.path = /usr/bin
INSTALLS += target 

translation.path = /usr/share/jactimer/locale
translation.files = translations/timer_*.qm

INSTALLS += translation

icon_low.path = /usr/share/pixmaps
icon_low.files = freedesktop/jactimer.xpm

INSTALLS += icon_low

icon_high.path = /usr/share/icons/hicolor/48x48/apps
icon_high.files = freedesktop/jactimer.png

INSTALLS += icon_high

desktop_file.path = /usr/share/applications
desktop_file.files = freedesktop/jactimer.desktop

INSTALLS += desktop_file
