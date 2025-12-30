#include <DHT.h>

#define DHTPIN 5
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial.println("=== Sensor DHT11 ===");
  Serial.println("Inicializando...\n");
  
  dht.begin();
}

void loop() {
  float umidade = dht.readHumidity();
  float temperatura = dht.readTemperature();

  // Verifica se a leitura falhou
  if (isnan(umidade) || isnan(temperatura)) {
    Serial.println("❌ Erro na leitura do sensor!");
    delay(3000);
    return;
  }

  // Exibe os dados
  Serial.println("--- Leitura ---");
  Serial.print("Temperatura: ");
  Serial.print(temperatura, 1);
  Serial.println(" °C");
  
  Serial.print("Umidade: ");
  Serial.print(umidade, 1);
  Serial.println(" %");
  
  Serial.println();

  delay(3000);
}