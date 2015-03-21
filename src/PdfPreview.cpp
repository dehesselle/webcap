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

#include "PdfPreview.hpp"

#define ELPP_QT_LOGGING
#define ELPP_THREAD_SAFE
#define ELPP_NO_DEFAULT_LOG_FILE
#include <easylogging++.h>

const char *PdfPreview::INI_PAGE_NO = "PdfPreview/pageNo";
const char *PdfPreview::INI_XRES = "PdfPreview/xRes";
const char *PdfPreview::INI_YRES = "PdfPreview/yRes";

PdfPreview::PdfPreview()
{
   initSettings();
}

const QPixmap &PdfPreview::getPreview(const QString &file)
{
   TIMED_FUNC(timerObj);

   readSettings();

   if (not m_previews.contains(file))
   {
      LOG(DEBUG) << "creating preview for " << file;
      m_previews[file] = createPixmap(file);
   }
   else
   {
      LOG(DEBUG) << "using cached preview for " << file;
   }

   return m_previews[file];
}

void PdfPreview::initSettings(const bool &force)
{
   IniFile settings;

   if (not settings.contains(INI_PAGE_NO) or force)
      settings.setValue(INI_PAGE_NO, 0);
   if (not settings.contains(INI_XRES) or force)
      settings.setValue(INI_XRES, 300);
   if (not settings.contains(INI_YRES) or force)
      settings.setValue(INI_YRES, 300);
}

void PdfPreview::readSettings()
{
   m_pageNo = m_settings.value(INI_PAGE_NO).toInt();
   m_xRes = m_settings.value(INI_XRES).toDouble();
   m_yRes = m_settings.value(INI_YRES).toDouble();
}
