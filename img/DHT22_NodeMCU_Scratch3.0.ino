/** Este projeto faz parte da disciplina Fundamentos de Internet das Coisas - USP
  * Elaine Silva Rocha Sobreira
  * Prof. Dr. Antonio Deusany de Carvalho Jr.
  *  
  * Possibilita a Conexão do Sensor DHT22, ligado ao Pino D2 (GPIO4), enviando dados
  * para o ThingSpeak que são interpretados pelo Scratch 3.0, por meio da Extensão DataViewer 
  * (https://github.com/jaafreitas/scratch-dataviewer.
  * Utilizamos o ScratchMod (modificação do Scratch), disponibilizado pelo HackEduca (https://www.hackeduca.com.br/).
  * 
  * O ScratchMod pode ser acessado em https://hackeduca.github.io/  
 **/

// ---------------------------  Declaração das Bibliotecas (library) --------------------------------------//
#include "DHT.h"
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>

// --------------  Declaração dos dados de acesso (ssid, senhas, canal  api -------------------------------//
#define ssid " "                 // Coloque o nome da sua rede Wifi
#define pass " "                 // Coloque a senha da sua rede Wifi
#define myChannelNumber XXXXX    // Coloque o ID do seu Canal no ThingSpeak
#define myWriteAPIKey " "        // Coloque a API do seu Canal no ThingSpeak

// ---------------------------------  Declaração das Bibliotecas  -----------------------------------------//
// Define o Sensor, o Pino e a inicialização
#define DHTTYPE DHT22
#define DHTPin D2
DHT dht(DHTPin, DHTTYPE);

// --------------  Define as variáveis para armazenamento de Umidade e Temperatura  -----------------------//
// Define as variáveis para armazenamento de Umidade e Temperatura
float h; // Umidade
float t; // Temperatura

// -------------------------  Cria uma instância para acesso a Web  ---------------------------------------//
WiFiClient client;

//*************//
void setup()
{
  // ----------------------  Define o modo do Wif-->  station mode ----------------------------------------//
  WiFi.mode(WIFI_STA);
  // ----------------------  Inicia processo para monitoramento serial  -----------------------------------//
  Serial.begin(115200);
  // -----------------------------  Inicializa o sensor DHT22  --------------------------------------------//
  dht.begin();
  // -----------------------------  Acessa o site ThingSpeak   --------------------------------------------//
  ThingSpeak.begin(client);
}

//*************//
void loop()
{
  // -----------------------------  Define quantos campos serão enviados ----------------------------------//
  ThingSpeak.setField(1, h);
  ThingSpeak.setField(2, t);
  // --------------  Efetua a leitura de Temperatura e Umidade (guarda nas variáveis t e h)  --------------//
  t = dht.readTemperature();
  h = dht.readHumidity();

  // -----------------------------  Inicia o acesso a rede Wifi selecionada  ------------------------------//
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Tentando conectar a rede ");
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);  
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConectado!!!");
    Serial.println(ssid);
    Serial.println(WiFi.localIP());
  }

  // ------------------------------  Envia dados ao ThingSpeak --------------------------------------------//
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200)
  {
    Serial.println("Dados foram enviados com Sucesso.");
  }
  else {
    Serial.println("Há algum problema no acesso ao Thingspeak: Erro #: " + String(x));
  }

  // ---------------------  Mostra dados na tela de monitoramento Serial ----------------------------------//
  Serial.print("Temperatura atual é : ");
  Serial.println(t);
  Serial.print("A umidade atual é : ");
  Serial.println(h);
  Serial.println("");

  // -------------  Aguarda 20 segundos antes de enviar um novo dado para o ThingSpeak  ---------------------//
  delay(20000);
}
// ---------------------------------------------  FIM  ----------------------------------------------------//
