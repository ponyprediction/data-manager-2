#ifndef UTIL_H
#define UTIL_H

#include <QString>

#define RESET "\033[0m"
#define ROUGE "\033[31m"

class Util
{
public:
    Util();
    static bool init();
    static QString getCommand(int argc, char *argv[]);
    static bool write(const QString &line);
    static bool writeError(const QString &line);
    static QString getRealDate(const QString &date);
    static QString getLineFromConf(const QString &id, bool *ok);
private:
    static QString confFileName;
};

#endif // UTIL_H
