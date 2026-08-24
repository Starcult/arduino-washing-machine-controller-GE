#include <Timer.h>
#include <LiquidCrystal.h>
/*  
  This section of #define's as well as function read_LCD_buttons is copied (with one small change) from example
  available here: http://www.dfrobot.com/wiki/index.php?title=Arduino_LCD_KeyPad_Shield_(SKU:_DFR0009)
*/
#define SHORT_PRESS_TIME 2000 // 1000 milliseconds
#define LONG_PRESS_TIME  500
#define ModeOff 0
#define ModeFilling 1
#define ModeWash 2
#define ModeRinse 3
#define ModeSpin 4
#define ModeDrain 5
#define ModeSoak 6
#define ModeNext 7
#define ModeFinish 8
#define ModeFilled 9
#define ModeChkWaterLow 10
#define StatusRun 1
#define StatusPause 2
#define StatusStop 3
//#define motorPowerPin A1 //R1
//#define motorDirectionPins1 A2 //R2 
//#define motorDirectionPins2 A3 //R3
#define motorWashPin A2
#define motorSpinPin A3
#define drainPumpPin A0 //R4
//#define coldValvePin A5 //R5
#define fillValvePin A1 //R6
#define WaterLevelLowPin  A4 
#define WaterLevelHighPin A5
#define PowerLed 3 
#define PowerBtn 2
#define StartBtn 10
#define StartLed 11
//#define StartLed 5
//#define WashLed  9
//#define RinseLed 11
//#define SpinLed  7
//#define StartBtn  4
//#define WashBtn 8
//#define RinseBtn 10 
//#define SpinBtn 6
LiquidCrystal LCD(8, 9, 4, 5, 6, 7);
// LCD = new LiquidCrystal(8, 9, 4, 5, 6, 7);
#define BLINK_INTERVAL  500 
#define FILLS_TIMEOUT 360000 // 6 mins
#define WASH_INTERVAL 480000 // 8 mins (x2)
#define SOAK_INTERVAL 120000 // 2 mins
#define RINSE_INTERVAL 480000 // 8 mins
#define DRAIN_INTERVAL 120000 // 2 mins
#define TEMP_SPIN_INTERVAL 120000 // 2 mins
#define SPIN_INTERVAL 480000 //8 mins
#define Relay_ON LOW
#define Relay_OFF HIGH
// Variables will change:
int Status = 0;
int PowerBtnlastState = HIGH;  // the previous state from the input pin
int PowerBtncurrentState;     // the current reading from the input pin
int StartBtnlastState = HIGH;  // the previous state from the input pin
int StartBtncurrentState;     // the current reading from the input pin
int WashBtnlastState = HIGH;  // the previous state from the input pin
int WashBtncurrentState;     // the current reading from the input pin
int RinseBtnlastState = HIGH;  // the previous state from the input pin
int RinseBtncurrentState;     // the current reading from the input pin
int SpinBtnlastState = HIGH;  // the previous state from the input pin
int SpinBtncurrentState;     // the current reading from the input pin
unsigned long previousMillis = 0; // will store last time LED was updated
int ledState = LOW;  // ledState used to set the LED
int ModeSelected = ModeOff;
int NextStep = ModeOff;
unsigned long pressedTime  = 0;
unsigned long releasedTime = 0;
unsigned long holdingTime = 0;
Timer RunningTimer;
Timer TotalTimer;
//Timer BtnTimer;
unsigned long StepTime = 0;
unsigned long CycleTime = 0;
bool timeout;
bool power;

