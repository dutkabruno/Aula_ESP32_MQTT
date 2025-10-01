#include <WiFi.h>

const String SSID = "iPhone";
const String PSW = "iot_sul_123";

void scanLocalNetworks();

void setup() {
  Serial.begin(115200);
  scanLocalNetworks();
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

void loop() {
  
}


void scanLocalNetworks(){
  Serial.println("Iniciando Scan de redes Wi-Fi");
  int number = WiFi.scanNetworks();
    if(number == -1){
      Serial.println("ERRO!. Não deu bom");
    }
    else{
      for(int net = 0; net < number; net++){
        Serial.printf("%d - %s | %d db\n", net, WiFi.SSID(net), WiFi.RSSI(net));
      }
      //Serial.printf("O número de redes encontradas é de: %d\n", number);
    }
}