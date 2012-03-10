#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);       

    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(updatePage()));

    ui->AreaP->setVisible(false);
    ui->btnMsg->setStyleSheet("QPushButton {color: blue}");

    /* HOME Page */
    updatingSecurity = false;
    connect(ui->btnSecurity,SIGNAL(clicked()),this,SLOT(updateSecurity()));
    connect(ui->btnScreenSaver,SIGNAL(clicked()),this,SLOT(screensaverOn()));
    connect(ui->btnScreenOff,SIGNAL(clicked()),this,SLOT(screenForceOff()));

    connect(ui->btnAlarm,SIGNAL(toggled(bool)),this,SLOT(saveGeneralSettings()));
    connect(ui->btnSound,SIGNAL(toggled(bool)),this,SLOT(saveSettings()));

    this->clockTimer = new QTimer(this);
    connect(clockTimer,SIGNAL(timeout()),this,SLOT(updateGuiClock()));    
    clockTimer->start(500);
    updateGuiClock();

    ui->lcdTemp->setDigits(2);    

    ui->lcdClkH->setDigits(2);
    ui->lcdClkH->setBorderColor(Qt::lightGray);
    ui->lcdClkM->setDigits(2);
    ui->lcdClkM->setBorderColor(Qt::lightGray);    

    ui->lcdDay->setDigits(7);
    ui->lcdDay->setBorderColor(Qt::yellow);
    ui->lcdDay->setBackgroundColor(Qt::darkYellow);
    ui->lcdDay->setValue(1234567);

    ui->lcdMonth->setDigits(7);
    ui->lcdMonth->setBorderColor(Qt::yellow);
    ui->lcdMonth->setBackgroundColor(Qt::darkYellow);
    ui->lcdMonth->setValue(0);

    ui->lcdPay->setDigits(7);
    ui->lcdPay->setBorderColor(Qt::yellow);
    ui->lcdPay->setBackgroundColor(Qt::darkYellow);
    ui->lcdPay->setValue(147712);

    connect(ui->cmdTest,SIGNAL(clicked()),this,SLOT(cmdEchoTest()));

    /* Security Config Page */
    connect(ui->btnNigthS,SIGNAL(clicked()),this,SLOT(restoreNigthSettings()));
    connect(ui->btnTravelS,SIGNAL(clicked()),this,SLOT(restoreTravelSettings()));
    connect(ui->btnUnlockS,SIGNAL(clicked()),this,SLOT(unlockPageS()));
    connect(ui->btnChangePass,SIGNAL(clicked()),this,SLOT(changePass()));

    /* General Config Page */
    connect(ui->dialLong,SIGNAL(valueChanged(int)),
            this,SLOT(updateDialLongValue(int)));
    connect(ui->dialShort,SIGNAL(valueChanged(int)),
            this,SLOT(updateDialShortValue(int)));

    connect(ui->btnSaveG,SIGNAL(clicked(bool)),this,SLOT(saveGeneralSettings()));
    connect(ui->btnSaveS,SIGNAL(clicked(bool)),this,SLOT(saveSecuritySettings()));

    connect(ui->chkMovSen,SIGNAL(clicked()),this,SLOT(chkWidgetsClearFocus()));
    connect(ui->chkMovSen2,SIGNAL(clicked()),this,SLOT(chkWidgetsClearFocus()));
    connect(ui->chkMovSen3,SIGNAL(clicked()),this,SLOT(chkWidgetsClearFocus()));
    connect(ui->chkMovSen4,SIGNAL(clicked()),this,SLOT(chkWidgetsClearFocus()));
    connect(ui->chkMovSen5,SIGNAL(clicked()),this,SLOT(chkWidgetsClearFocus()));
    connect(ui->chkMovSen6,SIGNAL(clicked()),this,SLOT(chkWidgetsClearFocus()));
    connect(ui->chkMovSen7,SIGNAL(clicked()),this,SLOT(chkWidgetsClearFocus()));
    connect(ui->chkMovSen8,SIGNAL(clicked()),this,SLOT(chkWidgetsClearFocus()));

    connect(ui->chkMagSen,SIGNAL(clicked()),this,SLOT(chkWidgetsClearFocus()));
    connect(ui->chkMagSen2,SIGNAL(clicked()),this,SLOT(chkWidgetsClearFocus()));
    connect(ui->chkMagSen3,SIGNAL(clicked()),this,SLOT(chkWidgetsClearFocus()));
    connect(ui->chkMagSen4,SIGNAL(clicked()),this,SLOT(chkWidgetsClearFocus()));
    connect(ui->chkMagSen5,SIGNAL(clicked()),this,SLOT(chkWidgetsClearFocus()));
    connect(ui->chkMagSen6,SIGNAL(clicked()),this,SLOT(chkWidgetsClearFocus()));
    connect(ui->chkMagSen7,SIGNAL(clicked()),this,SLOT(chkWidgetsClearFocus()));
    connect(ui->chkMagSen8,SIGNAL(clicked()),this,SLOT(chkWidgetsClearFocus()));

    connect(ui->chkLigth,SIGNAL(toggled(bool)),
            this,SLOT(restoreTimeLigthsSettings()));
    connect(ui->chkLigth2,SIGNAL(toggled(bool)),
            this,SLOT(restoreTimeLigthsSettings()));
    connect(ui->chkLigth3,SIGNAL(toggled(bool)),
            this,SLOT(restoreTimeLigthsSettings()));
    connect(ui->chkLigth4,SIGNAL(toggled(bool)),
            this,SLOT(restoreTimeLigthsSettings()));
    connect(ui->chkLigth5,SIGNAL(toggled(bool)),
            this,SLOT(restoreTimeLigthsSettings()));
    connect(ui->chkLigth6,SIGNAL(toggled(bool)),
            this,SLOT(restoreTimeLigthsSettings()));
    connect(ui->chkLigth7,SIGNAL(toggled(bool)),
            this,SLOT(restoreTimeLigthsSettings()));
    connect(ui->chkLigth8,SIGNAL(toggled(bool)),
            this,SLOT(restoreTimeLigthsSettings()));

    connect(ui->chkLigth,SIGNAL(pressed()),this,SLOT(ligthPressed()));
    connect(ui->chkLigth2,SIGNAL(pressed()),this,SLOT(ligthPressed2()));
    connect(ui->chkLigth3,SIGNAL(pressed()),this,SLOT(ligthPressed3()));
    connect(ui->chkLigth4,SIGNAL(pressed()),this,SLOT(ligthPressed4()));
    connect(ui->chkLigth5,SIGNAL(pressed()),this,SLOT(ligthPressed5()));
    connect(ui->chkLigth6,SIGNAL(pressed()),this,SLOT(ligthPressed6()));
    connect(ui->chkLigth7,SIGNAL(pressed()),this,SLOT(ligthPressed7()));
    connect(ui->chkLigth8,SIGNAL(pressed()),this,SLOT(ligthPressed8()));

    /* Password Dialog */
    passwordDialog = new QDialog(this);
    passUi = new Ui_PasswordDialog;
    passUi->setupUi(passwordDialog);
    passUi->txtPass->setEchoMode(QLineEdit::PasswordEchoOnEdit);

    connect(passUi->btnNum0,SIGNAL(clicked()),this,SLOT(pressed0()));
    connect(passUi->btnNum1,SIGNAL(clicked()),this,SLOT(pressed1()));
    connect(passUi->btnNum2,SIGNAL(clicked()),this,SLOT(pressed2()));
    connect(passUi->btnNum3,SIGNAL(clicked()),this,SLOT(pressed3()));
    connect(passUi->btnNum4,SIGNAL(clicked()),this,SLOT(pressed4()));
    connect(passUi->btnNum5,SIGNAL(clicked()),this,SLOT(pressed5()));
    connect(passUi->btnNum6,SIGNAL(clicked()),this,SLOT(pressed6()));
    connect(passUi->btnNum7,SIGNAL(clicked()),this,SLOT(pressed7()));
    connect(passUi->btnNum8,SIGNAL(clicked()),this,SLOT(pressed8()));
    connect(passUi->btnNum9,SIGNAL(clicked()),this,SLOT(pressed9()));
    connect(passUi->btnDel,SIGNAL(clicked()),this,SLOT(pressedDel()));
    connect(passUi->btnOk,SIGNAL(clicked()),this,SLOT(pressedOk()));
    connect(passUi->btnCancel,SIGNAL(clicked()),this,SLOT(pressedCancel()));


    /* Power */
    connect(ui->optMin,SIGNAL(clicked()),this,SLOT(graphMin()));
    connect(ui->optDay,SIGNAL(clicked()),this,SLOT(graphDay()));
    connect(ui->optMonth,SIGNAL(clicked()),this,SLOT(graphMonth()));


    powerMin = std::vector<double>(60,0.0);
    powerDay = std::vector<double>(30,0.0);
    powerMonth = std::vector<double>(12,0.0);

    connect(this,SIGNAL(refresh()),ui->powerGraph, SLOT(repaint()));
    graphMin();
    powerAcc=0;
    powerAccDay = 0;

    lastDate = QDate::currentDate();

    /* Initials */
    previousChkLigthIndex = -1;
    firstSecureModeEdit = true;
    firstClickNigth = true;
    firstClickTravel = true;
    firstRun = true;
    secured=true;
    securedMode=0;
    alarmActivated=false;
    alarmClockActivated=false;

    loadSettings();
    restoreSecuritySetting();
    restoreGeneralSettings();
    updatePage();

    serialName = defaultSerialName;
    serialAvr = new SerialPort();

    this->serialTimer = new QTimer(this);
    connect(serialTimer,SIGNAL(timeout()),this,SLOT(checkSerial()));
    serialTimer->start(200);
    serialCounter = 0;
    saveCounter = 0; //10 Minutes -> 3000
    initCounter = 0; //30 Seconds -> 150

    callApp::callApp(ScreensaverInhibit,this);

    /* Fuck yeah! */
    QTextCodec *linuxCodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(linuxCodec);
    QTextCodec::setCodecForCStrings(linuxCodec);
    QTextCodec::setCodecForLocale(linuxCodec);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(ui->btnSound->isChecked())
        voiceAlert("Saliendo del programa");

    //XXX This have to be removed!! ... the program should not be closed

    int r=QMessageBox::question(this,"AvrDomotic","<p><b>Realmente desea cerrar el"
                              " programa?</b> </p> El sistema seguirá activado "
                              "pero algunas funciones dejarán de funcionar.",
                              QMessageBox::Yes | QMessageBox::No);
    switch (r)
    {
        case QMessageBox::No:
          event->ignore();
          break;
        case QMessageBox::Yes:
          this->saveSettings();
          break;
    }
}

