#include "choghondar_main.h"
#include "ui_choghondar_main.h"
#include<QDebug>


choghondar_main::choghondar_main(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::choghondar_main)
{
    ui->setupUi(this);

    QWidget* addlineWidget = new QWidget;
    QHBoxLayout *addhorizontalLayout = new QHBoxLayout(addlineWidget);
    addButton = new QPushButton;
    QIcon addIcon(":/resources/icons/add-button.png");
    addButton->setIcon(addIcon);
    addButton->setText("New line");
    addButton->setToolTip("add a new line");
    connect(addButton, &QPushButton::clicked, this, &choghondar_main::addNewLine);
    addhorizontalLayout->addWidget(addButton);
    addlineWidget->setStyleSheet(
                "QWidget {"
                "    border : 0px;"
                "    margin : 0px;"
                "    padding: 0px;"
                "}"
                "QPushButton {"
                "    background-color: white;"
                "    border: 1px solid #CFCFCF;"
                "    border-radius: 5px;"
                "    padding: 7px;"
                "}"
                "QPushButton:hover {"
                "    border: 1px solid rgb(58, 195, 241);"
                "}"
                "QPushButton:pressed {"
                "    border: 2px solid rgb(58, 195, 241);"
                "}"
                "QLineEdit{"
                "    background-color: white;"
                "    border: 1px solid #CFCFCF;"
                "    border-radius: 5px;"
                "}"
    );

    startLabel = new QLabel;
    startLabel->setText("press Ctrl+B to paste ");
    startLabel->setAlignment(Qt::AlignHCenter);
    startLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui->toolBar->addWidget(startLabel);
    startLabel->setVisible(0);

    saveButton = new QPushButton;
    QIcon saveIcon(":/resources/icons/save-file.png");
    saveButton->setIcon(saveIcon);
    saveButton->setText("save");
    saveButton->setToolTip("Save the content of fields with 'Course unit code' and 'Course name'\nSaved file can be opened later");
    saveButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(saveButton, &QPushButton::clicked, this, &choghondar_main::saveData);
    ui->toolBar->addWidget(saveButton);

    openButton = new QPushButton;
    QIcon openIcon(":/resources/icons/open-folder.png");
    openButton->setIcon(openIcon);
    openButton->setText("open");
    openButton->setToolTip("open previosly saved field contents");
    openButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(openButton, &QPushButton::clicked, this, &choghondar_main::readData);
    ui->toolBar->addWidget(openButton);

    startButton = new QPushButton;
    startIcon.addFile(":/resources/icons/play.png");
    stopIcon.addFile(":/resources/icons/stop.png");
    startButton->setIcon(startIcon);
    startButton->setText("start");
    startButton->setToolTip("run the program to start pasting");
    startButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(startButton, &QPushButton::clicked, this, &choghondar_main::start);
    ui->toolBar->addWidget(startButton);

    verticalLayout = new QVBoxLayout();
    verticalLayout->setAlignment(Qt::AlignTop);
    verticalLayout->addWidget(addlineWidget);
    addNewLine();

    verticalLayout->setSpacing(0);
    QWidget* scrollContent = new QWidget();
    scrollContent->setLayout(verticalLayout);
    ui->scrollArea->setWidget(scrollContent);
    ui->scrollArea->setWidgetResizable(true);

    QWidget* mainLayout = new QWidget;
    mainLayout->setLayout(ui->mainLayout);
    setCentralWidget(mainLayout);


    QFile styleFile(":/resources/style.qss");
    if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream styleStream(&styleFile);
        QString styleSheet;
        styleSheet = styleStream.readAll();
        qApp->setStyleSheet(styleSheet);
        styleFile.close();
    }
}


void choghondar_main::start()
{
    if(!started){
        if(unchenged){
            if (!openDir.isEmpty()){pasterThread = new PasterThread(openDir);}
            else{pasterThread = new PasterThread(saveDir);}
        }else{
            if(lineEditLists.length()==1 and lineEditLists[0][0]->text().isEmpty()){
                readData();
                if(openDir.isEmpty()) return;
                pasterThread = new PasterThread(openDir);
            }else{
                saveData();
                if(saveDir.isEmpty()) return;
                pasterThread = new PasterThread(saveDir);
            }
        }
        pasterThread->start();
        startButton->setIcon(stopIcon);
        startButton->setText("stop");
        started = 1;
    }else{
        if (pasterThread && pasterThread->isRunning()){
            pasterThread->requestInterruption();
            pasterThread->wait();
            pasterThread->exit();
            delete pasterThread;
        }
        startButton->setIcon(startIcon);
        startButton->setText("start");
        started = 0;
    }
    startLabel->setVisible(started);
    saveButton->setVisible(!started);
    openButton->setVisible(!started);
    ui->scrollArea->setDisabled(started);
}

