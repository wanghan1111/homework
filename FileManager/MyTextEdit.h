#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QTextEdit>
#include <QWheelEvent>
#include <QMouseEvent>

class TextDlg;

class MyTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit MyTextEdit(QWidget* parent = 0);

    TextDlg* m_TextBrowser;

    void wheelEvent(QWheelEvent* event);
    void mousePressEvent(QMouseEvent* event);

signals:

public slots:

};

#endif // MYTEXTEDIT_H