void Wash() // WASH CYCLE
{
  unsigned long WashedTime = RunningTimer.read() - StepTime;
                //String Mode = "Washing";                 
              
                  //finish Fills
                //start Wash               
                switch(NextStep){
                  case ModeWash:
                          if (WashedTime >= WASH_INTERVAL){
                                  // Serial.println("WASH Mode - Soak");
                                  digitalWrite(motorWashPin, Relay_OFF);
                                  digitalWrite(motorSpinPin, Relay_OFF);
                                  StepTime = RunningTimer.read();
                                  NextStep = ModeSoak;
                           } else  if (Status == StatusPause){
                            // Serial.println("WASH Mode - Start Wash");
                           //  digitalWrite(coldValvePin, Relay_OFF);
                             delay(1000);
                             digitalWrite(fillValvePin, Relay_OFF);
                             //digitalWrite(motorDirectionPins1, Relay_OFF);
                             //digitalWrite(motorDirectionPins2, Relay_OFF);
                             //delay(1000);
                             digitalWrite(motorWashPin, Relay_ON);
                             digitalWrite(motorSpinPin, Relay_OFF);
                             }
                           break;
                  case ModeSoak:
                        if(WashedTime >= SOAK_INTERVAL){
                              // Serial.println("WASH Mode - Second Wash");
                               if(digitalRead(WaterLevelHighPin) == LOW){
                             //digitalWrite(motorDirectionPins1, Relay_OFF);
                             //digitalWrite(motorDirectionPins2, Relay_OFF);
                              digitalWrite(motorWashPin, Relay_ON);
                              digitalWrite(motorSpinPin, Relay_OFF);
                              StepTime = RunningTimer.read();
                              NextStep = ModeDrain;
                              }
                            } else  if (Status == StatusPause){
                              // Serial.println("WASH Mode - Soak");
                                  digitalWrite(motorWashPin, Relay_OFF);                             
                              }
                            break;
                  case ModeDrain:
                         if (WashedTime >= WASH_INTERVAL){
                             // Serial.println("WASH Mode - Drain");
                             digitalWrite(motorSpinPin, Relay_OFF);
                              digitalWrite(motorWashPin, Relay_OFF);
                              digitalWrite(drainPumpPin, Relay_ON);
                              StepTime = RunningTimer.read();
                              NextStep = ModeChkWaterLow; 
                              }
                           else  if (Status == StatusPause){                          
                            // Serial.println("WASH Mode - Second Wash");
                               if(digitalRead(WaterLevelHighPin) == LOW){
                            // digitalWrite(motorDirectionPins1, Relay_OFF);
                           //  digitalWrite(motorDirectionPins2, Relay_OFF);
                             delay(1000);
                              digitalWrite(motorWashPin, Relay_ON);
                              digitalWrite(motorSpinPin, Relay_OFF);                           
                            }
                           }
                              break;
                  case ModeSpin:
                          if (WashedTime >= DRAIN_INTERVAL){
                             // Serial.println("WASH Mode - Spin");
                             digitalWrite(motorSpinPin, Relay_OFF);
                              digitalWrite(motorWashPin, Relay_OFF);
                              delay(500);
                              digitalWrite(drainPumpPin,Relay_ON);
                              delay(500);
                             // digitalWrite(motorDirectionPins1, Relay_ON);
                            // digitalWrite(motorDirectionPins2, Relay_ON);
                             // delay(1000);
                             digitalWrite(motorSpinPin, Relay_ON);
                              digitalWrite(motorWashPin, Relay_OFF);
                              StepTime = RunningTimer.read();
                              NextStep = ModeNext;
                               }else  if (Status == StatusPause){
                              // Serial.println("WASH Mode - Drain");
                             digitalWrite(motorSpinPin, Relay_OFF);
                              digitalWrite(motorWashPin, Relay_OFF);
                              digitalWrite(drainPumpPin, Relay_ON);  
                                }
                               
                              break;

                      case ModeChkWaterLow:
                            if (digitalRead(WaterLevelLowPin) == LOW){
                              StepTime = RunningTimer.read();
                              NextStep = ModeSpin;
                              }else  if (Status == StatusPause){                           
                           // Serial.println("SPIN Mode -- DRIAN only");
                             digitalWrite(motorSpinPin, Relay_OFF);
                              digitalWrite(motorWashPin, Relay_OFF);
                            digitalWrite(drainPumpPin, Relay_ON);  
                            }                
                           break;
                           
                  case ModeNext:
                           if (WashedTime >= TEMP_SPIN_INTERVAL){
                              OFFMode();
                              StepTime = RunningTimer.read();
                               NextStep = ModeOff;
                               ModeSelected = ModeRinse;
                               }else  if (Status == StatusPause){
                             // Serial.println("WASH Mode - Spin");
                              digitalWrite(motorSpinPin, Relay_OFF);
                              digitalWrite(motorWashPin, Relay_OFF);
                              delay(500);
                              digitalWrite(drainPumpPin,Relay_ON);
                              delay(500);
                          //    digitalWrite(motorDirectionPins1, Relay_ON);
                          //    digitalWrite(motorDirectionPins2, Relay_ON);
                           //   delay(1000);
                              digitalWrite(motorSpinPin, Relay_ON);
                              digitalWrite(motorWashPin, Relay_OFF);    
                                }
                               break;
                 
                  default :
                            if(digitalRead(WaterLevelHighPin) == LOW){
                              delay(4000);
                                if(digitalRead(WaterLevelHighPin) == LOW){
                            // Serial.println("WASH Mode - Start Wash");
                             StepTime = RunningTimer.read();
                         //    digitalWrite(coldValvePin, Relay_OFF);
                             delay(1000);
                             digitalWrite(fillValvePin, Relay_OFF);
                           //  digitalWrite(motorDirectionPins1, Relay_OFF);
                           //  digitalWrite(motorDirectionPins2, Relay_OFF);
                             delay(1000);
                              digitalWrite(motorWashPin, Relay_ON);
                              digitalWrite(motorSpinPin, Relay_OFF); 
                             NextStep = ModeWash;
                             }else {WaterFills();}
                             }else {WaterFills();}
                             
                           break;                                   
                  }
                
                  }



