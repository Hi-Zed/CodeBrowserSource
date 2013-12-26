#ifndef DATA_H
#define DATA_H

class Data
{
private:
    double dynamicE;
    double staticE;
    double dynamicT;
    double staticT;
    int ID;

public:
    Data(double dyE, double stE, double dyT, double stT, int ID);
    double getDynamicE();
    double getStaticE();
    double getDynamicT();
    double getStaticT();
};

#endif // DATA_H
