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

#include "PdfToPixmap.hpp"
#include "Log.hpp"
#include <poppler/qt5/poppler-qt5.h>
#include <poppler/poppler-config.h>

PdfToPixmap::PdfToPixmap() :
   m_pageNo(0),
   m_xRes(300),
   m_yRes(300)
{
}

const QPixmap PdfToPixmap::createPixmap(const QString &file) const
{
   QPixmap result;

   static bool isFirstCall = true;
   if (isFirstCall)
   {
      isFirstCall = false;
      LOG(DEBUG) << "using Poppler v" << POPPLER_VERSION;
   }

   Poppler::Document *document = Poppler::Document::load(file);

   if (document)
   {
      Poppler::Page *page = document->page(m_pageNo);

      if (page)
      {
         QImage image = page->renderToImage(m_xRes, m_yRes);

         if (image.isNull())
            LOG(ERROR) << "failed to render image ("
                       << m_xRes << ", "
                       << m_yRes << ")";
         else
            result = QPixmap::fromImage(image);

         delete page;
      }
      else
      {
         LOG(ERROR) << "unable to get page " << m_pageNo;
      }

      delete document;
   }
   else
   {
      LOG(ERROR) << "unable to load " << file;
   }

   return result;
}
