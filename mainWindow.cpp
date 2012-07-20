#include <mainWindow.h>

MainWindow::MainWindow() : QWidget()
{
    setFixedSize(640, 480);

    m_SelectPathText = new QLabel("Select folder path :");
    m_SelectPathText->setGeometry(0, 0, 100, 20);
    m_SelectedPath = new QLineEdit();
    m_SelectedPath->setGeometry(100, 0, 400, 20);
    m_button = new QPushButton("MyButton");
    m_button->setCursor(Qt::PointingHandCursor);
    m_button->setIcon(QIcon(":/icons/add_folder"));
    m_checkSub = new QCheckBox("Include Sub-Folders?");
    m_fileList = new QListWidget;
    m_validate = new QPushButton("Generate hashes!");

    //Layouts

    m_gridLayout = new QGridLayout;
    m_gridLayout->addWidget(m_SelectPathText, 0, 0);
    m_gridLayout->addWidget(m_SelectedPath, 0, 1);
    m_gridLayout->addWidget(m_button, 0, 2);
    m_gridLayout->addWidget(m_checkSub, 1, 2);

    m_confirm = new QHBoxLayout;
    m_confirm->addWidget(m_validate);

    m_vboxLayout = new QVBoxLayout;
    m_vboxLayout->addLayout(m_gridLayout);
    m_vboxLayout->addWidget(m_fileList);
    m_vboxLayout->addLayout(m_confirm);

    this->setLayout(m_vboxLayout);

    // Result Window
    m_formattedResult = new QTextBrowser;
    m_windowResult = new QDialog(this);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_formattedResult);
    m_windowResult->setLayout(layout);

    //SLOTS & SIGNALS
    QObject::connect(m_button, SIGNAL(clicked()), this, SLOT(openFolderDialog()));
    QObject::connect(m_validate, SIGNAL(clicked()), this, SLOT(generateHash()));
    QObject::connect(m_checkSub, SIGNAL(clicked()), this, SLOT(updateList()));
}

void MainWindow::updateList()
{
    fileList.clear();
    m_fileList->clear();
    //If we want to go across all sub directories
    if(m_checkSub->isChecked())
    {
        //Looping through the dir and it's sub-dirs
        QDirIterator dirIterator(m_lastSelectedDirectory, listFilter, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);

        // While there are results
        while(dirIterator.hasNext())
        {
            fileList << dirIterator.next();
        }
    }
    else
    {
        //Looping through the dir and it's sub-dirs
        QDirIterator dirIterator(m_lastSelectedDirectory, listFilter, QDir::Files | QDir::NoSymLinks);

        // While there are results
        while(dirIterator.hasNext())
        {
            fileList << dirIterator.next();
        }
    }
    m_fileList->addItems(fileList);
}

void MainWindow::openFolderDialog()
 {
   //Update Last Selected Directory
    m_lastSelectedDirectory = "";
    QString result = QFileDialog::getExistingDirectory(this);
    if(result != m_lastSelectedDirectory && result != "")
    {
        m_lastSelectedDirectory = result;
        m_SelectedPath->setText(m_lastSelectedDirectory);
    }
    updateList();
 };

void MainWindow::generateHash()
{
    if(m_lastSelectedDirectory == "")
        QMessageBox::warning(this, "Warning", "No folder selected");
    else
    {
        t = "<xml>\n\t<folder src=\"" + m_lastSelectedDirectory.split("/").last() + "\">\n";
        foreach (QString str, fileList) {
            h = easyHash(str, QCryptographicHash::Md4);
            t += "\t\t<file>\n\t\t\t<name>" + str + "</name>\n\t\t\t<hash>"+h+"</hash>\n\t\t</file>\n";
            }
        t += "\t</folder>\n</xml>";
        m_formattedResult->setText(t);
        m_windowResult->exec();
    }
}

QString MainWindow::easyHash(QString &fileName, QCryptographicHash::Algorithm al)
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