#include "codebrowser.h"
#include "ui_codebrowser.h"
#include "specificiconprovider.h"
#include <QSplitter>
#include <QFileDialog>
#include <QMessageBox>
#include <QStringList>

CodeBrowser::CodeBrowser(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CodeBrowser)
{
    ui->setupUi(this);

    dataMng = new DataManager();
    mainEvent = true;
    QFileIconProvider *iconProv = new SpecificIconProvider();
    fileModel = new QFileSystemModel(this);
    fileModel->setIconProvider(iconProv);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    fileModel->setNameFilters(QStringList("*.c"));
    fileModel->setNameFilterDisables(false);
    ui->listView->setModel(fileModel);
    ui->dockFileList->setVisible(false);
    ui->actionFile_List->setEnabled(false);
    ui->codeArea->setVisible(false);
}

CodeBrowser::~CodeBrowser()
{
    delete ui;
}

void CodeBrowser::newTab(QString fileName, bool mode) {
    QFile file;

    ui->codeArea->setVisible(true);

    QWidget *newTab = new QWidget();
    tabList.append(newTab);

    QSplitter *splitter = new QSplitter(newTab);
    splitter->setOrientation(Qt::Horizontal);

    QWidget *lWidget = new QWidget(splitter);
    QWidget *rWidget = new QWidget(splitter);

    QHBoxLayout *layout = new QHBoxLayout(newTab);
    layout->setSpacing(6);
    layout->setContentsMargins(11, 11, 11, 11);
    layout->addWidget(splitter);

    QVBoxLayout *layoutL = new QVBoxLayout(lWidget);
    layoutL->setSpacing(6);
    layoutL->setContentsMargins(11, 11, 11, 11);

    QVBoxLayout *layoutR = new QVBoxLayout(rWidget);
    layoutR->setSpacing(6);
    layoutR->setContentsMargins(11, 11, 11, 11);

    QTextEdit *newCText = new QTextEdit(lWidget);
    layoutL->addWidget(newCText);
    newCText->setReadOnly(true);
    newCText->setLineWrapMode(QTextEdit::NoWrap);
    file.setFileName(fileName+".c");
    file.open(QIODevice::ReadOnly);
    newCText->setPlainText(file.readAll().constData());
    file.close();
    cList.append(newCText);
    QObject::connect(newCText, SIGNAL(cursorPositionChanged()), this, SLOT(on_cCode_selectionChanged()));

    QTextEdit *newLlvmText = new QTextEdit(rWidget);
    layoutR->addWidget(newLlvmText);
    newLlvmText->setReadOnly(true);
    newLlvmText->setLineWrapMode(QTextEdit::NoWrap);
    file.setFileName(fileName+".ll");
    file.open(QIODevice::ReadOnly);
    newLlvmText->setPlainText(file.readAll().constData());
    file.close();
    llvmList.append(newLlvmText);
    QObject::connect(newLlvmText, SIGNAL(cursorPositionChanged()), this, SLOT(on_llvmCode_selectionChanged()));

    QTableWidget *newCTable = new QTableWidget(4, 2, lWidget);
    QTableWidget *newLlvmTable = new QTableWidget(4, 2, rWidget);

    for(int i=0;i<4;i++) {
        for(int j=0;j<2;j++) {
            newCTable->setItem(i,j,new QTableWidgetItem);
            newLlvmTable->setItem(i,j,new QTableWidgetItem);
        }
    }

    layoutL->addWidget(newCTable);
    newCTable->horizontalHeader()->setVisible(false);
    newCTable->verticalHeader()->setVisible(false);
    newCTable->setMaximumHeight(122);
    newCTable->horizontalHeader()->setStretchLastSection(true);
    newCTable->setSelectionMode(QAbstractItemView::NoSelection);
    newCTable->horizontalHeader()->setDefaultSectionSize(150);
    newCTable->item(0,0)->setText("Static execution time");
    newCTable->item(1,0)->setText("Static energy");
    newCTable->item(2,0)->setText("Dynamic execution time");
    newCTable->item(3,0)->setText("Dynamic energy");
    cDataList.append(newCTable);

    layoutR->addWidget(newLlvmTable);
    newLlvmTable->horizontalHeader()->setVisible(false);
    newLlvmTable->verticalHeader()->setVisible(false);
    newLlvmTable->setMaximumHeight(122);
    newLlvmTable->horizontalHeader()->setStretchLastSection(true);
    newLlvmTable->setSelectionMode(QAbstractItemView::NoSelection);
    newLlvmTable->horizontalHeader()->setDefaultSectionSize(150);
    newLlvmTable->item(0,0)->setText("Static execution time");
    newLlvmTable->item(1,0)->setText("Static energy");
    newLlvmTable->item(2,0)->setText("Dynamic execution time");
    newLlvmTable->item(3,0)->setText("Dynamic energy");
    llvmDataList.append(newLlvmTable);

    if(mode)
        ui->codeArea->addTab(newTab, fileName);
    else {
        QFileInfo fileInfo;
        fileInfo.setFile(file);
        ui->codeArea->addTab(newTab, fileInfo.baseName());
    }
    file.close();

    ui->actionClose->setEnabled(true);
    ui->actionClose_All->setEnabled(true);
    ui->actionC->setEnabled(true);
    ui->actionC->setChecked(true);
    ui->actionLlvm->setEnabled(true);
    ui->actionLlvm->setChecked(true);
}

