#ifndef BBMODEL_H
#define BBMODEL_H

#include <QString>
#include <QHash>
#include <QList>
#include "data.h"

class BbModel
{
private:
    QHash<int, Data*> bbData;
    QHash<int, int*>  srcList;
    QList<int> startLine, endLine;
    void addAssociation(int id, int StL, int EnL);
public:
    BbModel();
    void addBbData(QString dataString);
    bool findBbLines(QString filename);
    int* bbLines(int id);
    int end(int line);
    int start(int line);
    int* getSrc(int line);
    Data* getData(int line);
};

#endif // BBMODEL_H
