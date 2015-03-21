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
#include <QCoreApplication>
#include <QFileInfo>

#define ELPP_QT_LOGGING
#define ELPP_THREAD_SAFE
#define ELPP_NO_DEFAULT_LOG_FILE
#include <easylogging++.h>

INITIALIZE_EASYLOGGINGPP

// order represents log hierarchy
static const char *INI_LOG_TRACE   = "main/logTrace";
static const char *INI_LOG_DEBUG   = "main/logDebug";
static const char *INI_LOG_VERBOSE = "main/logVerbose";
static const char *INI_LOG_INFO    = "main/logInfo";
static const char *INI_LOG_WARNING = "main/logWarning";
static const char *INI_LOG_ERROR   = "main/logError";
static const char *INI_LOG_FATAL   = "main/logFatal";
static const char *INI_LOG_FORMAT  = "main/logFormat";

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);

   // setup logging
   {
      IniFile settings;

      // create defaults if they don't exist
      {
         if (not settings.contains(INI_LOG_TRACE))
            settings.setValue(INI_LOG_TRACE, "false");
         if (not settings.contains(INI_LOG_DEBUG))
            settings.setValue(INI_LOG_DEBUG, "false");
         if (not settings.contains(INI_LOG_VERBOSE))
            settings.setValue(INI_LOG_VERBOSE, "false");
         if (not settings.contains(INI_LOG_INFO))
            settings.setValue(INI_LOG_INFO, "true");
         if (not settings.contains(INI_LOG_WARNING))
            settings.setValue(INI_LOG_WARNING, "true");
         if (not settings.contains(INI_LOG_ERROR))
            settings.setValue(INI_LOG_ERROR, "true");
         if (not settings.contains(INI_LOG_FATAL))
            settings.setValue(INI_LOG_FATAL, "true");
         if (not settings.contains(INI_LOG_FORMAT))
            settings.setValue(INI_LOG_FORMAT, "%datetime %level [%func] %msg");
      }

      QString logfile =
            settings.getPath() + "/" +
            QFileInfo(QCoreApplication::applicationFilePath()).baseName() +
            "/" + ".log";

      el::Configurations config;
      config.setToDefault();

      config.setGlobally(el::ConfigurationType::Filename,
                         logfile.toStdString());
      config.setGlobally(el::ConfigurationType::Format,
            settings.value(INI_LOG_FORMAT).toString().toStdString());
      config.setGlobally(el::ConfigurationType::ToFile, "true");

      config.set(el::Level::Trace, el::ConfigurationType::Enabled,
                 settings.value(INI_LOG_TRACE).toString().toStdString());
      config.set(el::Level::Debug, el::ConfigurationType::Enabled,
                 settings.value(INI_LOG_DEBUG).toString().toStdString());
      config.set(el::Level::Verbose, el::ConfigurationType::Enabled,
                 settings.value(INI_LOG_VERBOSE).toString().toStdString());
      config.set(el::Level::Info, el::ConfigurationType::Enabled,
                 settings.value(INI_LOG_INFO).toString().toStdString());
      config.set(el::Level::Warning, el::ConfigurationType::Enabled,
                 settings.value(INI_LOG_WARNING).toString().toStdString());
      config.set(el::Level::Error, el::ConfigurationType::Enabled,
                 settings.value(INI_LOG_ERROR).toString().toStdString());
      config.set(el::Level::Fatal, el::ConfigurationType::Enabled,
                 settings.value(INI_LOG_FATAL).toString().toStdString());

#ifdef QT_DEBUG
      // console logging (in addition to file) only in debug builds
      config.setGlobally(el::ConfigurationType::ToStandardOutput, "true");
      // force log levels in debug builds
      config.set(el::Level::Global, el::ConfigurationType::Enabled, "true");
#else
      config.setGlobally(el::ConfigurationType::ToStandardOutput, "false");
#endif

      el::Loggers::reconfigureAllLoggers(config);
   }

   LOG(INFO) << "WebCap v" << WEBCAP_VERSION;
   LOG(DEBUG) << "using Easylogging++ v" << el::VersionInfo::version();
   TIMED_FUNC(timerObj);

   MainWindow w;
   w.show();

   return a.exec();
}
