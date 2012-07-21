#include "Utils.h"

QString Utils::easyHash(QString &fileName, QCryptographicHash::Algorithm al)
{
    QCryptographicHash crypto(al);
    QFile file(fileName);
    file.open(QFile::ReadOnly);
    while(!file.atEnd()){
      crypto.addData(file.read(8192));
    }
    QString hash = crypto.result().toHex();
    return hash;
}
