#include <dht.h>
dht DHT;
#define DHT11_PIN 7
int temp; 
int est = 0;
int esta = 0;
int salida = 0;

void setup() {
  Serial.begin(9600);
  DDRD = B00111100;
  pinMode(8,INPUT);
  pinMode(9,OUTPUT);

}

void loop() {
  DHT.read11(DHT11_PIN); //Se inicializa el sensor
  Serial.print("Temperatura: ");
  Serial.println(DHT.temperature);

  Serial.print("Humedad: ");
  Serial.println(DHT.humidity);
  //Serial.println("9");
  delay(3000);

  est = digitalRead(8);

  if((est == HIGH) && (esta == LOW)){ //Se verifica el estado del pulsador
    salida = 1 - salida;
  }

  esta = est;

  if(salida == 1){// Se conmuta la  lectura de humedad y temperatura con el estado del pulsador
    
    temp = map(DHT.temperature, 20,29,0,9); //Se mapea el rango de temperatura que va de 0 a 50
    PORTD = temp<<2;
    digitalWrite(9,LOW);
    
    }

  else{
    temp = map(DHT.humidity/10, 2,9,2,9);//Se mapea el rango de humedad relativa que va de 20 a 90
    
    PORTD = temp<<2;
    digitalWrite(9,HIGH);
    
  }


  
  

}
