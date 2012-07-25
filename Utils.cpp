#include "Utils.h"

/**
  This method creates a hash for a fileName using a specific algoryth.
  @param fileName : the file path we want to create the hash.
  @param algo : The algorith we want to hash. possible values : MD4 / MD5 / SHA1.
  @author Caz
**/
QString Utils::easyHash(const QString &fileName, const QCryptographicHash::Algorithm &algo)
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
QStringList Utils::scanFolder(const QString &rootFolder, const bool &includeSubDir)
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
  This method generates a QDomDocument containing all the Hashes of the files included into the rootFolder.
  @param rootFolder : the folder path from which we want to generate the hashes.
  @param includeSubDir : a boolean that indicated if we navigate through sub directories or not.
  @author Caz
**/
QDomDocument Utils::generateXML(const QString &rootFolder, const bool &includeSubDir)
{
    QStringList fileList = scanFolder(rootFolder, includeSubDir);
    QString m_folderName = rootFolder.split("\\").last();
    QString m_folderLocation = rootFolder.split(m_folderName).first();

    QDomImplementation impl = QDomDocument().implementation();
    // document with document type
    QString name = "hash";
    QString publicId = "-//cazsan//DTD hash 1.0 //EN";
    QString systemId = "http://www-imagis.imag.fr/DTD/stone1.dtd";
    QDomDocument document(impl.createDocumentType(name,publicId,systemId));

    // add some XML comment at the beginning
    document.appendChild(document.createComment("This file contains the hashed of all files of "+m_folderName));
    document.appendChild(document.createTextNode("\n"));

    // root node
    QDomElement scannedFodler = document.createElement("folder");
    scannedFodler.setAttribute("src", m_folderLocation);
    scannedFodler.setAttribute("name", m_folderName);

    // files node
    QDomElement filesNode = document.createElement("files");
    foreach (QString str, fileList) {
        QString fileName, fileHash;
        fileName = str.split(m_folderName).last();
        if(str.split("/").last() == m_folderName + ".xml")
        {
            fileName = m_folderName + ".xml";
        }
        fileHash = Utils::easyHash(str, QCryptographicHash::Md4);

        QDomElement fileNode = document.createElement("file");
        QDomElement fileNameNode = document.createElement("name");
        QDomElement fileHashNode = document.createElement("hash");
        fileNameNode.appendChild(document.createTextNode(fileName));
        fileHashNode.appendChild(document.createTextNode(fileHash));
        fileNode.appendChild(fileNameNode);
        fileNode.appendChild(fileHashNode);
        filesNode.appendChild(fileNode);
    }
    scannedFodler.appendChild(filesNode);
    document.appendChild(scannedFodler);

    return document;
}

/**
  This method generates a QMap<QString, QString> containing all the Hashes and relative path of the files previously scanned and stored into an XML.
  @param map : The map object on which we want to add the elements.
  @param xmlDocument : The source xml document from which the info are retrieved.
  @author Caz
**/
void Utils::generateMap(QMap<QString, QString> &map, const QDomDocument &xmlDocument)
{
    // The root node is supposed to be a "folder" tag, we retrieve its name
    QDomElement root = xmlDocument.documentElement();
    qDebug() << "The folder name is " << root.attribute("name","NO NAME")<<endl;

    // We traverse its children
    QDomElement child = root.firstChild().toElement();
    while(!child.isNull())
    {
        // We know how to treat file
        if (child.tagName() == "files")
        {
            // We traverse children (file)
            QDomElement grandChild = child.firstChild().toElement();
            while(!grandChild.isNull())
            {
                // We know how to treat name and hash
                if (grandChild.tagName() == "file")
                {
                    // We traverse children (name & hash)
                    QDomNodeList fileNodes = grandChild.childNodes();
                    QString name, hash;
                    for(uint i=0; i<fileNodes.length(); i++)
                    {
                        QString nodeName = fileNodes.item(i).nodeName();
                        QDomElement nodeElement = fileNodes.item(i).toElement();
                        // We are looking for child "name"
                        if (nodeName == "name")
                            name = nodeElement.text();
                        // We are looking for child "hash"
                        if (nodeName == "hash")
                            hash = nodeElement.text();
                    }
                    map.insert(name, hash);
                }
                grandChild = grandChild.nextSibling().toElement();
            }
        }
        child = child.nextSibling().toElement();
    }
}

/**
  This method is used to save a file and it's content with the QT 4.7 serialization format.
  @param fileName : the file path we want to save.
  @param xmlDocument : the reference to xmlDocument from which we get the content.
  @author Caz
**/
void Utils::saveFile(QString fileName, const QDomDocument &xmlDocument)
{
    QFile file( fileName );
    if( !file.open( QIODevice::WriteOnly ) )
    {
        qDebug() << "Saving - Failed to save file.";
        return;
    }

    QTextStream ts( &file );
    ts << xmlDocument.toString();
    file.close();
}

/**
  This method is used to load a file saved with the QT 4.7 serialization format.
  @param fileName : the reference file path we want to load.
  @param xmlDocument : the reference xmlDocument to load the file in.
  @author Caz
**/
void Utils::loadFile(const QString &fileName, QDomDocument &xmlDocument)
{
    QFile xmlFile(fileName);
    if(!xmlDocument.setContent(&xmlFile))
    {
        qDebug() << "Loading - Failed to load file : " + xmlFile.fileName();
    } else {
        QDomElement root = xmlDocument.documentElement();
    }
}
