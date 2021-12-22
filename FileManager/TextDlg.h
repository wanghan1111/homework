#ifndef TEXTBROWSERDLG_H
#define TEXTBROWSERDLG_H

#include <QDialog>
#include <QFile>
#include <QTextEdit>
#include <QTextStream>
#include <QMessageBox>
#include <QWheelEvent>
#include <QMouseEvent>

namespace Ui {
class TextDlg;
}

class TextDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TextDlg(QWidget *parent = 0, QString sFilePath = QString(""));
    virtual ~TextDlg();

    void openFile(const QString &filePath);
    void MyTextEditMousePress(QMouseEvent *event);      //响应控件的中键按下消息
    void MyTextEditMouseWheel(QWheelEvent *event);      //响应控件的滑轮消息

protected:
    void resizeEvent(QResizeEvent *event);

private:
    Ui::TextDlg *ui;
    bool m_bIsMidPress;     //判断中键是否按下
};

#endif // TEXTBROWSERDLG_H
