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

#include "HtmlToPdf.hpp"
#include "PreferencesWindow.hpp"
#include <wkhtmltox/pdf.h>
#include <QUrl>
#include <QFileInfo>
#include <QFile>
#include <QStandardPaths>
#include <QDir>

#define ELPP_QT_LOGGING
#define ELPP_THREAD_SAFE
#define ELPP_NO_DEFAULT_LOG_FILE
#include <easylogging++.h>

const IniFile::KeyValue HtmlToPdf::INI_PROXY_HOST = {
   "HtmlToPdf/proxyHost", "localhost"
};
const IniFile::KeyValue HtmlToPdf::INI_PROXY_PORT = {
   "HtmlToPdf/proxyPort", "8080"
};
const IniFile::KeyValue HtmlToPdf::INI_PROXY_USER = {
   "HtmlToPdf/proxyUser", "Lieselotte"
};
const IniFile::KeyValue HtmlToPdf::INI_PROXY_ENABLE = {
   "HtmlToPdf/proxyEnable", "false"
};
const IniFile::KeyValue HtmlToPdf::INI_MARGIN_BOTTOM = {
   "HtmlToPdf/marginBottom", "1"
};
const IniFile::KeyValue HtmlToPdf::INI_MARGIN_TOP = {
   "HtmlToPdf/marginTop", "1"
};
const IniFile::KeyValue HtmlToPdf::INI_MARGIN_LEFT = {
   "HtmlToPdf/marginLeft", "1"
};
const IniFile::KeyValue HtmlToPdf::INI_MARGIN_RIGHT = {
   "HtmlToPdf/marginRight", "1"
};
const IniFile::KeyValue HtmlToPdf::INI_PDF_DIR = {
   "HtmlToPdf/pdfDirectory", QDir::homePath() + "/" +
   QStandardPaths::displayName(QStandardPaths::DocumentsLocation)
};

QString HtmlToPdf::m_proxyPassword;

HtmlToPdf::HtmlToPdf(QObject *parent) :
   QThread(parent)
{
}

void HtmlToPdf::setUrl(const QString &url)
{
   m_url = url;
}

void HtmlToPdf::initLib()
{
   wkhtmltopdf_init(false);
}

void HtmlToPdf::deinitLib()
{
   wkhtmltopdf_deinit();
}

void HtmlToPdf::setProxyPassword(const QString &password)
{
   m_proxyPassword = password;
}

const QString &HtmlToPdf::getProxyPassword()
{
   return m_proxyPassword;
}

const QString &HtmlToPdf::getOutFile() const
{
   return m_outFile;
}

void HtmlToPdf::run()
{
   TIMED_FUNC(timerObj);

   {
      QString host = QUrl(m_url).host();
      host.replace(".", "_");

      {  // create a unique filename by adding a number to it
         QFileInfo file;

         int i = 0;
         do
         {
            ++i;
            file = m_settings.value(INI_PDF_DIR).toString() +
                   "/" + host + "_" + QString::number(i) + ".pdf";
         }
         while (file.exists());

         m_outFile = file.absoluteFilePath();
      }

      {  /* Create a dummy to instantly "reserve" the filename/number.
          * Without this, the section above wouldn't work for quick
          * consecutive runs of this function (because PDF generation
          * takes time).
          */
         QFile dummy(m_outFile);
         dummy.open(QIODevice::WriteOnly | QIODevice::Text);
         dummy.close();
      }
   }

   LOG(INFO) << m_url << ", " << m_outFile;

   wkhtmltopdf_global_settings *globalSettings =  wkhtmltopdf_create_global_settings();
   wkhtmltopdf_set_global_setting(globalSettings, "out", m_outFile.toStdString().c_str());
   wkhtmltopdf_set_global_setting(globalSettings, "margin.top",
         m_settings.value(INI_MARGIN_TOP).toString().toStdString().c_str());
   wkhtmltopdf_set_global_setting(globalSettings, "margin.bottom",
         m_settings.value(INI_MARGIN_BOTTOM).toString().toStdString().c_str());
   wkhtmltopdf_set_global_setting(globalSettings, "margin.left",
         m_settings.value(INI_MARGIN_LEFT).toString().toStdString().c_str());
   wkhtmltopdf_set_global_setting(globalSettings, "margin.right",
         m_settings.value(INI_MARGIN_RIGHT).toString().toStdString().c_str());

   wkhtmltopdf_object_settings *objectSettings = wkhtmltopdf_create_object_settings();
   wkhtmltopdf_set_object_setting(objectSettings, "page", m_url.toStdString().c_str());

   if (m_settings.value(INI_PROXY_ENABLE).toBool())
   {
      QString host = m_settings.value(INI_PROXY_HOST).toString();
      QString user = m_settings.value(INI_PROXY_USER).toString();
      QString port = m_settings.value(INI_PROXY_PORT).toString();

      QString proxy = "http://" + user + ":" +
            m_proxyPassword + "@" + host + ":" + port;

      wkhtmltopdf_set_object_setting(objectSettings, "load.proxy",
                                     proxy.toStdString().c_str());

      LOG(DEBUG) << "using proxy http://"
                 << user << ":xxx@" << host << ":" << port;

   }

   wkhtmltopdf_converter *converter = wkhtmltopdf_create_converter(globalSettings);
   wkhtmltopdf_add_object(converter, objectSettings, 0);
   wkhtmltopdf_convert(converter);

   wkhtmltopdf_destroy_converter(converter);
   wkhtmltopdf_destroy_object_settings(objectSettings);
   wkhtmltopdf_destroy_global_settings(globalSettings);

   emit pdfCreated(this);
}
