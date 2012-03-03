#include <PinChangeInt.h>

// #define NO_PORTB_PINCHANGES // to indicate that port b will not be used for pin change interrupts
#define NO_PORTC_PINCHANGES // to indicate that port c will not be used for pin change interrupts
#define NO_PORTD_PINCHANGES // to indicate that port d will not be used for pin change interrupts
#define NO_PIN_STATE        // to indicate that you don't need the pinState
#define NO_PIN_NUMBER       // to indicate that you don't need the arduinoPin

#include "domo.h"

//Have to be interrupt pin!!!
byte mag[MAG_SENSORS] = {7,7,7,7,7,7,7,7}; //Magnetic sensors. pin definitions
byte mov[MOV_SENSORS] = {4,4,4,4,4,4,4,4}; //Movement sensors. pin definitions
byte swt[LIGTHS]    =   {5,5,5,5,5,5,5,5}; //Interruptors. pin definitions 
//Have to be PWM!!!
byte ligth[LIGTHS]    = {6,6,6,6,6,6,6,6}; //Ligths. pin definitions 

byte sirenPin = 11;
byte ledsPin = 13;
byte callPin = 12;
byte ligthSensor = 8;
byte analogTemp = A0;
byte analogCurrent = A1;
byte analogVoltage = A2;

byte longTime[LIGTHS] = {3,3,3,3,3,3,3,3};  // seconds
byte shortTime[LIGTHS] = {3,3,3,3,3,3,3,3}; // seconds
byte longTimeCount[LIGTHS] = {0}; //upto 240
byte shortTimeCount[LIGTHS] = {0}; //upto 240
byte ledPwmCount[LIGTHS] = {0};
boolean longState[LIGTHS] = {0};
boolean ligthState[LIGTHS] = {0};

/* Masks for ligths auto-on and critical sensors */
byte ligthMask = 0xFF;
byte movMask  = 0x00;
byte magMask  = 0x00;
byte otherMask = 0x00;

/* Settings for nigth mode */
byte nigthMov = 0xFF;
byte nigthMag = 0xFF;
byte nigthLig = 0xFF;
byte nigthOther = 0xFF;

/* Settings for travel mode */
byte travelMov = 0xFF;
byte travelMag = 0xFF;
byte travelLig = 0xFF;
byte travelOther = 0xFF;

/* General */
byte normalLig = 0xFF;
byte generalMask = 0x00;

boolean securedMode = 0; 
boolean secured = false;
boolean alarmActivated = false;
boolean isNigth = false;
boolean alarmClockActivated = false;
boolean playOnTone = false;
boolean playOffTone = false;

struct avrTime currentTime = {12,0,0};
struct avrTime nigthInitTime = {18,0,0};
struct avrTime nigthEndTime = {6,0,0};
struct avrTime alarmClockTime = {7,0,0};

word timer1Count = 0;
byte sirenCount = 0;
byte noteIndex=0;

unsigned long currentAcc = 0;
word voltage = 0;

char serialBufferIndex = 0;
byte serialBuffer[8];
byte prevSerialValue = 0;


                                 
/* Functions */