void CodeBrowser::on_actionOpen_File_triggered()
{
    QString Filename = QFileDialog::getOpenFileName(this,"Open File","","Source (*.c)");
    if(!Filename.isNull()) {
        Filename.remove(".c");
        if(dataMng->newFile(Filename))
            newTab(Filename);
        else
            QMessageBox::warning(this, "Error!", "Cannot open file. Some files are missing or are out of date");
    }
}

void CodeBrowser::on_actionClose_triggered()
{
    cDataList.removeAt(ui->codeArea->currentIndex());
    llvmDataList.removeAt(ui->codeArea->currentIndex());
    llvmList.removeAt(ui->codeArea->currentIndex());
    cList.removeAt(ui->codeArea->currentIndex());
    cDataList.removeAt(ui->codeArea->currentIndex());
    llvmDataList.removeAt(ui->codeArea->currentIndex());
    int n = ui->codeArea->currentIndex();
    ui->codeArea->removeTab(n);

    if(ui->codeArea->currentWidget() == NULL)
    {
        ui->actionClose->setEnabled(false);
        ui->actionClose_All->setEnabled(false);
        ui->actionC->setEnabled(false);
        ui->actionLlvm->setEnabled(false);
        ui->actionC->setChecked(false);
        ui->actionLlvm->setChecked(false);
        ui->codeArea->setVisible(false);
    }
}

void CodeBrowser::on_actionClose_All_triggered()
{
    while(ui->codeArea->currentWidget() != NULL) {
        ui->codeArea->removeTab(ui->codeArea->currentIndex());
    }
    cDataList.clear();
    llvmDataList.clear();
    llvmList.clear();
    cList.clear();
    cDataList.clear();
    llvmDataList.clear();
    ui->actionClose->setEnabled(false);
    ui->actionClose_All->setEnabled(false);
    ui->actionC->setEnabled(false);
    ui->actionLlvm->setEnabled(false);
    ui->actionC->setChecked(false);
    ui->actionLlvm->setChecked(false);
    ui->codeArea->setVisible(false);
}

void CodeBrowser::on_llvmCode_selectionChanged() {
    if(mainEvent) {
        mainEvent = false;
        QTextEdit *txtLlvm = llvmList.at(ui->codeArea->currentIndex());
        QTextEdit *txtC = cList.at(ui->codeArea->currentIndex());
        QTableWidget *tableLlvm = llvmDataList.at(ui->codeArea->currentIndex());
        QTableWidget *tableC = cDataList.at(ui->codeArea->currentIndex());
        QList<QTextEdit::ExtraSelection> extraSelLlvm, extraSelC;
        QTextEdit::ExtraSelection selection;
        int *limit = dataMng->bbLines(txtLlvm->textCursor().blockNumber()+1);
        if(limit[0] != 0 && limit[1]!=0){
            Data* data = dataMng->getBbModel()->getData(txtLlvm->textCursor().blockNumber()+1);
            tableLlvm->item(0,1)->setText(QString::number(data->getStaticT()));
            tableLlvm->item(1,1)->setText(QString::number(data->getStaticE()));
            tableLlvm->item(2,1)->setText(QString::number(data->getDynamicT()));
            tableLlvm->item(3,1)->setText(QString::number(data->getDynamicE()));

            tableC->item(0,1)->setText("");
            tableC->item(1,1)->setText("");
            tableC->item(2,1)->setText("");
            tableC->item(3,1)->setText("");

            QTextCursor ghostCursor = txtLlvm->textCursor();
            selection.format.setBackground(QColor(Qt::yellow).lighter(160));
            selection.format.setProperty(QTextFormat::FullWidthSelection, true);
            ghostCursor.setPosition(0);
            ghostCursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, limit[0]-1);
            ghostCursor.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor, limit[1]-limit[0]+1);
            selection.cursor = ghostCursor;
            extraSelLlvm.append(selection);
            txtLlvm->setExtraSelections(extraSelLlvm);

            int *lines = dataMng->getBbModel()->getSrc(txtLlvm->textCursor().blockNumber()+1);
            selection.cursor.clearSelection();
            ghostCursor = txtC->textCursor();
            ghostCursor.setPosition(0);
            ghostCursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, lines[0]-1);
            ghostCursor.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor, lines[1]-lines[0]+1);
            selection.cursor = ghostCursor;
            extraSelC.append(selection);
            ghostCursor.clearSelection();
            txtC->setTextCursor(ghostCursor);
            txtC->setExtraSelections(extraSelC);
        }
        mainEvent=true;
    }
}

