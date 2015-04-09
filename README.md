# WebCap - capture websites to PDF files
WebCap is a utility that renders websites to PDF files. It was inspired by the excellent [Paparazzi!](https://derailer.org/paparazzi/), which is an OS X only tool. I couldn't find a similar tool for Windows, but since the core technology itself is readily available, why not plug it all together myself?

## Why would you want to use it?
Utilizing the power of [wkhtmltopdf](http://wkhtmltopdf.org), capturing a website with WebCap gives you the following advantages:

* The layout of the website is kept alive in vectorized form. It scales up and down without any loss of quality.
* You retain access to the text (e.g. copy/paste); the PDF can be indexed by full-text search thingies.
* Links stay alive and clickable inside the PDF.
* The PDF file will most probably be less in size than a (high resolution) bitmap-based image. 

_(Some restrictions and/or limitations may apply, see the manual.)_

Still, if you have a Mac, go use [Paparazzi!](https://derailer.org/paparazzi/) instead. (Yes, it's that good.)

## Download binaries
I do supply release builds for the Windows 7/8.x 64-bit platform. You need three things to get up and running:

0. [WebCap v0.3 executable](http://rv1109.1blu.de/webcap/webcap_0.3.rar). This is a very small download and the (only) one that you'll have to redownload with every new release. Scanned with [virustotal](https://www.virustotal.com/de/file/5c39973dea771426173845ef188e69f46bbf5831c2925e326ce7f70dd1025ed4/analysis/1428575844/).
0. [WebCap base libraries v2](http://rv1109.1blu.de/webcap/webcap_base_libs_v2.rar). This is a huge download and contains all DLLs (dependencies) that WebCap needs to run _except_ wkhtmltopdf itself. It'll rarely change. Extract this package into the same directory as the executable. Scanned with [virustotal](https://www.virustotal.com/de/file/1e475e9c2dea86dfb0edd2b8100329fed99df11ce678b3f39bd074ce81913d1d/analysis/1428576089/).
0. [wkhtmltopdf](http://wkhtmltopdf.org/downloads.html). Download the "Windows (MSVC2013) 64-bit" version and copy the `wkhtmltox.dll` into the same directory as the executable.

## Finally...
More documentation can be found in the `MANUAL.md` file.

Please keep in mind that this is still __work in progress__. Consider nothing as finished and __everything is subject to change__ until we reach v1.x.

