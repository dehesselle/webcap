#include "GraphicsView.hpp"
#include <QDrag>
#include <QMimeData>
#include <QList>
#include <QUrl>
#include <QMouseEvent>

GraphicsView::GraphicsView(QWidget *parent) :
   QGraphicsView(parent)
{
}

void GraphicsView::setFilename(const QString &filename)
{
   m_filename = filename;
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
   if (event->button() == Qt::LeftButton)
   {
      QList<QUrl> urls;
      urls.append(QUrl::fromUserInput(m_filename));

      QMimeData *mimeData = new QMimeData;
      mimeData->setUrls(urls);

      QDrag *drag = new QDrag(this);
      drag->setMimeData(mimeData);
      drag->exec();
   }
}

