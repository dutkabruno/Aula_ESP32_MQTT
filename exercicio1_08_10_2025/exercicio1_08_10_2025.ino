//Nome: Bruno Davi Dutka
//Turma: TDESI-V1
//Grupo: Bruno, Paulo e Vinicius Angelo

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>

#define SCREEN_WIDTH 128  //Largura da tela em pixels
#define SCREEN_HEIGHT 64  //Altura da tela em pixels

#define I2C_SCK 6
#define I2C_SDA 5

Adafruit_SSD1306 tela(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const String SSID = "Paulo";
const String PSW = "cxos9674";

const String brokerURL = "test.mosquitto.org"; //URL do broker    (servidor)
const int porta = 1883;                        //Porta do broker  (servidor)

const String topic = "dupladosLoucos/estacao/dados";

const int led = 10;

WiFiClient espClient;                 //Criando Cliente WiFi
PubSubClient mqttClient(espClient);   //Criando Cliente MQTT

JsonDocument doc;

void conexaoWifi();
void conexaoBroker();
void reconexaoWifi();
void reconexaoBroker();

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCK);
  tela.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  tela.clearDisplay();
  tela.setCursor(0,0);
  tela.display();
  tela.setTextSize(1);
  tela.setTextColor(SSD1306_WHITE);
  conexaoWifi();
  conexaoBroker();
  pinMode(led, OUTPUT);
}

void loop() {
  mqttClient.loop();
  reconexaoWifi();
  reconexaoBroker();
  tela.setCursor(0, 0);
  while(Serial.available() == 0){}
  tela.clearDisplay();
  String text = Serial.readStringUntil('\n');
  tela.println(text);
  tela.display();
}

void conexaoWifi(){
  Serial.println("Iniciando conexão com rede WiFi");
  while(WiFi.status() != WL_CONNECTED){
    WiFi.begin(SSID, PSW);
    Serial.print(".");
    delay(400);
    Serial.print(".");
    delay(400);
    Serial.println(".");
    delay(5000);
  }
  Serial.println("Rede de Wi-Fi conectada");
}

void reconexaoWifi(){
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("Conexão de rede perdida!!!");
    conexaoWifi();
  }
}

void conexaoBroker(){
  Serial.println("Conectando ao broker...");
  mqttClient.setServer(brokerURL.c_str(), porta);
  String userId = "ESP_BRUNO";
  userId += String(random(0xffff), HEX);
  while(!mqttClient.connected()){
    mqttClient.connect(userId.c_str());
    Serial.print(".");
    delay(400);
    Serial.print(".");
    delay(400);
    Serial.println(".");
    delay(2000);
  }
  mqttClient.subscribe(topic.c_str());
  mqttClient.setCallback(callback);
  Serial.println("Conectado com sucesso!");
}

void callback(char* topic, byte* payload, unsigned int length){
  String resposta = "";
  for(int i = 0;  i < length; i++){
    resposta += (char) payload[i];
  }
  deserializeJson(doc, resposta);
  int distancia = doc["distancia_cm"];
  bool pir = doc["movimento"];
  int pot = doc["limiar_pot"];
  Serial.println(resposta);
  if(distancia < 20 && pir == true){
    digitalWrite(led, HIGH);
  }
  else{
    digitalWrite(led, LOW);
  }
  tela.clearDisplay();
  tela.display();
  tela.setCursor(0, 0);
  tela.print("Distância: ");
  tela.println(distancia);
  tela.print("Sensor PIR: ");
  tela.println(pir);
  tela.print("Potenciômetro: ");
  tela.println(pot);
  tela.display();
}

void reconexaoBroker(){
  if(!mqttClient.connected()){
      Serial.println("Conexão com o broker perdida!!!");
      conexaoBroker();
    }
}