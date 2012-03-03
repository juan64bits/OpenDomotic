#ifndef CALLAPP_H
#define CALLAPP_H

#include <QWidget>
#include <QtCore>

class callApp : public QWidget
{
Q_OBJECT
public:
    explicit callApp(QString,QWidget *parent = 0);
    QString readData()
        { return data;}
    bool isRuning();
    void writeData(QString);

signals:
    void newData();
    void appClosed();

public slots:
    void newStderr();
    void newStdout();
    void deleteProcess();

private:
    QProcess * myProcess;
    QString data;
};

#endif // CALLAPP_H
