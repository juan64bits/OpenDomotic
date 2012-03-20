#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QtXml>
#include <QCryptographicHash>

#include "ui_passworddialog.h"
class Ui_PasswordDialog;

#include "signaldisplay.h"
#include "domo.h"
#include "serialport.h"
#include "callapp.h"
#include "simplecrypt.h"

#define defaultSerialName       "/dev/ttyUSB2"
#define SettingsFileName        "settings.xml"
#define ScreensaverOn           "gnome-screensaver-command -a"
#define ScreensaverPoke         "gnome-screensaver-command -p"
#define ScreensaverInhibit      "gnome-screensaver-command -i"
#define ScreenForceOn           "xset dpms force on"
#define ScreenForceOff          "xset dpms force on" //TODO
#define VoiceMessage            "xset dpms force on"
#define OpenMusicPlayer         ""
#define TimeToInitSecuredMode   10 //Seconds
#define TimeToAutoSaveSettings  10 //MInutes

#define CryptoKey               Q_UINT64_C(0xFF00FF00FF00FF00) //TODO Change!


#include <QThread>

class Sleeper : public QThread
{
public:
    static void usleep(unsigned long usecs){QThread::usleep(usecs);}
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
    static void sleep(unsigned long secs){QThread::sleep(secs);}
};

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);

private slots:
    void updateGuiClock();
    void checkSerial();    

    void updatePage();
    void tryToDisarmSecurity();
    void updateSecurity();
    void uploadSecurity();

    void scanPorts();

    void screensaverOn();
    void screenForceOn();
    void screenForceOff();

    void voiceAlert(QString);

    void message(int mode = -1);

    void chkWidgetsSetCheckable(bool);
    void chkWidgetsSetChecked(bool);
    void chkWidgetsSetEnable(bool);
    void chkWidgetsClearFocus();
    void chkLigthWidgetsSetExclusiveCheckable(bool);

    void chkWidgetsDefaultIcon();        

    /* Energy */
    void graphMin();
    void graphDay();
    void graphMonth();

    /* Config  */
    void updateDialShortValue(int);
    void updateDialLongValue(int);

    int loadSettings();
    int saveSettings();
    void saveSecuritySettings();
    void saveGeneralSettings();
    void uploadSecuritySettings();
    void uploadGeneralSettings();

    void unlockPageS();
    void changePass();

    void cancelSecuredMode();

    void restoreSecuritySetting();
    void restoreNigthSettings();
    void restoreTravelSettings();
    void restorePassSettings();
    void restoreGeneralSettings();
    void restoreTimeLigthsSettings();

    void getSecuritySettings();
    void getNigthSettings();
    void getTravelSettings();
    void getPassSettings();
    void getGeneralSettings();
    void getTimeLigthsSettings();

    /* Pass dialog */
    int checkPassword();
    QString getPasswordIn(QString);
    void pressed0();
    void pressed1();
    void pressed2();
    void pressed3();
    void pressed4();
    void pressed5();
    void pressed6();
    void pressed7();
    void pressed8();
    void pressed9();
    void pressedDel();
    void pressedOk();
    void pressedCancel();
    void addToPass(int);

    /* Reconfiguration over serial */
    void sentDataFrame(char command, char byte0=NULL,
                      char byte1=NULL, char byte2=NULL);

    void setNigthMovMask();
    void setNigthMagMask();
    void setNigthLigMask();
    void setNigthOthMask();

    void setTravelMovMask();
    void setTravelMagMask();
    void setTravelLigMask();
    void setTravelOthMask();

    void setCurrentTime();
    void setNigthInitTime();
    void setNigthEndTime();
    void setAlarmClockTime();
    void setNormalLigMask();
    void setLigthTimeShort();
    void setLigthTimeLong();

    void setGeneralMask();

    int cmdEchoTest();
    void cmdInitNigthMode();
    void cmdInitTravelMode();
    void cmdDisarmSecurity();
    void cmdTriggerAlarm();
    void cmdTurnOnLigth(unsigned char id);
    void cmdTurnOnLigths();
    void cmdTurnOffLigth(unsigned char id);
    void cmdTurnOffLigths();
    void cmdDisarmAlarmClock();

    /* Other */
    void toogleLigth(int);
    void ligthPressed();
    void ligthPressed2();
    void ligthPressed3();
    void ligthPressed4();
    void ligthPressed5();
    void ligthPressed6();
    void ligthPressed7();
    void ligthPressed8();

