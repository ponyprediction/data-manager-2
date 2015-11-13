#include <manager.h>
#include "util.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    bool ok = true;
    QCoreApplication application(argc, argv);
    QString command;
    if(ok)
    {
        ok = Util::init();
    }
    if(ok)
    {
        ok = !(command = Util::getCommand(argc, argv)).isEmpty();
    }
    if(ok)
    {
        Manager::execute(command);
    }
    else
    {
    }
    return 0;
}
