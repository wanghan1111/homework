#include "ImageDlg.h"
#include "ui_ImageDlg.h"

#include <QDir>
#include <QMessageBox>

#include "ZoonView.h"

#define MYDELETE(p)  {if (p) { delete   p; p = NULL;}}
#define MYDELETES(p) {if (p) { delete[] p; p = NULL;}}

//构造函数 初始化各成员变量值 初始化界面相关信息
ImageDlg::ImageDlg(QWidget *parent , bool bIsFlip) :
    QDialog(parent),
    ui(new Ui::ImageDlg)
{
    ui->setupUi(this);

    this->setStyleSheet("background: rgb(230, 230, 230)");

    ui->gbNavigation->setStyleSheet("background: transparent;");
    ui->lnDelimiter1->setStyleSheet("background: gray");
    ui->lnDelimiter2->setStyleSheet("background: gray");
    ui->lnDelimiter3->setStyleSheet("background: gray");
    ui->lnDelimiter4->setStyleSheet("background: gray");

    m_pScene = new QGraphicsScene;

    m_pView = new ZoonView(m_pScene, this);

    ui->gbNavigation->raise( );

    m_fScale = 1.0f;
    m_iRotate = 0;
    m_bIsFullScreen = false;
    m_bIsFit = false;

    m_byteCount = 0;
    m_sTitleImgW = 0;
    m_sTitleImgH = 0;
    m_sTitleFileName = "";
    m_sTitle = "";

    m_bIsFlip = bIsFlip;

    //释放子窗口资源
    this->setAttribute(Qt::WA_DeleteOnClose);
}

//析构函 释放内存
ImageDlg::~ImageDlg()
{
    delete m_pScene;
    delete m_pView;

    delete ui;
}

//图片显示函数
bool ImageDlg::openImage(const QString &sFileName)
{
    //加载失败返回false
    QImage image;
    if (!image.load(sFileName))
    {
        QMessageBox::information(this, tr("提示信息"), tr("不能打开图片"));
        return false;
    }

    m_sTitleImgW = image.width();     //图片的宽
    m_sTitleImgH = image.height();    //图片的高

    //    m_byteCount = image.byteCount();  //图片大小
    QFile file(sFileName);
    m_byteCount = file.size();  //图片大小

    QFileInfo imagePath(sFileName);
    m_sTitleFileName = imagePath.fileName();//文件标题名是文件名

    QDir dir(sFileName);
    dir.cdUp();

    //过滤掉别的格式文件
    QStringList filter;//
    filter << "*.bmp" << "*.png" << "*.jpg"  << "*.jpeg" <<"*.yuv";
    dir.setNameFilters(filter);

    m_fileinfo.clear();
    m_fileinfo = QList<QFileInfo>(dir.entryInfoList(filter));

    m_nImageIndex = m_fileinfo.indexOf(imagePath);

    //将图片加载显示到场景中
    m_pScene->setSceneRect(0, 0, m_sTitleImgW, m_sTitleImgH);
    m_pScene->addPixmap(QPixmap::fromImage(image));

    if (m_sTitleImgW < 1920)
    {
        zoomImage(m_fScale);
    }
    else
    {
        on_tbZoomFix_clicked();
    }

    return true;
}

//设置打开图片后的缩放比例
//设置打开图片的旋转角度
//设置窗口标题栏文字内容
void ImageDlg::zoomImage(float fScale)
{
    QMatrix matrix;
    matrix.scale(fScale, fScale);

    m_pView->setMatrix(matrix);

    m_pView->rotate(m_iRotate);

    float fScaleW = this->geometry().width() / m_pScene->width();
    float fScaleH = this->geometry().height() / m_pScene->height();

    if (fScale <= (fScaleW < fScaleH ? fScaleW : fScaleH))
    {
        m_pView->setDragMode(QGraphicsView::RubberBandDrag);
    }
    else
    {
        m_pView->setDragMode(QGraphicsView::ScrollHandDrag);
    }

    //在标题栏显示图片信息
    m_sTitle.clear();

    m_sTitle.append("  看图助手\t\t");
    m_sTitle.append(m_sTitleFileName + "\t\t");
    m_sTitle.append(QString("%1×").arg(m_sTitleImgW / 1.0, 0, 'f', 0));
    m_sTitle.append(QString("%1\t\t").arg(m_sTitleImgH / 1.0, 0, 'f', 0));

    QString sByteCount = (m_byteCount >= 1024 *1024)
            ? (QString("%1MB\t\t").arg(m_byteCount / 1024.0 / 1024.0, 0, 'f', 2))
            : (QString("%1KB\t\t").arg(m_byteCount / 1024.0, 0, 'f', 2));

    m_sTitle.append(sByteCount);
    m_sTitle.append(QString("%1%").arg(fScale * 100, 0, 'f', 0));

    setWindowTitle(m_sTitle);
}

