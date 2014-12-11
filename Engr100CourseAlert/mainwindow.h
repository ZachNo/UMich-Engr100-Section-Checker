#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWindow>
#include <QWebView>
#include <QWebFrame>
#include <QPlainTextEdit>
#include <QDir>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
    void setupCheck();
    void parsePage(bool ok);
    void openLogWindow();
    void checkWebpageLoop();
    void stopLoop();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QUrl url;
    QString class1;
    bool class1OK;
    bool class1Open;
    QString class2;
    bool class2OK;
    bool class2Open;
    QString class3;
    bool class3OK;
    bool class3Open;
    QString class4;
    bool class4OK;
    bool class4Open;
    QString class5;
    bool class5OK;
    bool class5Open;
    QString class6;
    bool class6OK;
    bool class6Open;
    QWebView *view;
    QString logWindow;
    QImage unAvail;
    QImage avail;
    QTimer *loop;
    long timer;
    long loopTime;

    void updateStats();
    void reloadPage();
};

#endif // MAINWINDOW_H