signals:
    void refresh();
    void _messageState();
    void _homeState();

    void _cleanTabState();
    void _homeTabState();
    void _energyTabState();
    void _settingsGTabState();
    void _settingsSTabState();


private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsProxyWidget *proxyAreaP;
    QGraphicsProxyWidget *proxyPlanFrame;
    QGraphicsProxyWidget *proxyTabWidget;
    QTimer *clockTimer;
    QTimer *serialTimer;    
    SerialPort *serialAvr;  

    /* settings */
    bool nigthMov[MOV_SENSORS];
    bool nigthMag[MAG_SENSORS];
    bool nigthLig[LIGTHS];
    bool nigthLEDs;
    bool nigthTone;
    bool nigthCall;
    bool nigthLigths;
    bool nigthLigthsOff;
    bool nigthSiren;

    bool travelMov[MOV_SENSORS];
    bool travelMag[MAG_SENSORS];
    bool travelLig[LIGTHS];
    bool travelLEDs;
    bool travelTone;
    bool travelCall;
    bool travelLigths;
    bool travelLigthsOff;
    bool travelSiren;

    bool passActive;
    QString pass;

    bool timeAlarmOn;
    QString timeAlarm;
    bool alarmLigths;
    bool alarmSiren;
    bool nigthManual;
    QString timeNigthInit;
    QString timeNigthEnd;    

    int kwhPrice;
    unsigned char readDay;

    bool sensorOn[LIGTHS];
    unsigned char timeShort[LIGTHS];
    unsigned char timeLong[LIGTHS];   
        
    /* temporal */
    bool t_nigthMov[MOV_SENSORS];
    bool t_nigthMag[MAG_SENSORS];
    bool t_nigthLig[LIGTHS];
    bool t_nigthLEDs;
    bool t_nigthTone;
    bool t_nigthCall;
    bool t_nigthLigths;
    bool t_nigthLigthsOff;
    bool t_nigthSiren;

    bool t_travelMov[MOV_SENSORS];
    bool t_travelMag[MAG_SENSORS];
    bool t_travelLig[LIGTHS];
    bool t_travelLEDs;
    bool t_travelTone;
    bool t_travelCall;
    bool t_travelLigths;
    bool t_travelLigthsOff;
    bool t_travelSiren;

    bool t_sensorOn[LIGTHS];
    unsigned char t_timeShort[LIGTHS];
    unsigned char t_timeLong[LIGTHS];

    /* VARS */
    int previousChkLigthIndex;
    bool firstSecureModeEdit;
    bool firstClickNigth;
    bool firstClickTravel;
    bool firstRun;
    bool updatingSecurity;
    QString serialName;
    QStringList serials;
    int serialCounter;
    int saveCounter;
    int initCounter;

    double powerAcc;
    double powerAccDay;

    std::vector<double> powerMin;
    std::vector<double> powerDay;
    std::vector<double> powerMonth;

    QDate lastDate;

    bool secured;
    bool securedMode;
    bool alarmActivated;
    bool isNigth;
    bool alarmClockActivated;
    unsigned char ligthsState;

    /* Password */
    QDialog *passwordDialog;
    Ui_PasswordDialog *passUi;
    bool buttonOkPresed;

    /* Others */
    QStateMachine *machine;
    QState *stateHome;
    QState *stateMessage;
    QStateMachine *tabMachine;
    QState *rootTabState;
    QState *cleanTabState;
    QState *homeTabState;
    QState *energyTabState;
    QState *settingsGTabState;
    QState *settingsSTabState;
};

#endif // MAINWINDOW_H
