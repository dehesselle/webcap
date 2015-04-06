/* WebCap - capture websites to PDF files
 * Copyright (C) 2015 René de Hesselle <dehesselle@web.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "PdfPreview.hpp"
#include "HtmlToPdf.hpp"
#include "IniFile.hpp"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QListWidgetItem>
#include <QString>
#include <QResizeEvent>
#include <QProgressBar>

#ifdef Q_OS_WIN32
#include <QtWinExtras/QWinTaskbarProgress>
#endif

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   explicit MainWindow(QWidget *parent = 0);
   ~MainWindow();

   static const char *INI_GEOMETRY;
   static const char *INI_SPLITTER;
   static const IniFile::KeyValue INI_AUTO_PREVIEW;

protected:
   void resizeEvent(QResizeEvent *event);

private slots:
   void on_actionFileSettings_triggered();
   void on_actionFileShowInExplorer_triggered();
   void on_actionFileQuit_triggered();
   void on_actionHelpAboutQt_triggered();
   void on_actionHelpAboutWebCap_triggered();
   void on_documentList_itemDoubleClicked(QListWidgetItem *item);
   void on_documentList_itemSelectionChanged();

   /* reminder: don't use on_xxx_yyy syntax if you don't want to use
    * auto-connect from QMetaObject
    */
   void onClipboardMonitorUrlCaptured(const QString &url);
   void onHtmlToPdfPdfCreated(HtmlToPdf *htmlToPdf);
   void onHtmlToPdfProgressChanged(int percent);

private:
   Ui::MainWindow *ui;

   void previewDocument(const QString &file);

   QGraphicsScene *m_scene;
   PdfPreview     *m_pdfPreview;
   IniFile         m_settings;

   QProgressBar *m_progressBar;
#ifdef Q_OS_WIN32
   QWinTaskbarProgress *m_progressTaskbar;
#endif
};

#endif // MAINWINDOW_HPP