void setup()
{

  //Timer 1 (16-bit) .. reconfiguration
  TCCR1A = 0;
  TCCR1B = 0;  
  TIMSK1 = 0;  
  sbi(TCCR1B, CS12);  // Prescale factor
  sbi(TCCR1B, CS10);  // to 1024!    
  sbi(TCCR1B, WGM13); // Waveform Generation
  sbi(TCCR1B, WGM12); // CTC mode (12) -> ICR1    
  ICR1 = 24;          // ICR1 to 24 -> 16MHz/(1024*(1+24))=625 Interrupts/second
  sbi(TIMSK1,ICIE1);  // Enable input capture interrupt flag     
  TCNT1 = 0;          // Reset counter1
  
  //PINS  
  attachInterrupt(0,initTravelMode,FALLING);
  //attachInterrupt(1,initNigthMode,FALLING);
    
  for(int i=0; i<LIGTHS;i++)
    pinMode(ligth[0], OUTPUT);  

  //for(int i=0; i<MAG_SENSORS;i++)  
    PCintPort::attachInterrupt(mag[0],&magActiated_0,FALLING);
    PCintPort::attachInterrupt(mag[1],&magActiated_1,FALLING);
    PCintPort::attachInterrupt(mag[2],&magActiated_2,FALLING);
    PCintPort::attachInterrupt(mag[3],&magActiated_3,FALLING);
    PCintPort::attachInterrupt(mag[4],&magActiated_4,FALLING);
    PCintPort::attachInterrupt(mag[5],&magActiated_5,FALLING);
    PCintPort::attachInterrupt(mag[6],&magActiated_6,FALLING);
    PCintPort::attachInterrupt(mag[7],&magActiated_7,FALLING);
  
  //for(int i=0; i<MOV_SENSORS;i++)  
    PCintPort::attachInterrupt(mov[0],&movActiated_0,FALLING);
    PCintPort::attachInterrupt(mov[1],&movActiated_1,FALLING);
    PCintPort::attachInterrupt(mov[2],&movActiated_2,FALLING);
    PCintPort::attachInterrupt(mov[3],&movActiated_3,FALLING);
    PCintPort::attachInterrupt(mov[4],&movActiated_4,FALLING);
    PCintPort::attachInterrupt(mov[5],&movActiated_5,FALLING);
    PCintPort::attachInterrupt(mov[6],&movActiated_6,FALLING);
    PCintPort::attachInterrupt(mov[7],&movActiated_7,FALLING);              
/*
  //for(int i=0; i<LIGTHS;i++)  
    PCintPort::attachInterrupt(swt[0],&swtActiated_0,FALLING);
    PCintPort::attachInterrupt(swt[1],&swtActiated_1,FALLING);
    PCintPort::attachInterrupt(swt[2],&swtActiated_2,FALLING);
    PCintPort::attachInterrupt(swt[3],&swtActiated_3,FALLING);
    PCintPort::attachInterrupt(swt[4],&swtActiated_4,FALLING);
    PCintPort::attachInterrupt(swt[5],&swtActiated_5,FALLING);
    PCintPort::attachInterrupt(swt[6],&swtActiated_6,FALLING);
    PCintPort::attachInterrupt(swt[7],&swtActiated_7,FALLING);     
*/ 
  //XXXX Other sensors ??  
    
  Serial.begin(57600);
  Serial.print(rREPORT_RESET);
  
}

void loop()
{  
  //Handling ligths state
  for(int i=0; i<LIGTHS;i++)
  {
    if(ligthState[i])
    {
      if(ledPwmCount[i]<255)
      {
        ledPwmCount[i]++;
        analogWrite(ligth[i],ledPwmCount[i]);
      }
    }
    else
    {
      if(ledPwmCount[i]>0)
      {
        ledPwmCount[i]--;
        analogWrite(ligth[i],ledPwmCount[i]);
      }
    }  
  }
  
  delay(2); //XXX: Just this delay is allowed for this desing !!!
            //have to be adjust when sensor reading have been added              
}

ISR(TIMER1_CAPT_vect) 
{         
  currentAcc += pow(analogRead(analogCurrent)-CURRENT_CENTER,2);
  delayMicroseconds(10);
  int currentVoltage = analogRead(analogVoltage);
  if(currentVoltage > voltage)
    voltage = currentVoltage;
  
  timer1Count++;
  if(timer1Count==625)
  {        
      timer1Count=0;
      
      reportState();
      
      //Handling ligths state 
      for(int i=0; i<LIGTHS;i++)
      {  
          //ligthState[i]=!ligthState[i];
          if(longTimeCount[i]>0)
          {
            longTimeCount[i]--;
            if(!longTimeCount[i])
            {            
              longState[i]=0;
              ligthState[i]=LOW;
            }
          }
          
          if(shortTimeCount[i]>0)
          {
            shortTimeCount[i]--;
            if(!shortTimeCount[i])
            {
              ligthState[i]=LOW;        
            }
          }     
      } 
      
      //Update current time 
      currentTime.seconds++;
      if(currentTime.seconds==60)
      {
        currentTime.seconds=0;
        currentTime.minutes++;
        if(currentTime.minutes==60)
        {
          currentTime.minutes=0;
          currentTime.hours++;
          if(currentTime.hours==24)
          {
            currentTime.hours=0;
          }      
        }      
        checkAlarmClock(); 
        nigthDetection();
      }           
                               
      //Control the tones (alarm clock, alarm on, alarm off)
  } 

  checkSirenState();   
    
}

