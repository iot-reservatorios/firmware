#include "ThingsBoard.h"
#include <ESP8266WiFi.h>

#define BAUD_RATE 19200
#define THINGSBOARD_SERVIDOR "agrotechlab.lages.ifsc.edu.br"
#define TOKEN_ENTRADA "INSIRA_O_TOKEN_DO_ESP_DE_ENTRADA"
#define TOKEN_CENTRAL "INSIRA_O_TOKEN_DO_ESP_CENTRAL"
#define WIFI_NOME "INSIRA_O_NOME_DO_WIFI_SSID"
#define WIFI_SENHA "INSIRA_A_SENHA_DO_WIFI" 

float miliAmpere = 16; // Valor inicial para simulação

WiFiClient dispositivoEsp;
ThingsBoard tb(dispositivoEsp);
int wifiStatus = WL_IDLE_STATUS;

void setup() {
  Serial.begin(BAUD_RATE);
  WiFi.begin(WIFI_NOME, WIFI_SENHA);
  InicializarWifi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    ReconectarWifi();
  }

  if (!tb.connected()) {
    if (!tb.connect(THINGSBOARD_SERVIDOR, TOKEN_ENTRADA)) { // Altere: ou TOKEN_ENTRADA ou TOKEN_CENTRAL
      Serial.println("Falha ao conectar");
      return;
    }
  }

  // -- Início simulação --
  float incremento = random(-1, 2);
  incremento = incremento / 10;
  miliAmpere += incremento;

  if (miliAmpere < 4) {
    miliAmpere = 4;
  } else if (miliAmpere > 20) {
    miliAmpere = 20;
  }
  // -- Fim simulação --

  Serial.println("Enviando dados...");
  tb.sendTelemetryFloat("miliAmpere", miliAmpere);

  delay(120000);
  tb.loop();
}

void InicializarWifi() {
  Serial.println("Estabelecendo conexão WiFi ...");
  WiFi.begin(WIFI_NOME, WIFI_SENHA);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("*");
  }
  Serial.println("Conectado WiFi");
}

void ReconectarWifi() {
  wifiStatus = WiFi.status();
  if (wifiStatus != WL_CONNECTED) {
    WiFi.begin(WIFI_NOME, WIFI_SENHA);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
    Serial.println("Reconexão WiFi");
  }
}