void MainWindow::graphMin()
{ 
    ui->powerGraph->clearWaves();
    ui->powerGraph->setPointsPerPlot(60);
    ui->powerGraph->setVoltsPerDiv(1000);
    ui->powerGraph->setGridValue(5,5,true);
    ui->powerGraph->setTraceWidth(2);
    ui->powerGraph->setShow(false,true);

    for (std::vector<double>::iterator i = powerMin.begin(); i< powerMin.end();i ++)
    {
         ui->powerGraph->addPoint(*i);
    }
}

void MainWindow::graphDay()
{
    ui->powerGraph->clearWaves();
    ui->powerGraph->setPointsPerPlot(7);
    ui->powerGraph->setGridValue(7,5,true);
    ui->powerGraph->setVoltsPerDiv(10000);
    ui->powerGraph->setTraceWidth(2);
    ui->powerGraph->setShow(false,true);    

    for (std::vector<double>::iterator i = powerDay.end()-7; i< powerDay.end();i ++)
    {
         ui->powerGraph->addPoint(*i);
    }

}

void MainWindow::graphMonth()
{
    ui->powerGraph->clearWaves();
    ui->powerGraph->setPointsPerPlot(6);
    ui->powerGraph->setGridValue(6,5,true);
    ui->powerGraph->setVoltsPerDiv(100000);
    ui->powerGraph->setTraceWidth(2);
    ui->powerGraph->setShow(false,true);

    for (std::vector<double>::iterator i = powerMonth.end()-6;
         i< powerMonth.end(); i ++)
    {
         ui->powerGraph->addPoint(*i);
    }
}


void MainWindow::toogleLigth(int id)
{    
    if(tbi(ligthsState,id))
        cmdTurnOffLigth(id);
    else
        cmdTurnOnLigth(id);
}

void MainWindow::ligthPressed()
{
    if(ui->tabWidget->currentIndex()<2)
        toogleLigth(0);
}

void MainWindow::ligthPressed2()
{    
    if(ui->tabWidget->currentIndex()<2)
        toogleLigth(1);
}

void MainWindow::ligthPressed3()
{
    if(ui->tabWidget->currentIndex()<2)
        toogleLigth(2);
}

void MainWindow::ligthPressed4()
{
    if(ui->tabWidget->currentIndex()<2)
        toogleLigth(3);
}

void MainWindow::ligthPressed5()
{
    if(ui->tabWidget->currentIndex()<2)
        toogleLigth(4);
}

void MainWindow::ligthPressed6()
{
    if(ui->tabWidget->currentIndex()<2)
        toogleLigth(5);
}

void MainWindow::ligthPressed7()
{
    if(ui->tabWidget->currentIndex()<2)
        toogleLigth(6);
}

void MainWindow::ligthPressed8()
{    
    if(ui->tabWidget->currentIndex()<2)
        toogleLigth(7);
}

void MainWindow::tryToDisarmSecurity()
{
    ui->btnSecurity->setChecked(false);
    updateSecurity();
}


void MainWindow::updateSecurity()
{
    updatingSecurity = true;    

    if(checkPassword())
    {
        if(ui->btnSecurity->isChecked())
        {
            message(true,3);
            initCounter = TimeToInitSecuredMode*5;
        }
        else
            uploadSecurity();
    }
    else
    {
        ui->btnSecurity->setChecked(!ui->btnSecurity->isChecked());
    }

    updatingSecurity = false;
}

void MainWindow::uploadSecurity()
{
    cmdDisarmAlarmClock();
    cmdDisarmSecurity();
    if(ui->btnSecurity->isChecked())
    {

        if(ui->optNigth->isChecked())
            cmdInitNigthMode();
        else
            cmdInitTravelMode();

        ui->optNigth->setEnabled(false);
        ui->optTravel->setEnabled(false);
    }
    else
    {
        ui->optNigth->setEnabled(true);
        ui->optTravel->setEnabled(true);        
    }
}

void MainWindow::screensaverOn()
{
    callApp::callApp(ScreensaverOn,this);
}

void MainWindow::screenForceOn()
{
    callApp::callApp(ScreenForceOn,this);
    callApp::callApp(ScreensaverPoke,this);    
}

void MainWindow::screenForceOff()
{
    callApp::callApp(ScreenForceOff,this);
}

void MainWindow::voiceAlert(QString text)
{
    callApp::callApp("killall -9 espeak",this);
    Sleeper::msleep(100);
    callApp::callApp("espeak \"" + text + "\" -v es -s 180",this);
    Sleeper::msleep(100);
}

void MainWindow::updateGuiClock()
{
    ui->lblClk->setEnabled(!ui->lblClk->isEnabled());
    ui->lblNote1->setEnabled(!ui->lblNote1->isEnabled());
    QTime currentTime = QTime::currentTime();
    ui->lcdClkH->setValue(currentTime.hour());
    ui->lcdClkM->setValue(currentTime.minute());
}

void MainWindow::cancelSecuredMode()
{
    message();
    initCounter=0;
    ui->btnSecurity->setChecked(false);
}

void MainWindow::message(bool active, int mode)
{
    ui->btnMsg->disconnect();

    switch(mode)
    {
        case 0:
            ui->lblMsgTitle->setText("<span style=\" color:#ff5500;\">"
                                     "¡NO HAY COMUNICACIÓN!"
                                     "</span>");
            ui->lblMsgText->setText("<span style=\" color:#0055ff;\">"
                                     "Escaneando puertos... Conecte la tarjeta..."
                                     "</span>");
            ui->btnMsg->setVisible(false);
            ui->lblMsgIcon->setPixmap(QPixmap(":/stop"));
            break;
        case 1:
            ui->lblMsgTitle->setText("<span style=\" color:#ff5555;\">"
                                     "¡ALARMA DE RELOJ!"
                                     "</span>");
            ui->lblMsgText->setText("<span style=\" color:#0055ff;\">"
                                     "Se activó la alarma de reloj..."
                                     "</span>");
            ui->btnMsg->setVisible(true);                        
            connect(ui->btnMsg,SIGNAL(clicked()),this,SLOT(cmdDisarmAlarmClock()));
            ui->btnMsg->setText("Apagar alarma");
            ui->btnMsg->setPalette(QPalette(QColor(0xff,0x55,0x55)));
            ui->lblMsgIcon->setPixmap(QPixmap(":/alarm"));
            break;
        case 2:
            ui->lblMsgTitle->setText("<span style=\" color:#ff0000;\">"
                                     "¡ALARMA DE INTRUSO!"
                                     "</span>");
            ui->lblMsgText->setText("<span style=\" color:#0055ff;\">"
                                     "Se activó la alarma de intruso..."
                                     "</span>");
            ui->btnMsg->setVisible(true);            
            connect(ui->btnMsg,SIGNAL(clicked()),this,SLOT(tryToDisarmSecurity()));
            ui->btnMsg->setText("Desactivar alarma");
            ui->btnMsg->setPalette(QPalette(QColor(0xff,0,0)));
            ui->lblMsgIcon->setPixmap(QPixmap(":/unlock"));
            break;
        case 3:
            ui->lblMsgTitle->setText("<span style=\" color:#00b400;\">"
                                     "¡ACTIVANDO MODO SEGURO!"
                                     "</span>");
            ui->lblMsgText->setText("<span style=\" color:#0055ff;\">"
                                    ""
                                    "</span>");
            ui->btnMsg->setVisible(true);
            connect(ui->btnMsg,SIGNAL(clicked()),this,SLOT(cancelSecuredMode()));
            ui->btnMsg->setText("Cancelar activación");            
            ui->lblMsgIcon->setPixmap(QPixmap(":/time"));
            break;
        default:
            ui->btnMsg->setVisible(false);
            break;
    }

    ui->planFrame->setEnabled(!active);
    ui->tabWidget->setEnabled(!active);
    ui->AreaP->setVisible(active);
    repaint();
}

