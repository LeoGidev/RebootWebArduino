#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>



#define REQ_BUF_SZ   80

int RA=0;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };//mac
byte gateway[] = { 192,168,1,1}; //puerta
byte subnet[] = { 255, 255, 255, 0};//mascara
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
  EthernetClient client = server.available(); 
 
 
    if (client) {  // se fija si hay cliente
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {  
                char c = client.read(); 
                if (req_index < (REQ_BUF_SZ - 1)) {
                    HTTP_req[req_index] = c;        
                    req_index++;
                }//lee y almacena los caracteres de la solictud http del cliente
               
                if (c == '\n' && currentLineIsBlank) {
                    //busca un /n en la solictud del cliente
                    client.println("HTTP/1.1 200 OK");
                    //le envia una mensaje al cliente que dice "http/1.1200 ok" estableciendo asi la coneccion
                 
                   Serial.println("///////////////////////") ;
                   Serial.println(HTTP_req);
                         if (StrContains(HTTP_req, "ajax_inputs")) { //revisa si hay una solictud ajax
                        // Envia el encabezado HTTP 
                        client.println("Content-Type: text/xml");
                        client.println("Connection: keep-alive");
                        client.println();
                        //toma los valores recibidos del cliente con la funcion rebooteo()
                       XML_response(client);
                       

                       }
                         else if (StrContains(HTTP_req, "REBOOTER")) { //revisa si hay una solictud REBOOT
                        // Envia el encabezado HTTP 
                       
                        client.println("Content-Type: text/xml");
                        client.println("Connection: keep-alive");
                        client.println();
                        //toma los valores recibidos del cliente con la funcion rebooteo()
                         
                        Rebooteo();
                        XML_response(client);
                        }
                                 
                    
                                 
                         else if (StrContains(HTTP_req, "GET / ")|| StrContains(HTTP_req, "GET /index.htm")) {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connnection: close");
                        client.println();
                        webFile = SD.open("index.htm");        // envia la pagina web de la sd al cliente
                       
                           }
                          
                        

                         
                     
                     else if (StrContains(HTTP_req, "GET /pic.jpg")) {
                        webFile = SD.open("pic.jpg");
                        if (webFile) {
                            client.println("HTTP/1.1 200 OK");
                            client.println();}
                            }

                        if (webFile) {
                        while(webFile.available()) {
                        client.write(webFile.read()); 
                            }
                            webFile.close();
                            
                            
                        }
                    
                    
                    
                         //una vez acabada la comunicacion reseta el buffer 
                    req_index = 0;
                    StrClear(HTTP_req, REQ_BUF_SZ);
                    break;
                    
                    
                }
                
                if (c == '\n') {
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    currentLineIsBlank = false;
                }
            } 
        } 
             
        client.stop(); //cierra la coneccion
        client.flush();
        delay(100); 
    
        
    } 
   
}

// funcion que rebootea
void Rebooteo(void)
{
  EthernetClient client = server.available();
  ////////////////////////////////////////////////////////////////////////////////
   if (StrContains(HTTP_req, "&password")) {
        if (StrContains(HTTP_req, "&RebootA&")) {
        
        digitalWrite(2,HIGH);
        ok=1;
        Serial.print(F("ok:")); Serial.println(ok);
        delay(10000);
        digitalWrite(2,LOW);
       
}
    else if (StrContains(HTTP_req, "&RebootA&")==0){
    ok=0;
    
     Serial.println(F("todo en0 1"));
    }
   }else{
    ok=0;
    
    Serial.println(F("todo en0 2"));}
    
    }
