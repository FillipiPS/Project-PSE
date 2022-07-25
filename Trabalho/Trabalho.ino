#include <dht.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <queue.h>
#include <task.h>

#define DHT11_PIN 5 /// Leitura realizada pelo pino 5.

/// Estrutura utilizada para ler dados do sensor.
/// É necessária uma struct para criar a fila com xQueueCreate.
struct DHTTemperature {
  float value; /// Valor da temperatura.
};

dht DHT; /// Declaração do objeto dht utilizada para obter a temperatura do sensor DHT11.

/// Declaração da mutex Semaphore Handle que vai controlar a Serial Port.
/// Garante que apenas uma tarefa controla a serial a cada vez.
SemaphoreHandle_t xSerialSemaphore;

/// Vetor que guarda 10 dados lidos do sensor para ser calculada a média pela task TempMedia.
float temperatures[10];
bool isReadingFinished; /// Controla o buffer para cálculo da Média.
int temperaturesPosition; /// Contador de preenchimento do buffer da Média.
int i; /// Variável de controle do buzzer.
bool isChecked;
float avarage;

/// Handle da fila que a task AnalogRead envia dados lidos do sensor.
QueueHandle_t structQueue;

void TaskReadTemperature(void *pvParameters);
void TaskProcessTemperature(void *pvParameters);
void TaskTemperatureAvarege(void *pvParameters);
void TaskBuzzer(void *pvParameters);

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting process");
  pinMode(LED_BUILTIN, OUTPUT);

  if (xSerialSemaphore == NULL) { /// Verifica se o semáforo da porta serial já não foi criado.
    xSerialSemaphore = xSemaphoreCreateMutex(); /// Cria a mutex que controla a porta serial.
  } else {
    xSemaphoreGive(xSerialSemaphore); /// Torna a porta serial disponível, "dando" o semáforo.
  }

  /// Cria a fila de dados do sensor.
  structQueue = xQueueCreate(10, sizeof(struct DHTTemperature));

  if (structQueue != NULL) { /// Verifica se a fila foi criada.
    /// Cria tarefas que serão executadas independentemente.
    xTaskCreate(TaskProcessTemperature, "ProcessTemperature", 128, NULL, 2, NULL ); /// Cria a tarefa para consumir dados da fila.
    xTaskCreate(TaskTemperatureAvarege, "TempMedia", 128, NULL, 2, NULL ); /// Cria a tarefa para cálculo da média.
    xTaskCreate(TaskReadTemperature, "ReadTemperature", 128, NULL, 2, NULL); /// Cria a tarefa produtora de dados da fila.
    xTaskCreate(TaskBuzzer, "BuzzerTone", 128, NULL, 2, NULL); /// Cria a tarefa produtora de dados da fila.
  }
  /// Agora, o escalonador de tarefas, que assume o controle do escalonamento de tarefas individuais, é iniciado automaticamente.
}

void loop() { }

///*--------------------------------------------------*/
///*---------------------- Tasks ---------------------*/
///*--------------------------------------------------*/

void TaskReadTemperature(void *pvParameters __attribute__((unused))) /// Tarefa que lê dados do sensor.
{
  while (true) {
    struct DHTTemperature temperature;
    ///  Codificação dos valores lidos em tensão para temperatura.
    /// Fonte:
    int chk = DHT.read11(DHT11_PIN);
    // Serial.print("Temperature = ");
    // Serial.println(DHT.temperature);
    temperature.value = DHT.temperature;

    /// Posta um item na fila.
    /// https://www.freertos.org/a00117.html

    xQueueSend(structQueue, &temperature, portMAX_DELAY);
    vTaskDelay(2000 / portTICK_PERIOD_MS); /// Espera 2 segundos entre as leituras para estabilidade.
  }
}

