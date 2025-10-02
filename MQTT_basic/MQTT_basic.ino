#include <WiFi.h>
#include <PubSubClient.h>

//Nome: Bruno Davi Dutka
//Turma: TDESI-V1
//Grupo: Bruno, Paulo e Vinicius Angelo

const String SSID = "iPhone";
const String PSW = "iot_sul_123";

const String brokerURL = "test.mosquitto.org"; //URL do broker    (servidor)
const int porta = 1883;                        //Porta do broker  (servidor)

const String topic_1 = "Carrinho/Cheio/1";
const String topic_2 = "Carrinho/Cheio/2";

const int led = 10;

WiFiClient espClient;                 //Criando Cliente WiFi
PubSubClient mqttClient(espClient);   //Criando Cliente MQTT

void conexaoWifi();
void conexaoBroker();
void reconexaoWifi();
void reconexaoBroker();
void mandarMensagem();

void setup() {
  Serial.begin(115200);
  conexaoWifi();
  conexaoBroker();
  pinMode(led, OUTPUT);
}

void loop() {
  mqttClient.loop();
  reconexaoWifi();
  reconexaoBroker();
  if(Serial.available() > 0){
  mandarMensagem();
  }
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
  mqttClient.subscribe(topic_1.c_str());
  mqttClient.setCallback(callback);
  Serial.println("Conectado com sucesso!");
}

void callback(char* topic_1, byte* payload, unsigned long length){
  String resposta = "";
  for(int i = 0;  i < length; i++){
    resposta += (char) payload[i];
  }
  if(resposta == "ON"){
    analogWrite(led, 255);
    Serial.println("Led ligado!!!")
  }
  else if(resposta == "OFF"){
    analogWrite(led, 0);
    Serial.println("Led desligado!!!")
  }
}

void reconexaoBroker(){
  if(!mqttClient.connected()){
      Serial.println("Conexão com o broker perdida!!!");
      conexaoBroker();
    }
}

void mandarMensagem(){
  String text = Serial.readStringUntil('\n');
  mqttClient.publish(topic_2.c_str(), text.c_str());
}