#include "download-manager.h"
#include "util.h"
#include <QDate>
#include <QEventLoop>
#include <QFile>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

void DownloadManager::download(QDate dateStart, QDate dateEnd, bool force)
{
    for(QDate date = dateStart ; date <= dateEnd ; date = date.addDays(1))
    {
        DownloadManager::download(date, force);
    }
}

void DownloadManager::download(QDate date, bool force)
{
    // Init
    Util::write("Download : " + date.toString("yyyy-MM-dd"));
    bool ok = true;
    QString error = "";
    QString dayUrl = "";
    QString html = "";
    QString filename = Util::getLineFromConf("dayHtmlFilename", &ok);
    filename.replace("DATE", date.toString("yyyy-MM-dd"));
    QFile file;
    // Check date
    if(ok && (date > QDate::currentDate()))
    {
        ok = false;
        error = "invalid date : " + date.toString("yyyy-MM-dd") + " > today";
    }
    // Check if file exists
    if(ok && !force && QFile::exists(filename)) {
        ok = false;
        error = "the file already exists "
                + QFileInfo(filename).absoluteFilePath();
    }
    // Open file
    if(ok)
    {
        file.setFileName(filename);
        if (!file.open(QFile::WriteOnly)) {
            ok = false;
            Util::writeError("cannot open file " + QFileInfo(file).absoluteFilePath());
        }
    }
    // Prepare url
    if(ok)
    {
        dayUrl = Util::getLineFromConf("dayUrl", &ok);
        dayUrl.replace("DATE", date.toString("yyyy-MM-dd"));
    }
    // Download html & save
    if(ok) {
        html = getHtml(dayUrl);
        file.write(html.toUtf8());
        file.close();
    }
    // Parsing + Download other pages
    if(ok) {
        QVector<QString> reunions;
        QRegExp rx("href=\"([^\"]*id=([0-9]*)[^\"]*)\" "
                   "title=\"([^\"]*)\" "
                   "class=\"halfpill\">(R[0-9]+)<");
        int pos = 0;
        while ((pos = rx.indexIn(html, pos)) != -1) {
            pos += rx.matchedLength();
            QString url = rx.cap(1);
            QString zeturfId = rx.cap(2);
            QString name = rx.cap(3);
            QString reunionId = rx.cap(4);
            bool addReunion = true;
            for(int i = 0 ; i < reunions.size() ; i++) {
                if(zeturfId == reunions[i]) {
                    addReunion = false;
                }
            }
            if(addReunion) {
                downloadReunion(date.toString("yyyy-MM-dd"), zeturfId, name, reunionId, force);
                reunions.push_back(zeturfId);
            }
        }
    }
}

void DownloadManager::downloadReunion(const QString & date, const QString & zeturfId, const QString & name, const QString & reunionId, const bool & force) {
    // Start
    bool ok = true;
    QString url = Util::getLineFromConf("reunionUrl", &ok);
    url.replace("ID", zeturfId);
    QString filename = Util::getLineFromConf("reunionHtmlFilename", &ok);
    filename.replace("DATE", date);
    filename.replace("REUNION_ID", reunionId);
    QFile file;
    QString html = "";
    // Check if file already exists file
    if(ok && !force && QFile::exists(filename)) {
        ok = false;
        Util::writeError("the file already exists " + QFileInfo(filename).absoluteFilePath());
    }
    // Open file
    if(ok) {
        file.setFileName(filename);
        if (!file.open(QFile::WriteOnly)) {
            ok = false;
            Util::writeError("cannot open file " + QFileInfo(file).absoluteFilePath());
        }
    }
    // Download html & save
    if(ok) {
        html = getHtml(url);
        file.write(html.toUtf8());
        file.close();
    }
    // Parse
    if(ok) {
        QVector<QString> races;
        QRegExp rx("href=\"([^\"]*id=([0-9]*))\" "
                   "title=\""+name+" - ([^\"]*)\" "
                   +"class=\"pill\">&nbsp;"+reunionId+" (C[0-9]+)");
        int pos = 0;
        while ((pos = rx.indexIn(html, pos)) != -1) {
            pos += rx.matchedLength();
            QString url = rx.cap(1);
            QString zeturfId = rx.cap(2);
            QString name = rx.cap(3);
            QString raceId = rx.cap(4);
            bool addRace = true;
            for(int i = 0 ; i < races.size() ; i++) {
                if(name == races[i]) {
                    addRace = false;
                }
            }
            if(addRace) {
                downloadRaceStart(date, reunionId, raceId, zeturfId, name, force);
                downloadRaceOdds(date, reunionId, raceId, zeturfId, name, force);
                downloadRaceArrival(date, reunionId, raceId, zeturfId, name, force);
                races.push_back(name);
            }
        }
    }
}

