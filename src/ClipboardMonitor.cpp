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

#include "ClipboardMonitor.hpp"
#include <QMessageBox>
#include <QApplication>
#include <QMimeData>
#include <QDateTime>
#include <QUrl>

#define ELPP_QT_LOGGING
#define ELPP_THREAD_SAFE
#define ELPP_NO_DEFAULT_LOG_FILE
#include <easylogging++.h>

const IniFile::KeyValue ClipboardMonitor::INI_TIME_SPAN =
      { "ClipboardMonitor/timeSpan", "400" };

ClipboardMonitor::ClipboardMonitor(QObject *parent) :
   QObject(parent),
   m_clipboard(QApplication::clipboard()),
   m_lastEvent(QDateTime::currentMSecsSinceEpoch())
{
}

bool ClipboardMonitor::enable()
{
   if (m_clipboard)
   {
      m_connection = connect(m_clipboard, &QClipboard::changed,
                             this, &ClipboardMonitor::onClipboardChange);

   /* TODO
    * http://doc.qt.io/qt-5/qobject.html#connect
    * The function returns a QMetaObject::Connection that represents
    * a handle to a connection if it successfully connects the signal
    * to the slot. The connection handle will be invalid if it cannot
    * create the connection, for example, if QObject is unable to verify
    * the existence of either signal or method, or if their signatures
    * aren't compatible. You can check if the handle is valid by casting
    * it to a bool.
    *
    * How to correctly cast it to bool?
    */
   }
}

void ClipboardMonitor::disable()
{
   if (not disconnect(m_connection))
   {
      //TODO there was nothing to disconnect
   }
}

void ClipboardMonitor::onClipboardChange()
{
   qint64 event = QDateTime::currentMSecsSinceEpoch();

   if (m_clipboard->mimeData()->hasText())
   {
      QUrl url = QUrl::fromUserInput(m_clipboard->text());

      if (url.isValid())
      {
         quint64 span = event - m_lastEvent;

         if (url == m_lastUrl)
            if (span < m_settings.value(INI_TIME_SPAN).toInt())
               emit clipboardChanged(m_clipboard->text());
            else
               LOG(DEBUG) << "span=" << span;
         else
            LOG(DEBUG) << "new URL";

         m_lastUrl = url;
         m_lastEvent = event;
      }
      else
      {
         LOG(DEBUG) << "no URL";
      }
   }
   else
   {
      LOG(DEBUG) << "no text";
   }
}
