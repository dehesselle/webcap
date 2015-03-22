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

#ifndef INIFILE_HPP
#define INIFILE_HPP

#include <QSettings>
#include <QString>
#include <QVariant>

/**
 * This is a small wrapper-class for QSettings to handle the basic
 * parametrisation to use an INI file as storage backend. It is also
 * aware of portable/non-portable usage, checking the application
 * directory for an INI file and using that instead of the
 * default location in the user's profile.
 *
 * We're not deriving from QInstance on purpose, see the explanation
 * in IniFile::IniFile().
 */
class IniFile
{
public:
   IniFile();
   ~IniFile();

   /**
    * This is for the convenience of allowing us to have static key-value
    * pairs of default INI settings within each class that works with an
    * INI file.
    */
   struct KeyValue
   {
      const char *key;
      const QString value;
   };

   /** path of the INI file being used
    *
    * This function can be used to determine the general location to store other
    * files (e.g. logs). It either returns a path to the user's profile or the
    * application directory, depending on portable/non-portable usage.
    *
    * @return path of the INI file
    */
   QString getPath() const;

   /** forwards to respective QSettings method */
   bool contains(const QString &key) const;
   /** forwards to respective QSettings method */
   void setValue(const QString &key, const QVariant &value);
   /** forwards to respective QSettings method */
   void setValue(const KeyValue &keyValue, const QVariant &value);
   /** forwards to respective QSettings method */
   QVariant value(const QString &key,
                  const QVariant &defaultValue = QVariant()) const;
   /** forwards to respective QSettings method */
   QVariant value(const KeyValue &keyValue);

private:
   QSettings *m_settings;
};

#endif // INIFILE_HPP
