#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

byte LecturaUID[4];
byte Usuario1[4] = {0x47, 0xA7, 0x06, 0x68};
byte Usuario2[4] = {0x3C, 0x1E, 0x36, 0x32};
int led = 6;
int led2 = 5;

byte tiempo = 0;
byte segundos = 0;
byte cuentaRegresiva = 0;

boolean bandera = false;
boolean bandera2 = false;

void setup() {
  // put your setup code here, to run once:
  pinMode (led ,OUTPUT);
  pinMode (led2 ,OUTPUT);
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
}

void loop() {
  Serial.println(segundos);
  if(bandera == false && segundos >= 3){
    avisarApagado();
  }
  if(bandera == false) {
    antena();
  }
  if(segundos == 60) {
    tiempo++;
    segundos = 0;
    if(tiempo == 1){  // <<<--- Minutos 
      //digitalWrite(led, LOW);
      bandera = false;
      tiempo = 0;
    }
  }
  segundos++;
  delay(1000);
  // mfrc522.PICC_HaltA();
}

boolean comparaUID(byte a[], byte b[]){
  for(byte i=0; i< mfrc522.uid.size;i++){
    if(a[i] != b[i]) return(false);
  }
  return(true);
}

void avisarApagado(){
  if(segundos >= 30 && bandera2 == true){
    if(segundos % 2 == 0){
      digitalWrite(led2, HIGH);
    } else {
      digitalWrite(led2, LOW);
    }
    if(segundos==55){
      digitalWrite(led, LOW);
      digitalWrite(led2, LOW);
      // no volver a ejecutar
      bandera2=false;
    }
  }
}

void antena(){
  // put your main code here, to run repeatedly:
  if(!mfrc522.PICC_IsNewCardPresent()) return;
  if(!mfrc522.PICC_ReadCardSerial()) return;
  
  // Serial.print("UID: ");
  for(byte i=0; i< mfrc522.uid.size; i++){
    if(mfrc522.uid.uidByte[i] < 0x10){
      Serial.print("0");
    }else{
      Serial.print(" ");
    }
    //Serial.print(mfrc522.uid.uidByte[i], HEX);
    LecturaUID[i]=mfrc522.uid.uidByte[i];
  }
  // Serial.print("\t");
  if(comparaUID(LecturaUID,Usuario1) || comparaUID(LecturaUID,Usuario2)){
    // ##################################################################################################
     Serial.println("Usuario Aceptado");
     digitalWrite(led, HIGH);
     bandera = true;
     bandera2 = true;
     digitalWrite(led2, LOW);
     segundos = 0;
     tiempo = 0;
  } else {
    Serial.println("Usuario No Aceptado");
  }
  // ####################################################################################################
}
