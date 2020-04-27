//[sourcecode languaje=»cpp»]
/*
Web client

Circuit:
* Ethernet shield attached to pins 10, 11, 12, 13

*/
#include <Adafruit_Fingerprint.h>
#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <EEPROM.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
LiquidCrystal_I2C lcd(0x27,16,2);
IPAddress server(192, 168, 0, 16);


// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;
SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);


int potenciometro = 0;
int pos = 0;
int id = 22;
int hl = 0;
int est = 0;
int co = 100;
int esta = 0;
int obj = 1;
String val;
void setup() {

id = EEPROM.read(1);
pinMode(8,INPUT);

lcd.init();
  lcd.backlight();
  lcd.clear();
Serial.begin(9600);
while (!Serial); 
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
// start the Ethernet connection:

if (Ethernet.begin(mac) == 0) {
Serial.println("Failed to configure Ethernet using DHCP");
// no point in carrying on, so do nothing forevermore:
for(;;)
;
}


// give the Ethernet shield a second to initialize:
delay(1000);
Serial.println("connecting…");
lcd.setCursor(0,0);
  lcd.print(" ...CARGANDO...");
   while (co >= 0) {
    est = digitalRead(8);
    
  if((est == HIGH) && (esta == LOW)){ //Se verifica el estado del pulsador
    obj = 1-obj;
    lcd.clear();
    
    if(obj == 1){
      lcd.setCursor(0,0);
      lcd.print(">> ACCESO");
      lcd.setCursor(0,1);
      lcd.print("   NUEVO");
    }
    if(obj == 0){
      lcd.setCursor(0,0);
      lcd.print("   ACCESO");
      lcd.setCursor(0,1);
      lcd.print(">> NUEVO");
    }
  }
  
  esta = est;
    
    delay(100);
    co = co-1;
    
   }



}

void loop()
{
if(obj == 1){
 if (pos == 1){
  if (client.connect(server, 80)) {

    
Serial.println("connected");


client.print("GET /oximerch/pages/forms/regacc.php?valor=");
client.print(potenciometro);
client.println(" HTTP/1.0");

//client.println(«GET /ingenieros/comunicaciones/arduino/ethernet.php HTTP/1.0»);
client.println();
client.stop();
lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("   REGISTRADO");
    lcd.setCursor(0,1);
    lcd.print("     #ID: ");
    lcd.print(val);
    delay(3000);
tone(6,1000,500);
}
else {
// kf you didn’t get a connection to the server:
Serial.println("connection failed");
}
 
if (client.available()) {
char c = client.read();
Serial.print(c);
}
pos = 0;
 }
else {
  lcd.setCursor(0,0);
  lcd.print("    OXIMERC     ");
  lcd.setCursor(0,1);
  lcd.print(" ...LEYENDO...");
    uint8_t p = finger.getImage();
  
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  tone(6,300,1000);
  potenciometro = finger.fingerID;
  val = String(finger.fingerID);
  
    
    
  pos=1;
  
}



}
else{
  tone(6,600,500);



  
  
  
  int p = -1;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Coloque el dedo");
  delay(600);
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("OK");
      delay(600);
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    
    
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    
    default:
      Serial.println("Unknown error");
      return p;
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Retirelo");
  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  
  Serial.print("ID "); Serial.println(id);
  p = -1;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Coloque el dedo");
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    
    
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    
    
    
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } 
    
    
     
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    
    if(hl == 0){
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("OK.OK");
    delay(600);
    }
    if(hl == 1){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("HECHO #ID: ");
    lcd.print(id);
    EEPROM.write(1,id+1);
    delay(5000);
    obj = 1;
    }
    hl = hl+1;
    
    Serial.println("Stored!");
  } 
   
  
}
}
