/* Create a WiFi access point and provide a web server on it. */
/* Once data file is uploaded... access at 192.168.4.1/index.html */

//added a few simple patterns
//https://github.com/hydronics2/ESP8266---PCC-October-2017-Class


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>

#include <Adafruit_NeoPixel.h>

#define PIN 5           // Shows up as D1 on ESP8266

#define NUMBER_LEDs 5
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMBER_LEDs, PIN, NEO_GRB + NEO_KHZ800);


int red = 0;
int green = 0;
int blue = 0;

int lastRed = 0;
int lastGreen = 0;
int lastBlue = 0;

int currentPattern = 4;

int allOff = 0;

int wheelNumber = 0;
int change = 1;

/* Set these to your desired credentials. */
const char WiFiAPPSK[] = "jasper";

ESP8266WebServer server(80);


File fsUploadFile;

//format bytes
String formatBytes(size_t bytes){
  if (bytes < 1024){
    return String(bytes)+"B";
  } else if(bytes < (1024 * 1024)){
    return String(bytes/1024.0)+"KB";
  } else if(bytes < (1024 * 1024 * 1024)){
    return String(bytes/1024.0/1024.0)+"MB";
  } else {
    return String(bytes/1024.0/1024.0/1024.0)+"GB";
  }
}

String getContentType(String filename){
  if(server.hasArg("download")) return "application/octet-stream";
  else if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(String path){

  Serial.println(path);

  if(path.endsWith(")"))//these functions parse out RGB
  {
    String rgb = path.substring(5);
    int index = rgb.indexOf(',');
    String r = rgb.substring(0,index);
    Serial.println(r);
    String gb = rgb.substring(index+1);
    index = gb.indexOf(',');
    String g = gb.substring(0,index);
    Serial.println(g);
    int index2 = gb.indexOf(')');
    String b = gb.substring(index+1,index2);
    Serial.println(b);
  
    red = r.toInt();
    green = g.toInt();
    blue = b.toInt();
  }

  if(path.endsWith("pattern-1"))//next pattern indicator
  {
    if(currentPattern < 5)
    { 
      currentPattern++;
    }else{
      currentPattern = 0;
    }
    Serial.print("next pattern:");
    Serial.println(currentPattern);
    
  }

  if(path.endsWith("power-0"))//turns all LEDs off
  {
    Serial.println("turn all off");
    allOff = 1;
  }
  
  //these functions are needed for sending js content to the phone
  if(path.endsWith("/")) path += "index.htm";
  String contentType = getContentType(path);  
  String pathWithGz = path + ".gz";
  if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){
    if(SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

 //
void handleRoot() {

    File file = SPIFFS.open("/index.html", "r");
    size_t sent = server.streamFile(file, "text/html");
    file.close();
}

void setup() {
	delay(1000);
	Serial.begin(115200);
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
	Serial.println();
	Serial.print("Configuring access point...");
	/* You can remove the password parameter if you want the AP to be open. */
	//WiFi.softAP(ssid, password);

  String AP_NameString = "OMSI Featured Stairwell Lights";

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i=0; i<AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, WiFiAPPSK);
  
	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);
	server.on("/", handleRoot);

 //serves all SPIFFS with 24hr max-age control
  server.serveStatic("/font", SPIFFS, "/font","max-age=86400"); 
  server.serveStatic("/js",   SPIFFS, "/js"  ,"max-age=86400"); 
  server.serveStatic("/css",  SPIFFS, "/css" ,"max-age=86400");


  SPIFFS.begin();


  server.onNotFound([](){
    if(!handleFileRead(server.uri()))
      //server.send(404, "text/plain", "FileNotFound_server.onnotfound");
       server.send(200, "text/html", "<a href='index.html'><font size='7'>COLOR PICKER!</a>");
  });

  server.begin();
  Serial.println("HTTP server started");


  pinMode(BUILTIN_LED, OUTPUT); //built in LED pin on ESP8266 Sparkfun dev board

}



void loop() {
	server.handleClient();

  if(red != lastRed || green != lastGreen || blue != lastBlue){
    Serial.println("color change");
    currentPattern = 0;
    colorWipe(strip.Color(red, green, blue));  //changes the color selected
    lastRed = red;
    lastGreen = green;
    lastBlue = blue;
  }

  if(allOff)
  {
    currentPattern = 0;
    red = 0;
    green = 0;
    blue = 0;
    lastRed = 0;
    lastGreen = 0;
    lastBlue = 0;
    colorWipe(strip.Color(red, green, blue));
    allOff = 0;
  }

  if(currentPattern == 1)
  {
    for(int i=0; i<strip.numPixels(); i++) 
    {
      strip.setPixelColor(i, Wheel(wheelNumber));
    }
    strip.show();
    wheelNumber = wheelNumber + change;
    delay(50);
  }

  if(currentPattern == 2)
  {
    for(int i=0; i<strip.numPixels(); i++) 
    {
      strip.setPixelColor(i, Wheel(wheelNumber));
    }
    strip.show();
    wheelNumber = wheelNumber + change;
    if(wheelNumber == 80)
      change = -1;
    delay(50);
  }

  if(currentPattern == 3)
  {
    for(int i=0; i<strip.numPixels(); i++) 
    {
      strip.setPixelColor(i, Wheel(wheelNumber));
    }
    strip.show();
    wheelNumber = wheelNumber + change;
    if(wheelNumber == 160)
      change = -1;
    if(wheelNumber == 85 && change == -1)
      change = 1;
    delay(50);
  }

  if(currentPattern == 4)
  {
    for(int i=0; i<strip.numPixels(); i++) 
    {
      strip.setPixelColor(i, Wheel(wheelNumber));
    }
    strip.show();
    wheelNumber = wheelNumber + change;
    if(wheelNumber == 170 && change == -1)
      change = 1;    
    delay(50);
  }

  if(wheelNumber == 0)
    change = 1;
  if(wheelNumber == 254)
    change = -1;
  if(currentPattern > 0)
    Serial.println(wheelNumber);
  
}



// Fill the dots one after the other with a color
void colorWipe(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  //WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}



