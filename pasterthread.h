#ifndef PASTERTHREAD_H
#define PASTERTHREAD_H
#include <QThread>
#include <windows.h>
#include <QFile>
#include <QTextStream>

class PasterThread : public QThread
{
public:
    PasterThread(QString dataPath);

protected:
    void run() override;

private:
    void pasteTextAtCursor(QString text);
    QList<QList<QString>> dataList;
};

#endif // PASTERTHREAD_H
