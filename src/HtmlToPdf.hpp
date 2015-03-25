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

#ifndef HTMLTOPDF_HPP
#define HTMLTOPDF_HPP

#include "IniFile.hpp"
#include <QThread>
#include <QMutex>
#include <QString>
#include <QMap>
#include <wkhtmltox/pdf.h>

/**
 * This is a wrapper-class for using
 * <a href="http://wkhtmltopdf.org/">wkhtmltopdf</a> to convert a webpage
 * to a PDF document. It has been designed to run the conversion in a thread,
 * avoiding to freeze the GUI / offloading the heavy stuff from the main thread.
 * But wkhtmltopdf currently does not support running inside a thread.
 * Nevertheless, we're keeping this design. We just have to directly
 * call HtmlToPdf::run() instead of HtmlToPdf::start() for the time being.
 *
 * @see <a href="http://wkhtmltopdf.org/libwkhtmltox/">
 * libwkhtmltox Documentation</a>
 */
class HtmlToPdf : public QThread
{
   Q_OBJECT
public:
   explicit HtmlToPdf(QObject *parent = 0);

   void run();

   void setUrl(const QString &url);
   const QString &getUrl();

   const QString &getOutFile() const;

   static void initLib();
   static void deinitLib();

   static void setProxyPassword(const QString &password);
   static const QString &getProxyPassword();

   static const IniFile::KeyValue INI_PDF_DIR;
   static const IniFile::KeyValue INI_PROXY_HOST;
   static const IniFile::KeyValue INI_PROXY_PORT;
   static const IniFile::KeyValue INI_PROXY_USER;
   static const IniFile::KeyValue INI_PROXY_ENABLE;
   static const IniFile::KeyValue INI_MARGIN_TOP;
   static const IniFile::KeyValue INI_MARGIN_BOTTOM;
   static const IniFile::KeyValue INI_MARGIN_LEFT;
   static const IniFile::KeyValue INI_MARGIN_RIGHT;

private:
   QString m_outFile;
   QString m_url;

   static QString m_proxyPassword;

   static void logErrorCallback(wkhtmltopdf_converter *converter,
                         const char *message);
   static void logWarningCallback(wkhtmltopdf_converter *converter,
                         const char *message);
   static void logProgressCallback(wkhtmltopdf_converter *converter,
                                   const int progress);

   IniFile m_settings;

   typedef QMap<wkhtmltopdf_converter*, HtmlToPdf*> CallbackMap;
   static CallbackMap m_callbackMap;
   static QMutex m_callbackMapMutex;

signals:
   void pdfCreated(HtmlToPdf *myself);
   void progressChanged(int percent);

};

#endif // HTMLTOPDF_HPP
