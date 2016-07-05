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
~~I do supply release builds for the Windows 7/8.x 64-bit platform.~~  

## Finally...
More documentation can be found in the `MANUAL.md` file.

Please keep in mind that this is still __work in progress__. Consider nothing as finished and __everything is subject to change__ until we reach v1.x.
