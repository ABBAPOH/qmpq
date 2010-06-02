#include "myfileiconprovider.h"

MyFileIconProvider::MyFileIconProvider() :
       QFileIconProvider()
{
}

QIcon MyFileIconProvider::icon(const QFileInfo & info) const
{
    //    if (info.suffix() == "txt")
    //    {
    //        return QIcon(":/icons/images/list.png");
    //    }
    return QFileIconProvider::icon(info);
}
