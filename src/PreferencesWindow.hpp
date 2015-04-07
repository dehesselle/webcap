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

#ifndef PREFERENCESWINDOW_HPP
#define PREFERENCESWINDOW_HPP

#include "IniFile.hpp"
#include <QDialog>
#include <QString>

namespace Ui {
class PreferencesWindow;
}

class PreferencesWindow : public QDialog
{
   Q_OBJECT

public:
   explicit PreferencesWindow(QWidget *parent = 0);
   ~PreferencesWindow();

private slots:
   void on_buttonCancel_clicked();
   void on_buttonOk_clicked();
   void on_buttonTargetDir_clicked();
   void on_proxyEnable_toggled(bool checked);

private:
   /// @brief read parameters from INI file
   void readSettings();
   
   Ui::PreferencesWindow *ui;
   IniFile m_settings;   ///< our configurable settings
};

#endif // PREFERENCESWINDOW_HPP
