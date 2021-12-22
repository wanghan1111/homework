#include "TextDlg.h"
#include "ui_TextDlg.h"

TextDlg::TextDlg(QWidget *parent, QString sFilePath) :
    QDialog(parent),
    ui(new Ui::TextDlg)
{
    ui->setupUi(this);

    m_bIsMidPress = false;  //判断中键是否按下

    openFile(sFilePath);

    //如果调用形式为非模态窗口则执行下列代码用于结束后释放资源,若是模态窗口则不需要主动释放资源
    if (this->isVisible()) this->setAttribute(Qt::WA_DeleteOnClose);//调用释放窗口资源}
}

TextDlg::~TextDlg()
{
    delete ui;
}

//打开加载文本显示函数
void TextDlg::openFile(const QString & filePath)
{
    //后缀不区分大小写
    QString suff = filePath.right(4);
    Qt::CaseSensitivity cs = Qt::CaseInsensitive;
    if ((suff.compare(".ini", cs) == 0)
            || (suff.compare(".bak", cs) == 0)
            || (suff.compare(".txt", cs) == 0)
            || (suff.right(2).compare(".h", cs) == 0)
            || (suff.compare(".cpp", cs) == 0)
            || (suff.right(3).compare(".sh", cs) == 0))
    {
        QFile file(filePath);
        if (!file.open(QIODevice ::ReadOnly | QIODevice ::Text))
        {
            QMessageBox ::information(NULL, NULL, "Open file failed.");
            return;
        }

        QTextStream textStream(&file);
        while (!textStream.atEnd())
        {
            //QtextEdit按行显示文件内容
            ui->teTextEdit->append(textStream.readLine());
        }

        //滚动到头部
        ui->teTextEdit->moveCursor(QTextCursor::Start);

        setWindowTitle(filePath);
    }
    else
    {
        QMessageBox ::information(NULL, NULL, "Can't open the file.");
        this->close();
    }
}

//文本框随窗口变化而变化
void TextDlg::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    ui->teTextEdit->setGeometry(0, 0, this->geometry().width(), this->geometry().height());
    ui->teTextEdit->resize(this->geometry().width(), this->geometry().height());
}

//响应控件的中键按下消息
void TextDlg::MyTextEditMousePress(QMouseEvent* event)
{
    if (event->button() == Qt::MidButton) m_bIsMidPress = !m_bIsMidPress;
    event->setAccepted(false);
}

//响应控件的滑轮消息
void TextDlg::MyTextEditMouseWheel(QWheelEvent* event)
{
    Q_UNUSED(event);
    if(m_bIsMidPress)
    {
        if (event->delta() > 0)
        {
            ui->teTextEdit->zoomIn();    //设置文字的大小
        }
        else
        {
            ui->teTextEdit->zoomOut();   //设置文字的大小
        }
    }
    else
    {
        event->setAccepted(false);
    }
}
