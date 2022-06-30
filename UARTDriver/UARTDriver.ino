#include <UARTDriver.h>

int number = 0;
int testInteger;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  printf("number %d \n", number);
  number++;
  scanf("%d", &testInteger);
  printf("testInteger %d \n", testInteger);
  delay(1000);
}
