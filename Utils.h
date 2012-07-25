#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QStringList>
#include <QCryptographicHash>
#include <QFile>
#include <QDirIterator>
#include <QTextStream>
#include <QDebug>
#include <QDomImplementation>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDomNodeList>
#include <QMap>

class Utils
{
public:
    static QString easyHash(const QString &fileName, const QCryptographicHash::Algorithm &algo);
    static QStringList scanFolder(const QString &rootFolder, const bool &includeSubDir);
    static QString generateXML(QString rootFolder);
    static QDomDocument generateXML(const QString &rootFolder, const bool &includeSubDir);
    static void generateMap(QMap<QString, QString> &map, const QDomDocument &xmlDocument);
    static void saveFile(QString fileName, const QDomDocument &xmlDocument);
    static void loadFile(const QString &fileName, QDomDocument &xmlDocument);
};

#endif // UTILS_H
