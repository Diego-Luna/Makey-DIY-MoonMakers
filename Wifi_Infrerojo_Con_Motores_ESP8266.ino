/*
  Este codigo es para el proyecto de make robot con wifi y infrarojo
*/

/*
 * Numero : valor
 
 0: 16738455
 1: 16724175
 2: 16718055
 3: 16743045
 4: 16716015
 5: 16726215
 6: 16734885
 7: 16728765
 8: 16730805
 9: 16732845
 
*/
#include <Servo.h>
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

// ----- Servicio wifi -----
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* Put your SSID & Password */
const char* ssid = "NodeMCUMakey";  // Enter SSID here
const char* password = "nodemakey";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);


byte statusWifioInfra = 0; 

// ----- Servicio wifi -----

// An IR detector/demodulator is connected to GPIO pin 14(D5 on a NodeMCU
// board).
// Note: GPIO 16 won't work on the ESP8266 as it does not have interrupts.
const uint16_t kRecvPin = 14;

IRrecv irrecv(kRecvPin);

decode_results results;

Servo servoTovilloIzquierdo;
Servo servoPieIzquierdo;
Servo servoTovilloDerecho;
Servo servoPieDeerecho;
Servo servoMotorManos;
Servo servoCabeza;


void setup() {
  Serial.begin(115200);

  servoTovilloIzquierdo.attach(5); //D1
  servoPieIzquierdo.attach(4); //D2
  servoTovilloDerecho.attach(0); //D3
  servoPieDeerecho.attach(2); //D4
  servoMotorManos.attach(12); //D6
  servoCabeza.attach(13); //D7

  irrecv.enableIRIn();  // Start the receiver
  while (!Serial)  // Wait for the serial connection to be establised.
    delay(50);
  Serial.println();
  Serial.print("IRrecvDemo is now running and waiting for IR message on Pin ");
  Serial.println(kRecvPin);


  // ----- Servicio wifi -----

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.on("/accion1", disparadorAccion1 );
  server.on("/accion2", disparadorAccion2 );
  server.on("/accion3", disparadorAccion3 );
  server.on("/accion4", disparadorAccion4 );
  server.on("/accion5", disparadorAccion5 );

  
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started"); 

  // ----- Servicio wifi -----

  
}

void loop() {

  // ----- Servicio wifi -----

 server.handleClient();
    
  // ----- Servicio wifi -----

  
  if (irrecv.decode(&results)) {
    int val = results.value;
    Serial.print("valor: ");
    Serial.println(val); 
    valorControl(val);
    //Sevomotor();
    //SevomotorInfra(val);
    
    irrecv.resume();  // Receive the next value
    val = 0;

  }
    
  Sevomotor(); 
   
  

  delay(100);
  
}

void valorControl(int vereficar){
    if( vereficar == 16738455 ){ // al apretar 0, 
      statusWifioInfra = 1;
      Serial.println("al apretar 0");
     }

    if( vereficar == 16724175 ){ // al apretar 1
      statusWifioInfra = 2;
      Serial.println("al apretar 1");
     }

    if( vereficar == 16718055 ){ // al apretar 2
      statusWifioInfra = 3;
      Serial.println("al apretar 2");
     }
    
    if( vereficar == 16743045 ){ // al apretar 3
      statusWifioInfra = 4;
      Serial.println("al apretar 3");
     }
     if( vereficar == 16716015 ){ // al apretar 4
      statusWifioInfra = 5;
      Serial.println("al apretar 4");
     }

    /*if( vereficar == 0 ){
      statusWifioInfra = 0;
     }*/
  
  }

void Sevomotor(){

  switch (statusWifioInfra) {
  case 1:
    // al apretar 0
      servoTovilloIzquierdo.write(45); //45
      servoPieIzquierdo.write(85); //50
      delay(100);
      servoTovilloDerecho.write(45); //45
      servoPieDeerecho.write(85); //50
      delay(100);
      servoMotorManos.write(0);
      servoCabeza.write(180);

    statusWifioInfra = 0;
    
    break;
  case 2:
    // al apretar 1
    servoTovilloIzquierdo.write(95);
      servoPieIzquierdo.write(105);
      delay(100);
      servoTovilloDerecho.write(100);
      servoPieDeerecho.write(105);
      delay(100);
      servoMotorManos.write(90);
      servoCabeza.write(160);

    statusWifioInfra = 0;

    break;
  case 3:
    // al apretar 2
    servoTovilloIzquierdo.write(135); //135
      servoPieIzquierdo.write(150);   //160
      delay(100);
      servoTovilloDerecho.write(135); //135
      servoPieDeerecho.write(150);  //160
      delay(100);
      servoMotorManos.write(180);
      servoCabeza.write(20);

    statusWifioInfra = 0;

    break;
  case 4:
    // al apretar 3
    servoMotorManos.write(0);
    servoCabeza.write(100);
    delay(400);
    servoMotorManos.write(180);
    servoCabeza.write(180);
    delay(200);
    servoMotorManos.write(90);
    servoCabeza.write(160);

    statusWifioInfra = 0;
    
    break;
  case 5:
    // al apretar 4
    servoMotorManos.write(180);
    servoCabeza.write(180);
    delay(400);
    servoMotorManos.write(0);
    servoCabeza.write(160);
    delay(200);
    servoMotorManos.write(170);
    servoCabeza.write(20);

    statusWifioInfra = 0;

    
    break;
  }

 /* switch (statusWifioInfra) {
  case 1:
    // al apretar 0
    servoTovilloIzquierdo.write(45);
    servoPieIzquierdo.write(50);
    delay(100);
    servoTovilloDerecho.write(45);
    servoPieDeerecho.write(50);

    statusWifioInfra = 0;
    
    break;
  case 2:
    // al apretar 1
    servoTovilloIzquierdo.write(95);
    servoPieIzquierdo.write(105);
    delay(100);
    servoTovilloDerecho.write(100);
    servoPieDeerecho.write(105);

    statusWifioInfra = 0;

    break;
  case 3:
    // al apretar 2
    servoTovilloIzquierdo.write(135);
    servoPieIzquierdo.write(160);
    delay(100);
    servoTovilloDerecho.write(135);
    servoPieDeerecho.write(160);

    statusWifioInfra = 0;

    break;
  case 4:
    // al apretar 3
    servoMotorManos.write(90);

    statusWifioInfra = 0;
    
    break;
  case 5:
    // al apretar 4
    servoMotorManos.write(170);

    statusWifioInfra = 0;

    
    break;
  }*/

}


