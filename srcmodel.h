#ifndef SRCMODEL_H
#define SRCMODEL_H

#include <QString>
#include <QHash>
#include <QList>
#include "data.h"

class SrcModel
{
private:
    QHash<int, Data*> srcData;
    QHash<int, QList<int> >  bbList;
    void addAssociation(int id, int blockid);
public:
    SrcModel();
    void addSrcData(QString dataString);
    QList<int> getBb(int line);
    Data* getData(int line);
};

#endif // SRCMODEL_H
