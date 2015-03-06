#define LED          13
#define TRIGGEROUT   8
#define DELAYPOT     A0

#define DELAYRANGE0  0
#define DELAYRANGE1  256
#define DELAYRANGE2  512
#define DELAYRANGE3  768
#define DELAYRANGE4  1024

#define DELAY01      5000
#define DELAY12      7000
#define DELAY23      9000
#define DELAY34      11000

uint16_t triggerDelay;

uint32_t ledTimeout;
#define  LEDBLINKINTERVAL 2000

void setup()
{
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(TRIGGEROUT, OUTPUT);
  pinMode(TRIGGEROUT, OUTPUT);
  pinMode(DELAYPOT,    INPUT);

  for (int i = 0; i < 5; i++) {
    digitalWrite(LED, HIGH); // indicate operation
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
  }
  digitalWrite(TRIGGEROUT, LOW);

  attachInterrupt(0, triggerHandler, FALLING); // int 0 in D2
  ledTimeout = 0;
  triggerDelay = DELAY23;
  Serial.println("FlashTriggerSourceInterruptFast begin");
}

void loop()
{
  if (millis() > ledTimeout) {
    uint16_t adc;
    digitalWrite(LED, HIGH); // indicate operation
    delay(50);
    digitalWrite(LED, LOW);

    adc = analogRead(DELAYPOT);

    if (adc > DELAYRANGE2) {
      if (adc > DELAYRANGE3) {
        triggerDelay = DELAY34;
      } 
      else {
        triggerDelay = DELAY23;
      } 
    }
    else {
      if (adc > DELAYRANGE1) {
        triggerDelay = DELAY12;
      } 
      else {
        triggerDelay = DELAY01;
      }
    }
    Serial.print(adc);
    Serial.print(" ");
    Serial.println(triggerDelay);
    ledTimeout = millis() + LEDBLINKINTERVAL;
  }
}


// from http://www.arduino.cc/en/Reference/PortManipulation
void triggerHandler()
{
  delayMicroseconds(triggerDelay);
  PORTB = B00000001; // port B bit 0 is D8
    delayMicroseconds(10000); // can't use delay() in interrupts 
  PORTB = B00000000; 
  
  // grossly violating the most sacred rule of interrupt handlers, be fast.
  for (int i = 0; i < 50; i++) {
    digitalWrite(LED, HIGH);  // indicate trigger happened
    delayMicroseconds(15000); // can't use delay() in interrupts 
    delayMicroseconds(15000); // can't set delayMicroseconds to greater than 16000
    delayMicroseconds(15000); // need multiples to see the led
    digitalWrite(LED, LOW);
    delayMicroseconds(15000);
    delayMicroseconds(15000);
    delayMicroseconds(15000);
  }
  digitalWrite(TRIGGEROUT, LOW);
}

