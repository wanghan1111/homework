#include "FileThread.h"

#include <QDir>
#include <QFileInfo>
#include <QModelIndex>

// FileThread构造函数，初始化成员变量
FileThread::FileThread(QObject *parent)
{
    Q_UNUSED(parent)
    m_iCoverType = 0;
}

// 多线程启动的地方
void FileThread::run()
{
    //复制或者剪切的粘贴操作
    this->copyAndCut();

    //复制或者剪贴完成
    emit copyAndCutFinished();
}

// 复制或者剪切的具体操作
void FileThread::copyAndCut()
{
    //1. 遍历源路径
    int iPathCount = m_sSourcePath.count();
    if (iPathCount == 0) return;

    for (int i = 0; i < iPathCount; ++i)
    {
        bool bCover = true;
        QString sPathSrc = m_sSourcePath.at(i);
        QString sNameSrc = sPathSrc.mid(sPathSrc.lastIndexOf("/") + 1);
        QModelIndex tempIndex = m_pModel->index(sPathSrc);
        QFileInfo tempInfo = m_pModel->fileInfo(tempIndex);
        bool bFile = tempInfo.isFile();
        QString sPathDes = m_sReallyPath + "/" + sNameSrc;

        //2. 在当前文件夹内进行文件复制操作 m_iOperation == 0 复制文件
        if (sPathSrc == sPathDes && m_iOperation == 0)
        {
            //2.1 如果存在当前文件, 就修改名称为(copyx)
            while (dirOrFileIsExistsCurrentPath(sPathDes, m_sReallyPath))
            {
                int iNum = -1;
                QString sSuffix = "";
                QString sFileName = sPathDes;

                //2.2 获取文件或文件夹的名称
                if (bFile)
                {
                    int iIndex = sFileName.lastIndexOf(".");     //init.sh
                    if (iIndex != -1) sSuffix = sFileName.mid(iIndex);
                    if (sSuffix != "") sFileName = sFileName.left(iIndex);
                }

                //2.3 获取文件(copyx) x的编号
                QString sCopy = (sFileName.right(1) == ")") ? sFileName.mid(sFileName.lastIndexOf("(")) : "";          //(copy1)
                if (sCopy != "" && sCopy.left(5) == "(copy")
                {
                    QString sNum = sCopy.mid(5);        //1)
                    sNum = sNum.mid(0, sNum.size() - 1);
                    bool bNum = stringIsNumber(sNum);

                    iNum = bNum ? sNum.toInt() : -1;
                }

                //2.4 生成文件的全路径
                sPathDes = (iNum == -1) ? (sNameSrc.contains(".")
                                           ? (sPathDes.insert(sPathDes.lastIndexOf('.'), QString("(copy%1)").arg(1)))
                                           : (sPathDes + QString("(copy%1)").arg(1)))
                                        : sPathDes.left(sPathDes.lastIndexOf("(")) + QString("(copy%1)").arg(++iNum) + sSuffix;
            }
        }
        else
        {
            //2.5 如果当前文件存在 弹出是否覆盖选择对话框 m_iCoverType: -1:表示取消  1:表示覆盖 2:表示应用到所有文件
            bool bHave = this->dirOrFileIsExistsCurrentPath(sNameSrc, m_sReallyPath);
            if (bHave && m_iCoverType != 2)
            {
                m_iCoverType = 0;
                emit chooseCoverFileOrDir(!bFile, sNameSrc);        //发信号, 在FileManageDlg弹出选择覆盖对话框
                while (m_iCoverType == 0)
                {
                    msleep(1);
                    if (m_iCoverType != 0) break;
                }
            }
            if (m_iCoverType == -1) break;

            //2.6 如果在当前目录下剪切文件粘贴, 直接不做任何操作就行, bCover:false表示不覆盖文件, 即不做操作
            if (m_iOperation == 1 && bHave && sPathSrc == sPathDes) bCover = false;
        }

        //3. 复制文件或这文件夹
        QFileInfo info(sPathSrc);
        if (info.isDir())
        {
            QDir dir1(sPathSrc);
            QDir dir2(sPathDes);
            this->copyDirectoryFiles(dir1, dir2, bCover);
        }
        else
        {
            QFile::copy(sPathSrc, sPathDes);
        }

        //4. 如果不在本目录进行剪切粘贴, 选择覆盖的话, 要把源文件删除
        if (m_iOperation == 1 && sPathSrc != sPathDes) m_pModel->remove(m_pModel->index(sPathSrc));
    }

    m_iCoverType = 0;
}

//判断字符串是不是全部都是数字
bool FileThread::stringIsNumber(QString str)
{
    QByteArray ba = str.toLatin1(); //将QString转换成char*
    const char *s = ba.data();
    while(*s && *s >= '0' && *s <= '9') s++;
    return ((*s) == 0);
}

//判断字符串是不是全部都是数字
void FileThread::onChooseCoverFinished(int bCover)
{
    m_iCoverType = bCover;
}

//拷贝文件夹：
bool FileThread::copyDirectory(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
{
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);

    //如果目标目录不存在，则进行创建
    if(!targetDir.exists())
    {
        if(!targetDir.mkdir(targetDir.absolutePath()))return false;
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..") continue;

        //当为目录时，递归的进行copy
        if(fileInfo.isDir())
        {
            if(!copyDirectory(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName()),coverFileIfExist)) return false;
        }
        else
        {
            //当允许覆盖操作时，将旧文件进行删除操作
            if(coverFileIfExist && targetDir.exists(fileInfo.fileName()))
            {
                targetDir.remove(fileInfo.fileName());
            }

            //进行文件copy
            if(!QFile::copy(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName())))
            {
                    return false;
            }
        }
    }
    return true;
}

//拷贝文件夹
bool FileThread::copyDirectoryFiles(QDir &fromDir, QDir &toDir, bool coverFileIfExist)
{
    if (!toDir.exists())
    {
        if (!toDir.mkdir(toDir.absolutePath())) return false;
    }

    QFileInfoList fileInfoList = fromDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if (fileInfo.fileName() == "." || fileInfo.fileName() == "..") continue;

        if (fileInfo.isDir())
        {
            if (!copyDirectory(fileInfo.filePath(), toDir.filePath(fileInfo.fileName()), coverFileIfExist)) return false;
        }
        else
        {
            if (coverFileIfExist && toDir.exists(fileInfo.fileName()))
            {
                toDir.remove(fileInfo.fileName());
            }

            if (!QFile::copy(fileInfo.filePath(), toDir.filePath(fileInfo.fileName())))
            {
                return false;
            }
        }
    }

    return true;
}

//判断文件夹是否存在当前路径下
bool FileThread::dirOrFileIsExistsCurrentPath(QString dirName, QString currentPath)
{
    QDir dir(currentPath);
    return dir.exists(dirName);
}
