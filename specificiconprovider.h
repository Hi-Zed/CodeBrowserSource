#ifndef SPECIFICICONPROVIDER_H
#define SPECIFICICONPROVIDER_H

#include <QFileIconProvider>

class SpecificIconProvider : public QFileIconProvider
{
public:
    enum IconType {Ok, NotSynch, MissingFiles};
    SpecificIconProvider();
    QIcon icon(IconType type) const;
    QIcon icon(const QFileInfo &info) const;
    QString type(const QFileInfo &info) const;
};

#endif // SPECIFICICONPROVIDER_H
