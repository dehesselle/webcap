#ifndef GRAPHICSVIEW_HPP
#define GRAPHICSVIEW_HPP

#include <QGraphicsView>
#include <QString>

/**
 * We want to be able to drag and drop the previewed document from the
 * QGraphicsView widget. The way to do that is to derive from QGraphicsView
 * and implement mousePressEvent ourselves.
 */
class GraphicsView : public QGraphicsView
{
public:
   GraphicsView(QWidget *parent = 0);

   /** filename for drag and drop operation
    *
    * The QGraphicsView widget does not know for which file's preview we're
    * using it. We cannot facilitate the desired drag and drop operation
    * without supplying the filename one way or the other.
    *
    * @param[in] filename the filename that can be dragged and dropped
    */
   void setFilename(const QString &filename);

private:
   void mousePressEvent(QMouseEvent *event);

   QString m_filename;
};

#endif // GRAPHICSVIEW_HPP
