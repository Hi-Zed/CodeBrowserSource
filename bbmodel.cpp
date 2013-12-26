#include "bbmodel.h"
#include <QStringList>
#include <QFile>

BbModel::BbModel() {
}

void BbModel::addBbData(QString dataString) {
    int id = dataString.section(":",6,6).toInt();
    double staticT = dataString.section(":",11,11).toDouble();
    double staticEn = dataString.section(":",12,12).toDouble();
    double dynT = dataString.section(":",13,13).toDouble();
    double dynEn = dataString.section(":",14,14).toDouble();
    bbData.insert(id, new Data(dynEn, staticEn, dynT, staticT, id));
    addAssociation(id, dataString.section(":",7,7).toInt(), dataString.section(":",8,8).toInt());
}

void BbModel::addAssociation(int id, int StL, int EnL) {
    int *tmp;
    tmp = new int[2];
    tmp[0] = StL;
    tmp[1] = EnL;
    srcList.insert(id, tmp);
}

bool BbModel::findBbLines(QString filename) {
    QFile file;
    int n=1;
    QRegExp rx(".:");
    file.setFileName(filename+".ll");
    if(file.open(QIODevice::ReadOnly)) {
        QString line = file.readLine().constData();
        while(line.size() != 0) {
            if(line.contains(rx) && !line.contains("\"")){
                startLine.append(n);
                if(startLine.size() == endLine.size()+2) {
                    endLine.append(n-1);
                }
            }
            if(line.startsWith("}")) {
                endLine.append(n-1);
            }
            line = file.readLine().constData();
            n++;
        }
        file.close();
        return true;
    }
    else
        return false;
}

int BbModel::start(int line) {
    if(line < startLine.at(0))
        return 0;
    else {
        QList<int>::iterator itr = qLowerBound(startLine.begin(), startLine.end(), line);
        if(*itr == line)
            return line;
        else {
            itr--;
            return *itr;
        }
    }
}

int BbModel::end(int line) {
    if(line > endLine.at(endLine.size() - 1))
        return 0;
    else {
        QList<int>::iterator itr = qLowerBound(endLine.begin(), endLine.end(), line);
        if(line < startLine.at(endLine.indexOf(*itr)))
            return 0;
        else
            return *itr;
    }
}

int* BbModel::getSrc(int line) {
    return srcList.value(startLine.indexOf(start(line))+1);
}

int* BbModel::bbLines(int id) {
    int *result;
    result = new int[2];
    result[0] = startLine.at(id-1);
    result[1] = endLine.at(id-1);
    return result;
}

Data* BbModel::getData(int line) {
    return bbData.value(startLine.indexOf(start(line))+1);
}
