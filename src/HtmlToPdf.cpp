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
#include "Log.hpp"
#include <QUrl>
#include <QFileInfo>
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QMutexLocker>

const IniFile::KeyValue HtmlToPdf::INI_PROXY_HOST =    { "HtmlToPdf/proxyHost", "myproxy.mydomain.com" };
const IniFile::KeyValue HtmlToPdf::INI_PROXY_PORT =    { "HtmlToPdf/proxyPort", "8080" };
const IniFile::KeyValue HtmlToPdf::INI_PROXY_USER =    { "HtmlToPdf/proxyUser", "Lieselotte" };
const IniFile::KeyValue HtmlToPdf::INI_PROXY_ENABLE =  { "HtmlToPdf/proxyEnable", "false" };
const IniFile::KeyValue HtmlToPdf::INI_MARGIN_BOTTOM = { "HtmlToPdf/marginBottom", "1" };
const IniFile::KeyValue HtmlToPdf::INI_MARGIN_TOP =    { "HtmlToPdf/marginTop", "1" };
const IniFile::KeyValue HtmlToPdf::INI_MARGIN_LEFT =   { "HtmlToPdf/marginLeft", "1" };
const IniFile::KeyValue HtmlToPdf::INI_MARGIN_RIGHT =  { "HtmlToPdf/marginRight", "1" };
const IniFile::KeyValue HtmlToPdf::INI_PDF_DIR =       { "HtmlToPdf/pdfDirectory", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) };

QString HtmlToPdf::m_proxyPassword;

HtmlToPdf::CallbackMap HtmlToPdf::m_callbackMap;
QMutex HtmlToPdf::m_callbackMapMutex;

HtmlToPdf::HtmlToPdf(QObject *parent) :
   QThread(parent)
{
}

void HtmlToPdf::setUrl(const QString &url)
{
   m_url = url;
}

const QString &HtmlToPdf::getUrl()
{
   return m_url;
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
   HtmlToPdf::m_proxyPassword = password;
}

const QString &HtmlToPdf::getProxyPassword()
{
   return HtmlToPdf::m_proxyPassword;
}

void HtmlToPdf::logWarningCallback(wkhtmltopdf_converter *converter,
                                   const char *message)
{
   Q_UNUSED(converter);
   LOG(WARNING) << message;
}

void HtmlToPdf::emitProgressChanged(wkhtmltopdf_converter *converter,
                                    const int progress)
{
   HtmlToPdf *htmlToPdf = HtmlToPdf::m_callbackMap.value(converter, 0);

   if (htmlToPdf)
   {
      QFileInfo file = htmlToPdf->getOutFile();
      LOG(TRACE) << file.fileName() << ": " << progress;

      emit htmlToPdf->progressChanged(progress);
   }
   else
   {
      LOG(ERROR) << converter << ": " << progress;
   }
}

void HtmlToPdf::emitIsFinished(wkhtmltopdf_converter *converter,
                               const int finished)
{
   Q_UNUSED(finished);

   HtmlToPdf *htmlToPdf = HtmlToPdf::m_callbackMap.value(converter, 0);

   if (htmlToPdf)
   {
      QFileInfo file = htmlToPdf->getOutFile();
      LOG(TRACE) << file.fileName() << ": finished";

      // Being "finished" means we have created a PDF.
      emit htmlToPdf->pdfCreated(htmlToPdf);
   }
   else
   {
      LOG(ERROR) << converter << ": finished";
   }
}

const QString &HtmlToPdf::getOutFile() const
{
   return m_outFile;
}

void HtmlToPdf::logErrorCallback(wkhtmltopdf_converter *converter,
                                 const char *message)
{
   Q_UNUSED(converter);
   LOG(ERROR) << message;
}

void HtmlToPdf::run()
{
   TIMED_FUNC(timerObj);

   static bool isFirstCall = true;
   if (isFirstCall)
   {
      isFirstCall = false;
      LOG(DEBUG) << "using wkthmltopdf v" << wkhtmltopdf_version();
   }

   {
      QString host = QUrl::fromUserInput(m_url).host();
      host.replace(".", "_");

      // create a unique filename by adding a number to it
      {
         QFileInfo file;

         int i = 0;
         do   // increase number until we have filename that doesn't exist
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

   wkhtmltopdf_global_settings *globalSettings =
         wkhtmltopdf_create_global_settings();
   wkhtmltopdf_set_global_setting(globalSettings, "out",
                                  m_outFile.toStdString().c_str());
   wkhtmltopdf_set_global_setting(globalSettings, "margin.top",
         m_settings.value(INI_MARGIN_TOP).toString().toStdString().c_str());
   wkhtmltopdf_set_global_setting(globalSettings, "margin.bottom",
         m_settings.value(INI_MARGIN_BOTTOM).toString().toStdString().c_str());
   wkhtmltopdf_set_global_setting(globalSettings, "margin.left",
         m_settings.value(INI_MARGIN_LEFT).toString().toStdString().c_str());
   wkhtmltopdf_set_global_setting(globalSettings, "margin.right",
         m_settings.value(INI_MARGIN_RIGHT).toString().toStdString().c_str());

   wkhtmltopdf_object_settings *objectSettings =
         wkhtmltopdf_create_object_settings();
   wkhtmltopdf_set_object_setting(objectSettings, "page",
                                  m_url.toStdString().c_str());

   // check if we need to connect via proxy
   if (m_settings.value(INI_PROXY_ENABLE).toBool())
   {
      QString host = m_settings.value(INI_PROXY_HOST).toString();
      QString user = m_settings.value(INI_PROXY_USER).toString();
      QString port = m_settings.value(INI_PROXY_PORT).toString();

      /* syntax is
       * http://username:password@myhost.de:8080
       */

      QString proxy = "http://" + user + ":" +
            HtmlToPdf::m_proxyPassword + "@" + host + ":" + port;

      wkhtmltopdf_set_object_setting(objectSettings, "load.proxy",
                                     proxy.toStdString().c_str());

      LOG(DEBUG) << "using proxy http://"
                 << user << ":xxx@" << host << ":" << port;
   }

   wkhtmltopdf_converter *converter =
         wkhtmltopdf_create_converter(globalSettings);

   /* Save the "converter to HtmlToPdf" relation. We use this later
    * on when we need a way from a static callback function into
    * the respective HtmlToPdf instance.
    */
   {
      QMutexLocker(&HtmlToPdf::m_callbackMapMutex);
      HtmlToPdf::m_callbackMap[converter] = this;
   }

   wkhtmltopdf_add_object(converter, objectSettings, 0);
   wkhtmltopdf_set_error_callback(converter, logErrorCallback);
   wkhtmltopdf_set_warning_callback(converter, logWarningCallback);
   wkhtmltopdf_set_progress_changed_callback(converter, emitProgressChanged);
   wkhtmltopdf_set_finished_callback(converter, emitIsFinished);

   wkhtmltopdf_convert(converter);

   wkhtmltopdf_destroy_converter(converter);
   wkhtmltopdf_destroy_object_settings(objectSettings);
   wkhtmltopdf_destroy_global_settings(globalSettings);
}
