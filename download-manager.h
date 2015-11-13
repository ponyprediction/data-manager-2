#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QDate>

class DownloadManager
{
public:
    static void download(QDate dateStart, QDate dateEnd, bool force);
private:
    static void download(QDate date, bool force);
    static const QString getHtml(const QString &url);
    static void downloadReunion(const QString &date, const QString &zeturfId, const QString &name, const QString &reunionId, const bool &force);
    static void downloadRaceStart(const QString &date, const QString &reunionId, const QString &raceId, const QString &zeturfId, const QString &name, const bool &force);
    static void downloadRaceOdds(const QString &date, const QString &reunionId, const QString &raceId, const QString &zeturfId, const QString &name, const bool &force);
    static void downloadRaceArrival(const QString &date, const QString &reunionId, const QString &raceId, const QString &zeturfId, const QString &name, const bool &force);
};

#endif // DOWNLOADMANAGER_H