void MainWindow::checkSerial()
{
    if(!QFile::exists(serialName))
    {
        if(serialAvr->GetPortFD()>=0)
        {
            if(ui->btnSound->isChecked())
                voiceAlert("Tarjeta desconectada");           
           serialAvr->ClosePort();           
        } else if(serialName==defaultSerialName)
        {
            if(ui->btnSound->isChecked())
                voiceAlert("Sin comunicación");
        }

        message(true);

        Sleeper::msleep(500);

        /* Folder scanning */

        QDir directory = QDir("/dev/serial/by-id/");
        QDirIterator it(directory, QDirIterator::Subdirectories);
        
        serials.clear();
        QString next;
        while (it.hasNext())
        {
            next = QDir(it.next()).canonicalPath();
            if(next.contains("ttyUSB"))
                serials << next;
        }
        serials.removeDuplicates();
        scanPorts();
        return;
    }


    if(serialAvr->GetPortFD()>=0)
    {
        char *buf;
        unsigned char temp[64];
        int a, blen;
        a = serialAvr->ReadPort();
        if ((a & 1) == 1)
        {            
            buf = serialAvr->GetBufer();
            blen = serialAvr->GetBytesRead();
            memcpy( temp, buf, blen );

            //Parsing data
            QString state;
            for(int i = 0; i < blen; i++)
                state += "[" + QString::number(temp[i]) + "]";                
            ui->lblCom->setText(state);

            if(QString::fromAscii(buf,blen) == rREPORT_RESET)
            {
                uploadGeneralSettings();
                uploadSecuritySettings();

                Sleeper::msleep(100);

                if(secured)
                {
                    uploadSecurity();
                    if(alarmActivated)
                        cmdTriggerAlarm();
                    alarmActivated = false;
                }
                ui->lblCom->setText(QString::fromAscii(buf,blen));
                return;
            }

            if(blen == rDATA_LENGTH)
            {
                chkWidgetsDefaultIcon();
                if(tbi((temp[0]),0)) ui->chkMovSen->setIcon(QIcon(":/mov"));
                if(tbi((temp[0]),1)) ui->chkMovSen2->setIcon(QIcon(":/mov"));
                if(tbi((temp[0]),2)) ui->chkMovSen3->setIcon(QIcon(":/mov"));
                if(tbi((temp[0]),3)) ui->chkMovSen4->setIcon(QIcon(":/mov"));
                if(tbi((temp[0]),4)) ui->chkMovSen5->setIcon(QIcon(":/mov"));
                if(tbi((temp[0]),5)) ui->chkMovSen6->setIcon(QIcon(":/mov"));
                if(tbi((temp[0]),6)) ui->chkMovSen7->setIcon(QIcon(":/mov"));
                if(tbi((temp[0]),7)) ui->chkMovSen8->setIcon(QIcon(":/mov"));

                if(tbi((temp[1]),0)) ui->chkMagSen->setIcon(QIcon(":/mag"));
                if(tbi((temp[1]),1)) ui->chkMagSen2->setIcon(QIcon(":/mag"));
                if(tbi((temp[1]),2)) ui->chkMagSen3->setIcon(QIcon(":/mag"));
                if(tbi((temp[1]),3)) ui->chkMagSen4->setIcon(QIcon(":/mag"));
                if(tbi((temp[1]),4)) ui->chkMagSen5->setIcon(QIcon(":/mag"));
                if(tbi((temp[1]),5)) ui->chkMagSen6->setIcon(QIcon(":/mag"));
                if(tbi((temp[1]),6)) ui->chkMagSen7->setIcon(QIcon(":/mag"));
                if(tbi((temp[1]),7)) ui->chkMagSen8->setIcon(QIcon(":/mag"));

                if(tbi((temp[2]),0)) ui->chkLigth->setIcon(QIcon(":/saveron"));
                if(tbi((temp[2]),1)) ui->chkLigth2->setIcon(QIcon(":/saveron"));
                if(tbi((temp[2]),2)) ui->chkLigth3->setIcon(QIcon(":/saveron"));
                if(tbi((temp[2]),3)) ui->chkLigth4->setIcon(QIcon(":/saveron"));
                if(tbi((temp[2]),4)) ui->chkLigth5->setIcon(QIcon(":/saveron"));
                if(tbi((temp[2]),5)) ui->chkLigth6->setIcon(QIcon(":/saveron"));
                if(tbi((temp[2]),6)) ui->chkLigth7->setIcon(QIcon(":/saveron"));
                if(tbi((temp[2]),7)) ui->chkLigth8->setIcon(QIcon(":/saveron"));

                ligthsState =temp[2];

                bool tsecured = secured;
                bool talarmActivated = alarmActivated;
                bool talarmClockActivated = alarmClockActivated;

                if(tbi((temp[3]),rSECURED))
                    secured = true;
                else
                    secured = false;

                if(tbi((temp[3]),rSECURED_MODE))
                    securedMode = true;
                else
                    securedMode = false;

                if(tbi((temp[3]),rALARM_ACTIVATED))
                    alarmActivated=true;
                else
                    alarmActivated = false;

                if(tbi((temp[3]),rALARM_CLOCK_ACTIVATED))
                    alarmClockActivated = true;
                else
                    alarmClockActivated = false;

                if(tbi((temp[3]),rIS_NIGTH))
                    isNigth=true;
                else
                    isNigth = false;

                int temperature = (temp[4]-(273/2));
                ui->lcdTemp->setValue(temperature);                
                ui->lcdTemp->setBackgroundColor(
                        QColor(temperature*6,255-temperature*6,255-temperature*6));
                ui->lcdTemp->setBorderColor(
                        QColor(temperature*6,255-temperature*6,255-temperature*6));

                // (RMS)

                unsigned int currentAcc =
                        sqrt((temp[5]+(temp[6]<<8)+(temp[7]<<16))/CURRENT_SCALER);
                unsigned int voltageAcc =
                        (temp[8] + (temp[9]<<8))/VOLTAGE_SCALER;

                double power = (currentAcc*voltageAcc)/2.0;

                //UPDATES//
                
                /* Security state */                
                if(secured)
                {
                    if(!tsecured)
                    {
                        this->screenForceOff();
                        if(ui->btnSound->isChecked())
                            voiceAlert("Seguridad activada");
                        this->passwordDialog->close();
                        message();
                        cancelSecuredMode();
                    }
                }
                else
                {
                    if(tsecured)
                    {
                        this->screenForceOn();
                        if(ui->btnSound->isChecked())
                            voiceAlert("Seguridad desactivada");
                        this->passwordDialog->close();
                        message();
                    }
                }

                /* Security options state */
                if(!updatingSecurity)
                {
                    if(secured)
                    {
                        ui->optNigth->setEnabled(false);
                        ui->optTravel->setEnabled(false);
                        ui->optTravel->setChecked(!securedMode);
                        ui->btnSecurity->setChecked(true);                        
                    }
                    else if (initCounter == 0) //When count down is not set
                    {
                        ui->optNigth->setEnabled(true);
                        ui->optTravel->setEnabled(true);
                        ui->btnSecurity->setChecked(false);                        
                    }
                }
                else
                {
                    if(secured)
                        ui->optTravel->setChecked(!securedMode);
                }

                /* Alarms state */
                if(talarmActivated != alarmActivated)
                {
                    if(alarmActivated)
                    {
                        if(ui->btnSound->isChecked())
                            voiceAlert("Alarma de intruso activada");
                        message(true,2);
                        this->screenForceOn();
                    }
                    else
                        message();
                }
                else if(talarmClockActivated != alarmClockActivated)
                {
                    if(alarmClockActivated)
                    {
                        if(ui->btnSound->isChecked())
                            voiceAlert("Alarma de reloj activada");
                        if(initCounter==150)
                            message(true,1);
                        this->screenForceOn();
                    }
                    else
                        message();
                }



                /* Power measure and historics */
                if(ui->optMin->isChecked() && ui->tabWidget->currentIndex()==1)
                {
                    ui->powerGraph->addPoint((int)power);
                }

                powerMin.push_back(power);
                powerMin.erase(powerMin.begin());

                powerAcc += (double) power / 3600.0;

                QDate currentDate=QDate::currentDate();

                if(currentDate.day() != lastDate.day())
                {
                    //New day!!
                    powerDay.push_back(powerAcc);
                    powerDay.erase(powerDay.begin());
                    powerAccDay += powerAcc;
                    powerAcc = 0;

                    if(ui->optDay->isChecked() && ui->tabWidget->currentIndex()==1)
                    {
                        graphDay();
                    }

                    if(currentDate.day() == readDay)
                    {
                        //New month!!
                        powerMonth.push_back(powerAccDay);
                        powerMonth.erase(powerMonth.begin());
                        powerAccDay = 0;

                        if(ui->optMonth->isChecked() &&
                           ui->tabWidget->currentIndex()==1)
                        {
                            graphMonth();
                        }
                    }

                    ui->lcdMonth->setValue(powerAccDay/1000);
                    saveSettings();
                }

                ui->lcdMonth->setValue((powerAccDay+powerAcc)/1000);
                ui->lcdPay->setValue(kwhPrice*(powerAccDay+powerAcc)/1000);

                lastDate = currentDate;
                ui->lcdDay->setValue((int)powerAcc);
            }
            else
            {
                serialAvr->Flush();
            }

            serialCounter = 0;
        }
        else
        {
            serialCounter++;
            if(serialCounter==20)
            {
                //4 Seconds without data received
                message();
                passwordDialog->close();
                serialName = ""; // Close the port!
                serialCounter = 0;
            }
        }
    }
    else
    {
        if(serialAvr->OpenPort(serialName.toAscii().data(),B57600))
        {
            serialAvr->Flush();
            Sleeper::msleep(500);            

            printf("Serial opened. [%d]", serialAvr->GetPortFD());

            if(cmdEchoTest())
            {                
                uploadSecuritySettings();
                uploadGeneralSettings();

                Sleeper::msleep(100);

                if(ui->btnSound->isChecked())
                    voiceAlert("Conectado");

                if(secured)
                {
                    uploadSecurity();
                    if(alarmActivated)
                        cmdTriggerAlarm();
                    alarmActivated = false;
                }

                message();
                return;
            }
            else
            {
                if(ui->btnSound->isChecked())
                    voiceAlert("Sin respuesta");
                else
                    QMessageBox::critical(this,"Error","La tarjeta no responde "
                                          "a los comandos enviados. Verifique el"
                                          " cable.");
                scanPorts();
                return;
            }            
        }
        else
        {
            if(ui->btnSound->isChecked())
                voiceAlert("Puerto ocupado");
            else
                QMessageBox::critical(this,"Error","El recurso parece estar bloqueado."
                                     "Por favor seleccione otro puerto.");
            scanPorts();
            return;
        }
    }

    //Time to save
    saveCounter++;
    if(saveCounter>TimeToAutoSaveSettings*5*60)
    {
        //10 Minutes
        saveCounter = 0;
        saveSettings();
    }

    //Countdown to init security
    if(initCounter > 0)
    {                
        if(initCounter==1)
        {
            message();
            uploadSecurity();
        }
        else
        {            
            ui->lblMsgText->setText("<span style=\" color:#0055ff;\">"
                                    "El modo seguro se activará en :" +
                                    QString::number((int)initCounter/5) +
                                    " segundos ... </span>");
        }
        initCounter --;
    }
}

