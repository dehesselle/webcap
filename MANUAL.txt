# TOC

1. How to use this program
2. Known Issues and Limitations
3. other random stuff
4. Roll credits!

## How to use this program

todo

## Issues and Limitations

* The GUI can/will freeze during a capture. That is an expected behaviour and have to live with that for the time being. (reason: wkhtmltopdf runs in main thread only)

* links and wkhtmltopdf  todo

* todo

## other random stuff
### What about multi-platform?
WebCap has been created with the Windows platform in mind, but I guess using only Qt and freely available libraries is supposed to bestow the multi-platform capability upon it. That is, if I didn't do some platform dependent ~~shit~~ stuff in the code that I'm currently unaware of (keep your fingers crossed!). I'll be taking a closer look into this matter at some later point in the future.

## Roll credits!

So, I am not responsible for the magic that is happening here. In fact, most of the heavy lifting is done by [wkhtmltopdf](http://wkhtmltopdf.org/) to render websites to PDF (using WebKit) and [Poppler](http://poppler.freedesktop.org/) to render a PDF into an image (for the preview).

todo

