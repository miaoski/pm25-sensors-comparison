// Compare PM 2.5 detectors: A4 vs. G3
// Compare temperature sensors: DHT-22 vs. SHT-31
// Runs on the Arduino part of LinkIt Smart 7688 Duo

// A4 pin 1   -> +5V
// A4 pin 2   -> GND
// A4 pin 5   -> 7688 Duo pin D0
// G3 pin 5   -> 7688 Duo pin D11
// SHT-31 SDA -> 7688 Duo pin D2
// SHT-31 SCL -> 7688 Duo pin D3
// DHT-22 DAT -> 7688 Duo pin D4

#include <Wire.h>
#include <SoftwareSerial.h>
#include <Adafruit_SHT31.h>
#include <DHT.h>
#include <AM2321.h>

#define DHTTYPE DHT22
#define DHTPIN  4
#define G3PIN   8
#define A4PIN   10

int pm10;
int pm25;
int pm100;
byte buf[32];

Adafruit_SHT31 sht31 = Adafruit_SHT31();
SoftwareSerial g3(G3PIN, G3PIN+1);
SoftwareSerial a4(A4PIN, A4PIN+1);
DHT dht(DHTPIN, DHTTYPE);
AM2321 am2321;

void setup() {
  Serial.begin(9600);
  Serial1.begin(57600);
  g3.begin(9600);
  a4.begin(9600);
  dht.begin();
  if (!sht31.begin(0x44)) {
    Serial.println("Couldn't find SHT31");
  }
  delay(500);
}

void loop() {
  float t;
  float h;
  Serial.println("A4 vs. G3 / DHT22 vs. SHT31");
  /*
  if(readA4()) {
    Serial.print("A4 :: ");
    Serial.print("PM 1.0 = ");
    Serial.print(pm10);
    Serial.print("   PM 2.5 = ");
    Serial.print(pm25);
    Serial.print("   PM 10 = ");
    Serial.println(pm100);
  }
  if(readG3()) {
    Serial.print("G3 :: ");
    Serial.print("PM 1.0 = ");
    Serial.print(pm10);
    Serial.print("   PM 2.5 = ");
    Serial.print(pm25);
    Serial.print("   PM 10 = ");
    Serial.println(pm100);
  }
  */
  t = sht31.readTemperature();
  h = sht31.readHumidity();
  delay(100);
  if(!isnan(t) && !isnan(h)) {
    Serial.print("SHT31 :: temp=");
    Serial.print(t);
    Serial.print("   hum=");
    Serial.println(h);
  } else {
    t = -1;
    h = -1;
  }
  t = dht.readTemperature();
  h = dht.readHumidity();
  if(!isnan(t) && !isnan(h)) {
    Serial.print("AM2302 :: temp=");
    Serial.print(t);
    Serial.print("   hum=");
    Serial.println(h);
  } else {
    t = -1;
    h = -1;
  }
  delay(100);
  am2321.read();
  Serial.print("AM2320 :: temp=");
  Serial.print(am2321.temperature / 10.0);
  Serial.print("   hum=");
  Serial.println(am2321.humidity / 10.0);
  delay(1000);
}

boolean readA4() {
  unsigned short x;
  byte data;
  unsigned short start = 0;
  pm10 = -1;
  pm25 = -1;
  pm100 = -1;
  memset(buf, 0, 32);
  for(x = 0; x < 32;) {
    if(a4.available()) {
      data = a4.read();
      if(start == 0 && data != 0x32) {
        continue;
      } else {
        start++;
        buf[x++] = data;
        continue;        
      }
      if(start == 1 && data != 0x3D) {
        start = 0;
        continue;
      } else {
        start++;
        buf[x++] = data;
        continue;
      }
      buf[x++] = data;
    }
  }
  if(a4CheckSum(buf, 30)) {
    pm10 = ((unsigned int)buf[4] << 8) + (unsigned int)buf[5];
    pm25 = ((unsigned int)buf[6] << 8) + (unsigned int)buf[7];
    pm100 = ((unsigned int)buf[8] << 8) + (unsigned int)buf[9];
    return true;
  } else {
    return false;
  }
}

boolean a4CheckSum(byte *p, unsigned short offset) {
  unsigned short i;
  unsigned int calcsum = 0; // BM
  unsigned int exptsum;
  for(i = 0; i < offset; i++) {
    calcsum += (unsigned int)p[i];
  }
  exptsum = ((unsigned int)p[offset] << 8) + (unsigned int)p[offset+1];
  if(calcsum == exptsum) {
    return true;
  } else {
    return false;
  }
}


boolean readG3() {
  unsigned short x;
  byte data;
  boolean start = false;
  pm10 = -1;
  pm25 = -1;
  pm100 = -1;
  memset(buf, 0, 24);
  for(x = 0; x < 24;) {
    if(g3.available()) {
      data = g3.read();
      if(start == 0 && data != 0x42) {
        continue;
      } else {
        start++;
        buf[x++] = data;
        continue;        
      }
      if(start == 1 && data != 0x2D) {
        start = 0;
        continue;
      } else {
        start++;
        buf[x++] = data;
        continue;
      }
      buf[x++] = data;
    }
  }
  if(a4CheckSum(buf, 22)) {
    pm10 = ((unsigned int)buf[4] << 8) + (unsigned int)buf[5];
    pm25 = ((unsigned int)buf[6] << 8) + (unsigned int)buf[7];
    pm100 = ((unsigned int)buf[8] << 8) + (unsigned int)buf[9];
    return true;
  } else {
    return false;
  }
}

