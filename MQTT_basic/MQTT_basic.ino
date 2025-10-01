#include <WiFi.h>
#include <PubSubClient.h>

const String SSID = "iPhone";
const String PSW = "iot_sul_123";

const String brokerURL = "test.mosquitto.org"; //URL do broker    (servidor)
const int porta = 1883;                        //Porta do broker  (servidor)

WiFiClient espClient;                 //Criando Cliente WiFi
PubSubClient mqttClient(espClient);   //Criando Cliente MQTT

void conexaoWifi();

void setup() {
  Serial.begin(115200);
  conexaoWifi();
  Serial.println("Conectando ao broker...");
  mqttClient.setServer(brokerURL.c_str(), porta);
  String userId = "ESP_BRUNO";
  mqttClient.connect(userId.c_str());
  while(!mqttClient.connected()){
    Serial.println("Erro de conexão");
    delay(500);
  }
  Serial.println("Conectado com sucesso!");
}

void loop() {
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("Conexão perdida!!!");
    conexaoWifi();
  }
  mqttClient.loop();
}

void conexaoWifi(){
  Serial.println("Iniciando conexão com rede WiFi");
  WiFi.begin(SSID, PSW);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(400);
    Serial.print(".");
    delay(400);
    Serial.println(".");
    delay(400);
  }
  Serial.println("Rede de Wi-Fi conectada");
}