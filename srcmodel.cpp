#include "srcmodel.h"

SrcModel::SrcModel()
{
}

void SrcModel::addSrcData(QString dataString) {
    int id = dataString.section(":",4,4).toInt();
    double staticT = dataString.section(":",11,11).toDouble();
    double staticEn = dataString.section(":",12,12).toDouble();
    double dynT = dataString.section(":",13,13).toDouble();
    double dynEn = dataString.section(":",14,14).toDouble();
    srcData.insertMulti(id, new Data(dynEn, staticEn, dynT, staticT, id));
    addAssociation(id, dataString.section(":",6,6).toInt());
}

void SrcModel::addAssociation(int id, int blockid) {
    QList<int> list;
    if(bbList.contains(id)) {
        list = bbList.value(id);
    }
    list.append(blockid);
    bbList.insert(id, list);
}

QList<int> SrcModel::getBb(int line) {
    QList<int> list;
    list = bbList.value(line);
    return list;
}

Data* SrcModel::getData(int line){
    double staticT = 0, staticEn = 0, dynT = 0, dynE = 0;
    QList<Data *> dataList =  srcData.values(line);
    for(int i=0;i<dataList.size();i++) {
        staticT += dataList.at(i)->getStaticT();
        staticEn += dataList.at(i)->getStaticE();
        dynT += dataList.at(i)->getDynamicT();
        dynE += dataList.at(i)->getDynamicE();
    }
    return new Data(dynE, staticEn, dynT, staticT, line);
}
