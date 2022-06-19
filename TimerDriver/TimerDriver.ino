#include <TimerDriver.h>

const int led = LED_BUILTIN;
int ledState = LOW;
volatile unsigned long blinkCount = 0;
int seconds = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  Timer1.initialize(1000000); // microsegundos
  Timer1.attachInterrupt(blinkLED); // blinkLED to run every 1 seconds
  Serial.begin(9600);
}

void blinkLED(void)
{
  if (ledState == LOW) {
    ledState = HIGH;
    blinkCount = blinkCount + 1;
  } else {
    ledState = LOW;
  }

  seconds++;

  digitalWrite(led, ledState);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("seconds = ");
  Serial.println(seconds);
  delay(100);
}
