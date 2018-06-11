#include <SoftwareSerial.h>
bool lendo = false, estavaLendo = false;
char c, cartao[12] = {};
int idxCartao = 0;
SoftwareSerial portaRFID(4, 5);
void setup() {
  Serial.begin(9600);
  portaRFID.begin(9600);
  Serial.println("Inicio");
}
void loop() {
  while(portaRFID.available()) {
    if(idxCartao == 12) {
      lendo = false;
      idxCartao = 0;
    }
    c = portaRFID.read();
    if(c == 2) {
      lendo = true;
    } else if (lendo) {
      cartao[idxCartao] = c;
      idxCartao++;
    }
  }
  if(estavaLendo && !lendo) if(Serial.print(cartao)) Serial.println(idxCartao);
  estavaLendo = lendo;
}
