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

QStringList Utils::scanFolder(QString rootFolder, bool recursive)
{
    QStringList listFilter, fileList;
    if(recursive)
    {
        //Looping through the dir and it's sub-dirs
        QDirIterator dirIterator(rootFolder, listFilter, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);

        // While there are results
        while(dirIterator.hasNext())
        {
            fileList << dirIterator.next();
        }
    }
    else
    {
        //Looping through the dir and it's sub-dirs
        QDirIterator dirIterator(rootFolder, listFilter, QDir::Files | QDir::NoSymLinks);

        // While there are results
        while(dirIterator.hasNext())
        {
            fileList << dirIterator.next();
        }
    }
    return fileList;
}

QString Utils::generateXML(QString rootFolder)
{
    QString xml, hash;
    QStringList fileList = scanFolder(rootFolder, true);
    QString m_folderName = rootFolder.split("\\").last();
    xml = "<xml>\n\t<folder src=\"" + rootFolder.split(m_folderName).first() + "\" name=\"" + m_folderName + "\">\n";
    foreach (QString str, fileList) {
        hash = Utils::easyHash(str, QCryptographicHash::Md4);
        xml += "\t\t<file>\n\t\t\t<name>" + str.split(m_folderName).last() + "</name>\n\t\t\t<hash>"+ hash +"</hash>\n\t\t</file>\n";
    }
    xml += "\t</folder>\n</xml>";
    return xml;
}

void save(QString fileName, QString content)
{

}
