#include <mainWindow.h>
#include <Utils.h>

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
    m_windowResult->setFixedSize(800, 600);
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
    (m_checkSub->isChecked() ? fileList << Utils::scanFolder(m_lastSelectedDirectory, true) : fileList << Utils::scanFolder(m_lastSelectedDirectory, false));
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
        QString output = Utils::generateXML(m_lastSelectedDirectory);
        m_formattedResult->setText(output);
        m_windowResult->exec();
        Utils::saveFile(m_lastSelectedDirectory.split("\\").last()+".xml", output);
    }
}
