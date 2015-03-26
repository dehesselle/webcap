# TOC
1. How to use this program
2. Known issues and limitations
3. Other random stuff
4. Roll credits!

## How to use this program
### meant to be run in background
WebCap is meant to be used "indirectly". That means that once you have configured it to your liking (e.g. where to put the PDF files), you minimize it to the taskbar and do not need to touch it in order to capture a website to PDF. In fact, there's no point in trying to do something with WebCap itself to capture a website, since that's just not how it's designed to work.

### monitors clipboard for URLs
WebCap monitors your clipboard. If you quickly copy the same URL to the clipboard twice in a row (i.e. press <kbd>Ctrl</kbd>+<kbd>c</kbd> twice), it will interpret this action as "please capture this website to PDF". That's it.

## Known issues and limitations
* possible GUI freeze during capture
* layout not guaranteed to be 100% like source
* not all links will work

todo explain more

## Roll credits!
I am not responsible for the magic that is happening here. Most of the heavy lifting is done by [wkhtmltopdf](http://wkhtmltopdf.org/) to render websites to PDF (using WebKit) and [Poppler](http://poppler.freedesktop.org/) to render a PDF into an image (for the preview).

todo more credits


