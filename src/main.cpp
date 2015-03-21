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

#include "MainWindow.hpp"
#include "IniFile.hpp"
#include "Version.hpp"
#include <QApplication>

#define ELPP_QT_LOGGING
#define ELPP_THREAD_SAFE
#define ELPP_NO_DEFAULT_LOG_FILE
#include <easylogging++.h>

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);

   {  // setup logging
      IniFile settings;

      el::Configurations config;
      config.setToDefault();
      //TODO logfile name from application name, not hard-coded
      config.setGlobally(el::ConfigurationType::Filename,
                         QString(settings.getPath() + "/WebCap.log").toStdString());
      config.setGlobally(el::ConfigurationType::Format, "%datetime %level [%func] %msg");

#ifdef QT_DEBUG
      config.setGlobally(el::ConfigurationType::ToStandardOutput, "true");
#else
      config.setGlobally(el::ConfigurationType::ToStandardOutput, "false");
#endif
      config.setGlobally(el::ConfigurationType::ToFile, "true");
      el::Loggers::reconfigureAllLoggers(config);
   }

   LOG(INFO) << "WebCap v" << WEBCAP_VERSION;
   LOG(DEBUG) << "using Easylogging++ v" << el::VersionInfo::version();
   TIMED_FUNC(timerObj);

   MainWindow w;
   w.show();

   return a.exec();
}
