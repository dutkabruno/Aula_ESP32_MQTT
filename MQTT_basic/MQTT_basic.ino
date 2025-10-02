#include <WiFi.h>
#include <PubSubClient.h>

const String SSID = "iPhone";
const String PSW = "iot_sul_123";

const String brokerURL = "test.mosquitto.org"; //URL do broker    (servidor)
const int porta = 1883;                        //Porta do broker  (servidor)

const String topic = "AulaIoTSul/Chat";

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
  mqttClient.subscribe(topic.c_str());
  mqttClient.setCallback(callback);
  Serial.println("Conectado com sucesso!");
}

void callback(char* topic, byte* payload, unsigned long length){
  String resposta = "";
  for(int i = 0;  i < length; i++){
    resposta += (char) payload[i];
  }
  Serial.println(resposta);
}

void reconexaoBroker(){
  if(!mqttClient.connected()){
      Serial.println("Conexão com o broker perdida!!!");
      conexaoBroker();
    }
}

void mandarMensagem(){
  String text = Serial.readStringUntil('\n');
  text = "bruno: " + text;
  mqttClient.publish(topic.c_str(), text.c_str());
}