void checkSirenState()
{
  if(alarmActivated)
  {
    if(tbi(otherMask,sSIREN))
    {
      playOffTone = false;
      playOnTone = false;
      
      sirenCount++;
      if(sirenCount>=19)
      {
        if(noteIndex > 12)
          noteIndex = 0; 
        sirenCount = 0;
        tone(sirenPin,sirenNotes1[noteIndex],32);         
        noteIndex++;               
      }
    }
  }
  else if (alarmClockActivated)
  {
    if(tbi(generalMask,sALARM_CLOCK_TONE))
    {
      playOffTone = false;
      playOnTone = false;
      
      sirenCount++;
      if(sirenCount>=156)
      {
        if(noteIndex > 4)
          noteIndex = 0; 
        sirenCount = 0;
        tone(sirenPin,sirenNotes2[noteIndex],250);         
        noteIndex++;               
      }
    }    
  }
  else if(playOnTone)
  {    
    sirenCount++;
    if(sirenCount>=125)
    {
      sirenCount = 0;
      
      if(noteIndex > 5)
      {
        noteIndex = 0; 
        playOnTone = false;
      }
      tone(sirenPin,sirenNotes3[noteIndex],200);         
      noteIndex++;               
    }  
  }
  else if(playOffTone)
  {
    sirenCount++;
    if(sirenCount>=125)
    {
      sirenCount = 0;
      
      if(noteIndex > 5)
      {
        noteIndex = 0; 
        playOffTone = false;
      }
      tone(sirenPin,sirenNotes3[5-noteIndex],200);         
      noteIndex++;               
    }  
  }  
  else
  {      
    noteIndex = 0;
    sirenCount = 0;
  }   
}

void checkAlarmClock()
{
    //Check alarm clock 
    if(tbi(generalMask,sALARM_CLOCK_ON))
    {      
      if( (secured && securedMode && !alarmActivated) || !secured )
      {
         if(currentTime.hours == alarmClockTime.hours &&
            currentTime.minutes == alarmClockTime.minutes)
         {
            disarmSecurity();
            alarmClockActivated = true;
            
            if(tbi(generalMask,sALARM_CLOCK_LIGTHS))
              turnOnLigths();        
         }
         else
         {
            alarmClockActivated = false;
         }
      }
    }
}

void nigthDetection()
{
  //Nigth detection
  if(tbi(generalMask,sNIGTH_MANUAL))
  {  
    isNigth = false;
    if((currentTime.hours <= nigthEndTime.hours) &&
       (currentTime.minutes <= nigthEndTime.minutes))
       isNigth = true;

    if(currentTime.hours >= nigthInitTime.hours &&
       currentTime.minutes >= nigthInitTime.minutes)
       isNigth = true;    
  }  
  else
  {
    isNigth = digitalRead(ligthSensor); 
  }
}

void reportState()
{
    //prepare mask for movement sensors
    char mask=0;
    for(int i = 0; i < MOV_SENSORS; i++)
        if(digitalRead(mov[i]))
            sbi(mask,i);
    
    Serial.write(mask);

    //prepare mask for magnetic sensors    
    mask=0;
    for(int i = 0; i < MAG_SENSORS; i++)
        if(digitalRead(mag[i]))
            sbi(mask,i);
    
    Serial.write(mask);
    
    //prepare mask for ligths state       
    mask=0;
    for(int i = 0; i < LIGTHS; i++)
        if(ligthState[i])
            sbi(mask,i);
            
    Serial.write(mask);        
         
    //prepare mask for state flags
    mask=0;
    if(secured)
      sbi(mask,rSECURED);
      
    if(securedMode)
      sbi(mask,rSECURED_MODE);
      
    if(alarmActivated)
      sbi(mask,rALARM_ACTIVATED);

    if(isNigth)
      sbi(mask,rIS_NIGTH);  
  
    if(alarmClockActivated)
      sbi(mask,rALARM_CLOCK_ACTIVATED);    
      
    Serial.write(mask);    
    
    Serial.write(analogRead(A0)>>2); //Temperature    
    
    currentAcc /= 625;    
    Serial.write(currentAcc & 0xFF);       //Current L
    Serial.write((currentAcc>>8) & 0xFF);  //Current H1
    Serial.write((currentAcc>>16) & 0xFF); //Current H2
    
    Serial.write(voltage & 0xFF);   //Voltage L
    Serial.write(voltage >> 8);     //Voltage H

    currentAcc = 0;
    voltage = 0;

    // Testing! time 
    Serial.write(currentTime.hours);    
        Serial.write(currentTime.minutes);    
            Serial.write(currentTime.seconds);                        
}

