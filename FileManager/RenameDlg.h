#ifndef RENAMEDLG_H
#define RENAMEDLG_H

#include <QDialog>

namespace Ui {
class RenameDlg;
}

class RenameDlg : public QDialog
{
    Q_OBJECT

public:
    explicit RenameDlg(QWidget *parent = 0);
    virtual ~RenameDlg();

    void setRename(QString s);
    QString RenameInformation();

private slots:
    void on_pbClean_clicked();
    void on_pbOk_clicked();

private:
    Ui::RenameDlg *ui;

private:
    QString m_sOldName;
};

#endif // RENAMEDLG_H
