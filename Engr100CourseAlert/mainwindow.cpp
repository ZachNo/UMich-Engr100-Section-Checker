#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->StartButton, SIGNAL(clicked(bool)), this, SLOT(setupCheck()));
    connect(ui->logWindow, SIGNAL(clicked()), this, SLOT(openLogWindow()));
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(stopLoop()));
    ui->lWindow->hide();
    logWindow += "Class Watch started, current time is:\n   ";
    QTime time = time.currentTime();
    QDateTime date = date.currentDateTime();
    logWindow += date.toString("ddd MMM d yyyy h:mm AP");
    logWindow += "\n";
    view = NULL;

    QString noDir = QDir::currentPath() + "/no.png";
    QString yesDir = QDir::currentPath() + "/yes.png";
    unAvail.load(noDir);
    if (unAvail.isNull())
    {
        logWindow += "Unable to load status images\n   ";
        logWindow += noDir;
        logWindow += "\n";
    }
    avail.load(yesDir);
    if (avail.isNull())
    {
        logWindow += "Unable to load status images\n   ";
        logWindow += yesDir;
        logWindow += "\n";
    }
    ui->ClassStat_1->setPixmap(QPixmap::fromImage(unAvail));
    ui->ClassStat_2->setPixmap(QPixmap::fromImage(unAvail));
    ui->ClassStat_3->setPixmap(QPixmap::fromImage(unAvail));
    ui->ClassStat_4->setPixmap(QPixmap::fromImage(unAvail));
    ui->ClassStat_5->setPixmap(QPixmap::fromImage(unAvail));
    ui->ClassStat_6->setPixmap(QPixmap::fromImage(unAvail));

    class1Open = 0;
    class2Open = 0;
    class3Open = 0;
    class4Open = 0;
    class5Open = 0;
    class6Open = 0;

    ui->progressBar->setMaximum(100);
    ui->progressBar->setMinimum(0);

    loop = NULL;

    //Default for me
    ui->Url_Edit->setText("http://www.lsa.umich.edu/cg/cg_detail.aspx?content=2020ENGR100100&termArray=w_15_2020");
    ui->ClassEdit_1->setText("19400");
    ui->ClassEdit_2->setText("26749");
    ui->intervalInput->setText("300");
}

MainWindow::~MainWindow()
{
    delete loop;
    delete view;
    delete ui;
}

void MainWindow::setupCheck()
{
    bool isNumeric = true;
    loopTime = ui->intervalInput->text().toLong(&isNumeric);
    if(!ui->intervalInput->text().compare("") || !isNumeric)
    {
        ui->intervalInput->setText("Enter an number!");
        logWindow += "Interval slot not filled!\n";
        return;
    }

    url = QUrl(ui->Url_Edit->text());
    isNumeric = true;

    class1 = ui->ClassEdit_1->text();
    class1.toLong(&isNumeric);
    if(isNumeric == false && class1.compare(""))
    {
        ui->ClassEdit_1->setText("Non-number!");
        logWindow += "Class1 non-number or Empty\n";
        class1OK = false;
    }
    else if(!class1.compare(""))
        class1OK = false;
    else
        class1OK = true;

    class2 = ui->ClassEdit_2->text();
    class2.toLong(&isNumeric);
    if(isNumeric == false && class2.compare(""))
    {
        ui->ClassEdit_2->setText("Non-number!");
        logWindow += "Class2 non-number or Empty\n";
        class2OK = false;
    }
    else if(!class2.compare(""))
        class2OK = false;
    else
        class2OK = true;
    isNumeric = true;

    class3 = ui->ClassEdit_3->text();
    class3.toLong(&isNumeric);
    if(isNumeric == false && class3.compare(""))
    {
        ui->ClassEdit_3->setText("Non-number!");
        logWindow += "Class3 non-number or Empty\n";
        class3OK = false;
    }
    else if(!class3.compare(""))
        class3OK = false;
    else
        class3OK = true;
    isNumeric = true;

    class4 = ui->ClassEdit_4->text();
    class4.toLong(&isNumeric);
    if(isNumeric == false && class4.compare(""))
    {
        ui->ClassEdit_4->setText("Non-number!");
        logWindow += "Class4 non-number or Empty\n";
        class4OK = false;
    }
    else if(!class4.compare(""))
        class4OK = false;
    else
        class4OK = true;
    isNumeric = true;

    class5 = ui->ClassEdit_5->text();
    class5.toLong(&isNumeric);
    if(isNumeric == false && class5.compare(""))
    {
        ui->ClassEdit_5->setText("Non-number!");
        logWindow += "Class5 non-number or Empty\n";
        class5OK = false;
    }
    else if(!class5.compare(""))
        class5OK = false;
    else
        class5OK = true;
    isNumeric = true;

    class6 = ui->ClassEdit_6->text();
    class6.toLong(&isNumeric);
    if(isNumeric == false && class6.compare(""))
    {
        ui->ClassEdit_6->setText("Non-number!");
        logWindow += "Class6 non-number or Empty\n";
        class6OK = false;
    }
    else if(!class6.compare(""))
        class6OK = false;
    else
        class6OK = true;

    if(view != NULL)
    {
        delete view;
        view = NULL;
    }
    view = new QWebView(this);
    connect(view, SIGNAL(loadFinished(bool)), this, SLOT(parsePage(bool)));
    view->load(url);

    loop = new QTimer();
    connect(loop, SIGNAL(timeout()), this, SLOT(checkWebpageLoop()));
    loop->start(1000);
}

