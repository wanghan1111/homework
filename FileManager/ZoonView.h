#ifndef IMAGEVIEWGRAPHICS_H
#define IMAGEVIEWGRAPHICS_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QTimerEvent>

class ImageDlg;
class ZoonView : public QGraphicsView
{
public:
    ZoonView(QGraphicsScene *scene, QWidget *parent);
    int m_timerId;

protected:
    void wheelEvent(QWheelEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void timerEvent( QTimerEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    ImageDlg *m_pImageViewer;
};

#endif // IMAGEVIEW_H
