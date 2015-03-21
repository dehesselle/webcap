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

#include "PreferencesWindow.hpp"
#include "ui_PreferencesWindow.h"
#include "HtmlToPdf.hpp"
#include <QString>
#include <QFileDialog>
#include <QMessageBox>

PreferencesWindow::PreferencesWindow(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::PreferencesWindow)
{
   ui->setupUi(this);

   readSettings();
}

PreferencesWindow::~PreferencesWindow()
{
   delete ui;
}

void PreferencesWindow::readSettings()
{
   {  // settings from HtmlToPdf
      HtmlToPdf::initSettings();   // make sure we have settings

      ui->targetDir->setText(m_settings.value(HtmlToPdf::INI_PDF_DIR).toString());
      ui->proxyEnable->setChecked(m_settings.value(HtmlToPdf::INI_PROXY_ENABLE).toBool());
      ui->proxyHost->setText(m_settings.value(HtmlToPdf::INI_PROXY_HOST).toString());
      ui->proxyPort->setText(m_settings.value(HtmlToPdf::INI_PROXY_PORT).toString());
      ui->proxyUser->setText(m_settings.value(HtmlToPdf::INI_PROXY_USER).toString());
      ui->proxyPassword->setText(HtmlToPdf::getProxyPassword());

   }

   on_proxyEnable_toggled(ui->proxyEnable->isChecked());
}

void PreferencesWindow::on_buttonTargetDir_clicked()
{
   ui->targetDir->setText(QFileDialog::getExistingDirectory(this, "select target directory"));
}

void PreferencesWindow::on_buttonOk_clicked()
{
    QDir targetDir = ui->targetDir->text();

    bool isOk = true;

    if (targetDir.exists())
    {
       m_settings.setValue(HtmlToPdf::INI_PDF_DIR, targetDir.absolutePath());
    }

    else
    {
       QMessageBox::critical(this, "invalid directory", "Directory WTF!");

       isOk = false;
    }

    if (isOk and ui->proxyEnable->isChecked())
    {
       if (ui->proxyHost->text().isEmpty() or
           ui->proxyPort->text().isEmpty() or
           ui->proxyUser->text().isEmpty() or
           ui->proxyPassword->text().isEmpty())
       {
          QMessageBox::critical(this, "proxy settings incomplete",
                                QString("If you want to use a proxy, provide all the data below.\n") +
                                "If you do not want to use a proxy, uncheck the \"use proxy server\" checkbox.");
          isOk = false;

       }


    }

    // more checks...


   if (isOk)
   {
      m_settings.setValue(HtmlToPdf::INI_PROXY_HOST, ui->proxyHost->text());
      m_settings.setValue(HtmlToPdf::INI_PROXY_PORT, ui->proxyPort->text());
      m_settings.setValue(HtmlToPdf::INI_PROXY_USER, ui->proxyUser->text());
      m_settings.setValue(HtmlToPdf::INI_PROXY_ENABLE, QString::number(ui->proxyEnable->isChecked()));
      HtmlToPdf::setProxyPassword(ui->proxyPassword->text());   // don't save!

      close();
   }
}

void PreferencesWindow::on_proxyEnable_toggled(bool checked)
{
    if (checked)
    {
       ui->proxyHost->setEnabled(true);
       ui->proxyPort->setEnabled(true);
       ui->proxyUser->setEnabled(true);
       ui->proxyPassword->setEnabled(true);
    }
    else
    {
       ui->proxyHost->setEnabled(false);
       ui->proxyPort->setEnabled(false);
       ui->proxyUser->setEnabled(false);
       ui->proxyPassword->setEnabled(false);
    }
}

void PreferencesWindow::on_buttonCancel_clicked()
{
    close();
}
