/*********
 Creds to  Rui Santos
  Complete project details at https://randomnerdtutorials.com/esp8266-web-server/
  and Kevin Zhang
*********/

//Import appropriate libraries
#include <WiFiServerSecure.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <PubSubClient.h>

//Device Specific Definitions for IBM Cloud
#define ORG "2uvr4f"
#define DEVICE_TYPE "ESP8266_NodeMCU"
#define DEVICE_ID "AER201"
#define TOKEN "O6!_pNhOrq7edu)xfx"

// Replace with your network credentials
const char* ssid     = "SM-G950W3544";
const char* password = "ainb0725";

//for IBM stuff:
char dataReceive[2];

//Set up basics for IBM server
char server2[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/status/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

//Set up WiFi client
WiFiClient wifiClient;


// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String outputState = "off";
//String output4State = "off";

// Assign output variables to GPIO pins
const int output = 5; //D2
//const int output4 = 4;

void setup() {
  Serial.begin(9600);
  // Initialize the output variables as outputs
  pinMode(output, OUTPUT);
  //pinMode(output4, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output, LOW);
  //digitalWrite(output4, LOW);


  // Connect to Wi-Fi network with SSID and password
  //Serial.print("Connecting to ");
  //Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    //Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /5/on") >= 0) {
              //Serial.println("Robot Started");
              outputState = "";
              digitalWrite(output, HIGH);
            } else if (header.indexOf("GET /5/off") >= 0) {
              //Serial.println("Robot Stopped");
              outputState = "";
              digitalWrite(output, LOW);
            }/* else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 4 on");
              output4State = "on";
              digitalWrite(output4, HIGH);
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 off");
              output4State = "off";
              digitalWrite(output4, LOW);
            }
            */
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>Ball Robot Web Server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 5  
            client.println("<p>Remote Start" + outputState + "</p>");
            // If the output5State is off, it displays the ON button       
            if (outputState=="off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">START</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">STOP</button></a></p>");
            } 
            /*
            // Display current state, and ON/OFF buttons for GPIO 4  
            client.println("<p>GPIO 4 - State " + output4State + "</p>");
            // If the output4State is off, it displays the ON button       
            if (output4State=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            */
            client.println("</body></html>");
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
    //IBM stuff
    PubSubClient client(server2, 1883, NULL, wifiClient);
    dataReceive[0] = 0;
    while (1)
    {
      //IBM cloud connection as a client
      if (!client.connected()) {
        Serial.print("Reconnecting client to ");
        Serial.println(server2);
        while (!client.connect(clientId, authMethod, token)) {
          Serial.print(".");
          delay(200);
        }
        Serial.println();
       }

       if (Serial.available())
       {
          dataReceive[0]++;
          String payload = "{\"data\":{\"Name\":\"bcddc208ad8a\"";
          payload += ",\"PIC Data\":";
          payload += dataReceive[0];
          payload += "}}";
          client.publish(topic, (char*) payload.c_str());
       }
    }
  }
}

