#include "RenameDlg.h"
#include "ui_RenameDlg.h"

#include <QMessageBox>

RenameDlg::RenameDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RenameDlg)
{
    ui->setupUi(this);
}

RenameDlg::~RenameDlg()
{
    delete ui;
}

void RenameDlg::setRename(QString s)
{
    ui->leRenameEdit->setText(s);
    m_sOldName = s;
}

//键盘重新重命名之后，返回给FileManageDlg
QString RenameDlg::RenameInformation()
{
    QString str = ui->leRenameEdit->text();
    if (str.isEmpty()) str = m_sOldName;
    return str;
}

//清空重命名的内容
void RenameDlg::on_pbClean_clicked()
{
    ui->leRenameEdit->clear();
}

//确认重命名
void RenameDlg::on_pbOk_clicked()
{
    QString ret = ui->leRenameEdit->text();
    if (ret.isEmpty()) ret = m_sOldName;

    QString retSuffix = "";
    if (ret.contains(".")) retSuffix = ret.right(ret.size() - ret.lastIndexOf("."));

    QString OldSuffix = "";
    if (m_sOldName.contains("."))OldSuffix = m_sOldName.right(m_sOldName.size() - m_sOldName.lastIndexOf("."));

    if (OldSuffix != "" && retSuffix == "")
    {
        QMessageBox box(QMessageBox::Question
                        , "温馨提示", "原文件有后缀名称,是否改变文件后缀"
                        , QMessageBox::Cancel | QMessageBox::Ignore | QMessageBox::Ok
                        , this);
        box.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ignore | QMessageBox::Ok);
        box.setDefaultButton(QMessageBox::Ok);
        box.setButtonText(QMessageBox::Ok, "替换");
        box.setButtonText(QMessageBox::Cancel, "取消");
        box.setButtonText(QMessageBox::Ignore, "自动补上后缀");

        int selectNum = box.exec();

        if (selectNum == QMessageBox::Ok)
        {
            this->close();
        }
        else if (selectNum == QMessageBox::Cancel)
        {
            return;
        }
        else if (selectNum == QMessageBox::Ignore)
        {
            ui->leRenameEdit->setText(ret + OldSuffix);
            return;
        }
    }

    this->close();
}
