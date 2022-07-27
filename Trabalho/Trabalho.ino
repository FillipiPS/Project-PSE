#include <dht.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

/// Pino 5 para aquisição da temperatura pelo sensor.
#define DHT11_PIN 5
/// Pino 4 para a comunicação do LED verde (Temperatura fria).
#define LED_GREEN 4
/// Pino 3 para a comunicação do LED amarelo (Temperatura ambiente).
#define LED_YELLOW 3
/// Pino 2 para a comunicação do LED vermelho (Temperatura quente).
#define LED_RED 2
/// Quantidade de temperaturas a serem capturadas.
#define TEMPERATURE_QUANTITY 10

/// É necessária uma struct para criar a fila com xQueueCreate, sendo assim é utlizada para guardar o valor da temperatura.
struct DHTTemperature {
  /// Valor da temperatura.
  float value;
};

/// dht é utilizada para obter a temperatura do sensor DHT11.
dht DHT;

/// SemaphoreHandle responsável por controlar Serial Port, garante que apenas uma tarefa controle o Serial Port por vez.
SemaphoreHandle_t xSerialSemaphore;

/// QueueHandle utilizada por TaskReadTemperature para enviar a temperatura adquirida pelo sensor.
QueueHandle_t structQueue;

/// Vetor utilizado para guardar as temperaturas lidas pelo sensor.
float temperatures[TEMPERATURE_QUANTITY];
/// Variável que guarda a média das temperaturas.
float avarege;
/// Variável que guarda a quantidade de temperaturas adquiridas.
int temperaturesPosition;
/// Flag responsável por habilitar a tarefa que realiza a média das temeraturas.
bool isReadingFinished;
/// Flag responsável por habilitar a tarefa que realiza informa o status.
bool isChecked;

/// @private
void TaskReadTemperature(void *pvParameters);
/// @private
void TaskProcessTemperature(void *pvParameters);
/// @private
void TaskTemperatureAvarege(void *pvParameters);
/// @private
void TaskDisplayStatus(void *pvParameters);

/*! \fn void setup() 
 *  \brief Responsável por realizar a configuração inicial sistema.
 *  \paragraph configuracao Configurações 
 */

void setup()
{
  Serial.begin(9600); /// Baud Rate = 9600.
  Serial.println("Starting process!");

  pinMode(LED_BUILTIN, OUTPUT); /// Pino LED_BUILTIN como saída.
  pinMode(LED_GREEN, OUTPUT); /// Pino LED_GREEN como saída.
  pinMode(LED_YELLOW, OUTPUT); /// Pino LED_YELLOW como saída.
  pinMode(LED_RED, OUTPUT); /// Pino LED_RED como saída.

  if (xSerialSemaphore == NULL) { 
    xSerialSemaphore = xSemaphoreCreateMutex(); /// Cria a mutex que controla a porta serial.
  } else {
    xSemaphoreGive(xSerialSemaphore);
  }

  structQueue = xQueueCreate(10, sizeof(struct DHTTemperature)); /// Cria a Queue para envio de dados do sensor.

  if (structQueue != NULL) {
    xTaskCreate(TaskReadTemperature, "ReadTemperature", 128, NULL, 2, NULL); /// Cria task responsável por obter a temperatura.
    xTaskCreate(TaskProcessTemperature, "ProcessTemperature", 128, NULL, 2, NULL); /// Cria task responsável por inserir a temperatura obtida no array temperatures e mostrar no Serial a temperatura atual.
    xTaskCreate(TaskTemperatureAvarege, "TemperatureAvarege", 128, NULL, 2, NULL); /// Cria task responsável por realizar a média das temepraturas.
    xTaskCreate(TaskDisplayStatus, "DisplayStatus", 128, NULL, 2, NULL); /// Cria task responsável informar o status do ar-condicionado estar refrigerando.
  }
}

/*! \fn void loop() 
 *  \brief Método vazio, pois o fluxo é realizado pelas tasks.
 */
void loop() { }

///*--------------------------------------------------*/
///*---------------------- Tasks ---------------------*/
///*--------------------------------------------------*/

/*! \fn TaskReadTemperature(void *pvParameters __attribute__((unused)))
 *  \brief Tarefa responsável por realizar a leitura da temperatura utilizando a biblioteca dht.h.
 */
