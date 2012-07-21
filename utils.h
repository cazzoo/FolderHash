#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QCryptographicHash>
#include <QFile>

class Utils
{
public:
    static QString easyHash(QString &fileName, QCryptographicHash::Algorithm al);
};

#endif // UTILS_H
