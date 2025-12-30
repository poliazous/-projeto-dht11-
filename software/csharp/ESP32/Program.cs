using System;
using System.Net.Http;
using System.Text.Json;

namespace ESP32MonitorBasico
{
    class Program
    {
        // Endereço IP da sua ESP32 (quando está como Access Point)
        const string ESP32_IP = "http://192.168.4.1/";
        const int POLLING_INTERVAL_MS = 5000; // 5000 milissegundos = 5 segundos

        // =======================================================
        // 1. MÉTODO PRINCIPAL: Onde o programa começa
        // =======================================================
        static async Task Main(string[] args)
        {
            Console.WriteLine("Iniciando monitoramento em tempo real...");
            Console.WriteLine($"Conecte-se à rede Wi-Fi da ESP32 para iniciar a leitura.");
            Console.WriteLine("Pressione Ctrl+C para sair.\n");
            
            // Chama o loop principal
            await RunDataPollingLoop();
        }

        // =======================================================
        // 2. LOOP PRINCIPAL: Busca dados continuamente
        // =======================================================
        static async Task RunDataPollingLoop()
        {
            // Loop infinito para buscar dados continuamente
            while (true)
            {
                // 2.1 Busca e exibe os dados
                await GetAndDisplayData();

                // 2.2 Aguarda 5 segundos antes da próxima leitura
                Console.WriteLine($"Aguardando {(POLLING_INTERVAL_MS / 1000)} segundos...");
                await Task.Delay(POLLING_INTERVAL_MS);
                
                Console.WriteLine(); // Linha em branco para separar
            }
        }

        // =======================================================
        // 3. BUSCA E EXIBE OS DADOS: Comunicação com a ESP32
        // =======================================================
        static async Task GetAndDisplayData()
        {
            // Cria um cliente HTTP
            HttpClient client = new HttpClient();
            
            Console.WriteLine("------------------------------------------");
            Console.WriteLine($"ÚLTIMA LEITURA: {DateTime.Now.ToLongTimeString()}");
            Console.WriteLine("------------------------------------------");

            try
            {
                // 3.1 Faz a requisição HTTP GET para a ESP32
                HttpResponseMessage response = await client.GetAsync(ESP32_IP);

                // 3.2 Verifica se a requisição foi bem sucedida
                if (response.IsSuccessStatusCode)
                {
                    // 3.3 Lê o conteúdo da resposta (o JSON)
                    string jsonResponse = await response.Content.ReadAsStringAsync();
                    
                    // 3.4 Converte o JSON para um objeto C#
                    DhtData data = JsonSerializer.Deserialize<DhtData>(jsonResponse)!;

                    // 3.5 Exibe os dados
                    if (data != null)
                    {
                        Console.WriteLine($"Temperatura: {data.Temperatura:F1}°C");
                        Console.WriteLine($"Umidade:     {data.Umidade:F1}%");
                    }
                    else
                    {
                        Console.WriteLine("ERRO: Não foi possível entender os dados recebidos.");
                    }
                }
                else
                {
                    Console.WriteLine($"ERRO: A ESP32 respondeu com código {response.StatusCode}");
                }
            }
            catch (HttpRequestException)
            {
                Console.WriteLine("ERRO DE CONEXÃO: Verifique se:");
                Console.WriteLine("1. A ESP32 está ligada");
                Console.WriteLine("2. Você está conectado na rede da ESP32");
                Console.WriteLine("3. O IP está correto: " + ESP32_IP);
            }
            catch (Exception e)
            {
                Console.WriteLine($"Ocorreu um erro inesperado: {e.Message}");
            }
            finally
            {
                // 3.6 Fecha o cliente HTTP (boa prática)
                client.Dispose();
            }
        }
    }

    // =======================================================
    // 4. CLASSE DE DADOS: Representa os dados do sensor
    // =======================================================
    public class DhtData
    {
        // Propriedade para temperatura (vem do JSON como "temperatura")
        [System.Text.Json.Serialization.JsonPropertyName("temperatura")]
    
        public float Temperatura { get; set; }

        // Propriedade para umidade (vem do JSON como "umidade")
        [System.Text.Json.Serialization.JsonPropertyName("umidade")]
        public float Umidade { get; set; }
    }
}