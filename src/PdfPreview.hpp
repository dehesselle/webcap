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

#ifndef PDFPREVIEW_HPP
#define PDFPREVIEW_HPP

#include "PdfToPixmap.hpp"
#include "IniFile.hpp"
#include <QMap>
#include <QPixmap>
#include <QString>

/**
 * This class extends PdfToPixmap with the following abilities:
 * - reading parameters from *.ini file
 * - caching all generated Pixmaps
 */
class PdfPreview : protected PdfToPixmap
{
public:
   /** fetch preview from cache or generate it
    * @param[in] file a PDF file
    * @return a single page of the PDF
    */
   const QPixmap &getPreview(const QString &file);

   static const IniFile::KeyValue INI_PAGE_NO;
   static const IniFile::KeyValue INI_XRES;
   static const IniFile::KeyValue INI_YRES;

private:
   /** read parameters from INI file
    */
   void readSettings();

   IniFile m_settings;   ///< access INI file
   QMap<QString, QPixmap> m_previews;   ///< store all generated previews
};

#endif // PDFPREVIEW_HPP
