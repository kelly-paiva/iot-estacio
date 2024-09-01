#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

/* definicoes da rede wi-fi */
const char* ssid = "GALO_FORTE";      
const char* password = "VINGADOR";  

/* definicoes dos sensores de nivel de tinta */
const int sensorPin = A0; 
int nivelTinta = 0;

/* definicoes da API - NECESSARIO ALTERAR PARA O ENDERECO DA API QUANDO PRONTA */
const char* serverName = "http://alterar/api/status";

void setup() {
  /* inicializa a comunicação serial para debug */
  Serial.begin(115200);
  
  /* configura o pino do sensor como entrada */
  pinMode(sensorPin, INPUT);
  
  /* conecta na rede */
  WiFi.begin(ssid, password);
  Serial.print("Conectando-se à rede Wi-Fi...");
  
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("Conectado.");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  /* le o nível de tinta do sensor */
  nivelTinta = analogRead(sensorPin);
  
  /* converte o valor do sensor em um nivel de tinta percentual (0 a 100) */
  int nivelPercentual = map(nivelTinta, 0, 1023, 0, 100);
  
  /* exibe o nivel de tinta no console serial */
  Serial.print("Nivel de tinta: ");
  Serial.print(nivelPercentual);
  Serial.println("%");
  
  /* envia os dados para a API se estiver conectado na rede */
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    /* configura o endereco do servidor e o endpoint da API */
    http.begin(serverName);
    
    /* Prepara os dados em formato JSON para envio */
    String jsonPayload = "{\"nivelTinta\": " + String(nivelPercentual) + "}";
    
    /* envia uma requisição POST para a API */
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(jsonPayload);
    
    /* verifica a resposta do servidor */
    if(httpResponseCode > 0) {
      String response = http.getString();
      Serial.print("Resposta do servidor: ");
      Serial.println(response);
    } else {
      Serial.print("Erro ao enviar dados: ");
      Serial.println(httpResponseCode);
    }
    
    /* fecha a conexao com o servidor */
    http.end();
  } else {
    Serial.println("Falha na conexao. Dados não enviados.");
  }
  
  /* aguarda 60 segundos antes de ler o sensor novamente */
  delay(60000);
}