const QString DownloadManager::getHtml(const QString & url) {
    Util::write("Downloading " + url);
    QNetworkRequest request(url);
    QNetworkAccessManager manager;
    QNetworkReply * download = manager.get(request);
    QEventLoop eventLoop;
    QObject::connect(download, SIGNAL(finished()),
                     &eventLoop, SLOT(quit()));
    eventLoop.exec(); // wait until download is done
    QString html = download->readAll();
    return html;
}

void DownloadManager::downloadRaceStart(const QString & date,
                                        const QString & reunionId,
                                        const QString & raceId,
                                        const QString & zeturfId,
                                        const QString & name,
                                        const bool & force) {
    // Start
    (void)name;
    bool ok = true;
    QString error = "";
    QString url = Util::getLineFromConf("startUrl", &ok);
    url.replace("ID", zeturfId);
    QString filename = Util::getLineFromConf("startHtmlFilename", &ok);
    filename.replace("DATE", date);
    filename.replace("REUNION_ID", reunionId);
    filename.replace("RACE_ID", raceId);
    QFile file;
    QString html = "";
    // Check file
    if(ok && !force && QFile::exists(filename)) {
        ok = false;
        error = "the file already exists "
                + QFileInfo(filename).fileName();
    }
    // Open file
    if(ok) {
        file.setFileName(filename);
        if (!file.open(QFile::WriteOnly)) {
            ok = false;
            error = "cannot open file " + QFileInfo(file).absoluteFilePath();
        }
    }
    // Download html & save
    if(ok) {
        html = getHtml(url);
        file.write(html.toUtf8());
        file.close();
    }
    // End
    if(ok) {
    }
    if(!ok) {
        Util::writeError(error);
    }
}

void DownloadManager::downloadRaceOdds(const QString & date,
                                       const QString & reunionId,
                                       const QString & raceId,
                                       const QString & zeturfId,
                                       const QString & name,
                                       const bool & force) {
    // Start
    (void)name;
    bool ok = true;
    QString url = Util::getLineFromConf("oddsUrl", &ok);
    url.replace("ID", zeturfId);
    QString filename = Util::getLineFromConf("oddsHtmlFilename", &ok);
    filename.replace("DATE", date);
    filename.replace("REUNION_ID", reunionId);
    filename.replace("RACE_ID", raceId);
    QFile file;
    QString html = "";
    // Check file
    if(ok && !force && QFile::exists(filename)) {
        ok = false;
        Util::writeError("the file already exists "
                + QFileInfo(filename).fileName());
    }
    // Open file
    if(ok) {
        file.setFileName(filename);
        if (!file.open(QFile::WriteOnly)) {
            ok = false;
            Util::writeError("cannot open file " + QFileInfo(file).absoluteFilePath());
        }
    }
    // Download html & save
    if(ok) {
        html = getHtml(url);
        file.write(html.toUtf8());
        file.close();
    }
}

void DownloadManager::downloadRaceArrival(const QString & date,
                                          const QString & reunionId,
                                          const QString & raceId,
                                          const QString & zeturfId,
                                          const QString & name,
                                          const bool & force) {
    // Start
    (void)name;
    bool ok = true;
    QString url = Util::getLineFromConf("arrivalUrl", &ok);
    url.replace("ID", zeturfId);
    QString filename = Util::getLineFromConf("arrivalHtmlFilename", &ok);
    filename.replace("DATE", date);
    filename.replace("REUNION_ID", reunionId);
    filename.replace("RACE_ID", raceId);
    QFile file;
    QString html = "";
    // Check file
    if(ok && !force && QFile::exists(filename)) {
        ok = false;
        Util::writeError("the file already exists "
                + QFileInfo(filename).fileName());
    }
    // Open file
    if(ok)
    {
        file.setFileName(filename);
        if (!file.open(QFile::WriteOnly))
        {
            ok = false;
            Util::writeError("cannot open file " + QFileInfo(file).absoluteFilePath());
        }
    }
    // Download html & save
    if(ok) {
        html = getHtml(url);
        file.write(html.toUtf8());
        file.close();
    }
}