//Travel mode
void initTravelMode()
{
  uint8_t oldSREG = SREG;
  cli();
  
      playOffTone = false;
      playOnTone = false;
  
    if(alarmClockActivated)
    {
      alarmClockActivated = false;
      return;
    }
    
    if (secured)
    {
       disarmSecurity();
    }    
    else
    {      
      securedMode = 0; 
      ligthMask = travelLig;
      movMask  = travelMov;
      magMask  = travelMag;
      otherMask = travelOther;   
    
      if(tbi(travelOther,sLEDS))
        digitalWrite(ledsPin,HIGH);

      if(tbi(travelOther,sLIGTHS_OFF))
        turnOffLigths();         
    
      if(tbi(travelOther,sTONE))
        playOnTone = true;      
        
      secured = true;
    }
  
    noteIndex = 0;
    
  SREG = oldSREG;
}

//Nigth mode
void initNigthMode()
{
  uint8_t oldSREG = SREG;
  cli();  

      playOffTone = false;
      playOnTone = false;

    if(alarmClockActivated)
    {
      alarmClockActivated = false;      
      return;
    }
  
    if (secured)
    {
       disarmSecurity();
    }    
    else
    {        
      securedMode = 1;          
      ligthMask = nigthLig;
      movMask  = nigthMov;
      magMask  = nigthMag;
      otherMask = nigthOther;     
      
      if(tbi(nigthOther,sLEDS))
        digitalWrite(ledsPin,HIGH);

      if(tbi(nigthOther,sLIGTHS_OFF))
        turnOffLigths();         
      
      if(tbi(nigthOther,sTONE))
        playOnTone = true;      
      
      secured = true;                
    }
    
    noteIndex = 0;
  
  SREG = oldSREG;
}

void disarmSecurity()
{
    alarmActivated = false;
    secured = false;
    
    if(tbi(otherMask,sTONE))
      playOffTone = true;

    ligthMask = normalLig;
    movMask  = 0x00;
    magMask  = 0x00;
    otherMask = 0x00;    
    digitalWrite(ledsPin,LOW);  
    digitalWrite(callPin,LOW); 

    noteIndex = 0;
}

void turnOffLigths()
{  
  for(int i = 0; i < LIGTHS; i++)
    turnOffLigth(i);  
}

void turnOffLigth(byte index)
{
    ligthState[index] = LOW;
    longTimeCount[index]=0;
    shortTimeCount[index]=0;
    longState[index] = false;
}

void turnOnLigths()
{  
  for(int i = 0; i < LIGTHS; i++)
    shortTimeOn(i);
}

void alarmTriggered()
{
  if(secured)
  {
    if(tbi(otherMask,sLIGTHS_ON))
       turnOnLigths(); 

    if(tbi(otherMask,sCALL))
       digitalWrite(callPin,HIGH);        
     /*  
    if(tbi(otherMask,sSIREN))
       digitalWrite(sirenPin,HIGH);  
    */  
    alarmActivated = true;         
    ligthMask = normalLig;
  }
}

//Ligth mask is used for determine what ligths
//allow auto ON
//Movement sensors

