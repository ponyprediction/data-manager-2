#include "util.h"
#include <QDate>
#include <QFile>
#include <QFileInfo>
#include <QXmlStreamReader>
#include <iostream>

QString Util::confFileName = "./data-manager.conf";

Util::Util()
{

}

bool Util::init()
{
    bool ok = true;
    return ok;
}

QString Util::getCommand(int argc, char *argv[])
{
    QString command = NULL;
    if(argc > 1)
    {
        for(int i = 1 ; i < argc ; i++)
        {
            if(i-1)
            {
                command += " ";
            }
            command += argv[i];
        }
    }
    return command;
}

bool Util::write(const QString & message)
{
    std::cout << message.toStdString() << std::endl;
    return true;
}

bool Util::writeError(const QString & message)
{
    write(QString(ROUGE) + "Error : " + message.toLower() + RESET);
    return true;
}

QString Util::getRealDate(const QString & date)
{
    QString realDate = NULL;
    if(date == "today")
    {
        realDate = QDate::currentDate().toString("yyyy-MM-dd");
    }
    else if(date == "yesterday")
    {
        realDate = QDate::currentDate().addDays(-1).toString("yyyy-MM-dd");
    }
    else if(QDate::fromString(date, "yyyy-MM-dd").isValid())
    {
        realDate = date;
    }
    else
    {
        realDate = "";
        Util::writeError("cannot convert date : " + date);
    }
    return realDate;
}

QString Util::getLineFromConf(const QString & id, bool * ok)
{
    QString output = "";
    QFile file(confFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Util::writeError("can not find the conf file "
                         + QFileInfo(file).absoluteFilePath());
        return QString();
    }
    QXmlStreamReader xml(&file);
    while (!xml.atEnd())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        if(token == QXmlStreamReader::StartElement)
        {
            if(xml.name() == id)
            {
                output = xml.readElementText();
            }
        }
    }
    if(!output.size())
    {
        if(ok)
        {
            *ok = false;
        }
        Util::writeError("can not find config line <" + id + ">");
    }
    return output;
}
