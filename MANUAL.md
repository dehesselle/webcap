# TOC
1. How to use this program
2. Known issues and limitations
3. Other random stuff
4. Roll credits!

## How to use this program
### meant to be left alone/in background
WebCap is meant to be used "indirectly". That means that once you have configured it to your liking (e.g. where to put the PDF files), you minimize it to the taskbar and do not need to touch it in order to capture a website to PDF. In fact, there's no point in trying to do something with WebCap itself to capture a website, since that's just not how it was designed to work. It is supposed to not interrupt your work and to not burden you with having to learn new special shortcuts etc.

### monitors clipboard for URLs
WebCap monitors your clipboard. If you _quickly_ copy the same URL to the clipboard twice in a row (i.e. press <kbd>Ctrl</kbd>+<kbd>c</kbd> twice), it will interpret this action as "please capture this website to PDF". And that's it. 

## Known issues and limitations
* possible GUI freeze during capture
* layout/fonts/etc. not guaranteed to be 100% exactly like source
* not all links will work

todo explain more

## Other random stuff
* I guess my initial research was flawed - I eventually found a similar program (i.e. another GUI for wkhtmltopdf). It's called [Web2Pdf](http://cuteapps.blogspot.de/2010/11/web2pdf-convert-webpages-to-pdf-ebooks.html) and it has been around since 2010 - long before I even knew about wkhtmltopdf itself. It follows a different approach in comparison to WebCap, so go check it out.

## Roll credits!
I am not responsible for the magic that is happening here. Most of the heavy lifting is done by [wkhtmltopdf](http://wkhtmltopdf.org/) to render websites to PDF (using WebKit) and [Poppler](http://poppler.freedesktop.org/) to render a PDF into an image (for the preview).

todo more credits

