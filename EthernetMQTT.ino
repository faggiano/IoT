#include <UIPEthernet.h>
#include <PubSubClient.h>

//
const int led_vermelho = 7 ; // alterar para o vaor da sua porta
const int led_verde = 6 ; // alterar para o vaor da sua porta
//

// alterar algum valor do MAC para não haver conflitos
byte mac[] = { 0xDE , 0xAD, 0xBE, 0xEF, 0xAB, 0xAB};


char* servidor_mqtt = "m14.cloudmqtt.com"; // alterar para o valor do seu servidor
int porta_mqtt = 14717; // alterar para o valor do seu serviço MQTT

long lastReconnectAttempt = 0 ;

void mensagem_mqtt (char* topic, byte* payload, unsigned int length) {

  char* payload_char = payload;
  payload_char[length] = 0 ;
  String payload_string = String (payload_char);
  String topic_string = String (topic);
  int payload_int = payload_string.toInt();

  Serial.print("TOPIC: ");
  Serial.println(topic_string);
  Serial.print("PAYLOAD: ");
  Serial.println(payload_string);


  if ( topic_string == "led" && payload_int != "ok" )
  {
    digitalWrite(led_verde, HIGH);
    digitalWrite(led_vermelho, HIGH);
  }else
  {
    digitalWrite(led_verde, LOW);
    digitalWrite(led_vermelho, LOW);
    }

}

EthernetClient ethernet_client;

PubSubClient pubsub( servidor_mqtt, porta_mqtt, mensagem_mqtt, ethernet_client);

boolean reconnect ()
{
  Serial.println("reconectando");
  if (pubsub.connect("Identificação UNICA por ARDUINO", "usuário do seu serviço MQTT", "Senha do seu serviço MQTT")) 
  {
    Serial.println("conectado");
    pubsub.publish("Status", "ON");
    Serial.println("Voltei Enviado");
    pubsub.subscribe("led");

  }
}

void setup() {
  // put your setup code here, to run once:
  //
  pinMode(led_vermelho, OUTPUT);
  pinMode(led_verde, OUTPUT);

  //
  Serial.begin(9600);
  Serial.println("Ligou");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Não pegou DHCP");
  }
  else {
    Serial.println("pegou endereço via DHCP");
    Serial.println(Ethernet.localIP());
  }
  if (pubsub.connect("Identificação UNICA por ARDUINO", "usuário do seu serviço MQTT", "Senha do seu serviço MQTT"))
 {
    Serial.println("Conectado");
    pubsub.publish ("Teste", "OK");
    Serial.println ("Status enviado");
  }
  else {
    Serial.println("MQTT não esta conectado");
  }
  lastReconnectAttempt = 0 ;
}



void loop() {
  // put your main code here, to run repeatedly:
  if (!pubsub.connected())
  {
    long now = millis();
    if ( now - lastReconnectAttempt > 5000 )
    {
      Serial.println("Reconectando...");
      lastReconnectAttempt = now ;
      if (reconnect())
      {
        lastReconnectAttempt = 0 ;
      }
    }
  }
  else
  {
    pubsub.loop();
  }

}