void movActiated_0()
{ 
  if(tbi(ligthMask,0) && isNigth) shortTimeOn(0); 
  if(tbi(movMask,0) && secured) alarmTriggered(); 
}
void movActiated_1()
{ 
  if(tbi(ligthMask,1) && isNigth) shortTimeOn(1);  
  if(tbi(movMask,1) && secured) alarmTriggered(); 
}
void movActiated_2() 
{ 
  if(tbi(ligthMask,2) && isNigth) shortTimeOn(2); 
  if(tbi(movMask,2) && secured) alarmTriggered();
}
void movActiated_3() 
{ 
  if(tbi(ligthMask,3) && isNigth) shortTimeOn(3); 
  if(tbi(movMask,3) && secured) alarmTriggered();
}
void movActiated_4() 
{ 
  if(tbi(ligthMask,4) && isNigth) shortTimeOn(4); 
  if(tbi(movMask,4) && secured) alarmTriggered(); 
}
void movActiated_5() 
{ 
  if(tbi(ligthMask,5) && isNigth) shortTimeOn(5); 
  if(tbi(movMask,5) && secured) alarmTriggered(); 
}
void movActiated_6() 
{ 
  if(tbi(ligthMask,6) && isNigth) shortTimeOn(6); 
  if(tbi(movMask,6) && secured) alarmTriggered();
}
void movActiated_7() 
{  
  if(tbi(ligthMask,7) && isNigth) shortTimeOn(7);
  if(tbi(movMask,7) && secured) alarmTriggered(); 
}

void shortTimeOn(byte index)
{ 
  if(!longState[index])
  {      
    shortTimeCount[index]=shortTime[index];
    ligthState[index] = HIGH; 
  }  
}

//Ligth Switches 
void swtActiated_0() { longTimeOn(0); }
void swtActiated_1() { longTimeOn(1); }
void swtActiated_2() { longTimeOn(2); }
void swtActiated_3() { longTimeOn(3); }
void swtActiated_4() { longTimeOn(4); }
void swtActiated_5() { longTimeOn(5); }
void swtActiated_6() { longTimeOn(6); }
void swtActiated_7() { longTimeOn(7); }

void longTimeOn(byte index)
{ 
//If shortTime is active is deactivated
//and the long time is set.
//If longTime is active the ligth state is
//toggled.

  longTimeCount[index]=longTime[index];
      
  if(shortTimeCount[index])
  {
    ledPwmCount[index]=0;
    shortTimeCount[index]=0;
  }
  else
  { 
    ligthState[index] = !ligthState[index];
  }  
  
  longState[index] = ligthState[index];
}

//Magnetic Sensors
void magActiated_0() { if(tbi(magMask,0) && secured) alarmTriggered(); }
void magActiated_1() { if(tbi(magMask,1) && secured) alarmTriggered(); }
void magActiated_2() { if(tbi(magMask,2) && secured) alarmTriggered(); }
void magActiated_3() { if(tbi(magMask,3) && secured) alarmTriggered(); }
void magActiated_4() { if(tbi(magMask,4) && secured) alarmTriggered(); }
void magActiated_5() { if(tbi(magMask,5) && secured) alarmTriggered(); }
void magActiated_6() { if(tbi(magMask,6) && secured) alarmTriggered(); }
void magActiated_7() { if(tbi(magMask,7) && secured) alarmTriggered(); }

//XXX Other sensors ??



