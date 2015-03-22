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

#include "IniFile.hpp"
#include <QFileInfo>
#include <QCoreApplication>

IniFile::IniFile()
{
   m_settings = new QSettings(QSettings::IniFormat,
                              QSettings::UserScope,
                              "dehesselle",
                              "WebCap");

   /* This is the easiest way to switch between portable (i.e. INI in
    * application directory) and non-portable (i.e. INI in user's profile
    * directory) mode:
    * - create an instance of QSettings with QSettings::UserScope
    * - retrieve the filename of the INI file so we don't need to hardcode
    *   it or use some other means of getting a filename corresponding with
    *   the application's name
    * - check the application directory for that given filename
    * - if INI exists in application directory, delete current instance
    *   of QSettings and create a new one using the INI in the application
    *   directory
    */

   QFileInfo profileSettingsFile(m_settings->fileName());
   QFileInfo portableSettingsFile = QCoreApplication::applicationDirPath() +
         "/" + profileSettingsFile.fileName();

   if (portableSettingsFile.exists())
   {
      delete m_settings;
      m_settings = new QSettings(portableSettingsFile.absoluteFilePath(),
                                 QSettings::IniFormat);
   }
}

IniFile::~IniFile()
{
   delete m_settings;
}

QString IniFile::getPath() const
{
   QFileInfo settingsFile(m_settings->fileName());
   return settingsFile.absolutePath();
}

bool IniFile::contains(const QString &key) const
{
   return m_settings->contains(key);
}

void IniFile::setValue(const QString &key, const QVariant &value)
{
   m_settings->setValue(key, value);
}

QVariant IniFile::value(const QString &key, const QVariant &defaultValue) const
{
   return m_settings->value(key, defaultValue);
}

QVariant IniFile::value(const IniFile::KeyValue &keyValue)
{
   if (not m_settings->contains(keyValue.key))
      m_settings->setValue(keyValue.key, keyValue.value);

   return m_settings->value(keyValue.key);
}

void IniFile::setValue(const IniFile::KeyValue &keyValue, const QVariant &value)
{
   if (value.isNull())
      m_settings->setValue(keyValue.key, keyValue.value);
   else
      m_settings->setValue(keyValue.key, value);
}