void Rinse() // Rinse CYCLE
{
   unsigned long RinsedTime = RunningTimer.read() - StepTime;
  //String Mode = "Washing";

 switch(NextStep){
                 case ModeNext:
                           if (RinsedTime >= RINSE_INTERVAL){
                              OFFMode();
                               StepTime = RunningTimer.read();
                               NextStep = ModeOff;
                               ModeSelected = ModeSpin;
                               }else  if (Status == StatusPause){
                                  // Serial.println("Rinse Mode - Rinse");
                               if(digitalRead(WaterLevelHighPin) == LOW){
                        //     digitalWrite(coldValvePin, Relay_OFF);                       
                             digitalWrite(fillValvePin, Relay_OFF);
                             delay(1000);
                                digitalWrite(motorWashPin, Relay_ON);
                              digitalWrite(motorSpinPin, Relay_OFF); 
                               //   digitalWrite(motorDirectionPins1, Relay_OFF);
                                // digitalWrite(motorDirectionPins2, Relay_OFF);
                               //  delay(1000);
                                //  digitalWrite(motorPowerPin, Relay_ON);                           
                            }     
                                }
                               break;
                 default :
                            if(digitalRead(WaterLevelHighPin) == LOW){
                               delay(4000);
                                if(digitalRead(WaterLevelHighPin) == LOW){
                             // Serial.println("Rinse Mode - Rinse");
                             StepTime = RunningTimer.read();
                       //      digitalWrite(coldValvePin, Relay_OFF);
                             delay(1000);
                             digitalWrite(fillValvePin, Relay_OFF);
                           //  digitalWrite(motorDirectionPins1, Relay_OFF);
                           //  digitalWrite(motorDirectionPins2, Relay_OFF);
                            delay(1000);
                              digitalWrite(motorWashPin, Relay_ON);
                              digitalWrite(motorSpinPin, Relay_OFF); 
                             NextStep = ModeNext;
                             }else {WaterFills();}  
                             }else {WaterFills();}                           
                           break;   
  }


}

