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

class ClipboardMonitor : public QObject
{
   Q_OBJECT

public:
   explicit ClipboardMonitor(QObject *parent = 0);

   bool enable();
   void disable();

   static const IniFile::KeyValue INI_TIME_SPAN;

private:
   QClipboard *m_clipboard;
   QMetaObject::Connection m_connection;

   qint64 m_lastEvent;
   QUrl m_lastUrl;

   IniFile m_settings;

protected slots:
   void onClipboardChange();

signals:
   void clipboardChanged(QString bla);
};

#endif // CLIPBOARDMONITOR_HPP
