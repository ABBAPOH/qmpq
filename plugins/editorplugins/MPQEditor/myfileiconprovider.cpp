#include "myfileiconprovider.h"
#include <icore.h>

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
    ICore * core = ICore::instance();
    QIcon icon = core->editorFactoryManager()->icon(info.absoluteFilePath());
    if (!icon.isNull())
        return icon;
    if (info.absoluteFilePath().startsWith("mpq:"))
        return QFileIconProvider::icon(QFileIconProvider::File);
    return QFileIconProvider::icon(info);
}
