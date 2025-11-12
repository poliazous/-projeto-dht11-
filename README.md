🌡️ Sensor DHT11 - Monitor de Temperatura e Umidade
Mostrar Imagem
Mostrar Imagem
Projeto simples para leitura de temperatura e umidade utilizando o sensor DHT11 com Arduino/ESP32.
📋 Descrição
Este projeto permite monitorar em tempo real a temperatura (em Celsius) e umidade relativa do ar através do sensor DHT11, exibindo os dados no Monitor Serial.
🔧 Componentes Necessários

1x Arduino Uno / ESP32 / ESP8266
1x Sensor DHT11
3x Jumpers
1x Cabo USB
1x Resistor 10kΩ (opcional, alguns módulos já incluem)

📦 Bibliotecas Necessárias
Instale a biblioteca DHT através do gerenciador de bibliotecas do Arduino IDE:
Sketch → Include Library → Manage Libraries → Buscar "DHT sensor library" por Adafruit
Ou instale manualmente:

DHT sensor library
Adafruit Unified Sensor

🔌 Diagrama de Ligação
DHT11        Arduino
─────────────────────
VCC    →    5V
GND    →    GND
DATA   →    Pin 5
🚀 Como Usar
1. Clone o repositório
bashgit clone https://github.com/seu-usuario/projeto-dht11.git
cd projeto-dht11
2. Abra o arquivo no Arduino IDE
bash# Abra o arquivo src/dht11_sensor.ino
3. Configure a placa

Tools → Board → Selecione sua placa (Arduino Uno, ESP32, etc.)
Tools → Port → Selecione a porta COM

4. Faça o upload

Clique no botão Upload (→)
Aguarde a compilação e transferência

5. Abra o Monitor Serial

Tools → Serial Monitor
Configure a velocidade para 115200 baud

📊 Exemplo de Saída
=== Sensor DHT11 ===
Inicializando...

--- Leitura ---
Temperatura: 24.5 °C
Umidade: 62.3 %

--- Leitura ---
Temperatura: 24.6 °C
Umidade: 62.1 %
⚙️ Configurações
Para alterar o pino de conexão, edite a linha:
cpp#define DHTPIN 5  // Altere para o pino desejado
Para alterar o intervalo de leitura (padrão: 3 segundos):
cppdelay(3000);  // Altere o valor em milissegundos
🐛 Solução de Problemas
ProblemaSolução"Erro na leitura do sensor!"Verifique as conexões, especialmente o pino DATAValores sempre iguaisSensor pode estar danificado ou com mal contatoNada aparece no Monitor SerialVerifique se a velocidade está em 115200 baudErro de compilaçãoInstale as bibliotecas DHT e Adafruit Unified Sensor
📝 Especificações do DHT11

Faixa de temperatura: 0°C a 50°C (±2°C)
Faixa de umidade: 20% a 90% (±5%)
Tempo de resposta: ~2 segundos
Tensão de operação: 3.3V a 5V

🤝 Contribuindo
Contribuições são bem-vindas! Sinta-se à vontade para:

Fazer um Fork do projeto
Criar uma branch para sua feature (git checkout -b feature/MinhaFeature)
Commit suas mudanças (git commit -m 'Adiciona nova feature')
Push para a branch (git push origin feature/MinhaFeature)
Abrir um Pull Request

📄 Licença
Este projeto está sob a licença MIT. Veja o arquivo LICENSE para mais detalhes.
👤 Autor
Poliana Souza - @poliazous
🔗 Links Úteis

Documentação Arduino
Datasheet DHT11
Biblioteca DHT


⭐ Se este projeto foi útil, considere dar uma estrela!
