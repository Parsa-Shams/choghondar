#include "pasterthread.h"
#include <QDebug>

PasterThread::PasterThread(QString dataPath){
    QFile file(dataPath);
    QTextStream in(&file);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        dataList.clear();
        while (!in.atEnd()) {
            QString line = in.readLine();
            dataList.append(line.split(","));
        }
    }
}


void PasterThread::run(){
    if(!dataList.isEmpty()){
        while (!isInterruptionRequested())
        {
            while (!(GetAsyncKeyState(VK_CONTROL) & 0x8000) || !(GetAsyncKeyState('B') & 0x8000)) {
                Sleep(10);
                if(isInterruptionRequested()) return;
            }

            foreach(QList<QString> data, dataList){
                for(int i =0; i < 4; i++){
                    pasteTextAtCursor(data[i]);
                    Sleep(1);
                }
            }
        }
    }
}

void PasterThread::pasteTextAtCursor(QString text) {
    keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
    for (const QChar &c : text) {
        keybd_event(c.toLatin1(), 0, 0, 0);
        keybd_event(c.toLatin1(), 0, KEYEVENTF_KEYUP, 0);
    }
}