void MainWindow::scanPorts()
{
    Sleeper::msleep(1000);

    serialAvr->ClosePort();
    if(serials.size())
        serialName = serials.takeFirst();
    else
        serialName = "";
}



void MainWindow::updateDialShortValue(int value)
{
    if(value<60)
        ui->lblDialShort->setText("Tiempo  [" + QString::number(value) + "s]");    
    else
    {
        if(value%60!=0)
            ui->lblDialShort->setText("Tiempo  [" +
                                      QString::number(value/60) + "m " +
                                      QString::number(value%60) + "s]");
        else
            ui->lblDialShort->setText("Tiempo  [" +
                                      QString::number(value/60) + "m]");
    }
}

void MainWindow::updateDialLongValue(int value)
{
    //value *= 4;
    if(value==0)
    {
        ui->lblDialLong->setText("Tiempo  [Infinito]");
    }
    else if(value<60)
    {
        ui->lblDialLong->setText("Tiempo  [" + QString::number(value) + "s]");
    }
    else
    {
        if(value%60!=0)
            ui->lblDialLong->setText("Tiempo  [" +
                                      QString::number(value/60) + "m " +
                                      QString::number(value%60) + "s]");
        else
            ui->lblDialLong->setText("Tiempo  [" +
                                      QString::number(value/60) + "m]");
    }
}

int MainWindow::loadSettings()
{
    QDomDocument settingsDoc;
    QFile file("settings.xml");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        bool parsing=settingsDoc.setContent(&file);
        file.close();
        if(!parsing)
        {
            QMessageBox::warning(this,"Parsing warning","Invalid or void "
                                 " element found in file.");
            //return 0;
        }
    }
    else
    {
        QMessageBox::critical(this,"Error","Could not open file for read.");
        return 0;
    }

    QDomElement settings =
            settingsDoc.elementsByTagName("Settings").at(0).toElement();

    for (QDomNode node = settings.firstChild() ;
         !node.isNull() ;
         node = node.nextSibling())
    {
        QDomElement element = node.toElement();
        if(element.tagName()=="NigthSettings")
        {
            for (QDomNode node = element.firstChild() ;
                !node.isNull() ;
                node = node.nextSibling())
            {
                //Load sensors and ligths
                QDomElement element = node.toElement();
                if(element.tagName()=="MovSensor")
                {
                    nigthMov[element.attribute("ID").toInt()]=
                                   element.attribute("Value").toInt() ;
                }
                else if(element.tagName()=="MagSensor")
                {
                    nigthMag[element.attribute("ID").toInt()]=
                                   element.attribute("Value").toInt() ;
                }
                else if(element.tagName()=="Ligth")
                {
                    nigthLig[element.attribute("ID").toInt()]=
                                   element.attribute("Value").toInt() ;
                }
            }
            nigthLEDs=element.attribute("LEDs").toInt();
            nigthTone=element.attribute("Tone").toInt();
            nigthCall=element.attribute("Call").toInt();
            nigthLigths=element.attribute("Ligths").toInt();
            nigthLigthsOff=element.attribute("LigthsOff").toInt();
            nigthSiren=element.attribute("Siren").toInt();
        }
        else if(element.tagName()=="TravelSettings")
        {            
            for (QDomNode node = element.firstChild() ;
                !node.isNull() ;
                node = node.nextSibling())
            {
                //Load sensors and ligths
                QDomElement element = node.toElement();
                if(element.tagName()=="MovSensor")
                {
                    travelMov[element.attribute("ID").toInt()]=
                                   element.attribute("Value").toInt() ;                    
                }
                else if(element.tagName()=="MagSensor")
                {
                    travelMag[element.attribute("ID").toInt()]=
                                   element.attribute("Value").toInt() ;
                }
                else if(element.tagName()=="Ligth")
                {
                    travelLig[element.attribute("ID").toInt()]=
                                   element.attribute("Value").toInt() ;
                }
            }
            travelLEDs=element.attribute("LEDs").toInt();
            travelTone=element.attribute("Tone").toInt();
            travelCall=element.attribute("Call").toInt();
            travelLigths=element.attribute("Ligths").toInt();
            travelLigthsOff=element.attribute("LigthsOff").toInt();
            travelSiren=element.attribute("Siren").toInt();
        }
        else if(element.tagName()=="PassSettings")
        {
            passActive=element.attribute("Protected").toInt();
            pass=element.attribute("Pass"); // TODO encript password
        }
        else if(element.tagName()=="GeneralSettings")
        {
            for (QDomNode node = element.firstChild() ;
                !node.isNull() ;
                node = node.nextSibling())
            {
                //Load sensors and ligths
                QDomElement element = node.toElement();
                if(element.tagName()=="SensorOn")
                {
                    sensorOn[element.attribute("ID").toInt()]=
                                   element.attribute("Value").toInt() ;
                }
                else if(element.tagName()=="TimeShort")
                {
                    timeShort[element.attribute("ID").toInt()]=
                                   element.attribute("Value").toInt() ;
                }
                else if(element.tagName()=="TimeLong")
                {
                    timeLong[element.attribute("ID").toInt()]=
                                   element.attribute("Value").toInt() ;
                }
            }
            timeAlarmOn=element.attribute("TimeAlarmOn").toInt();
            timeAlarm=element.attribute("TimeAlarm");
            alarmLigths=element.attribute("AlarmLigths").toInt();
            alarmSiren=element.attribute("AlarmSiren").toInt();
            nigthManual=element.attribute("NigthManual").toInt();
            timeNigthInit=element.attribute("TimeNigthInit");
            timeNigthEnd=element.attribute("TimeNigthEnd");
            kwhPrice=element.attribute("kWhPrice").toInt();
            readDay=element.attribute("ReadDay").toInt();

            ui->btnSound->setChecked(element.attribute("SoundOn").toInt());
        }        
        else if(element.tagName()=="Historics")
        {
            for (QDomNode node = element.firstChild() ;
                !node.isNull() ;
                node = node.nextSibling())
            {
                //Load sensors and ligths
                QDomElement element = node.toElement();
                if(element.tagName()=="Last30Days")
                {
                    powerDay.at(element.attribute("ID").toInt())=
                                   element.attribute("Value").toDouble();
                }
                else if(element.tagName()=="Last12Months")
                {
                    powerMonth.at(element.attribute("ID").toInt())=
                                   element.attribute("Value").toDouble();
                }
            }
            powerAcc=element.attribute("PowerAccumulate").toDouble();
            powerAccDay=element.attribute("PowerAccumulateDay").toDouble();
            lastDate=QDate::fromString(element.attribute("LastDate"),"dd.MM.yyyy");
        }
    }

    return 1;
}

