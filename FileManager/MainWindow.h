#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "FileThread.h"

#include <QMainWindow>
#include <QProgressBar>
#include <QTreeWidgetItem>
#include <QFileSystemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void iniVar();//初始化变量
    void initUi();//初始化控件
    void createFileModel();//创建文件资源的模型
    void createSplitter();//对分割窗口QSplitter进行设置
    void createQTreeWidget();//创建左侧树形文件浏览器
    void createQTableView();//创建右侧的列表窗口视图
    void createQListView();//创建右侧的图标窗口视图
    void showChildrenCombox(QTreeWidgetItem *rootItem);
    void loadFiles(QString sPath, QTreeWidgetItem *item);
    bool treeWidgetItemIsExisted(QString sPath, QTreeWidgetItem *pParentItem);
    void leftAndRightSync();
    QTreeWidgetItem *getChildItemByPath(QTreeWidgetItem *parent, QString sPath);
    void getAllLastPath(QString sCurPath);
    void prepareCopyAndCutPath();
    QString getPathName(QString sPath);
    void freeListTreeWidgetItem();
    void updateTreeWidgetData();
    void createDirOrFile(bool bFile);
    bool dirIsExistsCurrentPath(QString dirName, QString sCurPath);
    bool stringIsNumber(QString str);
    int dirOrFileConflict(bool bDir, QString sNameSrc);

private slots:
    void on_tbFind_clicked();//搜索文件
    void on_tbList_clicked();//设置列表模式
    void on_tbIcon_clicked();//设置图标模式

    void onItemClicked(QTreeWidgetItem* item, int i);//左侧树鼠标点击响应函数
    void onSectionClicked(int a);//点击列表模式的文件头，对文件夹和文件做排序显示

    void on_lvFileList_doubleClicked(const QModelIndex &index);//鼠标在列表模式下双击(进入目录/打开文件)
    void on_listView_doubleClicked(const QModelIndex &index);//鼠标在图标模式下双击(进入目录/打开文件)
    void on_lvFileList_clicked(const QModelIndex &index);//点击列表模式的文件夹 或 文件 显示路径到届满
    void on_listView_clicked(const QModelIndex &index);

    void on_lvFileList_customContextMenuRequested(const QPoint &pos);//在列表模式下右键弹出菜单
    void on_listView_customContextMenuRequested(const QPoint &pos);//在列图标式下右键弹出菜单

    //自定义槽函数 点到Item上
    void onActionOpen();//打开
    void onActionCopy();//复制
    void onActionCut();//剪切
    void onActionRename();//重命名
    void onActionDelete();//删除

    //子菜单槽函数
    void onActionOpenImage();//打开图片浏览器
    void onActionOpenText();//打开文本浏览器

    //点到空白处
    void onActionNewDir();//新建文件夹
    void onActionNewFile();//新建文档
    void onActionPaste();//粘贴

    void onCopyAndCutFinished(); //线程复制或者剪切完毕后，给主线程反馈信号
    void onChooseCoverFileOrDir(bool bDir, QString sNameSrc);

signals:
    void chooseCoverFinished(int bCover);

private:
    void customContextMenuRequested(const QPoint &pos, bool bListView);

private:
    QString getDesktopPath();//获取系统桌面路径

protected:
    void timerEvent(QTimerEvent *event);
    void keyPressEvent(QKeyEvent* event);

private:
    Ui::MainWindow *ui;

    QFileSystemModel* m_pFileModel;  //文件资源的模型
    QTreeWidgetItem* m_pItem;
    QTreeWidgetItem* m_pOldItem;
    QList<QTreeWidgetItem*> m_items; //保存左侧的窗口树所有的Item对象;
    QString m_sCurPath;              //当前路径
    bool m_bViewState;               //当前右侧窗口是 列表视图 还是图标视图 true:列表 false:图标
    QStringList m_allPathList;       //所有文件的路径list
    QPoint m_ptPress;
    int m_iActionState;
    int m_iOperation;
    QStringList m_sSourcePath;
    QProgressBar* m_pProgressBar;
    FileThread* m_pThread;
};

#endif // MAINWINDOW_H
