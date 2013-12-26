#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "bbmodel.h"
#include "srcmodel.h"
#include "data.h"
#include <QString>

class DataManager
{
private:
    BbModel* bbModel;
    SrcModel* srcModel;
    QString extractBbData();
    QString extractSrcData();
public:
    DataManager();
    bool newFile(QString name);
    int* bbLines(int line);
    BbModel* getBbModel();
    SrcModel* getSrcModel();
};

#endif // DATAMANAGER_H
