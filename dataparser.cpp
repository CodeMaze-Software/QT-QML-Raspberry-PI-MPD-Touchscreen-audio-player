#include "dataparser.h"

QHash<QString, QString> Dataparser::parse(QString incommingData)
{
    QHash<QString, QString> packetHash;
    QString tmp;
    int i;
    foreach (tmp, incommingData.split("\n")) {
        i = tmp.indexOf(": ");
        if (i!=-1)
            packetHash.insertMulti(tmp.left(i).toLower(), tmp.mid(i+2));
    }
    return packetHash;
}