void MainWindow::parsePage(bool ok)
{
    if(!ok)
    {
        ui->Connection_Label->setText("Connection failed!");
        logWindow += "Connection to ";
        logWindow += url.toString();
        logWindow += " failed\n";
        return;
    }
    else
    {
        ui->Connection_Label->setText("Connection successful!");
        /*logWindow += "Connection to ";
        logWindow += url.toString();
        logWindow += " successful\n";*/
    }

    QString hyperText;
    if(view)
    {
        QWebPage *webPagePtr = view->page();
        if(webPagePtr)
        {
            QWebFrame *webFramePtr = webPagePtr->mainFrame();
            if(webFramePtr)
            {
                hyperText = webFramePtr->toHtml();
            }
        }
    }
    QTime time;
    if(class1OK)
    {
        int class1Index = hyperText.indexOf(ui->ClassEdit_1->text());
        if(class1Index == -1)
        {
            ui->ClassEdit_1->setText("Class not found");
            logWindow += "Class 1 not found on page.\n";
            class1OK = 0;
        }
        else
        {
            int open = hyperText.indexOf("Open", class1Index);
            int closed = hyperText.indexOf("Closed", class1Index);
            if(open == -1 || closed == -1)
            {
                if(open != -1)
                {
                    if(class1Open)
                    {
                        logWindow += "Class1 closed at ";
                        logWindow += time.toString(Qt::ISODate);
                        logWindow += " ";
                        logWindow += time.toString(Qt::TextDate);
                        logWindow += ".\n";
                    }
                    class1Open = false;
                }
                else
                {
                    if(!class1Open)
                    {
                        logWindow += "Class1 opened at ";
                        logWindow += time.toString(Qt::ISODate);
                        logWindow += " ";
                        logWindow += time.toString(Qt::TextDate);
                        logWindow += ".\n";
                    }
                    class1Open = true;
                }
            }
            else if(open < closed)
            {
                if(!class1Open)
                {
                    logWindow += "Class1 opened at ";
                    logWindow += time.toString(Qt::ISODate);
                        logWindow += " ";
                        logWindow += time.toString(Qt::TextDate);
                    logWindow += ".\n";
                }
                class1Open = true;
            }
            else
            {
                if(class1Open)
                {
                    logWindow += "Class1 closed at ";
                    logWindow += time.toString(Qt::ISODate);
                        logWindow += " ";
                        logWindow += time.toString(Qt::TextDate);
                    logWindow += ".\n";
                }
                class1Open = false;
            }
        }
    }
    if(class2OK)
    {
        int class2Index = hyperText.indexOf(ui->ClassEdit_2->text());
        if(class2Index == -1)
            {
                ui->ClassEdit_2->setText("Class not found");
                logWindow += "Class 2 not found on page.\n";
                class2OK = 0;
            }
            else
            {
                int open = hyperText.indexOf("Open", class2Index);
                int closed = hyperText.indexOf("Closed", class2Index);
                if(open == -1 || closed == -1)
                {
                    if(open != -1)
                    {
                        if(class2Open)
                        {
                            logWindow += "class2 closed at ";
                            logWindow += time.toString(Qt::ISODate);
                        logWindow += " ";
                        logWindow += time.toString(Qt::TextDate);
                            logWindow += ".\n";
                        }
                        class2Open = false;
                    }
                    else
                    {
                        if(!class2Open)
                        {
                            logWindow += "class2 opened at ";
                            logWindow += time.toString(Qt::ISODate);
                        logWindow += " ";
                        logWindow += time.toString(Qt::TextDate);
                            logWindow += ".\n";
                        }
                        class2Open = true;
                    }
                }
                else if(open < closed)
                {
                    if(!class2Open)
                    {
                        logWindow += "class2 opened at ";
                        logWindow += time.toString(Qt::ISODate);
                        logWindow += " ";
                        logWindow += time.toString(Qt::TextDate);
                        logWindow += ".\n";
                    }
                    class2Open = true;
                }
                else
                {
                    if(class2Open)
                    {
                        logWindow += "class2 closed at ";
                        logWindow += time.toString(Qt::ISODate);
                        logWindow += " ";
                        logWindow += time.toString(Qt::TextDate);
                        logWindow += ".\n";
                    }
                    class2Open = false;
                }
            }
        }
    if(class3OK)
        {
            int class3Index = hyperText.indexOf(ui->ClassEdit_3->text());
            if(class3Index == -1)
            {
                ui->ClassEdit_3->setText("Class not found");
                logWindow += "Class 3 not found on page.\n";
                class3OK = 0;
            }
            else
            {
                int open = hyperText.indexOf("Open", class3Index);
                int closed = hyperText.indexOf("Closed", class3Index);
                if(open == -1 || closed == -1)
                {
                    if(open != -1)
                    {
                        if(class3Open)
                        {
                            logWindow += "class3 closed at ";
                            logWindow += time.toString(Qt::ISODate);
                        logWindow += " ";
                        logWindow += time.toString(Qt::TextDate);
                            logWindow += ".\n";
                        }
                        class3Open = false;
                    }
                    else
                    {
                        if(!class3Open)
                        {
                            logWindow += "class3 opened at ";
                            logWindow += time.toString(Qt::ISODate);
                        logWindow += " ";
                        logWindow += time.toString(Qt::TextDate);
                            logWindow += ".\n";
                        }
                        class3Open = true;
                    }
                }
                else if(open < closed)
                {
                    if(!class3Open)
                    {
                        logWindow += "class3 opened at ";
                        logWindow += time.toString(Qt::ISODate);
                        logWindow += " ";
                        logWindow += time.toString(Qt::TextDate);
                        logWindow += ".\n";
                    }
                    class3Open = true;
                }
                else
                {
                    if(class3Open)
                    {
                        logWindow += "class3 closed at ";
                        logWindow += time.toString(Qt::ISODate);
                        logWindow += " ";
                        logWindow += time.toString(Qt::TextDate);
                        logWindow += ".\n";
                    }
                    class3Open = false;
                }
            }
        }
    if(class4OK)
        {
            int class4Index = hyperText.indexOf(ui->ClassEdit_4->text());
            if(class4Index == -1)
            {
                ui->ClassEdit_4->setText("Class not found");
                logWindow += "Class 4 not found on page.\n";
                class4OK = 0;
            }
            else
            {
                int open = hyperText.indexOf("Open", class4Index);
                int closed = hyperText.indexOf("Closed", class4Index);
                if(open == -1 || closed == -1)
                {
                    if(open != -1)
                    {
                        if(class4Open)
                        {
                            logWindow += "class4 closed at ";
                            logWindow += time.toString(Qt::ISODate);
                        logWindow += " ";
                        logWindow += time.toString(Qt::TextDate);
                            logWindow += ".\n";
                        }
                        class4Open = false;
                    }
                    else
                    {
                        if(!class4Open)
                        {
                            logWindow += "class4 opened at ";
                            logWindow += time.toString(Qt::ISODate);
                        logWindow += " ";
                        logWindow += time.toString(Qt::TextDate);
                            logWindow += ".\n";
                        }
                        class4Open = true;
                    }
                }
                else if(open < closed)
                {
                    if(!class4Open)
                    {
                        logWindow += "class4 opened at ";
                        logWindow += time.toString(Qt::ISODate);
                        logWindow += " ";
                        logWindow += time.toString(Qt::TextDate);
                        logWindow += ".\n";
                    }
                    class4Open = true;
                }
                else
                {
                    if(class4Open)
                    {
                        logWindow += "class4 closed at ";
                        logWindow += time.toString(Qt::ISODate);
                        logWindow += " ";
                        logWindow += time.toString(Qt::TextDate);
                        logWindow += ".\n";
                    }
                    class4Open = false;
                }
            }
        }
    if(class5OK)
        {
            int class5Index = hyperText.indexOf(ui->ClassEdit_5->text());
            if(class5Index == -1)
            {
                ui->ClassEdit_5->setText("Class not found");
                logWindow += "Class 5 not found on page.\n";
                class5OK = 0;
            }
            else
            {
                int open = hyperText.indexOf("Open", class5Index);
                int closed = hyperText.indexOf("Closed", class5Index);
                if(open == -1 || closed == -1)
                {
                    if(open != -1)
                    {
                        if(class5Open)
                        {
                            logWindow += "class5 closed at ";
                            logWindow += time.toString(Qt::ISODate);
                        logWindow += " ";
                        logWindow += time.toString(Qt::TextDate);
                            logWindow += ".\n";
                        }
                        class5Open = false;
                    }
                    else
                    {
                        if(!class5Open)
                        {
                            logWindow += "class5 opened at ";
                            logWindow += time.toString(Qt::ISODate);
                        logWindow += " ";
                        logWindow += time.toString(Qt::TextDate);
                            logWindow += ".\n";
                        }
                        class5Open = true;
                    }
                }
                else if(open < closed)
                {
                    if(!class5Open)
                    {
                        logWindow += "class5 opened at ";
                        logWindow += time.toString(Qt::ISODate);
                        logWindow += " ";
                        logWindow += time.toString(Qt::TextDate);
                        logWindow += ".\n";
                    }
                    class5Open = true;
                }
                else
                {
                    if(class5Open)
                    {
                        logWindow += "class5 closed at ";
                        logWindow += time.toString(Qt::ISODate);
                        logWindow += " ";
                        logWindow += time.toString(Qt::TextDate);
                        logWindow += ".\n";
                    }
                    class5Open = false;
                }
            }
        }
    if(class6OK)
        {
            int class6Index = hyperText.indexOf(ui->ClassEdit_6->text());
            if(class6Index == -1)
            {
                ui->ClassEdit_6->setText("Class not found");
                logWindow += "Class 6 not found on page.\n";
                class6OK = 0;
            }
            else
            {
                int open = hyperText.indexOf("Open", class6Index);
                int closed = hyperText.indexOf("Closed", class6Index);
                if(open == -1 || closed == -1)
                {
                    if(open != -1)
                    {
                        if(class6Open)
                        {
                            logWindow += "class6 closed at ";
                            logWindow += time.toString(Qt::ISODate);
                        logWindow += " ";
                        logWindow += time.toString(Qt::TextDate);
                            logWindow += ".\n";
                        }
                        class6Open = false;
                    }
                    else
                    {
                        if(!class6Open)
                        {
                            logWindow += "class6 opened at ";
                            logWindow += time.toString(Qt::ISODate);
                        logWindow += " ";
                        logWindow += time.toString(Qt::TextDate);
                            logWindow += ".\n";
                        }
                        class6Open = true;
                    }
                }
                else if(open < closed)
                {
                    if(!class6Open)
                    {
                        logWindow += "class6 opened at ";
                        logWindow += time.toString(Qt::ISODate);
                        logWindow += " ";
                        logWindow += time.toString(Qt::TextDate);
                        logWindow += ".\n";
                    }
                    class6Open = true;
                }
                else
                {
                    if(class6Open)
                    {
                        logWindow += "class6 closed at ";
                        logWindow += time.toString(Qt::ISODate);
                        logWindow += " ";
                        logWindow += time.toString(Qt::TextDate);
                        logWindow += ".\n";
                    }
                    class6Open = false;
                }
            }
        }
    updateStats();
    checkWebpageLoop();
}

