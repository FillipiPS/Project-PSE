# Project-PSE

## Entrada de dados
Usando o sensor de umidade e temperatura DHT11, é feita a aquisição da temperatura, sendo o dado alvo do projeto.

## Armazenamento de dados
Em consonância com a aquisição da temperatura, é realizado o armazenamento do valor de temperatura lido a cada 10 segundos. 
Processamento dos dados: na sequência, é calculada a média de 10 leituras, usando a biblioteca FreeRTOS e semphr.

## Saída de dados
Serão exibidas as médias no monitor serial do Arduino. Paralelamente, caso a média resultante seja maior do que 26 ºC será acionado um led vermelho. Se a temperatura média atingir um valor entre 23 °C e 25 °C, será ligado um led amarelo. Para temperaturas menores do que 23 ºC é acionado o led verde.

## Sincronização/comunicação
Uso de semáforo através da biblioteca semphr. permitindo o controle da porta serial, garantindo que apenas uma tarefa controle a porta serial por vez.

## Temporização
Uso de atraso na aquisição da temperatura por especificação do funcionamento do sensor de temperatura em questão.

![GitHub](https://img.shields.io/github/license/FillipiPS/Project-PSE)
![GitHub repo file count](https://img.shields.io/github/directory-file-count/FillipiPS/Project-PSE)
![GitHub closed pull requests](https://img.shields.io/github/issues-pr-closed/FillipiPS/Project-PSE)
![GitHub contributors](https://img.shields.io/github/contributors/FillipiPS/Project-PSE)
![GitHub last commit](https://img.shields.io/github/last-commit/FillipiPS/Project-PSE)