void MainWindow::saveSecuritySettings()
{
    if(ui->btnNigthS->isChecked())
        this->getNigthSettings();

    if(ui->btnTravelS->isChecked())
        this->getTravelSettings();

    chkWidgetsClearFocus();
    getSecuritySettings();
    saveSettings();
    uploadSecuritySettings();
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::saveGeneralSettings()
{
    cmdDisarmAlarmClock();
    restoreTimeLigthsSettings();
    chkWidgetsClearFocus();
    getGeneralSettings();
    saveSettings();
    uploadGeneralSettings();
    ui->tabWidget->setCurrentIndex(0);
}


void MainWindow::uploadSecuritySettings()
{
     setNigthMovMask();
     setNigthMagMask();
     setNigthLigMask();
     setNigthOthMask();

     setTravelMovMask();
     setTravelMagMask();
     setTravelLigMask();
     setTravelOthMask();
}

void MainWindow::uploadGeneralSettings()
{
     setCurrentTime();
     setNigthInitTime();
     setNigthEndTime();
     setAlarmClockTime();

     setNormalLigMask();
     setLigthTimeShort();
     setLigthTimeLong();
     setGeneralMask();
}


int MainWindow::saveSettings()
{
    QDomDocument settingsDoc;
    QDomElement settings;
    QDomElement nigthSettings;
    QDomElement travelSettings;
    QDomElement passSettings;
    QDomElement generalSettings;
    QDomElement historics;
    QDomElement element;

    QDomComment initialComments=settingsDoc.createComment(
        "\nConfiguration file for avrDomotic v1.0\n");
    settingsDoc.appendChild(initialComments);

    settings = settingsDoc.createElement("Settings");
    settingsDoc.appendChild(settings);

    /* Nigth Settings */
    nigthSettings = settingsDoc.createElement("NigthSettings");
    for(int i=0; i<MOV_SENSORS;i++ )
    {
        element = settingsDoc.createElement("MovSensor");
        element.setAttribute("ID",i);
        element.setAttribute("Value",nigthMov[i]);
        nigthSettings.appendChild(element);
    }
    for(int i=0; i<MAG_SENSORS;i++ )
    {
        element = settingsDoc.createElement("MagSensor");
        element.setAttribute("ID",i);
        element.setAttribute("Value",nigthMag[i]);
        nigthSettings.appendChild(element);
    }
    for(int i=0; i<LIGTHS;i++ )
    {
        element = settingsDoc.createElement("Ligth");
        element.setAttribute("ID",i);
        element.setAttribute("Value",nigthLig[i]);
        nigthSettings.appendChild(element);
    }
    nigthSettings.setAttribute("LEDs",nigthLEDs);
    nigthSettings.setAttribute("Tone",nigthTone);
    nigthSettings.setAttribute("Call",nigthCall);
    nigthSettings.setAttribute("Ligths",nigthLigths);
    nigthSettings.setAttribute("LigthsOff",nigthLigthsOff);
    nigthSettings.setAttribute("Siren",nigthSiren);

    settings.appendChild(nigthSettings);

    /* Travel Settings */
    travelSettings = settingsDoc.createElement("TravelSettings");
    for(int i=0; i<MOV_SENSORS;i++ )
    {
        element = settingsDoc.createElement("MovSensor");
        element.setAttribute("ID",i);
        element.setAttribute("Value",travelMov[i]);
        travelSettings.appendChild(element);

    }
    for(int i=0; i<MAG_SENSORS;i++ )
    {
        element = settingsDoc.createElement("MagSensor");
        element.setAttribute("ID",i);
        element.setAttribute("Value",travelMag[i]);
        travelSettings.appendChild(element);
    }
    for(int i=0; i<LIGTHS;i++ )
    {
        element = settingsDoc.createElement("Ligth");
        element.setAttribute("ID",i);
        element.setAttribute("Value",travelLig[i]);
        travelSettings.appendChild(element);
    }
    travelSettings.setAttribute("LEDs",travelLEDs);
    travelSettings.setAttribute("Tone",travelTone);
    travelSettings.setAttribute("Call",travelCall);
    travelSettings.setAttribute("Ligths",travelLigths);
    travelSettings.setAttribute("LigthsOff",travelLigthsOff);
    travelSettings.setAttribute("Siren",travelSiren);

    settings.appendChild(travelSettings);

    /* Pass Settings */
    passSettings = settingsDoc.createElement("PassSettings");
    passSettings.setAttribute("Protected",passActive);
    passSettings.setAttribute("Pass",pass);     // TODO: encript the password

    settings.appendChild(passSettings);

    /* General Settings */
    generalSettings = settingsDoc.createElement("GeneralSettings");
    for(int i=0; i<LIGTHS;i++ )
    {
        element = settingsDoc.createElement("SensorOn");
        element.setAttribute("ID",i);
        element.setAttribute("Value",sensorOn[i]);
        generalSettings.appendChild(element);
        element = settingsDoc.createElement("TimeShort");
        element.setAttribute("ID",i);
        element.setAttribute("Value",timeShort[i]);
        generalSettings.appendChild(element);
        element = settingsDoc.createElement("TimeLong");
        element.setAttribute("ID",i);
        element.setAttribute("Value",timeLong[i]);
        generalSettings.appendChild(element);
    }
    generalSettings.setAttribute("TimeAlarmOn",timeAlarmOn);
    generalSettings.setAttribute("TimeAlarm",timeAlarm);
    generalSettings.setAttribute("AlarmLigths",alarmLigths);
    generalSettings.setAttribute("AlarmSiren",alarmSiren);
    generalSettings.setAttribute("NigthManual",nigthManual);
    generalSettings.setAttribute("TimeNigthInit",timeNigthInit);
    generalSettings.setAttribute("TimeNigthEnd",timeNigthEnd);
    generalSettings.setAttribute("kWhPrice",kwhPrice);
    generalSettings.setAttribute("ReadDay",readDay);

    generalSettings.setAttribute("SoundOn",ui->btnSound->isChecked());

    settings.appendChild(generalSettings);

    /* Power historics */
    historics = settingsDoc.createElement("Historics");
    int j=0;
    for (std::vector<double>::iterator i = powerDay.begin(); i< powerDay.end();i ++)
    {
         element = settingsDoc.createElement("Last30Days");
         element.setAttribute("ID",j);
         element.setAttribute("Value",*i);
         historics.appendChild(element);
         j++;
    }
    j=0;
    for (std::vector<double>::iterator i = powerMonth.begin(); i< powerMonth.end();i ++)
    {
        element = settingsDoc.createElement("Last12Months");
        element.setAttribute("ID",j);
        element.setAttribute("Value",*i);
        historics.appendChild(element);
        j++;
    }    
    historics.setAttribute("PowerAccumulateDay",powerAccDay);
    historics.setAttribute("PowerAccumulate",powerAcc);
    historics.setAttribute("LastDate",lastDate.toString("dd.MM.yyyy"));

    settings.appendChild(historics);

    /* SAVE */
    QFile file("settings.xml");
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << settingsDoc.toString(4);
        file.close();
        return 1;
    }
    else
        QMessageBox::critical(this,"Error","Could not open file for write.");
    return 0;
}

void MainWindow::updatePage()

{
    if(ui->tabWidget->currentIndex()==0 || ui->tabWidget->currentIndex()==1)
    {
        chkWidgetsSetChecked(false);
        chkWidgetsSetEnable(true);
        chkWidgetsSetCheckable(false);
        chkWidgetsClearFocus();
        ui->btnAlarm->setChecked(timeAlarmOn);

        ui->optMin->setChecked(true);
        graphMin();
    }
    else if(ui->tabWidget->currentIndex()==2)
    {
        chkWidgetsSetCheckable(false);
        chkWidgetsSetEnable(false);
        chkLigthWidgetsSetExclusiveCheckable(true);
        chkWidgetsClearFocus();
        this->restoreGeneralSettings();        
    }
    else if(ui->tabWidget->currentIndex()==3)
    {
        chkWidgetsSetEnable(false);
        chkWidgetsSetCheckable(true);
        chkWidgetsClearFocus();
        chkWidgetsSetChecked(false);
        restorePassSettings();        
    }
}

void MainWindow::chkWidgetsDefaultIcon()
{
    ui->chkMovSen->setIcon(QIcon(":/movon"));
    ui->chkMovSen2->setIcon(QIcon(":/movon"));
    ui->chkMovSen3->setIcon(QIcon(":/movon"));
    ui->chkMovSen4->setIcon(QIcon(":/movon"));
    ui->chkMovSen5->setIcon(QIcon(":/movon"));
    ui->chkMovSen6->setIcon(QIcon(":/movon"));
    ui->chkMovSen7->setIcon(QIcon(":/movon"));
    ui->chkMovSen8->setIcon(QIcon(":/movon"));

    ui->chkMagSen->setIcon(QIcon(":/magon"));
    ui->chkMagSen2->setIcon(QIcon(":/magon"));
    ui->chkMagSen3->setIcon(QIcon(":/magon"));
    ui->chkMagSen4->setIcon(QIcon(":/magon"));
    ui->chkMagSen5->setIcon(QIcon(":/magon"));
    ui->chkMagSen6->setIcon(QIcon(":/magon"));
    ui->chkMagSen7->setIcon(QIcon(":/magon"));
    ui->chkMagSen8->setIcon(QIcon(":/magon"));

    ui->chkLigth->setIcon(QIcon(":/saver"));
    ui->chkLigth2->setIcon(QIcon(":/saver"));
    ui->chkLigth3->setIcon(QIcon(":/saver"));
    ui->chkLigth4->setIcon(QIcon(":/saver"));
    ui->chkLigth5->setIcon(QIcon(":/saver"));
    ui->chkLigth6->setIcon(QIcon(":/saver"));
    ui->chkLigth7->setIcon(QIcon(":/saver"));
    ui->chkLigth8->setIcon(QIcon(":/saver"));
}

void MainWindow::chkWidgetsSetCheckable(bool value)
{
    ui->chkMovSen->setCheckable(value);
    ui->chkMovSen2->setCheckable(value);
    ui->chkMovSen3->setCheckable(value);
    ui->chkMovSen4->setCheckable(value);
    ui->chkMovSen5->setCheckable(value);
    ui->chkMovSen6->setCheckable(value);
    ui->chkMovSen7->setCheckable(value);
    ui->chkMovSen8->setCheckable(value);

    ui->chkMagSen->setCheckable(value);
    ui->chkMagSen2->setCheckable(value);
    ui->chkMagSen3->setCheckable(value);
    ui->chkMagSen4->setCheckable(value);
    ui->chkMagSen5->setCheckable(value);
    ui->chkMagSen6->setCheckable(value);
    ui->chkMagSen7->setCheckable(value);
    ui->chkMagSen8->setCheckable(value);

    ui->chkLigth->setCheckable(value);
    ui->chkLigth2->setCheckable(value);
    ui->chkLigth3->setCheckable(value);
    ui->chkLigth4->setCheckable(value);
    ui->chkLigth5->setCheckable(value);
    ui->chkLigth6->setCheckable(value);
    ui->chkLigth7->setCheckable(value);
    ui->chkLigth8->setCheckable(value);

    ui->chkLigth->setAutoExclusive(false);
    ui->chkLigth2->setAutoExclusive(false);
    ui->chkLigth3->setAutoExclusive(false);
    ui->chkLigth4->setAutoExclusive(false);
    ui->chkLigth5->setAutoExclusive(false);
    ui->chkLigth6->setAutoExclusive(false);
    ui->chkLigth7->setAutoExclusive(false);
    ui->chkLigth8->setAutoExclusive(false);
}

void MainWindow::chkWidgetsSetChecked(bool value)
{
    ui->chkMovSen->setChecked(value);
    ui->chkMovSen2->setChecked(value);
    ui->chkMovSen3->setChecked(value);
    ui->chkMovSen4->setChecked(value);
    ui->chkMovSen5->setChecked(value);
    ui->chkMovSen6->setChecked(value);
    ui->chkMovSen7->setChecked(value);
    ui->chkMovSen8->setChecked(value);

    ui->chkMagSen->setChecked(value);
    ui->chkMagSen2->setChecked(value);
    ui->chkMagSen3->setChecked(value);
    ui->chkMagSen4->setChecked(value);
    ui->chkMagSen5->setChecked(value);
    ui->chkMagSen6->setChecked(value);
    ui->chkMagSen7->setChecked(value);
    ui->chkMagSen8->setChecked(value);

    ui->chkLigth->setChecked(value);
    ui->chkLigth2->setChecked(value);
    ui->chkLigth3->setChecked(value);
    ui->chkLigth4->setChecked(value);
    ui->chkLigth5->setChecked(value);
    ui->chkLigth6->setChecked(value);
    ui->chkLigth7->setChecked(value);
    ui->chkLigth8->setChecked(value);
}