void MainWindow::updateStats()
{
    if(class1Open)
        ui->ClassStat_1->setPixmap(QPixmap::fromImage(avail));
    else
        ui->ClassStat_1->setPixmap(QPixmap::fromImage(unAvail));
    if(class2Open)
        ui->ClassStat_2->setPixmap(QPixmap::fromImage(avail));
    else
        ui->ClassStat_2->setPixmap(QPixmap::fromImage(unAvail));
    if(class3Open)
        ui->ClassStat_3->setPixmap(QPixmap::fromImage(avail));
    else
        ui->ClassStat_3->setPixmap(QPixmap::fromImage(unAvail));
    if(class4Open)
        ui->ClassStat_4->setPixmap(QPixmap::fromImage(avail));
    else
        ui->ClassStat_4->setPixmap(QPixmap::fromImage(unAvail));
    if(class5Open)
        ui->ClassStat_5->setPixmap(QPixmap::fromImage(avail));
    else
        ui->ClassStat_5->setPixmap(QPixmap::fromImage(unAvail));
    if(class6Open)
        ui->ClassStat_6->setPixmap(QPixmap::fromImage(avail));
    else
        ui->ClassStat_6->setPixmap(QPixmap::fromImage(unAvail));
}

void MainWindow::openLogWindow()
{
    if(ui->lWindow->isVisible())
        ui->lWindow->hide();
    else
        ui->lWindow->show();
    ui->lWindow->setPlainText(logWindow);
}

void MainWindow::checkWebpageLoop()
{
    ui->lWindow->setPlainText(logWindow);
    if(timer < loopTime)
    {
        timer += 1;
        ui->progressBar->setValue((timer*100)/loopTime);
        int minutes = (loopTime - timer) / 60;
        int seconds = 59 - timer % 60;
        QString countLabel = QString::number(minutes);
        countLabel += ":";
        countLabel += QString::number(seconds);
        countLabel += " until reload.";
        ui->Countdown_Label->setText(countLabel);
        return;
    }
    view->reload();
    timer  = 0;
}

void MainWindow::stopLoop()
{
    loop->stop();
}