//视图窗口随场景窗口变化而变化
void ImageDlg::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    m_pView->resize(this->geometry().width(), this->geometry().height());
}

//视图鼠标双击图片退出全屏函数
void ImageDlg::viewMouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        myFullScreenImage();
        m_bIsFullScreen = !m_bIsFullScreen;
        m_bIsFit = true;
        ui->gbNavigation->setVisible(true);
    }
    event->setAccepted(false);
}

//视图滑轮滚动函数 滑轮滚动图片相应的缩放
void ImageDlg::viewWheelEvent(QWheelEvent *event)
{
    this->m_fScale *= (event->delta() > 0 ? 1.1f : 0.90909091f);
    if (this->m_fScale < 0.1f) this->m_fScale = 0.1f;
    if (this->m_fScale > 8) this->m_fScale = 8;

    this->m_pView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    this->zoomImage(this->m_fScale);
    ui->gbNavigation->setVisible(true);
}

//视图定时器事件, 定时器每隔1.5s, 隐藏控件按钮
void ImageDlg::viewTimerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_pView->m_timerId)  ui->gbNavigation->setVisible(false);

    event->setAccepted(false);
}

//视图鼠标移动事件函数 鼠标移动显示控件
void ImageDlg::viewMouseMove(QMouseEvent *event)
{
    Q_UNUSED(event)
    ui->gbNavigation->setVisible(true);
    this->m_pView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    event->setAccepted(false);
}

//图片全屏显示功能代码
void ImageDlg::myFullScreenImage()
{
    if ( !m_bIsFullScreen)
    {
        this->showMaximized();

        //判断旋转角度来确定图片适应屏幕的缩放比例
        if (m_iRotate == 0 || m_iRotate == 180 || m_iRotate == 360 || m_iRotate == -180 || m_iRotate == -360)
        {
            //图片的宽高放大比例
            float fScaleW = 1920 / m_pScene->width();
            float fScaleH = 1080 - 30 / m_pScene->height();

            m_fScale = ((fScaleW < fScaleH) ? fScaleW : fScaleH);
        }
        else
        {
            //图片的宽高放大比例
            float fScaleW = 1080 - 30 / m_pScene->width();
            float fScaleH = 1920 / m_pScene->height();

            m_fScale = ((fScaleW < fScaleH) ? fScaleW : fScaleH);
        }

        m_pView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
        zoomImage(m_fScale);
    }
    else
    {
        this->showNormal();
        on_tbZoomOriginal_clicked();//退出全屏后图片为原图
    }
}

//图片切换功能代码
void ImageDlg::myImageSwitcher()
{
    //判断Qt中Image图片加载失败或者不是yuv文件
    QImage image;
    if (!(image.load(m_fileinfo.at(m_nImageIndex).filePath())) &&
            !(m_fileinfo.at(m_nImageIndex).filePath().right(4) == ".yuv") )
    {
        QMessageBox::information(this, tr("提示信息"), tr("不能打开图片"));
        return;
    }

    QFileInfo imagePath(m_fileinfo.at(m_nImageIndex).filePath());
    m_sTitleFileName = imagePath.fileName();

    //切换到下一张后将旋转角度、缩放比例、是否适屏，还原为初始化值(非全屏状态下)
    m_iRotate = 0;
    m_bIsFit = false;
    m_sTitleImgW = image.width();   //图片的宽
    m_sTitleImgH = image.height();  //图片的高
    m_byteCount = image.byteCount();  //图片大小

    m_pScene->clear();
    m_pScene->setSceneRect(0, 0, m_sTitleImgW, m_sTitleImgH);

    m_pScene->addPixmap(QPixmap::fromImage(image));

    if (!m_bIsFullScreen)
    {
        if (m_sTitleImgW < 1920)
        {
            m_fScale = 1.0f;
            zoomImage(m_fScale);
        }
        else
        {
            on_tbZoomFix_clicked();
        }
    }
    else //全屏状态下切换的图片按比例放大
    {
        //图片的宽高放大比例
        float fScaleW = 1920 / m_pScene->width();
        float fScaleH = 1080 / m_pScene->height();

        m_fScale = ((fScaleW < fScaleH) ? fScaleW : fScaleH);
        on_tbZoomFix_clicked();
    }
}

