WebCap - capture websites to PDF files
======================================

WebCap is a screenshot utility that renders websites to PDF files. It was inspired by the excellent [Paparazzi!](https://derailer.org/paparazzi/), which is an OS X only tool. Since similar technology like the one behind Paparazzi! is available on Windows, too, but yet noone seems to have it all plugged together in similar fashion, I went ahead and created WebCap. 

The most important building blocks its based on are [wkhtmltopdf](http://wkhtmltopdf.org/) to render websites to PDF (using WebKit) and [Poppler](http://poppler.freedesktop.org/) to render a PDF into an image (so we can show a preview of the PDF). Also, I've been having a blast using [Qt](http://qt-project.org/) and Qt Creator; I can't believe it took so long to finally get acquainted with Qt. 

WebCap has been created with the Windows platform in mind, but I guess using only Qt and freely available libraries is supposed to bestow the multi-platform capability upon it. That is, if I didn't do some platform dependent ~~shit~~ stuff in the code that I'm currently unaware of (keep your fingers crossed!). I'll be taking a closer look into this matter at some later point in the future.

Why use WebCap?
---------------

So why am I making such a big fuss about capturing websites to PDF files instead of just doing what a lot of other screenshot utilities can do (e.g. PNG)?

* The layout of the website is kept alive in vectorized form. It scales up and down without any loss of quality.
* You retain access to the text (e.g. copy/paste), the PDF can be indexed by full-text search thingies.
* Links stay alive and clickable inside the PDF. (Well, at least to some large extent - wkhtmltopdf is great, but not perfect.)
* The PDF file will most probably be less in size than a (high resolution) bitmap-based image. 

Still, if you have a Mac, go use Paparazzi! instead.

How to use WebCap?
------------------

Ehm... I'll explain later. I promise.

