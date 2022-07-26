#include <Arduino_FreeRTOS.h>
#include <semphr.h>

SemaphoreHandle_t xSerialSemaphore;

QueueHandle_t structQueue;

void TaskReadTemperature(void *pvParameters);
void TaskProcessTemperature(void *pvParameters);
void TaskTemperatureAvarege(void *pvParameters);
void TaskDisplayStatus(void *pvParameters);

void setup()
{
  Serial.begin(9600); /// Configura o Baud Rate em 9600.
  Serial.println("Starting process!");

  pinMode(LED_BUILTIN, OUTPUT); /// Configura o pino LED_BUILTIN como saída.

  /// Fonte: https://create.arduino.cc/projecthub/feilipu/using-freertos-semaphores-in-arduino-ide-b3cd6c
  if (xSerialSemaphore == NULL) { /// Verifica se o SerialSemaphore ainda não foi criado.
    xSerialSemaphore = xSemaphoreCreateMutex(); /// Cria a mutex que controla a porta serial.
  } else {
    xSemaphoreGive(xSerialSemaphore); /// Torna a porta serial disponível, liberando o semáforo.
  }

  structQueue = xQueueCreate(10, sizeof(struct DHTTemperature)); /// Cria a Queue para envio de dados do sensor.

  if (structQueue != NULL) { /// Checa se structQueue é diferente de nulo para criar as tasks.
    xTaskCreate(TaskReadTemperature, "ReadTemperature", 128, NULL, 2, NULL); /// Cria task responsável por obter a temperatura.
    xTaskCreate(TaskProcessTemperature, "ProcessTemperature", 128, NULL, 2, NULL); /// Cria task responsável por inserir a temperatura obtida no array temperatures e mostrar no Serial a temperatura atual.
    xTaskCreate(TaskTemperatureAvarege, "TemperatureAvarege", 128, NULL, 2, NULL); /// Cria task responsável por realizar a média das temepraturas.
    xTaskCreate(TaskDisplayStatus, "DisplayStatus", 128, NULL, 2, NULL); /// Cria task responsável informar o status do ar-condicionado estar refrigerando.
  }
}

void loop() { }

///*--------------------------------------------------*/
///*---------------------- Tasks ---------------------*/
///*--------------------------------------------------*/

void TaskReadTemperature(void *pvParameters __attribute__((unused))) {
  while (true) {
  }
}

void TaskProcessTemperature(void *pvParameters __attribute__((unused))) {
  while (true) {
  }
}

void TaskTemperatureAvarege(void *pvParameters __attribute__((unused))) {
  while (true) {
  }
}

void TaskDisplayStatus(void *pvParameters __attribute__((unused))) {
  while (true) {
  }
}