void TaskReadTemperature(void *pvParameters __attribute__((unused))) {
  while (true) {
    struct DHTTemperature temperature;
    int chk = DHT.read11(DHT11_PIN);
    temperature.value = DHT.temperature;

    xQueueSend(structQueue, &temperature, portMAX_DELAY); /// Faz o envio da struct temperature.
    vTaskDelay(2000 / portTICK_PERIOD_MS); /// Espera 2 segundos entre as leituras para estabilidade do dado, pois o sensor DHT11 consegue ler a cada 1 segundo.
  }
}

/*! \fn TaskProcessTemperature(void *pvParameters __attribute__((unused)))
 *  \brief Tarefa responsável por imprimir no Serial Port a temperatura atual e salvar no array.
 */
void TaskProcessTemperature(void *pvParameters __attribute__((unused))) {
  while (true) {
    struct DHTTemperature temperature;
    /// Recebe a struct temperature da task TaskReadTemperature.
    if (xQueueReceive(structQueue, &temperature, portMAX_DELAY) == pdPASS) {
      temperatures[temperaturesPosition] = temperature.value; /// Guarda a temperatura adquirda no array temperatures.
      if (temperaturesPosition < TEMPERATURE_QUANTITY) { /// Checa se a quantidade de valores adquidos é menor que TEMPERATURE_QUANTITY.
        isReadingFinished = false; /// Não habilita o cálculo da média.
        if (xSemaphoreTake(xSerialSemaphore, (TickType_t)5) == pdTRUE) {
          Serial.print(temperaturesPosition + 1); /// Imprime a quantidade atual de leituras.
          Serial.print(" - Temperatura: ");
          Serial.println(temperature.value); /// Imprime o valor da temperatura recebido.
          xSemaphoreGive(xSerialSemaphore);
          temperaturesPosition++; /// Incrementa a variável temperaturesPosition.
        }
      } else {
        isReadingFinished = true; /// Habilita o cálculo da média.
      }
      isChecked = false; /// Não habilita o DisplayStatus.
    }
  }
}

/*! \fn TaskTemperatureAvarege(void *pvParameters __attribute__((unused)))
 *  \brief Tarefa responsável por realizar a média das temperaturas.
 */
void TaskTemperatureAvarege(void *pvParameters __attribute__((unused))) {
  while (true) {
    float temperatureSum;

    if (isReadingFinished) { /// \paragraph IF Se a leitura das temperaturas estão finalizadas
      for (int index = 0; index < TEMPERATURE_QUANTITY; index++) { /// Realiza a soma das temperaturas adquiridas.
        temperatureSum += temperatures[index];
      }

      avarege = temperatureSum / TEMPERATURE_QUANTITY; /// Faz a média das temperaturas adquiridas.

      temperaturesPosition = 0;  /// Reinicia a variável que guarda a quantidade de temperaturas adquiridas.
      isReadingFinished = false; /// Não habilita o cálculo que realiza a média.

      if (xSemaphoreTake(xSerialSemaphore, (TickType_t)5) == pdTRUE) {
        Serial.print("Média: ");
        Serial.println(avarege); /// Imprime a média das temeraturas.
        temperatureSum = 0; /// Limpa a soma das temperaturas.
        xSemaphoreGive(xSerialSemaphore);
      }
    } else { /// \paragraph ELSE Caso o contrário
      isReadingFinished = false;  /// Não habilita o cálculo que realiza a média.
      isChecked = true; /// Habilita o DisplayStatus.
    }
  }
}

/*! \fn TaskDisplayStatus(void *pvParameters __attribute__((unused)))
 *  \brief Tarefa responsável por ligar os LEDs de indicação de temperatura.
 */
void TaskDisplayStatus(void *pvParameters __attribute__((unused))) {
  while (true) {
    if (isChecked) { /// Se está habilitado o DisplayStatus, irá atualizar o valor do LED.
    if (avarege < 23) { /// Se a média for menor que 23 graus irá acender o LED verde.
        digitalWrite(LED_GREEN, HIGH);
        digitalWrite(LED_YELLOW, LOW);
        digitalWrite(LED_RED, LOW);
      }
      if (avarege > 23 && avarege < 26) { /// Se a média estiver entre 23 e 26 graus irá acender o LED amarelo.
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_YELLOW, HIGH);
        digitalWrite(LED_RED, LOW);
      }
      if (avarege > 26) { /// Se a média for maior que 26 graus irá acender o LED vermelho.
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_YELLOW, LOW);
        digitalWrite(LED_RED, HIGH);
      }
    } else {
      isChecked = false;
    }
  }
}