//放大图片
void ImageDlg::on_tbZoomIn_clicked()
{
    m_fScale *= 1.1f;
    if (m_fScale > 8) m_fScale = 8;

    m_pView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    zoomImage(m_fScale);
}

//缩小图片
void ImageDlg::on_tbZoomOut_clicked()
{
    m_fScale *= 0.90909091f;
    if (m_fScale < 0.1f) m_fScale = 0.1f;

    m_pView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    zoomImage(m_fScale);
}

//图片适应屏幕
void ImageDlg::on_tbZoomFix_clicked()
{
    //判断旋转角度来确定图片适应屏幕的缩放比例
    if (m_iRotate == 0 || m_iRotate == 180 || m_iRotate == 360 || m_iRotate == -180 || m_iRotate == -360)
    {
        float fScaleW = this->geometry().width() / m_pScene->width();
        float fScaleH = this->geometry().height() / m_pScene->height();
        m_fScale = ((fScaleW < fScaleH) ? fScaleW : fScaleH);
    }
    else
    {
        float fScaleW = this->geometry().height() / m_pScene->width();
        float fScaleH = this->geometry().width() / m_pScene->height();
        m_fScale = ((fScaleW < fScaleH) ? fScaleW : fScaleH);
    }
    m_bIsFit = true;//判断是否是全屏状态
    m_pView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    zoomImage(m_fScale);
}

//图片还原到100%
void ImageDlg::on_tbZoomOriginal_clicked()
{
    if (m_bIsFit) m_bIsFit = !m_bIsFit;
    m_fScale = 1.0f;

    m_pView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    zoomImage(m_fScale);
}

//显示前一张图片
void ImageDlg::on_tbNavigationPrevious_clicked()
{
    if (!m_bIsFlip) return;
    if (m_fileinfo.count() == 0)  return ;

    m_nImageIndex = (m_nImageIndex > 0 ? m_nImageIndex-1 : m_fileinfo.count()-1);

    myImageSwitcher();
}

//显示后一张一张图片
void ImageDlg::on_tbNavigationNext_clicked()
{
    if (!m_bIsFlip) return;
    if (m_fileinfo.count() == 0) return;
    m_nImageIndex++;
    if (m_nImageIndex == m_fileinfo.count()) m_nImageIndex = 0;

    myImageSwitcher();

}

//图片向左旋转
void ImageDlg::on_tbRotateLeft_clicked()
{
    m_iRotate -= 90;
    if (m_iRotate <= -360) m_iRotate = 0;

    if (m_fScale != 1.0f && m_bIsFit)
    {
        if(m_iRotate == -180 || m_iRotate == -360 || m_iRotate == 0 || m_iRotate == 180 || m_iRotate == 360)
        {
            float fScaleW = this->geometry().width() / m_pScene->width();
            float fScaleH = this->geometry().height() / m_pScene->height();

            m_fScale = ((fScaleW < fScaleH) ? fScaleW : fScaleH);
        }
        else
        {
            float fScaleW = this->geometry().height() / m_pScene->width();
            float fScaleH = this->geometry().width() / m_pScene->height();

            m_fScale = ((fScaleW < fScaleH) ? fScaleW : fScaleH);
        }
    }

    zoomImage(m_fScale);
}

//图片向右旋转
void ImageDlg::on_tbRotateRight_clicked()
{
    m_iRotate += 90;
    if (m_iRotate >= 360) m_iRotate = 0;

    if (m_fScale != 1.0f && m_bIsFit)
    {
        if(m_iRotate == -180 || m_iRotate == -360 || m_iRotate == 180 || m_iRotate == 360 || m_iRotate == 0)
        {
            float fScaleW = this->geometry().width() / m_pScene->width();
            float fScaleH = this->geometry().height() / m_pScene->height();

            m_fScale = ((fScaleW < fScaleH) ? fScaleW : fScaleH);
        }
        else
        {
            float fScaleW = this->geometry().height() / m_pScene->width();
            float fScaleH = this->geometry().width() / m_pScene->height();

            m_fScale = ((fScaleW < fScaleH) ? fScaleW : fScaleH);
        }
    }

    zoomImage(m_fScale);
}

//图片显示全屏
void ImageDlg::on_tbFullScreenImage_clicked()
{
    myFullScreenImage();
    m_bIsFullScreen = !m_bIsFullScreen;
    m_bIsFit = true;
}
