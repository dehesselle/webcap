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

#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "AboutWindow.hpp"
#include "ClipboardMonitor.hpp"
#include <QFileInfo>
#include "PreferencesWindow.hpp"
#include <QInputDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>

#define ELPP_QT_LOGGING
#define ELPP_THREAD_SAFE
#define ELPP_NO_DEFAULT_LOG_FILE
#include <easylogging++.h>

const char *MainWindow::INI_GEOMETRY = "MainWindow/geometry";
const char *MainWindow::INI_SPLITTER = "MainWindow/splitter";

/*
 *
 * https://openclipart.org/detail/20295/Camera%20Lens
 * https://openclipart.org/detail/175844/spider%20web
 * https://openclipart.org/detail/85327/Halloween%20Spider%20Web%20Icon
 * https://openclipart.org/detail/189336/Camera%20Pictogram
 *
 *
 *
 *
 *
 *
 *
 */

MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow),
   m_scene(0)
{
   ui->setupUi(this);

   m_pdfPreview = new PdfPreview;
   m_scene = new QGraphicsScene(this);
   ui->previewPicture->setScene(m_scene);


   //hirse


//   HtmlToPdf *h = new HtmlToPdf(this);
//   h->setUrl("http://www.heise.de");

// connect(h, &HtmlToPdf::pdfCreated, this, &MainWindow::on_pdfCreated);





//   h->start();

//   previewDocument(pdfFile);

   // bug in Qt Windows port: QKeySequence::Quit is empty
   ui->actionFileQuit->setShortcut(QKeySequence(Qt::ALT + Qt::Key_F4));

   HtmlToPdf::initLib();

   ClipboardMonitor *cmon = new ClipboardMonitor(this);

   connect(cmon, &ClipboardMonitor::clipboardChanged, this, &MainWindow::on_clipboardChanged);

   cmon->enable();


   if (m_settings.value(HtmlToPdf::INI_PROXY_ENABLE).toBool())
   {
      bool clickedOk = false;

      QString password = QInputDialog::getText(this, "bla", "Enter the proxy password for user X and click ok. Click cancel to disable proxy.", QLineEdit::Password,QString(),&clickedOk);

      if (clickedOk)
      {
         HtmlToPdf::setProxyPassword(password);
      }
      else
      {
         m_settings.setValue(HtmlToPdf::INI_PROXY_ENABLE, false);
      }
   }

   ui->mainToolBar->hide();
   this->setContextMenuPolicy(Qt::NoContextMenu);

   ui->statusBar->showMessage("Welcome :)", 5000);

   {  // restore windows position and layout
      restoreGeometry(m_settings.value(INI_GEOMETRY, saveGeometry()).toByteArray());
      ui->splitter->restoreState(m_settings.value(INI_SPLITTER, ui->splitter->saveState()).toByteArray());
   }

   {  // populate the document list with pre-existing PDF files
      HtmlToPdf::initSettings();
      QDir dir(m_settings.value(HtmlToPdf::INI_PDF_DIR).toString());
      dir.setFilter(QDir::Files);

      QFileInfoList fileList = dir.entryInfoList();

      for (int i=0; i< fileList.size(); ++i)
      {
         QFileInfo file = fileList.at(i);

         if (file.suffix().toLower() == "pdf")
            new QListWidgetItem(file.fileName(), ui->documentList);
      }
   }
}

MainWindow::~MainWindow()
{
// TODO: How to delete graphicsscene?

   {  // save windows position and layout
      m_settings.setValue(INI_GEOMETRY, saveGeometry());
      m_settings.setValue(INI_SPLITTER, ui->splitter->saveState());
   }

   HtmlToPdf::deinitLib();

   delete m_pdfPreview;
   delete ui;
}

void MainWindow::on_pdfCreated(HtmlToPdf *htmlToPdf)
{
//   previewDocument(htmlToPdf->getOutFile());

   QFileInfo file = htmlToPdf->getOutFile();

   new QListWidgetItem(file.fileName(), ui->documentList);


   htmlToPdf->deleteLater();

   ui->documentList->scrollToBottom();
   ui->statusBar->showMessage("Created " + file.fileName() + ".", 5000);

}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);

   if (m_scene)
      ui->previewPicture->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::previewDocument(const QString& file)
{
   m_scene->clear();
   m_scene->addPixmap(m_pdfPreview->getPreview(file));

   ui->previewPicture->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::on_actionHelpAboutWebCap_triggered()
{
   AboutWindow *aboutWindow = new AboutWindow(this);
   aboutWindow->show();

}

void MainWindow::on_actionFileQuit_triggered()
{
   close();
}

void MainWindow::on_documentList_itemSelectionChanged()
{
   QListWidgetItem *item = ui->documentList->currentItem();

   previewDocument("D:/Temp/" + item->text());
}

void MainWindow::on_clipboardChanged(const QString &url)
{
   ui->statusBar->showMessage("Capturing " + url + "...");

   HtmlToPdf *htmlToPdf = new HtmlToPdf(this);
   htmlToPdf->setUrl(url);

   {  /* This was meant to be run as a thread. wkhtmltopdf does not support
       * to be run inside a thread right now. If that changes we only
       * need to call start() instead of run() below.
       */
      connect(htmlToPdf, &HtmlToPdf::pdfCreated, this, &MainWindow::on_pdfCreated);
      htmlToPdf->run();   // run in main thread
   }
}

void MainWindow::on_actionFileSettings_triggered()
{
   PreferencesWindow *preferencesWindow = new PreferencesWindow(this);

   preferencesWindow->setWindowFlags(preferencesWindow->windowFlags() &
                                     ~Qt::WindowContextHelpButtonHint);
   preferencesWindow->show();
}

void MainWindow::on_actionFileShowInExplorer_triggered()
{
   HtmlToPdf::initSettings();   // make sure we have these settings

   QDesktopServices::openUrl(QUrl("file:///" +
         m_settings.value(HtmlToPdf::INI_PDF_DIR).toString()));
}

void MainWindow::on_documentList_itemDoubleClicked(QListWidgetItem *item)
{
   QFileInfo pdf = m_settings.value(HtmlToPdf::INI_PDF_DIR).toString() +
         "/" + item->text();

   if (pdf.exists())
      QDesktopServices::openUrl(QUrl("file:///" + pdf.absoluteFilePath()));
   else
      LOG(ERROR) << "file not found: " << pdf.absoluteFilePath();
}
