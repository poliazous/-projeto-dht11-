import paho.mqtt.client as mqtt
import json
from datetime import datetime
import time

# 🔥 CONFIG MQTT
MQTT_BROKER = "broker.hivemq.com"
MQTT_PORT = 1883
TOPICS = [
    "tonni/casa/sensor/temperature",
    "tonni/casa/sensor/humidity", 
    "tonni/casa/sensor/status"
]

class MQTTMonitor:
    def __init__(self):
        self.client = mqtt.Client()
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message
        
        # Armazenar últimos valores
        self.last_temperature = None
        self.last_humidity = None
        self.status = "offline"
    
    def on_connect(self, client, userdata, flags, rc):
        print(f"✅ Conectado ao broker MQTT! Código: {rc}")
        
        # Assinar todos os tópicos
        for topic in TOPICS:
            client.subscribe(topic)
            print(f"📡 Assinando tópico: {topic}")
    
    def on_message(self, client, userdata, msg):
        try:
            payload = json.loads(msg.payload.decode())
            timestamp = datetime.now().strftime("%H:%M:%S")
            
            if "temperature" in payload:
                self.last_temperature = payload["temperature"]
                print(f"[{timestamp}] 🌡️ Temperatura: {self.last_temperature}°C")
                
            elif "humidity" in payload:
                self.last_humidity = payload["humidity"] 
                print(f"[{timestamp}] 💧 Umidade: {self.last_humidity}%")
                
            elif "status" in payload:
                self.status = payload["status"]
                print(f"[{timestamp}] 📡 Status: {self.status} - IP: {payload.get('ip', 'N/A')}")
                
        except json.JSONDecodeError:
            print(f"❌ Mensagem não-JSON: {msg.payload}")
    
    def start(self):
        print("🚀 Iniciando monitoramento MQTT...")
        print(f"🌐 Broker: {MQTT_BROKER}:{MQTT_PORT}")
        print("Pressione Ctrl+C para parar\n")
        
        try:
            self.client.connect(MQTT_BROKER, MQTT_PORT, 60)
            self.client.loop_forever()
            
        except KeyboardInterrupt:
            print("\n✅ Monitoramento encerrado!")
        except Exception as e:
            print(f"❌ Erro: {e}")

    def get_summary(self):
        """Retorna resumo dos dados atuais"""
        return {
            "temperature": self.last_temperature,
            "humidity": self.last_humidity,
            "status": self.status,
            "timestamp": datetime.now().isoformat()
        }

# 🔥 USO AVANÇADO: Python também pode PUBLICAR
def send_command():
    """Exemplo: Python publicando comando para ESP32"""
    command_client = mqtt.Client()
    command_client.connect(MQTT_BROKER, MQTT_PORT)
    
    command = {"action": "reset", "source": "python"}
    command_client.publish("tonni/casa/sensor/commands", json.dumps(command))
    print("📤 Comando enviado para ESP32")
    command_client.disconnect()

if __name__ == "__main__":
    monitor = MQTTMonitor()
    
    # Iniciar monitoramento
    monitor.start()
    
    # Para enviar comando (descomente se quiser):
    # send_command()