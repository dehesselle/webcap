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

   void run();   ///< run wkhtmltopdf 

   /// set the URL to be captured with wkhtmltopdf
   void setUrl(const QString &url);
   /// get the URL that has previously been set
   const QString &getUrl();

   /// get the filename of the generated PDF
   const QString &getOutFile() const;

   static void initLib();     ///< initialize wkhtmltopdf
   static void deinitLib();   ///< deinitialize wkhtmltopdf

   /// set the password used to access the proxy server
   static void setProxyPassword(const QString &password);
   /// get the password used to access the proxy server
   static const QString &getProxyPassword();

   /// default target directory
   static const IniFile::KeyValue INI_PDF_DIR;
   /// default proxy hostname
   static const IniFile::KeyValue INI_PROXY_HOST;
   /// default proxy port
   static const IniFile::KeyValue INI_PROXY_PORT;
   /// default proxy username
   static const IniFile::KeyValue INI_PROXY_USER;
   /// default "use proxy: yes/no" setting
   static const IniFile::KeyValue INI_PROXY_ENABLE;
   /// default top margin for wkhtmltopdf
   static const IniFile::KeyValue INI_MARGIN_TOP;
   /// default bottom margin for wkhtmltopdf
   static const IniFile::KeyValue INI_MARGIN_BOTTOM;
   /// default left margin for wkhtmltopdf
   static const IniFile::KeyValue INI_MARGIN_LEFT;
   /// default right margin for wkhtmltopdf
   static const IniFile::KeyValue INI_MARGIN_RIGHT;

private:
   QString m_outFile;   ///< filename of generated PDF
   QString m_url;       ///< URL to be captured

   static QString m_proxyPassword;   ///< password proxy server

   /// callback function for wkhtmltopdf to report errors
   static void logErrorCallback(wkhtmltopdf_converter *converter,
                         const char *message);
   /// callback function for wkhtmltopdf to report warnings
   static void logWarningCallback(wkhtmltopdf_converter *converter,
                         const char *message);
   /// callback function for wkhtmltopdf to report progress of running capture
   static void emitProgressChanged(wkhtmltopdf_converter *converter,
                                  const int progress);
   /// callback function for wkhtmltopdf to report capturing has finished
   static void emitIsFinished(wkhtmltopdf_converter *converter,
                              const int finished);

   IniFile m_settings;   ///< our configurable settings

   typedef QMap<wkhtmltopdf_converter*, HtmlToPdf*> CallbackMap;
   /** 
    * Stores relation between wkhtmltopdf and HtmlToPdf. We need this as
    * bridge between static and non-static functions.
    */
   static CallbackMap m_callbackMap;
   /// to protect #m_callbackMap from concurrent write access
   static QMutex m_callbackMapMutex;

signals:
   /// signal capture has finished
   void pdfCreated(HtmlToPdf *myself);
   /// signal progress of running capture
   void progressChanged(int percent);
};

#endif // HTMLTOPDF_HPP
