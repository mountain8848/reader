#include <IridiumSBD.h>
#include <SoftwareSerial.h>

SoftwareSerial nss(2, 3);
IridiumSBD isbd(nss, 4);
static const int ledPin = 13;


// constants won't change. They're used here to 
// set pin numbers:
const int SentButtonPin = 6;     // the number of the sent pin
const int SleepButtonPin = 5;     // the number of the sleep pin
int err = 0;
int sbd_work = 0;

bool ISBDCallback()
{
   digitalWrite(ledPin, (millis() / 1000) % 2 == 1 ? HIGH : LOW);
   return true;
}

void my_sbd_sent(void)
{
  int signalQuality = -1;
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

void setup()
{
  

  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(SentButtonPin, INPUT_PULLUP);   
  // initialize the pushbutton pin as an input:
  pinMode(SleepButtonPin, INPUT_PULLUP);     
  
  Serial.begin(115200);
  nss.begin(19200);

  isbd.attachConsole(Serial);
  isbd.attachDiags(Serial);
  isbd.setPowerProfile(0);
 

  
}



///---------------------------------
void loop()
{
  
  if ( (LOW == digitalRead(SentButtonPin)) && 
       (0 == sbd_work)) 
  {   
    Serial.println("[NOTE] sent......");
    // turn LED on:    
    digitalWrite(ledPin, HIGH); 
    sbd_work = 1;
    my_sbd_sent(); 
  } 
  
  // check and force sleep
   if ( LOW == digitalRead(SleepButtonPin) &&
        1 == sbd_work)
   {
     Serial.println("[NOTE] sleep......");     
    // turn LED on:    
    digitalWrite(ledPin, HIGH); 
    isbd.sleep();
    sbd_work = 0;
  }  
  
    
}


