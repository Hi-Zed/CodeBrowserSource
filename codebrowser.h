#ifndef CODEBROWSER_H
#define CODEBROWSER_H

#include <QMainWindow>
#include <QTextEdit>
#include <QTableWidget>
#include <QFileSystemModel>
#include "datamanager.h"

namespace Ui {
    class CodeBrowser;
}

class CodeBrowser : public QMainWindow
{
    Q_OBJECT

public:
    explicit CodeBrowser(QWidget *parent = 0);
    ~CodeBrowser();

private:
    bool mainEvent;
    Ui::CodeBrowser *ui;
    QFileSystemModel *fileModel;
    QList<QWidget*> tabList;
    QList<QTextEdit*> cList;
    QList<QTextEdit*> llvmList;
    QList<QTableWidget*> llvmDataList;
    QList<QTableWidget*> cDataList;
    DataManager* dataMng;

    void newTab(QString fileName, bool mode = true);

private slots:
    void on_dockFileList_visibilityChanged(bool visible);
    void on_listView_doubleClicked(QModelIndex index);
    void on_actionClose_All_triggered();
    void on_actionClose_triggered();
    void on_actionOpen_File_triggered();
    void on_llvmCode_selectionChanged();
    void on_cCode_selectionChanged();
    void on_actionFile_List_triggered(bool checked);
    void on_actionC_triggered(bool checked);
    void on_actionLlvm_triggered(bool checked);
    void on_codeArea_currentChanged(int index);
    void on_actionOpen_Project_triggered();
};

#endif // CODEBROWSER_H
