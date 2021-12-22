#ifndef FILETHREAD_H
#define FILETHREAD_H

#include <QObject>
#include <QThread>
#include <QStringList>

#include <QObject>
#include <QThread>
#include <QStringList>
#include <QFileSystemModel>

class FileThread : public QThread
{
    Q_OBJECT

public:
    FileThread(QObject *parent = NULL);

private:
    void run();//多线程启动的地方
    void copyAndCut();//复制或者剪切的具体操作

    bool stringIsNumber(QString str);   //判断字符串是否是全数字
    bool dirOrFileIsExistsCurrentPath(QString dirName, QString currentPath);//判断文件夹是否存在当前路径下
    bool copyDirectoryFiles(QDir &fromDir, QDir &toDir, bool coverFileIfExist);   //拷贝文件夹
    bool copyDirectory(const QString &fromDir, const QString &toDir, bool coverFileIfExist);

public:

    //设置拷贝和剪切参数
    inline void setCopyAndCutParam(
              QStringList sSourcePath
            , int iOperation
            , QFileSystemModel* pModel
            , QString sReallyPath)
    {
        m_sSourcePath = sSourcePath;
        m_iOperation = iOperation;
        m_sReallyPath = sReallyPath;
        m_pModel = pModel;
    }

signals:
    void copyAndCutFinished();//拷贝或者剪切完成信号
    void chooseCoverFileOrDir(bool bDir, QString sNameSrc);//选择覆盖已存在的文件或者文件夹信号

private slots:
    void onChooseCoverFinished(int bCover);//选择是否覆盖完成槽函数

private:
    QStringList m_sSourcePath;//源文件全路径列表

    QString m_sReallyPath;//当前路径
    QFileSystemModel* m_pModel;//文件模型

    int m_iOperation;//操作 0:复制, 1:剪切
    int m_iCoverType;//1:覆盖  2:覆盖应用到所有程序中 -1:取消
};
#endif // FILETHREAD_H