void MainWindow::chkWidgetsSetEnable(bool value)
{

    ui->chkMovSen->setEnabled(value);
    ui->chkMovSen2->setEnabled(value);
    ui->chkMovSen3->setEnabled(value);
    ui->chkMovSen4->setEnabled(value);
    ui->chkMovSen5->setEnabled(value);
    ui->chkMovSen6->setEnabled(value);
    ui->chkMovSen7->setEnabled(value);
    ui->chkMovSen8->setEnabled(value);

    ui->chkMagSen->setEnabled(value);
    ui->chkMagSen2->setEnabled(value);
    ui->chkMagSen3->setEnabled(value);
    ui->chkMagSen4->setEnabled(value);
    ui->chkMagSen5->setEnabled(value);
    ui->chkMagSen6->setEnabled(value);
    ui->chkMagSen7->setEnabled(value);
    ui->chkMagSen8->setEnabled(value);

    ui->chkLigth->setEnabled(value);
    ui->chkLigth2->setEnabled(value);
    ui->chkLigth3->setEnabled(value);
    ui->chkLigth4->setEnabled(value);
    ui->chkLigth5->setEnabled(value);
    ui->chkLigth6->setEnabled(value);
    ui->chkLigth7->setEnabled(value);
    ui->chkLigth8->setEnabled(value);
}

void MainWindow::chkWidgetsClearFocus()
{
    ui->chkMovSen->clearFocus();
    ui->chkMovSen2->clearFocus();
    ui->chkMovSen3->clearFocus();
    ui->chkMovSen4->clearFocus();
    ui->chkMovSen5->clearFocus();
    ui->chkMovSen6->clearFocus();
    ui->chkMovSen7->clearFocus();
    ui->chkMovSen8->clearFocus();

    ui->chkMagSen->clearFocus();
    ui->chkMagSen2->clearFocus();
    ui->chkMagSen3->clearFocus();
    ui->chkMagSen4->clearFocus();
    ui->chkMagSen5->clearFocus();
    ui->chkMagSen6->clearFocus();
    ui->chkMagSen7->clearFocus();
    ui->chkMagSen8->clearFocus();

    ui->chkLigth->clearFocus();
    ui->chkLigth2->clearFocus();
    ui->chkLigth3->clearFocus();
    ui->chkLigth4->clearFocus();
    ui->chkLigth5->clearFocus();
    ui->chkLigth6->clearFocus();
    ui->chkLigth7->clearFocus();
    ui->chkLigth8->clearFocus();
}

void MainWindow::chkLigthWidgetsSetExclusiveCheckable(bool value)
{
    ui->chkLigth->setCheckable(value);
    ui->chkLigth2->setCheckable(value);
    ui->chkLigth3->setCheckable(value);
    ui->chkLigth4->setCheckable(value);
    ui->chkLigth5->setCheckable(value);
    ui->chkLigth6->setCheckable(value);
    ui->chkLigth7->setCheckable(value);
    ui->chkLigth8->setCheckable(value);

    ui->chkLigth->setAutoExclusive(value);
    ui->chkLigth2->setAutoExclusive(value);
    ui->chkLigth3->setAutoExclusive(value);
    ui->chkLigth4->setAutoExclusive(value);
    ui->chkLigth5->setAutoExclusive(value);
    ui->chkLigth6->setAutoExclusive(value);
    ui->chkLigth7->setAutoExclusive(value);
    ui->chkLigth8->setAutoExclusive(value);

    ui->chkLigth->setEnabled(true);
    ui->chkLigth2->setEnabled(true);
    ui->chkLigth3->setEnabled(true);
    ui->chkLigth4->setEnabled(true);
    ui->chkLigth5->setEnabled(true);
    ui->chkLigth6->setEnabled(true);
    ui->chkLigth7->setEnabled(true);
    ui->chkLigth8->setEnabled(true);
}



void MainWindow::restoreSecuritySetting()
{
    for(int i=0; i<MOV_SENSORS;i++ )
    {
        t_nigthMov[i]=nigthMov[i];
        t_travelMov[i]=travelMov[i];
    }
    for(int i=0; i<MAG_SENSORS;i++ )
    {
        t_nigthMag[i]=nigthMag[i];
        t_travelMag[i]=travelMag[i];
    }
    for(int i=0; i<LIGTHS;i++ )
    {
        t_nigthLig[i]=nigthLig[i];
        t_travelLig[i]=travelLig[i];
    }

    t_nigthLEDs=nigthLEDs;
    t_nigthTone=nigthTone;
    t_nigthCall=nigthCall;
    t_nigthLigths=nigthLigths;
    t_nigthLigthsOff=nigthLigthsOff;
    t_nigthSiren=nigthSiren;

    t_travelLEDs=travelLEDs;
    t_travelTone=travelTone;
    t_travelCall=travelCall;
    t_travelLigths=travelLigths;
    t_travelLigthsOff=travelLigthsOff;
    t_travelSiren=travelSiren;

    firstSecureModeEdit = true;
    firstClickNigth = true;
    firstClickTravel = true;

    if(ui->btnNigthS->isChecked())
        restoreNigthSettings();
    else
        restoreTravelSettings();    
}

void MainWindow::restoreNigthSettings()
{
    if(firstClickNigth)
    {
        if(!firstSecureModeEdit)
            this->getTravelSettings();

        ui->chkMovSen->setChecked(t_nigthMov[0]);
        ui->chkMovSen2->setChecked(t_nigthMov[1]);
        ui->chkMovSen3->setChecked(t_nigthMov[2]);
        ui->chkMovSen4->setChecked(t_nigthMov[3]);
        ui->chkMovSen5->setChecked(t_nigthMov[4]);
        ui->chkMovSen6->setChecked(t_nigthMov[5]);
        ui->chkMovSen7->setChecked(t_nigthMov[6]);
        ui->chkMovSen8->setChecked(t_nigthMov[7]);

        ui->chkMagSen->setChecked(t_nigthMag[0]);
        ui->chkMagSen2->setChecked(t_nigthMag[1]);
        ui->chkMagSen3->setChecked(t_nigthMag[2]);
        ui->chkMagSen4->setChecked(t_nigthMag[3]);
        ui->chkMagSen5->setChecked(t_nigthMag[4]);
        ui->chkMagSen6->setChecked(t_nigthMag[5]);
        ui->chkMagSen7->setChecked(t_nigthMag[6]);
        ui->chkMagSen8->setChecked(t_nigthMag[7]);

        ui->chkLigth->setChecked(t_nigthLig[0]);
        ui->chkLigth2->setChecked(t_nigthLig[1]);
        ui->chkLigth3->setChecked(t_nigthLig[2]);
        ui->chkLigth4->setChecked(t_nigthLig[3]);
        ui->chkLigth5->setChecked(t_nigthLig[4]);
        ui->chkLigth6->setChecked(t_nigthLig[5]);
        ui->chkLigth7->setChecked(t_nigthLig[6]);
        ui->chkLigth8->setChecked(t_nigthLig[7]);

        ui->chkLed->setChecked( t_nigthLEDs);
        ui->chkTone->setChecked(t_nigthTone);
        ui->chkPhone->setChecked(t_nigthCall);
        ui->chkLigths->setChecked(t_nigthLigths);
        ui->chkLigthsOff->setChecked(t_nigthLigthsOff);
        ui->chkSiren->setChecked(t_nigthSiren);
    }
    firstSecureModeEdit = false;
    firstClickNigth = false;
    firstClickTravel = true;
}

void MainWindow::restoreTravelSettings()
{
    if(firstClickTravel)
    {
        if(!firstSecureModeEdit)
            this->getNigthSettings();

        ui->chkMovSen->setChecked(t_travelMov[0]);
        ui->chkMovSen2->setChecked(t_travelMov[1]);
        ui->chkMovSen3->setChecked(t_travelMov[2]);
        ui->chkMovSen4->setChecked(t_travelMov[3]);
        ui->chkMovSen5->setChecked(t_travelMov[4]);
        ui->chkMovSen6->setChecked(t_travelMov[5]);
        ui->chkMovSen7->setChecked(t_travelMov[6]);
        ui->chkMovSen8->setChecked(t_travelMov[7]);

        ui->chkMagSen->setChecked(t_travelMag[0]);
        ui->chkMagSen2->setChecked(t_travelMag[1]);
        ui->chkMagSen3->setChecked(t_travelMag[2]);
        ui->chkMagSen4->setChecked(t_travelMag[3]);
        ui->chkMagSen5->setChecked(t_travelMag[4]);
        ui->chkMagSen6->setChecked(t_travelMag[5]);
        ui->chkMagSen7->setChecked(t_travelMag[6]);
        ui->chkMagSen8->setChecked(t_travelMag[7]);

        ui->chkLigth->setChecked(t_travelLig[0]);
        ui->chkLigth2->setChecked(t_travelLig[1]);
        ui->chkLigth3->setChecked(t_travelLig[2]);
        ui->chkLigth4->setChecked(t_travelLig[3]);
        ui->chkLigth5->setChecked(t_travelLig[4]);
        ui->chkLigth6->setChecked(t_travelLig[5]);
        ui->chkLigth7->setChecked(t_travelLig[6]);
        ui->chkLigth8->setChecked(t_travelLig[7]);

        ui->chkLed->setChecked(t_travelLEDs);
        ui->chkTone->setChecked(t_travelTone);
        ui->chkPhone->setChecked(t_travelCall);
        ui->chkLigths ->setChecked(t_travelLigths);
        ui->chkLigthsOff ->setChecked(t_travelLigthsOff);
        ui->chkSiren ->setChecked(t_travelSiren);
    }

    firstSecureModeEdit=false;    
    firstClickTravel = false;
    firstClickNigth = true;
}

void MainWindow::restorePassSettings()
{
    ui->chkPass->setChecked(passActive);
    ui->btnUnlockS->setEnabled(true);
    ui->frameS1->setEnabled(false);
    ui->frameS2->setEnabled(false);
    ui->btnSaveS->setEnabled(false);
}

