#ifndef MYFILEICONPROVIDER_H
#define MYFILEICONPROVIDER_H

#include <QtGui/QFileIconProvider>

class MyFileIconProvider : public QFileIconProvider
{
public:
    MyFileIconProvider();
    QIcon icon(const QFileInfo & info) const;
};

#endif // MYFILEICONPROVIDER_H
