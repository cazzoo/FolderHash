#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QStringList>
#include <QCryptographicHash>
#include <QFile>
#include <QDirIterator>
#include <QDataStream>
#include <QDebug>

class Utils
{
public:
    static QString easyHash(QString &fileName, QCryptographicHash::Algorithm al);
    static QStringList scanFolder(QString rootFolder, bool recursive);
    static QString generateXML(QString rootFolder);
    static void saveFile(QString fileName, QString content);
    static QString loadFile(QString fileName);
};

#endif // UTILS_H
