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

#ifndef PDFTOPIXMAP_HPP
#define PDFTOPIXMAP_HPP

#include <QPixmap>
#include <QString>

/**
 * This is a small wrapper-class that uses
 * <a href="http://poppler.freedesktop.org/">Poppler</a> to convert a page
 * from a PDF file to a QPixmap.
 *
 * @see <a href="http://people.freedesktop.org/~aacid/docs/qt5/">
 * Poppler QT5 interface library</a>
 */
class PdfToPixmap
{
public:
   PdfToPixmap();

   /**
    * @param[in] file a PDF file
    * @return a single page of the PDF
    */
   const QPixmap createPixmap(const QString &file) const;

protected:
   int m_pageNo;    ///< page no to convert, first page = 0
   double m_xRes;   ///< horizontal image resolution
   double m_yRes;   ///< vertical image resolution
};

#endif // PDFTOPIXMAP_HPP
