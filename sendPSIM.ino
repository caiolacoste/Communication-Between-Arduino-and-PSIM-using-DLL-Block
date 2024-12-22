void setup() {
  Serial.begin(9600); // Inicializa a comunicação serial
}

void loop() {
  float potValue = analogRead(A1); // Lê o valor do potenciômetro
  byte* bytePtr = (byte*)&potValue; // Obtem o endereço do float como um array de bytes
  
  // Envia o valor byte a byte
  for (int i = 0; i < sizeof(potValue); i++) {
    Serial.write(bytePtr[i]);
  }

  delay(1000); // Aguarda 1 segundo antes de enviar novamente
}

// void loop() {
//     static unsigned long lastMicros = 0;
//     unsigned long currentMicros = micros();

//     // Envia dados somente a cada 20 us
//     if (currentMicros - lastMicros >= 20) {
//       lastMicros = currentMicros;
//       float potValue = analogRead(A1); // Lê o valor do potenciômetro
//       byte* bytePtr = (byte*)&potValue; // Obtem o endereço do float como um array de bytes
      
//       // Envia o valor byte a byte
//       for (int i = 0; i < sizeof(potValue); i++) {
//         Serial.write(bytePtr[i]);
//       }
//     }
// }


