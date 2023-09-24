#ifndef DATAPARSER_H
#define DATAPARSER_H

#include <QString>
#include <QHash>

class Dataparser
{
public:
    static QHash<QString, QString> parse(QString incommingData);
    static QByteArray makeRequest(QByteArray request) {return request + "\n"; }

    template<typename T> static QByteArray makeReguestWithValue(QString command, T value)
    { return (QString("%1 %2\n").arg(command).arg(value)).toUtf8();}

    template<typename T> static QByteArray makeReguestWithValue(QString command, T value1, T value2)
    { return (QString("%1 %2 %3\n").arg(command).arg(value1).arg(value2)).toUtf8();}

    template<typename T> static QByteArray makeReguestWithEscapedValue(QString command, T value1, T value2)
    { return (QString("%1 \"%2\" \"%3\"\n").arg(command).arg(value1).arg(value2)).toUtf8();}
};

#endif // DATAPARSER_H
