#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QStringList>
#include <QCryptographicHash>
#include <QFile>
#include <QDirIterator>

class Utils
{
public:
    static QString easyHash(QString &fileName, QCryptographicHash::Algorithm al);
    static QStringList scanFolder(QString rootFolder, bool recursive);
    static QString generateXML(QString rootFolder);
    static void save(QString fileName, QString content);
};

#endif // UTILS_H
