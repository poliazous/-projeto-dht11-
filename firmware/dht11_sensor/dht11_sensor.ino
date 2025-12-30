#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

#define DHTPIN 5
#define DHTTYPE DHT11

// Configuração do Ponto de Acesso (AP)
const char* ssid = "ESP32_DHT_AP";  // Nome da rede Wifi da ESP32
const char* password = "senhasegura"; // Senha 8 caracteres

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80); // O servidor HTTP irá rodar na porta 80

void setup() {
  Serial.begin(115200);
  dht.begin();

  // 4. Iniciar o Ponto de Acesso
  Serial.print("Configurando Ponto de Acesso (AP)... ");
  WiFi.softAP(ssid, password);
  
  // 5. Exibir o IP local (geralmente será 192.168.4.1)
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // 6. Configurar as rotas do Servidor (o que fazer quando o navegador acessa o IP)
  server.on("/", handleRoot); // Quando alguém acessar o IP (a raiz /), chame a função handleRoot
  server.begin(); // Inicia o servidor
  Serial.println("HTTP server iniciado.");
}

void loop() {
  server.handleClient(); // Permanece escutando por requisições HTTP
  delay(3000); // Não usamos o loop para leitura, vamos ler apenas na requisição para ter dados frescos.
}

// Função para ler o sensor e formatar a resposta
void handleRoot() {
  // 1. Leitura do Sensor DHT11
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  // 2. Checagem de Erros de Leitura
  if (isnan(h) || isnan(t)) {
    Serial.println("Falha ao ler o sensor DHT!");
    server.send(500, "text/plain", "Falha ao ler o sensor DHT!");
    return;
  }

  // 3. Criação da Estrutura JSON (para ser fácil de ler pelo C# ou qualquer cliente)
  // Exemplo: {"temperatura": 25.5, "umidade": 60.2}
  String jsonResponse = "{";
  jsonResponse += "\"temperatura\": " + String(t);
  jsonResponse += ", \"umidade\": " + String(h);
  jsonResponse += "}";

  // 4. Envio da Resposta HTTP para o cliente (navegador/C#)
  server.sendHeader("Access-Control-Allow-Origin", "*"); // Permite que qualquer cliente acesse (bom para testes)
  server.send(200, "application/json", jsonResponse);
  
  Serial.print("Dados enviados: ");
  Serial.println(jsonResponse);
}