void choghondar_main::removeLine(QWidget* lineWidget) {
    lineEditLists.removeAt(verticalLayout->indexOf(lineWidget)-1);
    verticalLayout->removeWidget(lineWidget);
    delete lineWidget;

    unchenged = 0;
}


void choghondar_main::addNewLine() {
    QWidget* lineWidget = new QWidget;
    QHBoxLayout *horizontalLayout = new QHBoxLayout(lineWidget);

    QPushButton *removeButton = new QPushButton;
    QIcon removeIcon(":/resources/icons/trash.png");
    removeButton->setIcon(removeIcon);
    removeButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(removeButton, &QPushButton::clicked, [=]() { removeLine(lineWidget); });
    removeButton->setMinimumHeight(50);
    removeButton->setMaximumWidth(50);
    horizontalLayout->addWidget(removeButton);

    QLineEdit *courseCode = new QLineEdit;
    courseCode->setAlignment(Qt::AlignHCenter);
    courseCode->setPlaceholderText("Enter curse unit code");
    courseCode->setMaxLength(9);
    connect(courseCode, &QLineEdit::editingFinished, this, &choghondar_main::setCursorLineEdit);
    courseCode->setMinimumHeight(50);
    horizontalLayout->addWidget(courseCode);

    QLineEdit *courseName = new QLineEdit;
    courseName->setAlignment(Qt::AlignHCenter);
    courseName->setPlaceholderText("Course name (optional)");
    courseName->setMinimumHeight(50);
    horizontalLayout->addWidget(courseName);

    QList<QLineEdit*> lineEdits;
    lineEdits.clear();
    lineEdits.append(courseCode);
    lineEdits.append(courseName);
    lineEditLists.append(lineEdits);

    lineWidget->setStyleSheet(
                "QWidget {"
                "    border : 0px;"
                "    margin : 0px;"
                "    padding: 0px;"
                "}"
                "QPushButton {"
                "    background-color: white;"
                "    border: 1px solid #CFCFCF;"
                "    border-radius: 5px;"
                "    padding: 5px;"
                "}"
                "QPushButton:hover {"
                "    border: 1px solid rgb(58, 195, 241);"
                "}"
                "QPushButton:pressed {"
                "    border: 2px solid rgb(58, 195, 241);"
                "}"
                "QLineEdit{"
                "    background-color: white;"
                "    border: 1px solid #CFCFCF;"
                "    border-radius: 5px;"
                "}"
    );
    verticalLayout->addWidget(lineWidget);

    unchenged = 0;
}


void choghondar_main::setCursorLineEdit(){
    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(sender());
    if (lineEdit->text().length() > 0) {
        lineEdit->setInputMask("99,99,999,99");
        lineEdit->clearFocus();

        unchenged = 0;
    }

}


void choghondar_main::saveData(){
    saveDir = QFileDialog::getSaveFileName(nullptr, "Select File for Saving", QDir::homePath(), "CSV Files (*.csv)");
    QFile* outFile = new QFile(saveDir);
    if(!outFile or !outFile->open(QIODevice::WriteOnly | QIODevice::Text)) return;


    QTextStream out(outFile);
    foreach(QList<QLineEdit*> lineEdits, lineEditLists){
        if(lineEdits[0]->text().length()==12){out << lineEdits[0]->text() << "," << lineEdits[1]->text() << "\n";}
        else{lineEdits[1]->setText("Not saved (invalid course code format)");}
    }

    out.flush();
    outFile->close();
    delete outFile;

    unchenged = 1;
}


void choghondar_main::readData(){
    openDir = QFileDialog::getOpenFileName(nullptr, "Select File To Open", "*.csv");
    QFile* inFile = new QFile(openDir);
    if (!inFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(inFile);
    QList<QString> dataList;

    while (!in.atEnd()) {
        QString line = in.readLine();
        dataList.append(line);
    }

    if(!lineEditLists[lineEditLists.length()-1][0]->text().isEmpty())addNewLine();

    foreach(QString data, dataList){
        lineEditLists[lineEditLists.length()-1][0]->setMaxLength(12);
        lineEditLists[lineEditLists.length()-1][0]->setText(data.left(12));
        lineEditLists[lineEditLists.length()-1][1]->setText(data.mid(13));
        addNewLine();
    }

    in.flush();
    inFile->close();
    delete inFile;
    unchenged = 1;
}

choghondar_main::~choghondar_main()
{
    delete ui;
}


