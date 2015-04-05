#-------------------------------------------------
#
# Project created by QtCreator 2015-03-13T19:24:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WebCap
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    PdfToPixmap.cpp \
    PdfPreview.cpp \
    AboutWindow.cpp \
    HtmlToPdf.cpp \
    ClipboardMonitor.cpp \
    IniFile.cpp \
    PreferencesWindow.cpp \
    GraphicsView.cpp

HEADERS  += MainWindow.hpp \
    PdfToPixmap.hpp \
    easylogging++.h \
    PdfPreview.hpp \
    AboutWindow.hpp \
    HtmlToPdf.hpp \
    ClipboardMonitor.hpp \
    IniFile.hpp \
    PreferencesWindow.hpp \
    Version.hpp \
    GraphicsView.hpp

FORMS    += MainWindow.ui \
    AboutWindow.ui \
    PreferencesWindow.ui

LIBS += -LD:/Dev/msys64/mingw64/bin -lpoppler-qt5-1

LIBS += -LD:/Dev/msys64/opt/wkhtmltopdf/bin -lwkhtmltox

CONFIG += c++11

INCLUDEPATH += D:/Dev/msys64/opt/wkhtmltopdf/include

RC_ICONS = ../res/34423_tango_camera_photo.ico
