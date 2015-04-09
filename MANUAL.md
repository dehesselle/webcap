# TOC
0. TL;DR? - Quickstart!
0. More detailed blabla
0. Known issues and limitations
0. Other random stuff
0. Roll credits!

## TL;DR? - Quickstart!
WebCap monitors your clipboard. If you _quickly_ copy the same URL to the clipboard twice in a row - i.e. press <kbd>Ctrl</kbd>+<kbd>c</kbd> twice - it will interpret this action as _"Please capture this website to PDF!"_. And that's it. Off you go!

## More detailed blabla
WebCap is meant to be used "indirectly". That means that once you have configured it to your liking - and there really isn't a lot of stuff to configure right now - you can minimize it to the taskbar and do not need to touch it in order to capture a website to PDF. In fact, there's no point in trying to do something with WebCap itself to capture a website, since that's just not how it was designed to work. It is supposed to not interrupt your work and to not burden you with having to learn new special shortcuts etc. 

So I came up with the idea of monitoring the clipboard for URLs as this is an easy common ground that would work out of the box with pretty much every program (that supports copy & paste). But how to filter out "URLs I want to capture" from "URLs I don't want to capture"? Well, we could monitor the clipboard for an URL that has been copied _twice_ in a row to the clipboard within a defined, short time span. That'd be easy to remember and does not deviate too much from such common things as copying stuff to the clipboard. And WebCap was born!

Besides that, there is not really a long list of features:

* You can configure the target directory (i.e. where to put PDF files), the time span between two "copy to the clipboard"-events and a proxy server in the preferences dialog.
* All captured websites / PDF files appear color-coded in the listbox on the left. Pre-existing PDFs are light gray, erroneous PDFs are red (those files are 0 bytes in size as result of an unsuccessful capture) and newly created/captured PDFs are black.
* Selecting a PDF from the list will preview its first page in the box on the right.
* Doubleclicking a PDF in the listbox will open it in the system's default PDF viewer.
* Dragging from the preview window will copy the selected PDF to the location where you drop it.

## Known issues and limitations
* The overall size of the release build (WebCap and all required DLLs) is huge! The toolchain I'm using is pulling in a lot of dependencies by default and I'd have to recompile quite a few packages and tweak some options to reduce the number of files. I'll take a look into this eventually, but it is of lowest priority to me.
* Although I'm using a GCC toolchain myself and not MSVC, WebCap only works with the MSVC version of wkhtmltopdf due to conflicting DLLs between their toolchain and mine. No biggie, but anyhow.
* The GUI can and will freeze if a capture takes an elongated period of time. Granted, it's not pretty when you see the "program doesn't seem to respond"-message from Windows, but we'll have to live with it for the time being. It's a technical issue; we cannot decouple wkhtmltopdf from the main thread right now.
* wkhtmltopdf does a great job, but like any other software, it is not perfect. So there's no guarantees that a successful capture yields in a PDF with 100% exactly the same layout/fonts/etc. like the original source material. YMMV on each and every website.
* Not all links in the resulting PDF will work. 
* During capture, the progress bar will fill up and empty a couple of times. That is "works as designed" since the capture process consists of sub-processes, each one going from 0 to 100% completion.
* WebCap does currently not support all the configuration options that wkhtmltopdf has to offer. (Because I don't need them.)
* WebCap only works if you copy one single URL to the clipboard. Nothing more, nothing less.

## Other random stuff
* I guess my initial research was flawed - I eventually found a similar program (i.e. another GUI for wkhtmltopdf). It's called [Web2Pdf](http://cuteapps.blogspot.de/2010/11/web2pdf-convert-webpages-to-pdf-ebooks.html) and it has been around since 2010 - long before I even knew about wkhtmltopdf itself. It follows a different approach in comparison to WebCap, so you might want to check it out.

## Roll credits!
I am not responsible for the magic that is happening here. The heavy lifting is done by [wkhtmltopdf](http://wkhtmltopdf.org/) to render websites to PDF and [Poppler](http://poppler.freedesktop.org/) to render a PDF into an image. 

A more detailed list can be found in the "About WebCap" window.
