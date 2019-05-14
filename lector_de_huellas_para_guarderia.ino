#include <Adafruit_Fingerprint.h>

//variable donde guardaremos el valor de la fotoresistencia
float lightValue;
//variable ajustable que define el valor limite de la fotoresistencia que representa la presencia de un dedo sobre el lector
float lightThreshold = 680.00;
//definiendo los pines sobre los cuales esta conectado el lector de huellas (Aqui son los pines D2 - cable verde y D3 - blanco)
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
float finger_id; // variable donde guardaremos el ID del dedo escaneado
uint8_t id; // variable donde guardaremos el ID de una nueva huella o de una huella que será borrada
String bufer; // variable donde guardaremos nuestro payload
String bufer2="\n";   // agregamos un salto de linea al final de nuestro payload

void setup() {
  Serial.begin(9600);
  // lo que sigue es el setup del lector de huellas
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit lector de huellas ");
  
  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("¡Lector de huellas detectado!");
  } else {
    Serial.println("No se pudo detectar un lector de huellas :(");
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print("El lector contiene "); Serial.print(finger.templateCount); Serial.println(" huellas registradas");
}

// función que usamos para identificar una huella escaneada por el lector
// regresa el ID de la huella o -1 si la huella no existe o que hubo un error
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  int seconds = 0;
  while (p == FINGERPRINT_NOFINGER && seconds <= 10)
  {
    p = finger.getImage();
    seconds += 1;
    delay(1000);
  }
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("ID #"); Serial.print(finger.fingerID); 
  Serial.print(" identificado con una confianza de "); Serial.println(finger.confidence);
  return finger.fingerID; 
}

void add_int(int var2)    //funcion para agregar enteros al payload (hasta 255)
{
  byte* a2 = (byte*) &var2; //convertimos el dato a bytes
  String str2;
  str2=String(a2[0], HEX);  //convertimos el valor hex a string 
  //verificamos si nuestro byte esta completo
  if(str2.length()<2)
  {
    bufer+=0+str2;    //si no, se agrega un cero
  }
  else
  {
    bufer+=str2;     //si esta completo, se copia tal cual
  }
}

void send_message(String payload)
{
  delay(500);
  //agregamos el salto de linea "\n"
  bufer+=bufer2;
  //*******************
  //Habilitamos el modulo Sigfox
  digitalWrite(7, HIGH);
  delay(1000);
  //Reset del canal para asegurar que manda en la frecuencia correcta
  Serial.print("AT$RC\n"); 
  //************************
  //Enviamos la informacion por sigfox
  Serial.print(bufer);
  delay(3000);
  //deshabilitamos el modulo Sigfox
  digitalWrite(7, LOW);
}

// función para leer un número en entrada en la terminal que no sea 0
uint8_t readnumber(void) {
  uint8_t num = 0;
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

// función para registrar una nueva huella en el lector
uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Esperando por dedo valido para registrar bajo el ID #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Scan completado");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Error de communicación");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Error en el proceso del scan");
      break;
    default:
      Serial.println("Error desconocido");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Imagen convertida");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Imagen no lo suficientemente nítida");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Error de communicación");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("No se pudo identificar los atributos de la huella");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("No se pudo identificar los atributos de la huella");
      return p;
    default:
      Serial.println("Error desconocido");
      return p;
  }
  
  Serial.println("Levante el dedo");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Por favor, vuelva a escanear el mismo dedo");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Scan completado");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Error de communicación");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Error en el proceso del scan");
      break;
    default:
      Serial.println("Error desconocido");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Imagen convertida");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Imagen no lo suficientemente nítiday");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Error de communicación");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("No se pudo identificar los atributos de la huella");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("No se pudo identificar los atributos de la huella");
      return p;
    default:
      Serial.println("Error desconocido");
      return p;
  }
  
  // OK converted!
  Serial.print("Creando modelo para #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("¡Huellas correspondientes!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Error de communicación");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Las huellas no son iguales");
    return p;
  } else {
    Serial.println("Error desconocido");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Guardado!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Error de comunicación");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("No se pudo guardar en este lector");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error escribiendo al flash");
    return p;
  } else {
    Serial.println("Error desconocido");
    return p;
  }   
}

uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;
  
  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.println("Borrado!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Error de comunicación");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("No se pudo borrar de este lector");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error escribiendo al flash");
    return p;
  } else {
    Serial.print("Error desconocido: 0x"); Serial.println(p, HEX);
    return p;
  }   
}

void loop(){
  bufer = "AT$SF="; // Creando el payload que le enviaremos a Sigfox
  lightValue = analogRead(A0); // leyendo el valor de la fotoresistencia conectada al pin A0
  finger_id = -2;
  if (lightValue > lightThreshold) // si un dedo esta sobre el lector de huellas, este se prende
  {
    finger_id = getFingerprintIDez(); // scan de la huella
    add_int(finger_id); // el ID de la huella se agrega al payload
    if (finger_id != -1 && finger_id != -2 && finger_id != 1){ // si la huella está registrada como la de un niño
      add_int(0);
      add_int(0); // agregamos 0000 al payload para que cada envio sea del mismo tamaño
      send_message(bufer); // enviamos el payload a Sigfox
      Serial.print("Bienvenido! Finger ID: ");Serial.println(finger_id);
      finger_id = -2;
    }
    if (finger_id == 1){ // el ID 1 del lector tiene que ser el de la huella del administrador
      int choice = -1;
      Serial.print("Bienvenido! Finger ID: ");Serial.println(finger_id);
      Serial.println("¿Qué quiere hacer? 1: Registrar una huella nueva, 2: Borrar huella existente");
      choice = readnumber();
      if (choice == 1){
        add_int(choice);
        Serial.println("¡Listo para registrar une huella nueva!");
        Serial.println("Por favor ingrese el ID # (entre 1 y 127) que quiere usar para registrar esta huella...");
        id = readnumber();
        if (id == 0) {// ID #0 not allowed, try again!
           return;
        }
        Serial.print("Registrando al ID #");
        Serial.println(id);
        add_int(id);
        getFingerprintEnroll();
        send_message(bufer);
      }
      else if (choice == 2){
        add_int(choice);
        Serial.println("Por favor ingrese el ID existiente # (entre 1 y 127) que quiere borrar...");
        uint8_t id = readnumber();
        if (id == 0) {// ID #0 not allowed, try again!
           return;
        }
        Serial.print("Borrando el ID #");
        Serial.println(id);
        add_int(id);
        deleteFingerprint(id);
        send_message(bufer);
      }
    }
  }
}