void Spin() {//Spin 
unsigned long SpinTime = RunningTimer.read() - StepTime;
// Drain
    switch(NextStep){
                 case ModeNext:
                      if (SpinTime >= SPIN_INTERVAL){                       
                        // Serial.println("SPIN Mode -- OFF");
                        NextStep = ModeFinish;
                        ModeSelected = ModeOff;
                        power = false;
                        RunningTimer.stop();
                        TotalTimer.stop();
                        LCD.clear();
                        LCD.print("Done");
                        OFFMode();
                       }else  if (Status == StatusPause){  
                         // Serial.println("SPIN Mode-- Start Spin");
                              digitalWrite(motorSpinPin, Relay_OFF);
                              digitalWrite(motorWashPin, Relay_OFF);
                              delay(500);
                              digitalWrite(drainPumpPin,Relay_ON);
                              delay(500);
                             // digitalWrite(motorDirectionPins1, Relay_ON);
                            //  digitalWrite(motorDirectionPins2, Relay_ON);
                              digitalWrite(motorWashPin, Relay_OFF);
                              digitalWrite(motorSpinPin, Relay_ON);          
                        }
                        break;

                 case ModeSpin:
                       if (SpinTime >= DRAIN_INTERVAL){
                             // Serial.println("SPIN Mode-- Start Spin");
                              digitalWrite(motorSpinPin, Relay_OFF);
                              digitalWrite(motorWashPin, Relay_OFF);
                              delay(500);
                              digitalWrite(drainPumpPin,Relay_ON);
                              delay(500);
                              //digitalWrite(motorDirectionPins1, Relay_ON);
                            //  digitalWrite(motorDirectionPins2, Relay_ON);
                            //  delay(1000);
                              digitalWrite(motorWashPin, Relay_OFF);
                              digitalWrite(motorSpinPin, Relay_ON);    
                              StepTime = RunningTimer.read();
                              NextStep = ModeNext;
                               } else  if (Status == StatusPause){                           
                           // Serial.println("SPIN Mode -- DRIAN only");
                              digitalWrite(motorSpinPin, Relay_OFF);
                              digitalWrite(motorWashPin, Relay_OFF);
                            digitalWrite(drainPumpPin, Relay_ON);  
                            }
                              break;
                     case ModeChkWaterLow:
                            if (digitalRead(WaterLevelLowPin) == LOW){
                              StepTime = RunningTimer.read();
                              NextStep = ModeSpin;
                              }else  if (Status == StatusPause){                           
                           // Serial.println("SPIN Mode -- DRIAN only");
                              digitalWrite(motorSpinPin, Relay_OFF);
                              digitalWrite(motorWashPin, Relay_OFF);
                            digitalWrite(drainPumpPin, Relay_ON);  
                            }                
                           break;
               default:
                          // Serial.println("SPIN Mode -- DRIAN only");
                              digitalWrite(motorSpinPin, Relay_OFF);
                              digitalWrite(motorWashPin, Relay_OFF);
                            digitalWrite(drainPumpPin, Relay_ON);
                            StepTime = RunningTimer.read();
                            NextStep = ModeChkWaterLow;
                            break;
    }
}


void WaterFills()
{         
    unsigned long FilledTime = RunningTimer.read() - StepTime;
   // Serial.println("Filling Water");

   switch(NextStep){
    
    case ModeFilling:
            if(FilledTime >= FILLS_TIMEOUT){
              // Serial.println("Water Fill Time out");
                        NextStep = ModeFinish;
                   //     ModeSelected = ModeOff;
                        power = false;
                        RunningTimer.stop();
                        TotalTimer.stop();
                 //       LCD.clear();
                  //      LCD.print("Fill - Time out");
                  //      OFFMode();
                }else   if (Status == StatusPause){
                       digitalWrite(fillValvePin, Relay_ON);
                      // digitalWrite(coldValvePin, Relay_ON);        
                  }
          break;
          
    
    default:
          if (digitalRead(WaterLevelLowPin) == LOW){
         // Serial.println("Filling Water pin - HIGH");
            digitalWrite(fillValvePin, Relay_ON);
       //     digitalWrite(coldValvePin, Relay_ON);   
            StepTime = RunningTimer.read();
            NextStep = ModeFilling;
        }
        break;
    
    }
}


void OFFMode() 
{
 // Serial.println("Turning Off Machine");
 digitalWrite(motorSpinPin, Relay_OFF);
 digitalWrite(motorWashPin, Relay_OFF);
  digitalWrite(drainPumpPin, Relay_OFF);
  //digitalWrite(coldValvePin, Relay_OFF);
  delay(1000);
  digitalWrite(fillValvePin, Relay_OFF);
  //delay(1000);
  //digitalWrite(motorDirectionPins1, Relay_OFF);
 // digitalWrite(motorDirectionPins2, Relay_OFF);
}


