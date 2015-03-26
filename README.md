# WebCap - capture websites to PDF files

WebCap is a screenshot utility that renders websites to PDF files. It was inspired by the excellent [Paparazzi!](https://derailer.org/paparazzi/), which is an OS X only tool. Since similar technology is available on Windows, too, but yet noone seems to have plugged it all together in similar fashion, I went ahead and created WebCap. 

## Why would you want to use it?

Utilizing the power of [wkhtmltopdf](http://wkhtmltopdf.org), capturing a website with WebCap gives you the following advantages:

* The layout of the website is kept alive in vectorized form. It scales up and down without any loss of quality.
* You retain access to the text (e.g. copy/paste); the PDF can be indexed by full-text search thingies.
* Links stay alive and clickable inside the PDF. _(Restrictions do apply, see manual.)_
* The PDF file will most probably be less in size than a (high resolution) bitmap-based image. 

Still, if you have a Mac, go use [Paparazzi!](https://derailer.org/paparazzi/) instead. (Yes, it is that good.)

## Download binaries

I do supply release builds for the Windows x64 platform. You can find them ... __TODO__.

## Finally...
More documentation can be found in the `MANUAL.md` file.

Please keep in mind that this is still __work in progress__. Consider nothing as finished and __everything is subject to change__ until we reach v1.x.

