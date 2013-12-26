#include "specificiconprovider.h"
#include <QDateTime>

SpecificIconProvider::SpecificIconProvider() : QFileIconProvider()
{}

QIcon SpecificIconProvider::icon(IconType type) const {
    QIcon icon;
    //icon.addFile(":/image/resource/file.png", QSize(), QIcon::Normal, QIcon::Off);
    switch(type) {
    case SpecificIconProvider::Ok :
        icon.addFile(":/image/resource/correct.png", QSize(), QIcon::Normal, QIcon::Off);
        break;
    case SpecificIconProvider::NotSynch :
        icon.addFile(":/image/resource/clock.png", QSize(), QIcon::Normal, QIcon::Off);
        break;
    case SpecificIconProvider::MissingFiles :
        icon.addFile(":/image/resource/alert.png", QSize(), QIcon::Normal, QIcon::Off);
        break;
    }
    return icon;
}
QString SpecificIconProvider::type(const QFileInfo &info) const {
    QFile file;
    QString fileName = info.absoluteFilePath();
    fileName.remove(".c");
    if(file.exists(fileName+".bbmodel") && file.exists(fileName+".lnmodel")) {
        QFileInfo bbmodelFile, lnmodelFile;
        bbmodelFile.setFile(fileName+".bbmodel");
        lnmodelFile.setFile(fileName+".lnmodel");
        if((bbmodelFile.lastModified() == info.lastModified()) && (lnmodelFile.lastModified() == info.lastModified()))
            return "0";
        else
            return "1";
    }
    else
        return "2";
}
QIcon SpecificIconProvider::icon(const QFileInfo &info) const {
    switch(type(info).toInt()) {
    case 0:
        return icon(SpecificIconProvider::Ok);
        break;
    case 1:
        return icon(SpecificIconProvider::NotSynch);
        break;
    case 2:
        return icon(SpecificIconProvider::MissingFiles);
        break;
    }
    return icon(SpecificIconProvider::MissingFiles);
}

