/* Misc */
#define tbi(port, bit)   (port) & (1 << (bit)) 
#define sbi(port, bit)   (port) |= (1 << (bit))
#define cbi(port, bit)   (port) &= ~(1 << (bit))

struct avrTime
{
  unsigned char hours;
  unsigned char minutes;
  unsigned char seconds;
};

/* Power constants */
#define VOLTAGE_SCALER      9    //110v -> 19v -> 5v
#define CURRENT_SCALER      4    //20mV/A   -> 5/1024 ~= 5mV
#define CURRENT_CENTER      512  //Value for 0 Amps

/* Size */
#define MOV_SENSORS     8
#define MAG_SENSORS     8
#define LIGTHS          8

/* Mode settings map */
#define sLEDS         0   //Set leds when SECURE MODE INIT
#define sTONE         1   //Play tone when SECURE MODE INIT
#define sLIGTHS_OFF   2   //Clear ligths when SECURE MODE INIT

#define sCALL         4   //Make call when ALARM TRIGGERED
#define sLIGTHS_ON    5   //Set ligths when ALARM TRIGGERED
#define sSIREN        6   //Play siren when ALARM TRIGGERED

/* General settings map */
#define sALARM_CLOCK_ON      0  //Flag position  for alarm clock 
#define sALARM_CLOCK_LIGTHS  1  //Flag position  for ligths ON 
#define sALARM_CLOCK_TONE    2  //Flag position  for siren 

#define sNIGTH_MANUAL        4  //Flag for nigth detection 

/* Reporting State map  */
#define rSECURED               0  //Flag position for secured state
#define rSECURED_MODE          1  //Flag position  for secured mode 0 (Travel) 1 (Nigth)
#define rALARM_ACTIVATED       2  //Flag position  for alarm activated
#define rIS_NIGTH              3  //Flag position  for nigth state 
#define rALARM_CLOCK_ACTIVATED 4  //Flag position  for alarm clock activated

#define rDATA_LENGTH           13 //XXX update when add new data to report frame

#define rREPORT_RESET  "xOpenDomoticResetx"

/* Protocol map */
// Protocol maximum frame : 8 Bytes -> 1KB
// Frame:
//       [START]  [COMMAND] [VALUE*]   [END]
//      [FF] [FF] [COMMAND] [VALUE*] [FF] [FF]

#define rMAX_FRAME_LENGTH     8     //Maximum length 1Byte

#define START                 0xFF  //Value for frame start
#define END                   0xFF  //Value for frame end

#define SET_NIGTH_MOV_MASK    01    // [START][01][VV][END]
#define SET_NIGTH_MAG_MASK    02    // [START][02][VV][END]
#define SET_NIGTH_LIG_MASK    03    // [START][03][VV][END]
#define SET_NIGTH_OTH_MASK    04    // [START][04][VV][END]

#define SET_TRAVEL_MOV_MASK   11    // [START][11][VV][END]
#define SET_TRAVEL_MAG_MASK   12    // [START][12][VV][END]
#define SET_TRAVEL_LIG_MASK   13    // [START][13][VV][END]
#define SET_TRAVEL_OTH_MASK   14    // [START][14][VV][END]

#define SET_CURRENT_TIME      20    // [START][20][HH][MM][SS][END]    
#define SET_NIGTH_INIT_TIME   21    // [START][21][HH][MM][SS][END]    
#define SET_NIGTH_END_TIME    22    // [START][22][HH][MM][SS][END]    
#define SET_ALARM_CLOCK_TIME  23    // [START][23][HH][MM][SS][END]    
#define SET_NORMAL_LIG_MASK   24    // [START][24][VV][END]    
#define SET_LIGTH_TIME_SHORT  25    // [START][25][ID][VV][END]    
#define SET_LIGTH_TIME_LONG   26    // [START][26][ID][VV][END]  

#define SET_GENERAL_MASK      28    // [START][28][VV][END] 
#define SET_IS_NIGTH          29    // [START][29][VV][END] 

#define CMD_ECHO_TEST         100   // [START][100][VV][END]
#define CMD_INIT_NIGTH_MODE   101   // [START][101][END]
#define CMD_INIT_TRAVEL_MODE  102   // [START][102][END]
#define CMD_DISARM_SECURITY   103   // [START][103][END]
#define CMD_TRIGGER_ALARM     104   // [START][104][END]
#define CMD_TURN_ON_LIGTH     105   // [START][105][ID][END]
#define CMD_TURN_ON_LIGTHS    106   // [START][106][END]
#define CMD_TURN_OFF_LIGTH    107   // [START][107][ID][END]
#define CMD_TURN_OFF_LIGTHS   108   // [START][108][END]
#define CMD_DISARM_ALARM_CLOCK 109  // [START][109][END]

/* Notes */                                 
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978                                  

//TODO Compose better melodies :D
const int sirenNotes1[13] = {   0       ,
                                  NOTE_C5 ,
                                  NOTE_CS5,
                                  NOTE_D5 ,
                                  NOTE_DS5,
                                  NOTE_E5 ,
                                  NOTE_F5 ,
                                  NOTE_FS5,
                                  NOTE_G5 ,
                                  NOTE_GS5,
                                  NOTE_A5 ,
                                  NOTE_AS5,
                                  NOTE_B5    };

const int sirenNotes2[4] = {      0 ,
                                  NOTE_A5 ,
                                  0 ,
                                  NOTE_A4 };

const int sirenNotes3[5] = {      0 ,
                                  NOTE_C5 ,
                                  0 ,
                                  NOTE_C7 ,
                                  0    };
