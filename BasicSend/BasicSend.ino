#include <IridiumSBD.h>
#include <SoftwareSerial.h>

SoftwareSerial nss(2, 3);
IridiumSBD isbd(nss, 10);
static const int ledPin = 13;
int err = 0;

// constants won't change. They're used here to 
// set pin numbers:
const int SentButtonPin = 6;     // the number of the sent pin
const int SleepButtonPin = 5;     // the number of the sleep pin
int err = 0;
int sbd_sleep = 0;
void setup()
{
  int signalQuality = -1;

  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);
  nss.begin(19200);

  isbd.attachConsole(Serial);
  isbd.attachDiags(Serial);
  isbd.setPowerProfile(0);
 

  
}

sent()
{
   isbd.begin();
  isbd.useMSSTMWorkaround(false);

  err = isbd.getSignalQuality(signalQuality);
  if (err != 0)
  {
    Serial.print("SignalQuality failed: error ");
    Serial.println(err);
    return;
  }

  Serial.print("Signal quality is ");
  Serial.println(signalQuality);
  err = isbd.sendSBDText("Hello, world!");
  if (err != 0)
  {
    Serial.print("sendSBDText failed: error ");
    Serial.println(err);
    return;
  }

  Serial.println("Hey, it worked!");
  Serial.print("Messages left: ");
  Serial.println(isbd.getWaitingMessageCount()); 
}
void loop()
{
  if ( (0 == digitalRead(SentButtonPin)) && 
       (0 == sbd_sleep) {     
    // turn LED on:    
    digitalWrite(ledPin, HIGH); 
    sbd_sleep = 1;
    sent(); 
  } 
  
  // check and force sleep
   if ( 0 == digitalRead(SleepButtonPin) )
   {     
    // turn LED on:    
    digitalWrite(ledPin, HIGH); 
    isbd.sleep();
    sbd_sleep = 0;
  }  
  

 // if it is, the buttonState is HIGH:
  if (0 == digitalRead(SentButtonPin)) {     
    // turn LED on:    
    digitalWrite(ledPin, HIGH); 
    sent(); 
  }   
     
}

bool ISBDCallback()
{
   digitalWrite(ledPin, (millis() / 1000) % 2 == 1 ? HIGH : LOW);
   return true;
}
