#include "choghondar_main.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("choghondar");
    a.setWindowIcon(QIcon(":/resources/icons/choghondar.png"));

    choghondar_main w;
    w.show();

    return a.exec();
}
