#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>



#define REQ_BUF_SZ   80

int RA=0;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };//mac
byte gateway[] = { 192,168,1,1}; //puerta
byte subnet[] = { 255, 255, 255, 0 };//mascara
IPAddress ip(192,168,1,200); // configura la ip
EthernetServer server(80);  // crea un servidor en el puerto 80
File webFile;               // la pagina de la sd
char HTTP_req[REQ_BUF_SZ] = {0}; 
char req_index = 0;              
boolean CheckState[5] = {0};
int passw=1;
int ok=0;
int okB=0;
int okC=0;
int okD=0;

void setup()
{
    //desabilitamos el chip del ethernet para usar la sd
   pinMode(10, OUTPUT);
   digitalWrite(10, HIGH);
   pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
    
           
    
    // inicializamos sd
    
   if (!SD.begin(4)) {
       
       return;    
    }
    
    
    if (!SD.exists("index.htm")) {
     
        return;  
    }
    
  
  Serial.begin(9600); 
Ethernet.begin(mac, ip);  //  le da los valores que antes indicamos de ip y mac al ethernet shield  
server.begin();           // se pone a ver si hay clientes
    
    //setamos el pin que maneja el relé en nuesto caso el pin 2
   
    pinMode(2,OUTPUT);
    // Atencion! no es posible usar pinMode(4, OUTPUT) porque no se puede usar el pin 4 con ethernet shield
    //ponemos el pin en estado bajo
    digitalWrite(2,LOW);
   
    
    
    
}

void loop() {
  // put your main code here, to run repeatedly:

}
