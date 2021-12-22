#ifndef IMAGEVIEWERDLG_H
#define IMAGEVIEWERDLG_H

#include <QDialog>
#include <QFileInfo>
#include <QGraphicsScene>

class ZoonView;

namespace Ui {
class ImageDlg;
}

class ImageDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ImageDlg(QWidget *parent = 0, bool bIsFlip = true);
    virtual ~ImageDlg();

    bool openImage(const QString &sFileName);
    void zoomImage(float fScale);
    void viewMouseDoubleClickEvent(QMouseEvent *event);
    void viewWheelEvent(QWheelEvent *event);
    void viewTimerEvent(QTimerEvent *event);
    void viewMouseMove(QMouseEvent* event);

    void myFullScreenImage();   //图片全屏功能代码
    void myImageSwitcher();     //图片切换功能代码

    QGraphicsScene *m_pScene; //场景指针
    ZoonView *m_pView;       //场景上视图指针

private:
    Ui::ImageDlg *ui;

    QList<QFileInfo> m_fileinfo; //只存储QFileInfo类型的列表容器
    int m_nImageIndex;           //列表容器的索引

    QString m_sTitle;          //窗口标题
    QString m_sTitleFileName;  //标题文件名

    int m_sTitleImgW;     //图片的宽高
    int m_sTitleImgH;     //图片的宽高
    int m_byteCount;      //图片大小

    float m_fScale;            //缩放比例
    int m_iRotate;             //旋转角度
    bool m_bIsFit;             //判断是否适应屏幕
    bool m_bIsFullScreen;      //判断是否是全屏

    bool m_bIsFlip; //使否允许下一张, 上一张, true为允许, false为不允许

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void on_tbZoomIn_clicked();
    void on_tbZoomOut_clicked();

    void on_tbZoomFix_clicked();
    void on_tbZoomOriginal_clicked();

    void on_tbNavigationPrevious_clicked();
    void on_tbNavigationNext_clicked();

    void on_tbRotateLeft_clicked();
    void on_tbRotateRight_clicked();

    void on_tbFullScreenImage_clicked();
};

#endif // IMAGEVIEWERDLG_H