void CodeBrowser::on_cCode_selectionChanged() {
    if(mainEvent) {
        mainEvent = false;
        QTextEdit *txtC = cList.at(ui->codeArea->currentIndex());
        QTextEdit *txtLlvm = llvmList.at(ui->codeArea->currentIndex());
        QTableWidget *tableC = cDataList.at(ui->codeArea->currentIndex());
        QTableWidget *tableLlvm = llvmDataList.at(ui->codeArea->currentIndex());
        QList<QTextEdit::ExtraSelection> extraSelC, extraSelLlvm;
        QTextEdit::ExtraSelection selection;
        QColor lineColor = QColor(Qt::yellow).lighter(160);

        QList<int> list = dataMng->getSrcModel()->getBb(txtC->textCursor().blockNumber()+1);
        if(list.size()!=0) {
            Data* data = dataMng->getSrcModel()->getData(txtC->textCursor().blockNumber()+1);
            tableC->item(0,1)->setText(QString::number(data->getStaticT()));
            tableC->item(1,1)->setText(QString::number(data->getStaticE()));
            tableC->item(2,1)->setText(QString::number(data->getDynamicT()));
            tableC->item(3,1)->setText(QString::number(data->getDynamicE()));

            tableLlvm->item(0,1)->setText("");
            tableLlvm->item(1,1)->setText("");
            tableLlvm->item(2,1)->setText("");
            tableLlvm->item(3,1)->setText("");

            selection.format.setBackground(lineColor);
            selection.format.setProperty(QTextFormat::FullWidthSelection, true);
            selection.cursor = txtC->textCursor();
            selection.cursor.clearSelection();
            extraSelC.append(selection);

            txtC->setExtraSelections(extraSelC);

            int *limit;
            QTextCursor ghostCursor = txtLlvm->textCursor();

            for(int i=0; i<list.size(); i++) {
                 limit = dataMng->getBbModel()->bbLines(list.at(i));
                 ghostCursor.setPosition(0);
                 ghostCursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, limit[0]-1);
                 ghostCursor.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor, limit[1]-limit[0]+1);
                 selection.cursor = ghostCursor;
                 extraSelLlvm.append(selection);
            }
            ghostCursor.clearSelection();
            txtLlvm->setTextCursor(ghostCursor);
            txtLlvm->setExtraSelections(extraSelLlvm);
        }
        mainEvent = true;
    }
}

void CodeBrowser::on_actionFile_List_triggered(bool checked)
{
    ui->dockFileList->setVisible(checked);
}

void CodeBrowser::on_actionC_triggered(bool checked)
{
    if(llvmList.at(ui->codeArea->currentIndex())->isVisible()) {
        cList.at(ui->codeArea->currentIndex())->setVisible(checked);
        cDataList.at(ui->codeArea->currentIndex())->setVisible(checked);
        cDataList.at(ui->codeArea->currentIndex())->parentWidget()->setVisible(checked);
    }
    else
        ui->actionC->setChecked(true);

}

void CodeBrowser::on_actionLlvm_triggered(bool checked)
{
    if(cList.at(ui->codeArea->currentIndex())->isVisible()) {
        llvmList.at(ui->codeArea->currentIndex())->setVisible(checked);
        llvmDataList.at(ui->codeArea->currentIndex())->setVisible(checked);
        llvmDataList.at(ui->codeArea->currentIndex())->parentWidget()->setVisible(checked);
    }
    else
        ui->actionLlvm->setChecked(true);
}

void CodeBrowser::on_codeArea_currentChanged(int index)
{
    if(ui->codeArea->count() != 0) {
        ui->actionC->setChecked(cList.at(index)->isVisible());
        ui->actionLlvm->setChecked(llvmList.at(index)->isVisible());
    }
    else {
        ui->actionC->setChecked(false);
        ui->actionLlvm->setChecked(false);
    }
}

void CodeBrowser::on_actionOpen_Project_triggered()
{
    QString directory = QFileDialog::getExistingDirectory(this,"Select directory","");
    if(directory != "") {
        ui->dockFileList->setVisible(true);
        ui->actionFile_List->setEnabled(true);
        ui->actionFile_List->setChecked(true);
        ui->listView->setRootIndex(fileModel->setRootPath(directory));
        ui->statusBar->showMessage(directory);
    }
}

void CodeBrowser::on_listView_doubleClicked(QModelIndex index)
{
    QString Filename = fileModel->fileInfo(index).absoluteFilePath();
    if(!Filename.isNull()) {
        Filename.remove(".c");
        if(dataMng->newFile(Filename))
            newTab(Filename, false);
        else
            QMessageBox::warning(this, "Error!", "Cannot open file. Some files are missing or are out of date");
    }
}

void CodeBrowser::on_dockFileList_visibilityChanged(bool visible)
{
    ui->actionFile_List->setChecked(visible);
}
