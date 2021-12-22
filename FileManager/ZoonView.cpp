#include "ZoonView.h"
#include "ImageDlg.h"

ZoonView::ZoonView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
{
    m_timerId = this->startTimer(1500);//开始初始化定时器
    m_pImageViewer = (ImageDlg *)parent;

    this->setDragMode(QGraphicsView::RubberBandDrag);
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //设置视图大小
    this->setGeometry(0, 0, m_pImageViewer->geometry().width(), m_pImageViewer->geometry().height());
}

//鼠标滚轮事件
void ZoonView::wheelEvent(QWheelEvent *event)
{
    m_pImageViewer->viewWheelEvent(event);
    if (!event->isAccepted()) QGraphicsView::wheelEvent(event);
}

//鼠标双击退出全屏
void ZoonView::mouseDoubleClickEvent(QMouseEvent *event)
{
    m_pImageViewer->viewMouseDoubleClickEvent(event);
    if (!event->isAccepted()) QGraphicsView::mouseDoubleClickEvent(event);
}

//鼠标移动事件
void ZoonView::mouseMoveEvent(QMouseEvent *event)
{
    m_pImageViewer->viewMouseMove(event);
    if (!event->isAccepted()) QGraphicsView::mouseMoveEvent(event);
}

//定时器事件
void ZoonView::timerEvent(QTimerEvent *event)
{
    m_pImageViewer->viewTimerEvent(event);
    if (!event->isAccepted()) QGraphicsView::timerEvent(event);
}