void MainWindow::restoreGeneralSettings()
{    
    ui->gboxAlarmOn->setChecked(timeAlarmOn);    
    ui->timeAlarm->setTime(QTime::fromString(timeAlarm,"hh:mm AP"));
    ui->chkLigthsG->setChecked(alarmLigths);
    ui->chkSirenG->setChecked(alarmSiren);
    ui->gboxNigth->setChecked(nigthManual);
    ui->timeInit->setTime(QTime::fromString(timeNigthInit,"hh:mm AP"));
    ui->timeEnd->setTime(QTime::fromString(timeNigthEnd,"hh:mm AP"));
    ui->spnPrice->setValue(kwhPrice);
    ui->spnDay->setValue(readDay); 

    for(int i=0;i<LIGTHS;i++)
    {
        t_sensorOn[i]=sensorOn[i];
        t_timeShort[i]=timeShort[i];
        t_timeLong[i]=timeLong[i];        
    }

    previousChkLigthIndex=-1;
    restoreTimeLigthsSettings();
    chkWidgetsClearFocus();
}

void MainWindow::restoreTimeLigthsSettings()
{    
    if(previousChkLigthIndex!=-1)
        getTimeLigthsSettings(); //Save previous setting

    int index=-1;

    if(ui->chkLigth->isChecked())
        index=(0);
    else if(ui->chkLigth2->isChecked())
        index=(1);
    else if(ui->chkLigth3->isChecked())
        index=(2);
    else if(ui->chkLigth4->isChecked())
        index=(3);
    else if(ui->chkLigth5->isChecked())
        index=(4);
    else if(ui->chkLigth6->isChecked())
        index=(5);
    else if(ui->chkLigth7->isChecked())
        index=(6);
    else if(ui->chkLigth8->isChecked())
        index=(7);
    
    if(index!=-1)
    {
        ui->gboxIntOn->setEnabled(true);
        ui->gboxSensorOn->setEnabled(true);
        ui->gboxSensorOn->setChecked(t_sensorOn[index]);
        ui->dialShort->setValue(t_timeShort[index]);
        ui->dialLong->setValue(t_timeLong[index]);        
    }
    else
    {
        ui->gboxIntOn->setEnabled(false);
        ui->gboxSensorOn->setEnabled(false);
        ui->gboxSensorOn->setChecked(false);
        ui->dialShort->setValue(0);
        ui->dialLong->setValue(0);
    }

    previousChkLigthIndex = index;
    chkWidgetsClearFocus();
}

void MainWindow::getSecuritySettings()
{
    /* Nigth: temporal values */
    for(int i=0; i<MOV_SENSORS;i++)
    {
        nigthMov[i]=t_nigthMov[i];
        travelMov[i]=t_travelMov[i];
    }
    for(int i=0; i<MAG_SENSORS;i++)
    {
        nigthMag[i]=t_nigthMag[i];
        travelMag[i]=t_travelMag[i];
    }
    for(int i=0; i<LIGTHS;i++)
    {
        nigthLig[i]=t_nigthLig[i];
        travelLig[i]=t_travelLig[i];
    }

    nigthLEDs=t_nigthLEDs;
    nigthTone=t_nigthTone;
    nigthCall=t_nigthCall;
    nigthLigths=t_nigthLigths;
    nigthLigthsOff=t_nigthLigthsOff;
    nigthSiren=t_nigthSiren;

    travelLEDs=t_travelLEDs;
    travelTone=t_travelTone;
    travelCall=t_travelCall;
    travelLigths=t_travelLigths;
    travelLigthsOff=t_travelLigthsOff;
    travelSiren=t_travelSiren;

    getPassSettings();
}

void MainWindow::getNigthSettings()
{
    t_nigthMov[0]=ui->chkMovSen->isChecked();
    t_nigthMov[1]=ui->chkMovSen2->isChecked();
    t_nigthMov[2]=ui->chkMovSen3->isChecked();
    t_nigthMov[3]=ui->chkMovSen4->isChecked();
    t_nigthMov[4]=ui->chkMovSen5->isChecked();
    t_nigthMov[5]=ui->chkMovSen6->isChecked();
    t_nigthMov[6]=ui->chkMovSen7->isChecked();
    t_nigthMov[7]=ui->chkMovSen8->isChecked();

    t_nigthMag[0]=ui->chkMagSen->isChecked();
    t_nigthMag[1]=ui->chkMagSen2->isChecked();
    t_nigthMag[2]=ui->chkMagSen3->isChecked();
    t_nigthMag[3]=ui->chkMagSen4->isChecked();
    t_nigthMag[4]=ui->chkMagSen5->isChecked();
    t_nigthMag[5]=ui->chkMagSen6->isChecked();
    t_nigthMag[6]=ui->chkMagSen7->isChecked();
    t_nigthMag[7]=ui->chkMagSen8->isChecked();

    t_nigthLig[0]=ui->chkLigth->isChecked();
    t_nigthLig[1]=ui->chkLigth2->isChecked();
    t_nigthLig[2]=ui->chkLigth3->isChecked();
    t_nigthLig[3]=ui->chkLigth4->isChecked();
    t_nigthLig[4]=ui->chkLigth5->isChecked();
    t_nigthLig[5]=ui->chkLigth6->isChecked();
    t_nigthLig[6]=ui->chkLigth7->isChecked();
    t_nigthLig[7]=ui->chkLigth8->isChecked();

    t_nigthLEDs=ui->chkLed->isChecked();
    t_nigthTone=ui->chkTone->isChecked();
    t_nigthCall=ui->chkPhone->isChecked();
    t_nigthLigths=ui->chkLigths->isChecked();
    t_nigthLigthsOff=ui->chkLigthsOff->isChecked();
    t_nigthSiren=ui->chkSiren->isChecked();

}

void MainWindow::getTravelSettings()
{
    t_travelMov[0]=ui->chkMovSen->isChecked();
    t_travelMov[1]=ui->chkMovSen2->isChecked();
    t_travelMov[2]=ui->chkMovSen3->isChecked();
    t_travelMov[3]=ui->chkMovSen4->isChecked();
    t_travelMov[4]=ui->chkMovSen5->isChecked();
    t_travelMov[5]=ui->chkMovSen6->isChecked();
    t_travelMov[6]=ui->chkMovSen7->isChecked();
    t_travelMov[7]=ui->chkMovSen8->isChecked();

    t_travelMag[0]=ui->chkMagSen->isChecked();
    t_travelMag[1]=ui->chkMagSen2->isChecked();
    t_travelMag[2]=ui->chkMagSen3->isChecked();
    t_travelMag[3]=ui->chkMagSen4->isChecked();
    t_travelMag[4]=ui->chkMagSen5->isChecked();
    t_travelMag[5]=ui->chkMagSen6->isChecked();
    t_travelMag[6]=ui->chkMagSen7->isChecked();
    t_travelMag[7]=ui->chkMagSen8->isChecked();

    t_travelLig[0]=ui->chkLigth->isChecked();
    t_travelLig[1]=ui->chkLigth2->isChecked();
    t_travelLig[2]=ui->chkLigth3->isChecked();
    t_travelLig[3]=ui->chkLigth4->isChecked();
    t_travelLig[4]=ui->chkLigth5->isChecked();
    t_travelLig[5]=ui->chkLigth6->isChecked();
    t_travelLig[6]=ui->chkLigth7->isChecked();
    t_travelLig[7]=ui->chkLigth8->isChecked();

    t_travelLEDs=ui->chkLed->isChecked();
    t_travelTone=ui->chkTone->isChecked();
    t_travelCall=ui->chkPhone->isChecked();
    t_travelLigths=ui->chkLigths->isChecked();
    t_travelLigthsOff=ui->chkLigthsOff->isChecked();
    t_travelSiren=ui->chkSiren->isChecked();
}

void MainWindow::getPassSettings()
{
    passActive=ui->chkPass->isChecked();
}

void MainWindow::getGeneralSettings()
{    
    timeAlarmOn=ui->gboxAlarmOn->isChecked();
    timeAlarm=ui->timeAlarm->time().toString("hh:mm AP");
    alarmLigths=ui->chkLigthsG->isChecked();
    alarmSiren=ui->chkSirenG->isChecked();
    nigthManual=ui->gboxNigth->isChecked();
    timeNigthInit=ui->timeInit->time().toString("hh:mm AP");
    timeNigthEnd=ui->timeEnd->time().toString("hh:mm AP");
    kwhPrice=ui->spnPrice->value();
    readDay=ui->spnDay->value();

    for(int i=0;i<LIGTHS;i++)
    {
        sensorOn[i]=t_sensorOn[i];
        timeShort[i]=t_timeShort[i];
        timeLong[i]=t_timeLong[i];
    }
}

void MainWindow::getTimeLigthsSettings()
{
    t_sensorOn[previousChkLigthIndex]=ui->gboxSensorOn->isChecked();
    t_timeShort[previousChkLigthIndex]=ui->dialShort->value();
    t_timeLong[previousChkLigthIndex]=ui->dialLong->value();
}

void MainWindow::unlockPageS()
{
    if(!checkPassword())
        return;

    chkWidgetsSetEnable(true);
    this->restoreSecuritySetting();
    ui->frameS1->setEnabled(true);
    ui->frameS2->setEnabled(true);
    ui->btnUnlockS->setEnabled(false);
    ui->btnSaveS->setEnabled(true);
}

int MainWindow::checkPassword()
{
    QString passIn = getPasswordIn("<p><b>Ingrese Clave<\b></p>" );
    int failCount=0;
    while(passIn != pass)
    {
        if(passIn==QString::null)
            return 0;

        failCount++;

        if(failCount<3)
            passIn = getPasswordIn("<p><b>Clave incorrecta<\b></p> <p>Ingrese Clave</p>");
        else
            return 0;
    }
    return 1;
}