void setup()
{
// Serial.begin(115200);
  timeout = false;
  power = false;
  LCD.begin(16, 2);
  LCD.clear();
  LCD.print("OFF");
  pinMode(motorWashPin, OUTPUT);
    pinMode(motorSpinPin, OUTPUT);
//  pinMode(motorDirectionPins1, OUTPUT);
//  pinMode(motorDirectionPins2, OUTPUT);
  pinMode(drainPumpPin, OUTPUT);
 // pinMode(coldValvePin, OUTPUT);
  pinMode(fillValvePin, OUTPUT);
  pinMode(WaterLevelLowPin, INPUT_PULLUP);
  pinMode(WaterLevelHighPin, INPUT_PULLUP);
  pinMode(PowerBtn, INPUT_PULLUP);
  pinMode(StartBtn, INPUT_PULLUP);
 // pinMode(WashBtn, INPUT_PULLUP);
//  pinMode(RinseBtn, INPUT_PULLUP);
//  pinMode(SpinBtn, INPUT_PULLUP);
  pinMode(PowerLed, OUTPUT);
  pinMode(StartLed, OUTPUT);
 // pinMode(WashLed, OUTPUT);
//  pinMode(RinseLed, OUTPUT);
//  pinMode(SpinLed, OUTPUT);
  digitalWrite(PowerLed, HIGH);
  digitalWrite(StartLed, HIGH);
 // digitalWrite(WashLed, HIGH); //ledState
//  digitalWrite(RinseLed, HIGH);
//  digitalWrite(SpinLed, HIGH);

 digitalWrite(motorSpinPin, Relay_OFF);
 digitalWrite(motorWashPin, Relay_OFF);
 // digitalWrite(motorDirectionPins1, Relay_OFF);
 // digitalWrite(motorDirectionPins2, Relay_OFF);
  digitalWrite(drainPumpPin, Relay_OFF);
  //digitalWrite(coldValvePin, Relay_OFF);
  digitalWrite(fillValvePin, Relay_OFF);


}

void detectPowerBtn()
{
  
  PowerBtncurrentState = digitalRead(PowerBtn);
  if (PowerBtncurrentState == LOW){
       delay(100);       
       PowerBtncurrentState = digitalRead(PowerBtn);
        if (PowerBtncurrentState == LOW){    
    if (power == true){
         power = false;
         StepTime = 0;
         RunningTimer.stop();
         TotalTimer.stop();
         LCD.clear();
        LCD.print("OFF");
          CycleTime = 0;
          ModeSelected == ModeOff;
          NextStep = ModeOff;
          Status = 0;
         OFFMode();
      
      }else{
      switch(ModeSelected)
     {
      case ModeWash :
        LCD.setCursor(0,0);
         LCD.clear();
        LCD.print("-> RINSE 18mins");
         ModeSelected = ModeRinse;
        break;

      case ModeRinse :
         LCD.setCursor(0,0);
        LCD.clear();
         LCD.print("-> Spin 10mins");
         ModeSelected = ModeSpin;
        break;

      case ModeSpin :
         LCD.setCursor(0,0);
         LCD.clear();
         LCD.print("-> OFF ");
         ModeSelected = ModeOff;
         break;

      default :
         LCD.setCursor(0,0);
         LCD.clear();
         LCD.print("-> WASH 45mins");
         ModeSelected = ModeWash;
         break;
    }
  }
  }
  }
  /*   if (power){
         power = false;
         StepTime = 0;
         RunningTimer.stop();
         OFFMode();
       // Serial.println("Power Off");     
      }
      else{
          power = true;
          RunningTimer.stop();
         // Serial.println("Power On"); 
        }
    }
    */
    
 //  PowerBtnlastState = PowerBtncurrentState;
}
void detectStartBtn()
{
  
  StartBtncurrentState = digitalRead(StartBtn);
  if ( StartBtncurrentState == LOW){
    delay(100);
      StartBtncurrentState = digitalRead(StartBtn);
     if (StartBtncurrentState == LOW){
   if (ModeSelected == ModeOff){
         power = false;
         StepTime = 0;
         RunningTimer.stop();
         TotalTimer.stop();
        LCD.clear();
         LCD.print("OFF");
          CycleTime = 0;
          ModeSelected == ModeOff;
          NextStep = ModeOff;
          Status = 0;
         OFFMode();
   } else{
     power = true;
       switch(RunningTimer.state()){
           case RUNNING:
                RunningTimer.pause();
                 TotalTimer.pause();
                // Serial.println("Running Timer Paused");
                 break;
           case STOPPED:
                RunningTimer.start();
                 TotalTimer.start();
               // Serial.println("Running Timer Started");
                 break;      
           case PAUSED:
                RunningTimer.resume();
                TotalTimer.resume();
              // Serial.println("Running Timer Resumed");
                 break; 
        }
    }
}
  }
 /*
  //if (StartBtnlastState == HIGH && StartBtncurrentState == LOW){       // button is pressed
   // pressedTime = millis();
 //   if (holdingTime == 0)
  //    holdingTime = millis();
  //  long pressholdDuration = pressedTime - holdingTime;
  // // Serial.println(pressholdDuration);
 //if(pressholdDuration>LONG_PRESS_TIME){
   //   // Serial.println("A StartBtn press is detected");
 //         power = false;
 //         StepTime = 0;
 //         RunningTimer.stop();
//Serial.println("Power Off");
 //         holdingTime = 0;
 //   } else 
 //         holdingTime = millis();
 //   }
 // else if (StartBtnlastState == LOW && StartBtncurrentState == HIGH) { // button is released
 //   delay(200);
 //   releasedTime = millis();
//    long pressDuration = releasedTime - pressedTime;
//    if ( pressDuration < SHORT_PRESS_TIME ){
 //    // Serial.println("A StartBtn short press is detected");
 //       if (power == true){
 //         switch(RunningTimer.state()){
 //          case RUNNING:
 //               RunningTimer.pause();
  //              // Serial.println("Running Timer Paused");
  ///               break;
   //        case STOPPED:
  //              RunningTimer.start();
 //              // Serial.println("Running Timer Started");
 //                break;      
 //          case PAUSED:
 //               RunningTimer.resume();
//               // Serial.println("Running Timer Resumed");
 //                break; 
            }
        } else {
          power = true;
          RunningTimer.stop();
         // Serial.println("Power On");
        }
         holdingTime = 0;
  }
 
  }
  
  // save the the last state
}
*/
//StartBtnlastState = StartBtncurrentState;
}

