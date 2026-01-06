#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

#define DHTPIN 5
#define DHTTYPE DHT11

// Configuração do Ponto de Acesso (AP)
const char* ssid = "ESP32_DHT_AP";
const char* password = "senhasegura";

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);

// Variável para controle de tempo
unsigned long lastUpdate = 0;
const unsigned long UPDATE_INTERVAL = 3000;

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Iniciar AP
  Serial.print("Configurando Ponto de Acesso (AP)... ");
  WiFi.softAP(ssid, password);
  
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Configurar rotas
  server.on("/", handleRoot); // Página HTML com JavaScript SSE
  server.on("/stream", handleStream); // Endpoint SSE
  server.begin();
  Serial.println("HTTP server iniciado.");
}

void loop() {
  server.handleClient();
  
  // Verifica se é hora de enviar dados (opcional, para limpar buffer)
  if (millis() - lastUpdate > UPDATE_INTERVAL) {
    lastUpdate = millis();
    // Aqui não enviamos ativamente, SSE é baseado em requisição contínua
  }
}

// Página HTML com JavaScript para receber SSE
void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>DHT11 - Tempo Real</title>
    <meta charset="UTF-8">
    <style>
        body { font-family: Arial; text-align: center; margin: 50px; }
        .data { font-size: 24px; margin: 20px; padding: 20px; border: 2px solid #333; }
        .temp { color: red; }
        .hum { color: blue; }
    </style>
</head>
<body>
    <h1>Monitor DHT11 - Tempo Real</h1>
    <div class="data">
        <h2>Temperatura: <span id="temp" class="temp">--</span> °C</h2>
        <h2>Umidade: <span id="hum" class="hum">--</span> %</h2>
    </div>
    <div id="status"></div>
    
    <script>
        const eventSource = new EventSource('/stream');
        
        eventSource.onmessage = function(event) {
            const data = JSON.parse(event.data);
            document.getElementById('temp').textContent = data.temperatura.toFixed(1);
            document.getElementById('hum').textContent = data.umidade.toFixed(1);
            document.getElementById('status').innerHTML = 
                'última atualização: ' + new Date().toLocaleTimeString();
        };
        
        eventSource.onerror = function() {
            document.getElementById('status').innerHTML = 
                'Conexão perdida. Tentando reconectar...';
        };
    </script>
</body>
</html>
)rawliteral";
  
  server.send(200, "text/html", html);
}

// Endpoint SSE que mantém conexão aberta
void handleStream() {
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/event-stream; charset=utf-8", "");
  
  // Envia dados periodicamente
  while (true) {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    
    if (!isnan(h) && !isnan(t)) {
      String jsonResponse = "{\"temperatura\": " + String(t) + 
                           ", \"umidade\": " + String(h) + "}";
      
      // Formato SSE
      server.sendContent("data: " + jsonResponse + "\n\n");
      Serial.print("Dados enviados via SSE: ");
      Serial.println(jsonResponse);
    }
    
    delay(3000); // Aguarda 3 segundos
    
    // Verifica se cliente ainda está conectado
    if (!server.client().connected()) {
      break;
    }
  }
}
// Adiconado
// Para rotas não encontradas
void handleNotFound() {
  server.send(404, "text/plain", "Página não encontrada");
}