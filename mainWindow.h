#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QListWidget>
#include <QCheckBox>
#include <QDirIterator>
#include <QStringList>
#include <QHBoxLayout>
#include <QDialog>
#include <QTextBrowser>
#include <QByteArray>
#include <QFile>
#include <QCryptographicHash>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    void openFolderDialog();
    void generateHash();
    void updateList();

private:
    QString easyHash(QString &fileName, QCryptographicHash::Algorithm al);

    QLabel *m_SelectPathText;
    QLineEdit *m_SelectedPath;
    QPushButton *m_button, *m_validate;
    QString m_lastSelectedDirectory, t, h;
    QCheckBox *m_checkSub;
    QGridLayout *m_gridLayout;
    QVBoxLayout *m_vboxLayout;
    QHBoxLayout *m_confirm;
    QListWidget *m_fileList;
    QStringList fileList, listFilter;
    QDialog *m_windowResult;
    QTextBrowser *m_formattedResult;
    QFile *currentFile;
};

#endif // MAINWINDOW_H
