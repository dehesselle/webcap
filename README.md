# WebCap - capture websites to PDF files
WebCap is a utility that renders websites to PDF files. It was inspired by the excellent [Paparazzi!](https://derailer.org/paparazzi/), which is an OS X only tool. I couldn't find a similar tool for Windows, but since the core technology itself is readily available, why not plug it all together myself?

## Why would you want to use it?
Utilizing the power of [wkhtmltopdf](http://wkhtmltopdf.org), capturing a website with WebCap gives you the following advantages:

* The layout of the website is kept alive in vectorized form. It scales up and down without any loss of quality.
* You retain access to the text (e.g. copy/paste); the PDF can be indexed by full-text search thingies.
* Links stay alive and clickable inside the PDF.
* The PDF file will most probably be less in size than a (high resolution) bitmap-based image. 
_(Some restrictions/limitations may apply, see the manual.)_

Still, if you have a Mac, go use [Paparazzi!](https://derailer.org/paparazzi/) instead. (Yes, it's that good.)

## Download binaries
I do supply release builds for the Windows 64bit platform. You need three things to get up and running:

0. [WebCap v0.2 executable](http://rv1109.1blu.de/webcap/webcap_0.2.zip). This is a very small download and the one that you'll have to redownload with every new release. Scanned with [virustotal](https://www.virustotal.com/de/file/3b39f971e0b79b535a4e33263f8fe7a5c170faf87d347fb6d928f31719cc2d10/analysis/1427589386/).
0. [WebCap base libraries](http://rv1109.1blu.de/webcap/webcap_base_libs.zip). This is a huge download and contains all but two DLLs that WebCap needs to run. This is supposed to be a one-time-only download unless stated otherwise. Extract this package into the same directory as the executable. Scanned with [virustotal](https://www.virustotal.com/de/file/88ef559ef977b1e096172a322bbe9df74aa84d3afcd3261ecfdc264ecb526ffd/analysis/1427589745/).
0. [Visual C++ Redistributable Packages for Visual Studio 2013](http://www.microsoft.com/en-us/download/details.aspx?id=40784), also a one-time-only download. You need the 64bit version named `vcredist_x64.exe`. This package contains the two DLLs missing from the package above (`msvcp120.dll` and `msvcr120.dll`) that I do not distribute myself.

## Finally...
More documentation can be found in the `MANUAL.md` file.

Please keep in mind that this is still __work in progress__. Consider nothing as finished and __everything is subject to change__ until we reach v1.x.

