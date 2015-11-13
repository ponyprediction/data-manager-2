#ifndef MANAGER_H
#define MANAGER_H

#include <QMap>
#include <QString>

class Manager
{
public:
    Manager();
    static bool execute(const QString &command);
    static bool download(const QMap<QString, QString> &args);
    static QMap<QString, QString> getArguments(const QString &command);
};

#endif // MANAGER_H
