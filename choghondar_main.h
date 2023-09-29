#ifndef CHOGHONDAR_MAIN_H
#define CHOGHONDAR_MAIN_H

#include "pasterthread.h"
#include <windows.h>
#include <QMainWindow>
#include <QApplication>
#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QToolBar>
#include <QFileDialog>
#include <QFile>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class choghondar_main; }
QT_END_NAMESPACE

class choghondar_main : public QMainWindow
{
    Q_OBJECT

public:
    choghondar_main(QWidget *parent = nullptr);
    ~choghondar_main();

private:
    Ui::choghondar_main *ui;
    QVBoxLayout* verticalLayout;
    QList<QList<QLineEdit*>> lineEditLists;
    QString saveDir;
    QString openDir;
    bool started = 0;
    bool unchenged = 1;
    PasterThread* pasterThread;
    QPushButton *startButton;
    QPushButton *addButton;
    QPushButton *saveButton;
    QPushButton *openButton;
    QLabel* startLabel;
    QIcon startIcon;
    QIcon stopIcon;

private slots:
    void saveData();
    void readData();
    void addNewLine();
    void start();
    void removeLine(QWidget* lineWidget);
    void setCursorLineEdit();
};
#endif // CHOGHONDAR_MAIN_H