void MainWindow::changePass()
{
    //TODO: Reenter new password (validation)

    QString passIn = getPasswordIn("<p><b>Ingrese Clave Nueva<\b></p>");
    if(passIn!=QString::null)
    {
        pass = passIn;
        QMessageBox::information(this,"Cambio de clave", "La clave ha sido cambiada.");
        saveSettings();
    }
    else
        QMessageBox::warning(this,"Cambio de clave", "La clave NO ha sido cambiada.");
}



/* Password Dialog */
QString MainWindow::getPasswordIn(QString dafaultText)
{
    buttonOkPresed=false;
    passUi->lblDefault->setText(dafaultText);
    passwordDialog->exec();

    QString passOut = passUi->txtPass->text();
    passUi->txtPass->setText("");
    passUi->btnDel->setEnabled(false);

    if(buttonOkPresed)
        return passOut;

    return QString::null;
}


void MainWindow::pressed0()
{
    addToPass(0);
}

void MainWindow::pressed1()
{
    addToPass(1);
}

void MainWindow::pressed2()
{
    addToPass(2);
}

void MainWindow::pressed3()
{
    addToPass(3);
}

void MainWindow::pressed4()
{
    addToPass(4);
}

void MainWindow::pressed5()
{
    addToPass(5);
}

void MainWindow::pressed6()
{
    addToPass(6);
}

void MainWindow::pressed7()
{
    addToPass(7);
}

void MainWindow::pressed8()
{
    addToPass(8);
}

void MainWindow::pressed9()
{
    addToPass(9);    
}

void MainWindow::pressedDel()
{
    addToPass(-1);
}

void MainWindow::pressedOk()
{
    if(passUi->txtPass->text()!="")
    {
        buttonOkPresed=true;
        passwordDialog->close();
        passUi->txtPass->update();     
    }
}

void MainWindow::pressedCancel()
{
    buttonOkPresed=false;    
    passwordDialog->close();    
}

void MainWindow::addToPass(int value)
{
    bool ok;
    int current = passUi->txtPass->text().toInt(&ok,10);

    if(value>=0)
    {
        if(current*10>9999)
            current = current;
        else
            current = current*10 + value;
    }
    else if(ok)
    {
        if(current<10)        
            current = -1;
        else
            current = int(current/10);
    }

    if(current==-1)
    {
        passUi->txtPass->setText("");
        passUi->btnDel->setEnabled(false);
    }
    else
    {
        passUi->txtPass->setText(QString::number(current));
        passUi->btnDel->setEnabled(true);
    }

    if(current*10>9999)
    {
        pressedOk();
    }
}


/* Reconfiguration over serial */
void MainWindow::setNigthMovMask()
{
    //prepare mask
    char mask=0;
    for(int i = 0; i < MOV_SENSORS; i++)
        if(this->nigthMov[i])
            sbi(mask,i);

    sentDataFrame(char(SET_NIGTH_MOV_MASK),mask);
}

void MainWindow::setNigthMagMask()
{
    //prepare mask
    char mask=0;
    for(int i = 0; i < MAG_SENSORS; i++)
        if(this->nigthMag[i])
            sbi(mask,i);

    sentDataFrame(char(SET_NIGTH_MAG_MASK),mask);
}

void MainWindow::setNigthLigMask()
{
    //prepare mask
    char mask=0;
    for(int i = 0; i < LIGTHS; i++)
        if(this->nigthLig[i])
            sbi(mask,i);

    sentDataFrame(char(SET_NIGTH_LIG_MASK),mask);
}

void MainWindow::setNigthOthMask(){
    //prepare mask
    char mask=0;
    if(nigthLEDs)       sbi(mask,sLEDS);
    if(nigthTone)       sbi(mask,sTONE);
    if(nigthCall)       sbi(mask,sCALL);

    if(nigthLigths)     sbi(mask,sLIGTHS_ON);
    if(nigthLigthsOff)  sbi(mask,sLIGTHS_OFF);
    if(nigthSiren)      sbi(mask,sSIREN);

    sentDataFrame(char(SET_NIGTH_OTH_MASK),mask);
}

void MainWindow::setTravelMovMask()
{
    //prepare mask
    char mask=0;
    for(int i = 0; i < MOV_SENSORS; i++)
        if(this->travelMov[i])
            sbi(mask,i);

    sentDataFrame(char(SET_TRAVEL_MOV_MASK),mask);
}

void MainWindow::setTravelMagMask()
{
    //prepare mask
    char mask=0;
    for(int i = 0; i < MAG_SENSORS; i++)
        if(this->travelMag[i])
            sbi(mask,i);

    sentDataFrame(char(SET_TRAVEL_MAG_MASK),mask);
}

void MainWindow::setTravelLigMask()
{
    //prepare mask
    char mask=0;
    for(int i = 0; i < LIGTHS; i++)
        if(this->travelLig[i])
            sbi(mask,i);

    sentDataFrame(char(SET_TRAVEL_LIG_MASK),mask);
}

void MainWindow::setTravelOthMask()
{
    //prepare mask
    char mask=0;
    if(travelLEDs)       sbi(mask,sLEDS);
    if(travelTone)       sbi(mask,sTONE);
    if(travelCall)       sbi(mask,sCALL);

    if(travelLigths)     sbi(mask,sLIGTHS_ON);
    if(travelLigthsOff)  sbi(mask,sLIGTHS_OFF);
    if(travelSiren)      sbi(mask,sSIREN);

    sentDataFrame(char(SET_TRAVEL_OTH_MASK),mask);
}


void MainWindow::setCurrentTime()
{
    QTime current = QTime::currentTime();
    sentDataFrame(char(SET_CURRENT_TIME),(char) current.hour(),
                  (char) current.minute(),(char) current.second());
}

void MainWindow::setNigthInitTime()
{
    QTime current = QTime::fromString(timeNigthInit,"hh:mm AP");
    sentDataFrame(char(SET_NIGTH_INIT_TIME),(char) current.hour(),
                  (char) current.minute(),(char) current.second());
}

void MainWindow::setNigthEndTime(){
    QTime current = QTime::fromString(timeNigthEnd,"hh:mm AP");
    sentDataFrame(char(SET_NIGTH_END_TIME),(char) current.hour(),
                  (char) current.minute(),(char) current.second());
}

void MainWindow::setAlarmClockTime(){
    QTime current = QTime::fromString(timeAlarm,"hh:mm AP");
    sentDataFrame(char(SET_ALARM_CLOCK_TIME),(char) current.hour(),
                  (char) current.minute(),(char) current.second());
}

void MainWindow::setNormalLigMask()
{
    //prepare mask
    char mask=0;
    for(int i = 0; i < LIGTHS; i++)
        if(this->sensorOn[i])
            sbi(mask,i);

    sentDataFrame(char(SET_NORMAL_LIG_MASK),mask);
}

void MainWindow::setLigthTimeShort()
{
    for(int i = 0; i < LIGTHS; i++)
    {
        sentDataFrame(char(SET_LIGTH_TIME_SHORT),i,timeShort[i]);        
    }
}

void MainWindow::setLigthTimeLong()
{
    for(int i = 0; i < LIGTHS; i++)
    {
        sentDataFrame(char(SET_LIGTH_TIME_LONG),i,timeLong[i]);
    }
}


void MainWindow::setGeneralMask()
{
    //prepare mask
    char mask=0;
    if(timeAlarmOn)      sbi(mask,sALARM_CLOCK_ON);
    if(alarmLigths)      sbi(mask,sALARM_CLOCK_LIGTHS);
    if(alarmSiren)       sbi(mask,sALARM_CLOCK_TONE);
    if(nigthManual)      sbi(mask,sNIGTH_MANUAL);

    sentDataFrame(char(SET_GENERAL_MASK),mask);
}

int MainWindow::cmdEchoTest()
{    
    serialAvr->Flush(); //Clean the buffer

    char *buf;
    unsigned char temp[64];
    int a, blen,i=5;

    while(i>0)
    {        
        sentDataFrame(char(CMD_ECHO_TEST),'X');

        Sleeper::msleep(50);

        a = serialAvr->ReadPort();
        if ((a & 1) == 1)
        {
            buf = serialAvr->GetBufer();
            blen = serialAvr->GetBytesRead();
            memcpy( temp, buf, blen );
            if(temp[0]=='X')
             return 1;                         
        }       
        i--;        
    }
    return 0;
}

void MainWindow::cmdInitNigthMode()
{
    sentDataFrame(char(CMD_INIT_NIGTH_MODE));
}

void MainWindow::cmdInitTravelMode()
{
    sentDataFrame(char(CMD_INIT_TRAVEL_MODE));
}

void MainWindow::cmdDisarmSecurity()
{
    sentDataFrame(char(CMD_DISARM_SECURITY));
}

void MainWindow::cmdTriggerAlarm()
{
    sentDataFrame(char(CMD_TRIGGER_ALARM));
}

void MainWindow::cmdTurnOnLigth(unsigned char id)
{
    sentDataFrame(char(CMD_TURN_ON_LIGTH),(char)id);
}

void MainWindow::cmdTurnOnLigths()
{
    sentDataFrame(char(CMD_TURN_ON_LIGTHS));
}

void MainWindow::cmdTurnOffLigth(unsigned char id)
{
    sentDataFrame(char(CMD_TURN_OFF_LIGTH),(char)id);
}

void MainWindow::cmdTurnOffLigths()
{
    sentDataFrame(char(CMD_TURN_OFF_LIGTHS));
}

void MainWindow::cmdDisarmAlarmClock()
{
    sentDataFrame(char(CMD_DISARM_ALARM_CLOCK));
}

void MainWindow::sentDataFrame(char command,char byte0,char byte1,char byte2)
{
    char data[rMAX_FRAME_LENGTH] = {
                                        char(START),
                                        char(START),
                                        command,
                                        byte0,
                                        byte1,
                                        byte2,
                                        char(END),
                                        char(END)
                                    };
    if(serialAvr->GetPortFD()>=0)
        serialAvr->WritePort(data,rMAX_FRAME_LENGTH);

    //Sleeper::msleep(1); //Prevent crash!
}
