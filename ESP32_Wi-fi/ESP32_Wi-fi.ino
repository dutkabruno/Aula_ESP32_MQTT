#include <WiFi.h>

void scanLocalNetworks();

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando Scan de redes Wi-Fi");
  scanLocalNetworks();
}

void loop() {
  
}


void scanLocalNetworks(){
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