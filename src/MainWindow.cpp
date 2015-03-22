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
#include "PreferencesWindow.hpp"
#include <QFileInfo>
#include <QInputDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QColor>
#include <QMessageBox>

#define ELPP_QT_LOGGING
#define ELPP_THREAD_SAFE
#define ELPP_NO_DEFAULT_LOG_FILE
#include <easylogging++.h>

const char *MainWindow::INI_GEOMETRY = "MainWindow/geometry";
const char *MainWindow::INI_SPLITTER = "MainWindow/splitter";
const IniFile::KeyValue MainWindow::INI_AUTO_PREVIEW = {
   "MainWindow/autoPreview", "false"
};

MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow),
   m_scene(0)
{
   // basic UI setup
   {
      ui->setupUi(this);

      ui->statusBar->showMessage("Welcome   :-)", 5000);

      // bug in Qt Windows port: QKeySequence::Quit is empty
      ui->actionFileQuit->setShortcut(QKeySequence(Qt::ALT + Qt::Key_F4));

      // restore windows position and splitter layout
      restoreGeometry(m_settings.value(INI_GEOMETRY, saveGeometry()).toByteArray());
      ui->splitter->restoreState(m_settings.value(INI_SPLITTER, ui->splitter->saveState()).toByteArray());

      // hide toolbar and prevent unhiding it
      ui->mainToolBar->hide();
      setContextMenuPolicy(Qt::NoContextMenu);   // seems to be the only way
   }

   // setup PDF preview / graphics scene
   {
      m_pdfPreview = new PdfPreview;
      m_scene = new QGraphicsScene(this);
      ui->previewPicture->setScene(m_scene);
   }

   // setup clipboard monitor
   {
      ClipboardMonitor *clipboardMonitor = new ClipboardMonitor(this);

      connect(clipboardMonitor, &ClipboardMonitor::clipboardChanged,
              this, &MainWindow::on_clipboardChanged);

      QDir dir = m_settings.value(HtmlToPdf::INI_PDF_DIR).toString();

      if (dir.exists())
      {
         clipboardMonitor->enable();
      }
      else
      {
         m_settings.setValue(HtmlToPdf::INI_PDF_DIR);
         QMessageBox::critical(
                  this, "invalid settings",
                  QString("The configured target directory does not exist.\n") +
                  "The configuration has been reset to a default value.");
      }
   }

   // setup HTML-to-PDF converter
   {
      HtmlToPdf::initLib();   // initialize 3rd-party library (wkhtmltopdf)

      /* Right now we're not storing the proxy password anywhere and it
       * gets lost once you quit the application. If we detect that
       * a proxy should be used during startup, ask for the password.
       */
      if (m_settings.value(HtmlToPdf::INI_PROXY_ENABLE).toBool())
      {
         bool clickedOk = false;

         QString password = QInputDialog::getText(
                  this,
                  "password required",
                  "Please enter the proxy password for user " +
                  m_settings.value(HtmlToPdf::INI_PROXY_USER).toString() +
                  + ".\n" + "Click cancel to disable proxy.",
                  QLineEdit::Password,
                  QString(),
                  &clickedOk);

         if (clickedOk)
            HtmlToPdf::setProxyPassword(password);
         else
            m_settings.setValue(HtmlToPdf::INI_PROXY_ENABLE, false);
      }
   }

   // populate the document list with pre-existing PDF files
   {
      QDir dir(m_settings.value(HtmlToPdf::INI_PDF_DIR).toString());
      dir.setFilter(QDir::Files);

      QFileInfoList fileList = dir.entryInfoList();

      for (int i=0; i< fileList.size(); ++i)
      {
         QFileInfo file = fileList.at(i);

         if (file.suffix().toLower() == "pdf")
         {
            QListWidgetItem *item = new QListWidgetItem(file.fileName(), ui->documentList);
            if (file.size())
               item->setForeground(Qt::gray);
            else
               item->setForeground(QColor(Qt::red).lighter());
         }
      }
   }
}

MainWindow::~MainWindow()
{
   // save window position and layout
   {
      m_settings.setValue(INI_GEOMETRY, saveGeometry());
      m_settings.setValue(INI_SPLITTER, ui->splitter->saveState());
   }

   HtmlToPdf::deinitLib();

   delete m_pdfPreview;
   delete ui;
}

void MainWindow::on_pdfCreated(HtmlToPdf *htmlToPdf)
{
   QFileInfo file = htmlToPdf->getOutFile();
   QListWidgetItem *item = new QListWidgetItem(file.fileName(),
                                               ui->documentList);

   if (file.exists() and file.size())
   {
      if (m_settings.value(INI_AUTO_PREVIEW).toBool())
      {
         ui->documentList->scrollToBottom();
         ui->documentList->setCurrentItem(item);
      }

      ui->statusBar->showMessage("creating preview");
      ui->statusBar->showMessage(file.fileName() + ", " +
                                 QString::number(file.size()) + " bytes");
   }
   else
   {
      item->setForeground(QColor(Qt::red).lighter());
      ui->statusBar->showMessage("error: " + file.fileName() + ".", 5000);
      LOG(ERROR) << htmlToPdf->getUrl() << ", "
                 << file.absoluteFilePath()
                 << " (" << file.size() << " bytes)";
   }

   htmlToPdf->deleteLater();
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
   ui->statusBar->clearMessage();

   QListWidgetItem *item = ui->documentList->currentItem();
   QFileInfo file = m_settings.value(HtmlToPdf::INI_PDF_DIR).toString() +
         "/" + item->text();

   if (item->foreground() == QColor(Qt::red).lighter())
   {
      ui->statusBar->showMessage("error: " + file.fileName() + ".", 5000);
   }
   else
   {
      ui->statusBar->showMessage(file.fileName() + ", " +
                                 QString::number(file.size()) + " bytes");
      previewDocument(file.absoluteFilePath());
   }
}

void MainWindow::on_clipboardChanged(const QString &url)
{
   ui->statusBar->showMessage("capture: " + url);

   HtmlToPdf *htmlToPdf = new HtmlToPdf(this);
   htmlToPdf->setUrl(url);

   connect(htmlToPdf, &HtmlToPdf::pdfCreated, this, &MainWindow::on_pdfCreated);
   /* This was meant to be run as a thread. wkhtmltopdf does not support
    * to be run inside a thread right now. If that changes, we only
    * need to call start() instead of run() below.
    */
   htmlToPdf->run();   // run in main thread
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
   QDesktopServices::openUrl(QUrl("file:///" +
         m_settings.value(HtmlToPdf::INI_PDF_DIR).toString()));
}

void MainWindow::on_documentList_itemDoubleClicked(QListWidgetItem *item)
{
   ui->statusBar->clearMessage();

   QFileInfo pdf = m_settings.value(HtmlToPdf::INI_PDF_DIR).toString() +
         "/" + item->text();

   if (pdf.exists())
      QDesktopServices::openUrl(QUrl("file:///" + pdf.absoluteFilePath()));
   else
      LOG(ERROR) << "file not found: " << pdf.absoluteFilePath();
}