/*
void detectWashBtn()
{
  WashBtncurrentState = digitalRead(WashBtn);
if (WashBtnlastState == LOW && WashBtncurrentState == HIGH) { // button is released
  delay(500);
    ModeSelected = ModeWash;
     // Serial.println("A WashBtn is detected");
  }
  WashBtnlastState = WashBtncurrentState;
  // save the the last state
}
void detectRinseBtn()
{
  RinseBtncurrentState = digitalRead(RinseBtn);
if (RinseBtnlastState == LOW && RinseBtncurrentState == HIGH) { // button is released
 delay(500);
    ModeSelected = ModeRinse;
     // Serial.println("A RinseBtn is detected");
  }
  RinseBtnlastState = RinseBtncurrentState;
  // save the the last state
}
void detectSpinBtn()
{
  SpinBtncurrentState = digitalRead(SpinBtn);
if (SpinBtnlastState == LOW && SpinBtncurrentState == HIGH) { // button is released
 delay(500);
    ModeSelected = ModeSpin;
     // Serial.println("A SpinBtn is detected");
  }
  SpinBtnlastState = SpinBtncurrentState;
  // save the the last state
}
*/
void updateLCD(){
unsigned long LCDUpdateTime = TotalTimer.read() - CycleTime;

if (power == true){
   digitalWrite(PowerLed, LOW);
  if (RunningTimer.state() == RUNNING){
   digitalWrite(StartLed, LOW);
 if (LCDUpdateTime >= 1000){
      switch(ModeSelected)
     {
      case ModeWash :
         LCD.setCursor(0,0);
         LCD.clear();
         LCD.print("WASH 45mins");
        break;

      case ModeRinse :
         LCD.setCursor(0,0);
         LCD.clear();
         LCD.print("RINSE 18mins");
        
        break;

      case ModeSpin :
         LCD.setCursor(0,0);
         LCD.clear();
         LCD.print("Spin 10mins");
         break;

      default :
         LCD.setCursor(0,0);
         LCD.clear();
         LCD.print("OFF ");
         break;
    }
      switch(NextStep){
                  case ModeWash:
                           LCD.setCursor(0,1);                        
                           LCD.print("Washing..");
                           break;
                  case ModeSoak:
                           LCD.setCursor(0,1);                        
                           LCD.print("Soak..");
                           break;
                  case ModeDrain:
                           LCD.setCursor(0,1);                        
                           LCD.print("Drain..");
                           break;                  
                  case ModeSpin:
                           LCD.setCursor(0,1);                        
                           LCD.print("Spin..");
                           break;                          
                  case ModeChkWaterLow:
                           LCD.setCursor(0,1);                        
                           LCD.print("Drain..");
                           break;                    
                  case ModeNext:
                           LCD.setCursor(0,1);                        
                           LCD.print("Finshing...");
                           break;  
                  case ModeFilling:
                           LCD.setCursor(0,1);                        
                           LCD.print("Filling...");
                           break;                             
                  default :
                           LCD.setCursor(0,1);                        
                           LCD.print(".....");
                           break;  
        }
   
   timerTOStr();
   CycleTime = TotalTimer.read();
     } 
      }
   else{
    digitalWrite(StartLed, ledState);
   }

    if (RunningTimer.state() == PAUSED){
       LCD.setCursor(0,1);
       LCD.print("PAUSED ");
      }
    }
    else {
         digitalWrite(PowerLed, HIGH);
            digitalWrite(StartLed, HIGH);
      }  
}

