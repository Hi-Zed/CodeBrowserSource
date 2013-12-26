#include <QtGui/QApplication>
#include "codebrowser.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CodeBrowser w;
    w.show();

    return a.exec();
}
