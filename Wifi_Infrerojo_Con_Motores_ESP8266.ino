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

uint8_t LED1pin = D7;
bool LED1status = LOW;

uint8_t LED2pin = D6;
bool LED2status = LOW;

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
 // servoTovilloDerecho.attach(0); //D3
 // servoPieDeerecho.attach(2); //D4
 // servoMotorManos.attach(12); //D6
 // servoCabeza.attach(13); //D7

  irrecv.enableIRIn();  // Start the receiver
  while (!Serial)  // Wait for the serial connection to be establised.
    delay(50);
  Serial.println();
  Serial.print("IRrecvDemo is now running and waiting for IR message on Pin ");
  Serial.println(kRecvPin);


  // ----- Servicio wifi -----

  pinMode(LED1pin, OUTPUT);
  pinMode(LED2pin, OUTPUT);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.on("/led2on", handle_led2on);
  server.on("/led2off", handle_led2off);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");

  // ----- Servicio wifi -----

  
}

void loop() {

  // ----- Servicio wifi -----

  server.handleClient();
  
  if(LED1status)
  {digitalWrite(LED1pin, HIGH);}
  else
  {digitalWrite(LED1pin, LOW);}
  
  if(LED2status)
  {digitalWrite(LED2pin, HIGH);}
  else
  {digitalWrite(LED2pin, LOW);}
  
  // ----- Servicio wifi -----

  
  if (irrecv.decode(&results)) {
    int val = results.value;
    valorControl(val);
    Sevomotor();
    irrecv.resume();  // Receive the next value

    Serial.print("valor: ");
    Serial.println(val);

    delay(100);
  }
  
  Sevomotor();

  
}

void valorControl(int vereficar){
    if( vereficar == 16738455 ){ // al apretar 0, 
      statusWifioInfra = 1;
     }

    if( vereficar == 16724175 ){ // al apretar 1
      statusWifioInfra = 2;
     }

    if( vereficar == 16718055 ){ // al apretar 2
      statusWifioInfra = 3;
     }

    if( vereficar == 0 ){
      statusWifioInfra = 0;
     }
  
  }

void Sevomotor(){

  switch (statusWifioInfra) {
  case 1:
    // al apretar 0
    servoTovilloIzquierdo.write(45);
    servoPieIzquierdo.write(50);
   /* servoTovilloDerecho.write(0);
    servoPieDeerecho.write(0);
    servoMotorManos.write(0);
    servoCabeza.write(0); */
    
      Serial.println("servo motro 0");
    break;
  case 2:
    // al apretar 1
    servoTovilloIzquierdo.write(95);
    servoPieIzquierdo.write(105);
   /* servoTovilloDerecho.write(90);
    servoPieDeerecho.write(90);
    servoMotorManos.write(90);
    servoCabeza.write(90);*/

    
      Serial.println("servo motro 90");
    break;
  case 3:
    // al apretar 2
    servoTovilloIzquierdo.write(135);
    servoPieIzquierdo.write(160);
   /* servoTovilloDerecho.write(180);
    servoPieDeerecho.write(180);
    servoMotorManos.write(180);
    servoCabeza.write(180); */

    
      Serial.println("servo motro 180");

    break;
}

}

  // ----- Servicio wifi -----
  
  
void handle_OnConnect() {
  statusWifioInfra = 0;
  Serial.println("GPIO7 Status: OFF | GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status)); 
}

void handle_led1on() {
  statusWifioInfra = 1;
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(true,LED2status)); 
}

void handle_led1off() {
  statusWifioInfra = 0;
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(false,LED2status)); 
}

void handle_led2on() {
  statusWifioInfra = 2;
  Serial.println("GPIO6 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status,true)); 
}

void handle_led2off() {
  statusWifioInfra = 0;
  Serial.println("GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,false)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led1stat,uint8_t led2stat){
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
  ptr +="<h1>ESP8266 Web Server</h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";
  
   if(led1stat)
  {ptr +="<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";}
  else
  {ptr +="<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";}

  if(led2stat)
  {ptr +="<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/led2off\">OFF</a>\n";}
  else
  {ptr +="<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/led2on\">ON</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
  
  // ----- Servicio wifi -----
