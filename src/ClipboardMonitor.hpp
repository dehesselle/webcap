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

#ifndef CLIPBOARDMONITOR_HPP
#define CLIPBOARDMONITOR_HPP

#include "IniFile.hpp"
#include <QObject>
#include <QClipboard>
#include <QUrl>

/** watch clipboard for URLs
 *
 * This class monitors changes to the clipboard. If it detects that the same
 * URL has been copied to it twice in a row, it emits the urlCaptured() signal.
 */
class ClipboardMonitor : public QObject
{
   Q_OBJECT

public:
   explicit ClipboardMonitor(QObject *parent = 0);

   /** connect to the system clipboard
    *
    * Establish a connection to get notified of all changes to the clipboard. 
    */
   bool enable();
   /// disconnect from the system clipboard
   void disable();

   /** 
    * In order for us to emit the urlCaptured() signal, the same URL must have
    * been copied twice to the clipboard in less than x milliseconds. This is
    * the default value for that.
    */
   static const IniFile::KeyValue INI_TIME_SPAN;

private:
   QClipboard *m_clipboard;
   /// We need to store the connection if we want to be able to disconnect.
   QMetaObject::Connection m_connection;

   qint64 m_lastEvent;   ///< unix time in milliseconds of last URL captured
   QUrl m_lastUrl;       ///< last captured URL

   IniFile m_settings;   ///< access our configurable settings

protected slots:
   /** process changes to clipboard
    * 
    * Getting notified of each and every change to the clipboard is not what we
    * want. We need to filter out everything but the "have you copied the same
    * URL twice in a row"-event, and that is being done here.
    */
   void on_ClipboardChange();

signals:
   /** caught an URL that should be processed */
   void urlCaptured(const QString &url);
};

#endif // CLIPBOARDMONITOR_HPP
