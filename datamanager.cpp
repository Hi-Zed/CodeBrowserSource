#include "datamanager.h"
#include <QFile>
#include <QFileInfo>
#include <QDateTime>

DataManager::DataManager() {
    srcModel = new SrcModel();
    bbModel = new BbModel();
}

SrcModel* DataManager::getSrcModel() {
    return srcModel;
}

BbModel* DataManager::getBbModel() {
    return bbModel;
}

bool DataManager::newFile(QString name) {
    QFile file;
    if(file.exists(name+".bbmodel") && file.exists(name+".lnmodel")) {
        QFileInfo bbmodelFile, lnmodelFile, cFile;
        bbmodelFile.setFile(name+".bbmodel");
        lnmodelFile.setFile(name+".lnmodel");
        cFile.setFile(name+".c");
        if((bbmodelFile.lastModified() == cFile.lastModified()) && (lnmodelFile.lastModified() == cFile.lastModified())) {
            file.setFileName(name+".bbmodel");
            file.open(QIODevice::ReadOnly);
            QString line = file.readLine().constData();
            while(line.size() != 0) {
                if(!line.startsWith("#") && !line.startsWith("\n")) {
                    bbModel->addBbData(line);
                }
                line = file.readLine().constData();
            }
            file.close();
            if(bbModel->findBbLines(name)) {
                file.setFileName(name+".lnmodel");
                file.open(QIODevice::ReadOnly);
                line = file.readLine().constData();
                while(line.size() != 0) {
                    if(!line.startsWith("#") && !line.startsWith("\n")) {
                        srcModel->addSrcData(line);
                    }
                    line = file.readLine().constData();
                }
                file.close();
                return true;
            }
        }
    }
    return false;
}


int* DataManager::bbLines(int line) {
    int *result;
    result = new int[2];
    result[0] = bbModel->start(line);
    result[1] = bbModel->end(line);
    return result;
}
