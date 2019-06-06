#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include <SPI.h>
#include <WiFi101.h>
#include "arduino_secrets.h"
int flexPin = 4;

//FINGERS 
#define RUBBERCORDPIN0 A0  //This is the pin where the cord is connected to
#define RUBBERCORDPIN1 A1 
#define RUBBERCORDPIN2 A2 
#define RUBBERCORDPIN3 A3 
#define RUBBERCORDPIN4 A4 


/* BNO055 Gyro-accelormeter*/ 
#define BNO055_SAMPLERATE_DELAY_MS (100)
Adafruit_BNO055 bno = Adafruit_BNO055(55);

#define INPUT_SIZE 30

int thumb_v;
int index_v;
int middle_v;
int ring_v;
int pinky_v;

//Vibrating motors PWM
int vib_thumb = 11;
int vib_index = 12;
int vib_middle = 9;
int vib_ring = 13;
int vib_pinky = 10;

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
IPAddress ip(192,168,0,3);

int keyIndex = 0;                // your network key Index number (needed only for WEP)
int debugInt = 0;

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/

int led =  LED_BUILTIN;
int status = WL_IDLE_STATUS;
boolean alreadyConnected = false; // whether or not the client was connected previously
WiFiServer server(80);


/*****
 values that hold the string from GitHub example
 https://gist.github.com/mattfelsen/9467420
 *****/
// const int numberOfPieces = 2;
// String pieces[numberOfPieces];
#define INPUT_SIZE 10
String read_string = "";
// string input = "";

 //keep counter of current position
 int counter = 0;
 //keep track of the last ':'
 int lastIndex = 0;
 

void setup(void)
{
  //Initiating the vibrating motors
  pinMode(vib_thumb, OUTPUT);
  pinMode(vib_index, OUTPUT);
  pinMode(vib_middle, OUTPUT);
  pinMode(vib_ring, OUTPUT);
  pinMode(vib_pinky, OUTPUT);

  //With the feather we are using PWM we are doing an analog write starting at 0
  analogWrite(vib_thumb, 0);
  analogWrite(vib_index, 0);
  analogWrite(vib_middle, 0);
  analogWrite(vib_ring, 0);
  analogWrite(vib_pinky, 0);
 
  WiFi.setPins(8,7,4,2);

  //Serial for wifi
  Serial.begin(9600);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }

  Serial.println("Access Point Web Server");

  pinMode(led, OUTPUT);

 // check for the presence of the shield:
   if (WiFi.status() == WL_NO_SHIELD) {
     Serial.println("WiFi shield not present"); 
     // don't continue:
     while(true);
   } 

    WiFi.config(ip);

       // print the network name (SSID);
    Serial.print("Creating access point named: ");
    Serial.println(ssid);

    // Create open network. Change this line if you want to create an WEP network:
    status = WiFi.beginAP(ssid);
    if (status != WL_AP_LISTENING) {
      Serial.println("Creating access point failed");
      // don't continue
    while (true);
    }
    
     // wait 10 seconds for connection:
     delay(10000);
  
    server.begin(); 
     // you're connected now, so print out the status:
     printWiFiStatus(); 
     
     SetupIMU(); //set up the IMU
}


void SetupIMU(){ 
  Serial.begin(1200);
  Serial.println("Orientation Sensor Raw Data Test"); Serial.println("");

   /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  delay(1000);

  bno.setExtCrystalUse(true);

  uint8_t system, gyro, accel, mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);

    Serial.print("\t");
  if (!system)
  {
    Serial.print("! ");
  }
 
  /* Display the individual values */
  Serial.print("Sys:");
  Serial.print(system, DEC);
  Serial.print(" G:");
  Serial.print(gyro, DEC);
  Serial.print(" A:");
  Serial.print(accel, DEC);
  Serial.print(" M:");
  Serial.println(mag, DEC);
   /* Display some basic information on this sensor */
  displaySensorDetails();
  
}

void displaySensorDetails(void)
{
  sensor_t sensor;
  bno.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void)
{
    // compare the previous status to the current status
  if (status != WiFi.status()) {
    // it has changed update the variable
    status = WiFi.status();

    if (status == WL_AP_CONNECTED) {
      byte remoteMac[6];

      // a device has connected to the AP
      Serial.print("Device connected to AP, MAC address: ");
      WiFi.APClientMacAddress(remoteMac);
      printMacAddress(remoteMac);
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
    }
  }

    WiFiClient client = server.available();   // Wait for a new client

    if (client) {                             // if you get a client,
        Serial.println("new client");           // print a message out the serial port
        String currentLine = "";                // make a String to hold incoming data from the client
          while (client.connected()) {            // loop while the client's connected

              //Rubber Core 
              thumb_v = analogRead(RUBBERCORDPIN0);
              index_v = analogRead(RUBBERCORDPIN1);
              middle_v = analogRead(RUBBERCORDPIN2);
              ring_v = analogRead(RUBBERCORDPIN3);
              pinky_v = analogRead(RUBBERCORDPIN4);

              if(thumb_v != 1023 || index_v != 1023 || middle_v != 1023 || ring_v != 1023 || pinky_v != 1023)
              {

                thumb_v = map(thumb_v, 60, 68, -38, -70);
                index_v = map(index_v, 95, 112, -9, -70);
                middle_v = map(middle_v, 80, 115, -8.6, -70);
                ring_v = map(ring_v, 76, 120, -9, -70);
                pinky_v = map(pinky_v, 115, 150, -6.5, -70); 
                
                //IMU Sending obtaining data
               imu::Quaternion quat = bno.getQuat();
               
               String message = String(thumb_v) + "," + String(index_v) + "," + String(middle_v) + "," + String(ring_v) + "," + String(pinky_v) +  "/" + String(quat.x(), 4) + "," + String(quat.y(), 4) + "," +  String(quat.z(), 4) + "," + String(quat.w(),4);
               client.println(message);

              char thisChar = client.read();
              if(thisChar == '&'){
                 int fin = getValue(read_string, ':', 0).toInt();
                 int value = getValue(read_string, ':', 1).toInt();
                  SplitString(fin, value);
                  read_string = "";
               }
               else
               {
                 read_string += thisChar;   
               }               
            }
          }
             delay(100);
        } else{
             analogWrite(vib_thumb, 0);
             analogWrite(vib_index, 0);
             analogWrite(vib_middle, 0);
             analogWrite(vib_ring, 0);
             analogWrite(vib_pinky, 0);   
        }
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

//Split the string by ':'
void SplitString(int finger, int value){
    Serial.println(String(finger) + "|" + String(value));
    analogWrite(finger, value);
}
 
void printWiFiStatus() {
// print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}
