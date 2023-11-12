#include <SoftwareSerial.h>
#include <WiFi.h> 
#include <WiFiUdp.h>

#define MYPORT_TX 32
#define MYPORT_RX 33

EspSoftwareSerial::UART myPort;

WiFiUDP udp;

const char* ssid = "RosAP";
const char* password = "fdp123@X";

const char* host = "192.168.0.25";
const int port = 3505;

const int pulsosPorRotacao = 20;
volatile long totalPulsos = 0;
volatile long totalPulsos2 = 0;
float raioRoda = 0.34;
float voltas = 0.0;
float voltas2 = 0.0;
float distancia = 0.0;
int d2 = 0;
float circunferencia = 2 * PI * raioRoda;

int message;

void contagemPulsosD() {
  totalPulsos++;
}

void contagemPulsosE() {
  totalPulsos2++;
}

void setup() {   
  Serial.begin(115200);

  myPort.begin(38400, SWSERIAL_8N1, MYPORT_RX, MYPORT_TX, false);
  if (!myPort) { 
    Serial.println("Invalid EspSoftwareSerial pin configuration, check config"); 
    while (1) { 
      delay (1000);
    }
  } 
  attachInterrupt(digitalPinToInterrupt(18), contagemPulsosD, RISING);  
  attachInterrupt(digitalPinToInterrupt(19), contagemPulsosE, RISING);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
    
  Serial.print("Conectado! IP: ");
  Serial.println(WiFi.localIP());
  myPort.println(1);
  Serial.println(1);
}

void loop() {
  if (myPort.available()){

    message = myPort.parseInt();

    voltas = (((float)totalPulsos+(float)totalPulsos2)/2) / pulsosPorRotacao;
    voltas2 = (float)totalPulsos2 / pulsosPorRotacao;
    distancia = (circunferencia*voltas)/100;


    Serial.println(distancia);    

    char buffer[50];

    if(message == 2){
        // Envia a mensagem para o host
        sprintf(buffer, "%c%f", 'a', distancia);
        //buffer[0] = 'a';
    }
    else if(message == 4){
        // Envia a mensagem para o host
        sprintf(buffer, "%c%f", 'd', distancia);
        //buffer[0] = 'd';
    }
    else if(message == 6){
        // Envia a mensagem para o host
        sprintf(buffer, "%c%f", 'e', distancia);
        //buffer[0] = 'e';
    } 
    else if(message == 8){
        // Envia a mensagem para o host
        sprintf(buffer, "%c%f", 'f', distancia);
        //buffer[0] = 'f';
    } 

    uint8_t udpBuffer[100];  
    strcpy((char*)udpBuffer, buffer);

    udp.beginPacket(host, port);
    udp.write(udpBuffer, strlen(buffer));
    udp.endPacket();

  }
}

  