void SevomotorInfra(int valor){
  /*
 * Numero : valor
 
 0: 16738455
 1: 16724175
 2: 16718055
 3: 16743045
 4: 16716015
 5: 16726215
 6: 16734885
 7: 16728765
 8: 16730805
 9: 16732845
 
*/

  switch (valor) {
    case 16738455:
      // al apretar 0
      servoTovilloIzquierdo.write(45); //45
      servoPieIzquierdo.write(85); //50
      delay(100);
      servoTovilloDerecho.write(45); //45
      servoPieDeerecho.write(85); //50
      delay(100);
      servoMotorManos.write(0);
      servoCabeza.write(180);
      
      break;
    case 16724175:
      // al apretar 1
      servoTovilloIzquierdo.write(95);
      servoPieIzquierdo.write(105);
      delay(100);
      servoTovilloDerecho.write(100);
      servoPieDeerecho.write(105);
      delay(100);
      servoMotorManos.write(90);
      servoCabeza.write(160);
      
      break;
    case 16718055:
      // al apretar 2
      servoTovilloIzquierdo.write(135); //135
      servoPieIzquierdo.write(105);   //160
      delay(100);
      servoTovilloDerecho.write(135); //135
      servoPieDeerecho.write(105);  //160
      delay(100);
      servoMotorManos.write(180);
      servoCabeza.write(20);
  
      break;
   case 16743045:
    // al apretar 3
    servoMotorManos.write(0);
    servoCabeza.write(100);
    delay(400);
    servoMotorManos.write(180);
    servoCabeza.write(180);
    delay(200);
    servoMotorManos.write(90);
    servoCabeza.write(160);
    
    break;
  case 16716015:
    // al apretar 4
    /*servoMotorManos.write(170);
    servoCabeza.write(20);*/

    servoMotorManos.write(180);
    servoCabeza.write(180);
    delay(400);
    servoMotorManos.write(0);
    servoCabeza.write(160);
    delay(200);
    servoMotorManos.write(170);
    servoCabeza.write(20);
    
    break;
  }
}

  // ----- Servicio wifi -----
  

void handle_OnConnect() {
  statusWifioInfra = 0;
  server.send(200, "text/html", SendHTML());  
}

void disparadorAccion1(){
  
  if(statusWifioInfra == 1){
    statusWifioInfra = 0;
  }else {
    statusWifioInfra = 1;
    }
  
  server.send(200, "text/html", SendHTML()); 
}

void disparadorAccion2() {
  
 if(statusWifioInfra == 2){
    statusWifioInfra = 0;
  }else {
    statusWifioInfra = 2;
    }
    
  server.send(200, "text/html", SendHTML()); 
}

void disparadorAccion3(){
  if(statusWifioInfra == 3){
    statusWifioInfra = 0;
  }else {
    statusWifioInfra = 3;
    }
    
  server.send(200, "text/html", SendHTML()); 
 }

void disparadorAccion4(){
  if(statusWifioInfra == 4){
    statusWifioInfra = 0;
  }else {
    statusWifioInfra = 4;
    }
    
  server.send(200, "text/html", SendHTML()); 
 }
void disparadorAccion5(){
  if(statusWifioInfra == 5){
    statusWifioInfra = 0;
  }else {
    statusWifioInfra = 5;
    }
    
  server.send(200, "text/html", SendHTML()); 
 } 

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(){

  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>Makey Web Server</h1>\n";
  ptr +="<h3>Control web</h3>\n";

  // botton 1
  ptr +="<p>Makey piernas hacia 0 grados </p><a class=\"button button-on\" href=\"/accion1\">ON</a>\n";
  
  // botton 2
  ptr +="<p>Makey piernas centrada</p><a class=\"button button-on\" href=\"/accion2\">ON</a>\n";

  // botton 3
  ptr +="<p>Makey piernas hacia 180 grados</p><a class=\"button button-on\" href=\"/accion3\">ON</a>\n";

  // botton 4
  ptr +="<p>Makey brasos hacia 90 grados</p><a class=\"button button-on\" href=\"/accion4\">ON</a>\n";

  // botton 5
  ptr +="<p>Makey brasos hacia 180 grados</p><a class=\"button button-on\" href=\"/accion5\">ON</a>\n";


  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
  
  // ----- Servicio wifi -----
