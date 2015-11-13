#include "download-manager.h"
#include "manager.h"
#include "util.h"
#include <QStringList>

#include <QDebug>

Manager::Manager()
{

}

bool Manager::execute(const QString & command)
{
    Util::write("Treating : " + command);
    bool ok = true;
    QMap<QString, QString> arguments = Manager::getArguments(command);
    if(arguments["type"] == "download")
    {

        Manager::download(arguments);
    }
    else
    {
        ok = false;
        Util::writeError("unable to process command : " + command);
    }
    return ok;
}

bool Manager::download(const QMap<QString, QString> & args)
{
    bool ok = true;
    bool force = true;
    DownloadManager::download(QDate::fromString(args["dateStart"], "yyyy-MM-dd"), QDate::fromString(args["dateEnd"], "yyyy-MM-dd"), force);
    return ok;
}

QMap<QString, QString> Manager::getArguments(const QString & command)
{
    QMap<QString, QString> arguments;
    arguments["dateStart"] = "today";
    arguments["dateEnd"] = "today";
    QStringList args = command.split(' ');
    for(int i = 0 ; i < args.size() ; i++)
    {
        if(i == 0)
        {
            arguments["type"] = args[i];
        }
        else if(args[i] == "from")
        {
            i++;
            arguments["dateStart"] = args[i];
        }
        else if(args[i] == "to")
        {
            i++;
            arguments["dateEnd"] = args[i];
        }
    }
    arguments["dateStart"] = Util::getRealDate(arguments["dateStart"]);
    arguments["dateEnd"] = Util::getRealDate(arguments["dateEnd"]);
    return arguments;
}