/*
  switch(ModeSelected)
    {
      case ModeWash :
      
      if (RunningTimer.state() == STOPPED){       
        digitalWrite(WashLed, LOW); //ledState
        digitalWrite(RinseLed, LOW);
        digitalWrite(SpinLed, LOW);
      } else {
         LCD.setCursor(0,0);
         LCD.clear();
          LCD.print("WASH 45mins");
        digitalWrite(WashLed, ledState); //ledState
        digitalWrite(RinseLed, LOW);
        digitalWrite(SpinLed, LOW);

      }
        break;

      case ModeRinse :
  if (RunningTimer.state() == STOPPED){
        digitalWrite(WashLed, HIGH); //ledState
        digitalWrite(RinseLed, LOW);
        digitalWrite(SpinLed, LOW);
      } else {
        digitalWrite(WashLed, HIGH); //ledState
        digitalWrite(RinseLed, ledState);
        digitalWrite(SpinLed, LOW);
      }
     
        break;
      case ModeSpin :
        if (RunningTimer.state() == STOPPED){
            digitalWrite(WashLed, HIGH); //ledState
        digitalWrite(RinseLed, HIGH);
        digitalWrite(SpinLed, LOW);
        }else {
        digitalWrite(WashLed, HIGH); //ledState
        digitalWrite(RinseLed, HIGH);
        digitalWrite(SpinLed, ledState);
        }
         break;
    }
}
else {
        digitalWrite(PowerLed, HIGH);
        digitalWrite(StartLed, HIGH);
       // digitalWrite(WashLed, HIGH); //ledState
       // digitalWrite(RinseLed, HIGH);
       // digitalWrite(SpinLed, HIGH);
}*/

void timerTOStr(){
  int Minutes = 0;
  int Seconds = 0;
  int buttonState = 0;
  unsigned long allSeconds ;
  int secsRemaining;
  char buf[5];
  char Displaymsg[15];
    allSeconds = TotalTimer.read() / 1000;
    secsRemaining=allSeconds%3600;
    Minutes=secsRemaining/60;
    Seconds=secsRemaining%60;
    sprintf(buf,"%02d:%02d",Minutes,Seconds);
    LCD.setCursor(11,1);
    LCD.print(buf);
  }

  
void loop()
{
  if (power== false)
  {// ModeSelected = ModeWash;
  //  NextStep = ModeOff;
    StepTime = 0;
    }
unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= BLINK_INTERVAL) {
   ledState = (ledState == LOW) ? HIGH : LOW;
   previousMillis = currentMillis;
   }
  detectPowerBtn();
  detectStartBtn(); 
  updateLCD();
if (power == true && RunningTimer.state() == STOPPED){
//  detectWashBtn();
  //detectRinseBtn();
//  detectSpinBtn();
  ModeSelected == ModeOff;
  NextStep = ModeOff;
  StepTime = 0;
  Status = StatusStop;
}
if (power == true && RunningTimer.state() == RUNNING){

switch(ModeSelected)
    {
      case ModeWash :
        Wash();
        break;

      case ModeRinse :
        Rinse();
        break;

      case ModeSpin :
          Spin();
         break;

    }
Status = StatusRun;
}

  if (power == true && RunningTimer.state()== PAUSED){
    if(Status != StatusPause){
  switch(ModeSelected)
    {
      case ModeWash :
        OFFMode();
        break;

      case ModeRinse :
        OFFMode();
     
        break;
      case ModeSpin :
       OFFMode();
         break;
    }
    }
    Status = StatusPause;
  }

   
}