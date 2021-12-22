#include "MyTextEdit.h"
#include "TextDlg.h"

MyTextEdit::MyTextEdit(QWidget *parent) :
    QTextEdit(parent)
{
    m_TextBrowser = (TextDlg*)parent;
}

//鼠标滑轮消息转到文本浏览器类中处理
void MyTextEdit::wheelEvent(QWheelEvent *event)
{
    m_TextBrowser->MyTextEditMouseWheel(event);
    if (!event->isAccepted()) QTextEdit::wheelEvent(event);
}

//鼠标按下消息转到文本浏览器类中处理
void MyTextEdit::mousePressEvent(QMouseEvent *event)
{
    m_TextBrowser->MyTextEditMousePress(event);
    if (!event->isAccepted()) QTextEdit::mousePressEvent(event);
}
