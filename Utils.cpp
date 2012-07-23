#include "Utils.h"

/**
  This method creates a hash for a fileName using a specific algoryth.
  @param fileName : the file path we want to create the hash.
  @param algo : The algorith we want to hash. possible values : MD4 / MD5 / SHA1.
  @author Caz
**/
QString Utils::easyHash(QString &fileName, QCryptographicHash::Algorithm algo)
{
    QCryptographicHash crypto(algo);
    QFile file(fileName);
    file.open(QFile::ReadOnly);
    while(!file.atEnd()){
        crypto.addData(file.read(8192));
    }
    QString hash = crypto.result().toHex();
    return hash;
}

/**
  This method is used to scan a folder and return a QStringList of all the files.
  You can scan only the rootFolder or all it's subdirs.
  @param rootFolder : the folder path from which we want to scan.
  @param includeSubDir : Indicated weither or not we scan subdirs.
  @author Caz
**/
QStringList Utils::scanFolder(QString rootFolder, bool includeSubDir)
{
    QStringList listFilter, fileList;
    if(includeSubDir)
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

/**
  This method generates an XML output containign all the Hashes of the files included into the rootFolder.
  @param rootFolder : the folder path from which we want to generate the hashes.
  @author Caz
**/
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

/**
  This method is used to save a file and it's content with the QT 4.7 serialization format.
  @param fileName : the file path we want to save.
  @param content : the content of the file we want to save.
  @author Caz
**/
void Utils::saveFile(QString fileName, QString content)
{
    if (fileName.isEmpty())
        return;
    else
    {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            qDebug() << "Unable to open file" + file.errorString();
            return;
        }
        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_4_7);
        out << content;
    }
}

/**
  This method is used to load a file saved with the QT 4.7 serialization format.
  @param fileName : the file path we want to load.
  @author Caz
**/
QString Utils::loadFile(QString fileName)
{
    QString content;
    if (fileName.isEmpty())
        return "";
    else
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "Unable to open file" + file.errorString();
            return "";
        }
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_4_7);
        in >> content;
    }
    return content;
}
