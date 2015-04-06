#ifndef GRAPHICSVIEW_HPP
#define GRAPHICSVIEW_HPP

#include <QGraphicsView>
#include <QString>

/**
 * We want to be able to drag and drop the previewed document from the
 * QGraphicsView widget. The way to do that is to derive from QGraphicsView
 * and implement mousePressEvent() ourselves.
 *
 * @see <a href="http://doc.qt.io/qt-5/dnd.html">Drag and Drop in Qt 5</a>
 */
class GraphicsView : public QGraphicsView
{
public:
   GraphicsView(QWidget *parent = 0);

   /** set filename for drag and drop operation
    *
    * The QGraphicsView widget does not know for which file's preview we're
    * using it. We cannot facilitate the desired drag and drop operation
    * without supplying the filename one way or the other.
    *
    * @param[in] filename the file that will be dragged and dropped
    */
   void setFilename(const QString &filename);

private:
   /** implementation for drag and drop
    *
    * @param event
    */
   void mousePressEvent(QMouseEvent *event);

   QString m_filename;   ///< filename for drag and drop
};

#endif // GRAPHICSVIEW_HPP