void serialEvent() {
  while (Serial.available()) 
  {
    byte data = (byte)Serial.read(); 
    serialBuffer[serialBufferIndex] = data; 
    if(data ==START && prevSerialValue==START)
    {   
      //uint8_t oldSREG = SREG;
      //cli(); 
          
      switch (serialBuffer[0])
      {
          
	case SET_NIGTH_MOV_MASK    :    // [START][01][VV][END]
          if(serialBufferIndex>0) 
            nigthMov = serialBuffer[1];
	  break;
	case SET_NIGTH_MAG_MASK    :    // [START][02][VV][END]
          if(serialBufferIndex>0) 
            nigthMag = serialBuffer[1];
	  break;
	case SET_NIGTH_LIG_MASK    :    // [START][03][VV][END]
          if(serialBufferIndex>0) 
            nigthLig = serialBuffer[1];
	  break;
	case SET_NIGTH_OTH_MASK    :    // [START][04][VV][END]
          if(serialBufferIndex>0)  
            nigthOther = serialBuffer[1];
	  break;

	case SET_TRAVEL_MOV_MASK   :    // [START][11][VV][END]
          if(serialBufferIndex>0)  
            travelMov = serialBuffer[1];
	  break;
	case SET_TRAVEL_MAG_MASK   :    // [START][12][VV][END]
          if(serialBufferIndex>0)  
            travelMag = serialBuffer[1];
	  break;
	case SET_TRAVEL_LIG_MASK   :    // [START][13][VV][END]
          if(serialBufferIndex>0)  
            travelLig = serialBuffer[1];
	  break;
	case SET_TRAVEL_OTH_MASK   :    // [START][14][VV][END]
          if(serialBufferIndex>0)  
            travelOther = serialBuffer[1];
	  break;

        case SET_CURRENT_TIME      :    // [START][20][HH][MM][SS][END]   
          if(serialBufferIndex>2) 
          {
            currentTime.hours = serialBuffer[1];
            currentTime.minutes = serialBuffer[2];
            currentTime.seconds = serialBuffer[3];
            //XXX may need some fix! (pseudo-concurrent modification)
          }
	  break;
	case SET_NIGTH_INIT_TIME   :    // [START][21][HH][MM][SS][END]    
          if(serialBufferIndex>2)
          {
            nigthInitTime.hours = serialBuffer[1];
            nigthInitTime.minutes = serialBuffer[2];
            nigthInitTime.seconds = serialBuffer[3];
          }
	  break;
	case SET_NIGTH_END_TIME    :    // [START][22][HH][MM][SS][END]    
          if(serialBufferIndex>2)
          {
            nigthEndTime.hours = serialBuffer[1];
            nigthEndTime.minutes = serialBuffer[2];
            nigthEndTime.seconds = serialBuffer[3];
          }
	  break;
	case SET_ALARM_CLOCK_TIME  :    // [START][23][HH][MM][SS][END]   
          if(serialBufferIndex>2)
          {
            alarmClockTime.hours = serialBuffer[1];
            alarmClockTime.minutes = serialBuffer[2];
            alarmClockTime.seconds = serialBuffer[3];
          }
	  break;
	case SET_NORMAL_LIG_MASK   :    // [START][24][VV][END]  
          if(serialBufferIndex>0) 
          {
            normalLig = serialBuffer[1]; 
            if(!secured) ligthMask = normalLig;
          }
	  break;
	case SET_LIGTH_TIME_SHORT  :    // [START][25][ID][VV][END] 
          if(serialBufferIndex>1 && serialBuffer[1] < LIGTHS)
            shortTime[serialBuffer[1]] = serialBuffer[2];
	  break;
	case SET_LIGTH_TIME_LONG   :    // [START][26][ID][VV][END]    
          if(serialBufferIndex>1 && serialBuffer[1] < LIGTHS)
            longTime[serialBuffer[1]] = serialBuffer[2];
          break;
          
	case SET_GENERAL_MASK  :    // [START][28][VV][END]    
          if(serialBufferIndex>0) 
            generalMask = serialBuffer[1]; 
          break;
          
        case CMD_ECHO_TEST         :   // [START][100][VV][END]
          if(serialBufferIndex>0)
            Serial.write(serialBuffer[1]);
	  break;
        //XXX Modes may need some fix! ¿interrupts disbled?
	case CMD_INIT_NIGTH_MODE   :   // [START][101][END]
          initNigthMode();
	  break;
	case CMD_INIT_TRAVEL_MODE  :   // [START][102][END]
          initTravelMode();
	  break;
	case CMD_DISARM_SECURITY   :   // [START][103][END]
          disarmSecurity();
	  break;
	case CMD_TRIGGER_ALARM     :   // [START][104][END]
          alarmTriggered();
	  break;
	case CMD_TURN_ON_LIGTH     :   // [START][105][ID][END]
          if(serialBufferIndex>0 && serialBuffer[1] < LIGTHS)           
            shortTimeOn(serialBuffer[1]);        
	  break;
	case CMD_TURN_ON_LIGTHS    :   // [START][106][END]
          turnOnLigths();
	  break;
	case CMD_TURN_OFF_LIGTH    :   // [START][107][ID][END]
          if(serialBufferIndex>0 && serialBuffer[1] < LIGTHS)           
            turnOffLigth(serialBuffer[1]);          
	  break;
	case CMD_TURN_OFF_LIGTHS   :   // [START][108][END]
          turnOffLigths();
          break;          
	case CMD_DISARM_ALARM_CLOCK:   // [START][109][END]
          alarmClockActivated=false;
          break;          
          
        default: 
          break;      
      }
      
      //SREG = oldSREG;      
      serialBufferIndex=-1;
    }       
    prevSerialValue = data;
    serialBufferIndex=(serialBufferIndex+1)%8;
  }
}
