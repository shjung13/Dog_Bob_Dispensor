/*
 WiFiEsp example: WebServerLed
 
 A simple web server that lets you turn on and of an LED via a web page.
 This sketch will print the IP address of your ESP8266 module (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 13.

 For more details see: http://yaab-arduino.blogspot.com/p/wifiesp.html
*/

#include "WiFiEsp.h"

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

char ssid[] = "gicheol";            // your network SSID (name)
char pass[] = "12231223";        // your network password
int status = WL_IDLE_STATUS;
int response_type;
// DC A pin number 4
int pin_DC_A = 4;
// DC B pin number 5
int pin_DC_B = 5;


WiFiEspServer server(80);

// use a ring buffer to increase speed and reduce memory allocation
RingBuffer buf(25);

void setup()
{
  Serial.begin(115200);   // initialize serial for debugging
  Serial1.begin(9600);    // initialize serial for ESP module
  WiFi.init(&Serial1);    // initialize ESP module

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // DC A pin Output Setup
  pinMode(pin_DC_A, OUTPUT);
  // DC B pin Output Setup
  pinMode(pin_DC_B, OUTPUT);

  Serial.println("You're connected to the network");
  printWifiStatus();
  
  // start the web server on port 80
  server.begin();
}

// DC Motor control
// Motor CW
void Motor_CW(unsigned char Speed)
{  
  digitalWrite(pin_DC_A, LOW);
  analogWrite(pin_DC_B, Speed);
}

// Motor CCW
void Motor_CCW(unsigned char Speed)
{  
  digitalWrite(pin_DC_B, LOW);
  analogWrite(pin_DC_A, Speed);
}

// Stop
void Stop(void)
{  
  digitalWrite(pin_DC_A, LOW);
  digitalWrite(pin_DC_B, LOW);
}

void loop()
{
  WiFiEspClient client = server.available();  // listen for incoming clients

  if (client) {                               // if you get a client,
    Serial.println("New client");             // print a message out the serial port
    buf.init();                               // initialize the circular buffer
    while (client.connected()) {              // loop while the client's connected
      if (client.available()) {               // if there's bytes to read from the client,
        char c = client.read();               // read a byte, then
        buf.push(c);                          // push it to the ring buffer

        // printing the stream to the serial monitor will slow down
        // the receiving of data from the ESP filling the serial buffer
        //Serial.write(c);
        
        // you got two newline characters in a row
        // that's the end of the HTTP request, so send a response
        if (buf.endsWith("\r\n\r\n")) {
          break;
        }

        else if (buf.endsWith("GET / ")) 
          response_type = 1;
        else if (buf.endsWith("GET /remain.cgi")) 
          response_type = 2;        
        else if (buf.endsWith("GET /feed.cgi")) 
          response_type = 3;
      }
    }

    switch(response_type){
        case 1: show_main_page(client); break;
        case 2: show_remain(client); break;
        case 3: show_feed(client); break;
    }
    
    // close the connection
    client.stop();
    Serial.println("Client disconnected");
  }
}


void show_remain(WiFiEspClient client)
{
  
  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  
  // the content of the HTTP response follows the header:
  client.println("<html>");
  client.println("<head>");
  client.println("<meta charset='UTF-8'>");
  client.println("</head>");
  
  client.println("<body>");
  
  client.print("Remaining BOB Status is : ");
  client.print(analogRead(0));
  client.println("gram.");
  
  client.println("<br /><br />");
  client.println("<a href=\"/\">Main Control Page</a>");

  client.println("</body>");
  client.println("</html>");
  // The HTTP response ends with another blank line:
  client.println();
}




void show_feed(WiFiEspClient client)
{
  int Speed;
  
  for(Speed=0; Speed <256; Speed++)
  {
   Motor_CW(Speed);
   delay(20);
  }
  
  delay(1000);
  Stop();

  
  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  
  // the content of the HTTP response follows the header:
  client.println("<html>");
  client.println("<head>");
  client.println("<meta charset='UTF-8'>");
  client.println("</head>");
  
  client.println("<body>");
  
  client.print("Feeding successful!!");
  
  client.println("<br /><br />");
  client.println("<a href=\"/\">Main Control Page</a>");

  client.println("</body>");
  client.println("</html>");
  // The HTTP response ends with another blank line:
  client.println();
}


void show_main_page(WiFiEspClient client)
{
  
  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  
  // the content of the HTTP response follows the header:
  client.println("<html>");
  client.println("<body>");
  
  client.print("Hello Everybody~~~~");
  
  client.println("<br />");
  client.println("<FORM method=\"get\" action=\"/remain.cgi\">");
  client.println("<P><INPUT type=\"submit\" value=\"REMAIN?\"></FORM>");
  client.println("<FORM method=\"get\" action=\"/feed.cgi\">");
  client.println("<P><INPUT type=\"submit\" value=\"FEED NOW\"></FORM>");
  
  client.println("</body>");
  client.println("</html>");
  // The HTTP response ends with another blank line:
  client.println();
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in the browser
  Serial.println();
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
  Serial.println();
}
