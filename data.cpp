#include "data.h"

Data::Data(double dyE, double stE, double dyT, double stT, int ID) {
    dynamicE = dyE;
    staticE = stE;
    dynamicT = dyT;
    staticT = stT;
    this->ID = ID;
}

double Data::getDynamicE() {
    return dynamicE;
}

double Data::getStaticE() {
    return staticE;
}

double Data::getDynamicT() {
    return dynamicT;
}

double Data::getStaticT() {
    return staticT;
}