void TaskProcessTemperature(void *pvParameters __attribute__((unused))) ///Tarefa que consome dado do buffer se disponível;
{
  while (true) {
    struct DHTTemperature currentPinRead;

    /// Read an item from a queue.
    /// https://www.freertos.org/a00118.html
    if (xQueueReceive(structQueue, &currentPinRead, portMAX_DELAY) == pdPASS) {
      temperatures[temperaturesPosition] = currentPinRead.value;
      if (temperaturesPosition < 10) { /// Verifica se ainda não foram armazenados 10 dados no buffer da média.
        i = temperaturesPosition; /// A variável de controle do buzzer recebe contador do buffer.
        isReadingFinished = false; /// Caso não, flag continua em 0.
        if (xSemaphoreTake(xSerialSemaphore, (TickType_t)5) == pdTRUE) {
          /// Se o semáforo estiver disponível, a tarefa consegue o controle da porta serial.
          Serial.print("Temp Atual: "); /// Comunica o valor lido da fila.
          Serial.println(currentPinRead.value);
          Serial.println(temperaturesPosition); /// Posição do buffer no momento.
          xSemaphoreGive(xSerialSemaphore); /// Libera a porta serial.
          temperaturesPosition = temperaturesPosition + 1; /// Incrementa a variável de controle do buffer.
        }
      } else {  /// Caso o contador atinja 10,
        i = 0;  /// reseta a variável de controle do buzzer para evitar leitura do buffer.
        isReadingFinished = true; /// Altera a flag e sinaliza que a média pode ser calculada.
      }
    }
  }
}

void TaskTemperatureAvarege(void *pvParameters __attribute__((unused))) /// Tarefa que consome o buffer para cálculo da média.
{
  while (true) {
    float media; /// Variável que guarda a média.
    float acumulado; /// Variável que guarda o acumulado dos dados do buffer.

    if (isReadingFinished) { /// Verifica se a flag foi alterada para 1.
      /// Executa laço para cálculo da média dos valores guardados no buffer.
      for (int index = 0; index < 10; index++) {
        acumulado += temperatures[index];
      }
      media = acumulado / 10;
      avarage = media;

      isReadingFinished = false; /// Reseta a flag para confirmar que os dados do buffer foram consumidos e podem ser substituidos.
      temperaturesPosition = 0;  /// Reseta variável de controle do buffer.

      if (xSemaphoreTake(xSerialSemaphore, (TickType_t)5) == pdTRUE) {
        /// Verifica se a porta serial está disponível.
        /// Caso obtenha o controle do semáforo,
        Serial.print("******Media: "); /// comunica o valor da média pela porta serial.
        Serial.println(media);
        media = 0; /// Reseta a variável da média.
        acumulado = 0; /// Reseta variável do acumulado.
        xSemaphoreGive(xSerialSemaphore); /// Libera a porta serial.
      }
    } else {
      isReadingFinished = false;  /// Se ainda não foram feitas 10 leituras, a média não será calculada.
      i = temperaturesPosition;
    }
  }
}

void TaskBuzzer(void *pvParameters __attribute__((unused)))///Tarefa que consome o buffer para cálculo da média.
{
  /// Fonte:http://www.squids.com.br/arduino/index.php/projetos-arduino/projetos-squids/basico/137-projeto-36-controlando-frequencia-de-um-buzzer-com-potenciometro
  while (true) {
    /*int frequency; ///Frequência tocada no buzzer.
      int atual; /// Variável para guardar o valor consumido do buffer.
      if (i > 0) {
      atual =  temperatures[i]; /// Consome dado do buffer.
      if (atual > 29) { /// Caso a temperatura atinja valor superior a 29, codifica os valores de temperatura
        frequency = map(atual, 30, 80, 0, 2500); /// entre 30 e 80 para valores de frequencia entre 0 e 2500.
      } /// Se não for detectada temperatura superior a 29, o som do buzzer apenas será alterado na chamada de noTone().
      }
      else { /// Se não há dados no buffer (k=0), buzzer não consome dados.
      i = 0; /// Reseta variável de controle do buzzer.
      }*/
    if (i > 0) {
      /*if (xSemaphoreTake(xSerialSemaphore, (TickType_t)5) == pdTRUE) {
        Serial.println("******TaskBuzzer******");
        xSemaphoreGive(xSerialSemaphore);
        }*/
      // digitalWrite(LED_BUILTIN, LOW);
      if (avarage > 26.00) {
        digitalWrite(LED_BUILTIN, HIGH);
      } else {
        digitalWrite(LED_BUILTIN, LOW);
      }
    } else {
      i = 0;
      // digitalWrite(LED_BUILTIN, LOW);

    }
  }
}
