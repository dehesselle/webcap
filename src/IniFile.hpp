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

/** @brief store settings with INI storage backend
 *
 * This is a wrapper-class for QSettings to handle the basic
 * parametrisation to use an INI file as storage backend. It is also
 * aware of portable/non-portable usage, i.e. checking the application
 * directory for an INI file and using that instead of the
 * default location in the user's profile.
 *
 * We're not deriving from QSettings on purpose to keep things simple.
 * (See detailed explanation in the constructor.)
 */
class IniFile
{
public:
   IniFile();
   ~IniFile();

   /** @brief name of INI setting and its default value
    *
    * We use this to access a setting from an INI file as well as supplying
    * a default value if that setting does not exist (e.g. on first program
    * start with no pre-existing INI file). Best practice is to
    * have each class that needs settings from an INI file define them as
    * KeyValue types.
    */
   struct KeyValue
   {
      const char *key;       ///< syntax is @c section.key
      const QString value;   ///< default value
   };

   /** @brief path of the INI file being used
    *
    * This function will return one of two possible paths:
    * - a path inside the user's profile directory (i.e. normal mode) or
    * - the application directory (i.e. portable mode).
    *
    * @return path of the INI file
    */
   QString getPath() const;

   /** @brief forwards to the respective QSettings method
    *
    * @see <a href="http://doc.qt.io/qt-5/qsettings.html#contains">
    * QSettings::contains</a>
    */
   bool contains(const QString &key) const;
   /** @brief forwards to the respective QSettings method
    * 
    * @see <a href="http://doc.qt.io/qt-5/qsettings.html#setValue">
    * QSettings::setValue</a>
    */
   void setValue(const QString &key, const QVariant &value);
   /** @brief forwards to the respective QSettings method
    * 
    * @see <a href="http://doc.qt.io/qt-5/qsettings.html#setValue">
    * QSettings::setValue</a>
    */
   void setValue(const KeyValue &keyValue, const QVariant &value = QVariant());
   /** @brief forwards to the respective QSettings method
    * 
    * @see <a href="http://doc.qt.io/qt-5/qsettings.html#value">
    * QSettings::value</a>
    */
   QVariant value(const QString &key,
                  const QVariant &defaultValue = QVariant()) const;
   /** @brief forwards to the respective QSettings method
    * 
    * @see <a href="http://doc.qt.io/qt-5/qsettings.html#value">
    * QSettings::value</a>
    */
   QVariant value(const KeyValue &keyValue);

private:
   QSettings *m_settings;
};

#endif // INIFILE_HPP
