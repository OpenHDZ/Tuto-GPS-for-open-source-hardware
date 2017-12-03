 /*
Programme qui enregistre les données de déplacement GPS sur une carte SD .

Ecrit par Areour Mohamed Cherif
Date : 15/05/2017
E-mail : openhardwaredz@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#include <SPI.h>
#include <SD.h>
#include "TinyGPS.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Fonts/FreeMono12pt7b.h"

#define BUFFPIXEL 20

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
// If using the breakout, change pins as desired
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

// conection SD
#define SD_CS 4

// Create an instance of the TinyGPS object
// Création d'une instance pour l'objet TinyGPS
TinyGPS gps;

// Create the file when we store our GPS data
// Le fichier qui va stocké les données GPS
File data_GPS;

float oldLat,oldLong;
float distance = 0.0;

unsigned short compt = 0;

void setup() 
{
  tft.begin();
  
  Serial.begin(9600);
  SD.begin(SD_CS);
  
  tft.setRotation(1);
  background();
  delay(1000);
  
}

void loop(void){
  byte a;
  if ( Serial.available() > 0 ) 
  {
    a = Serial.read(); 
    if(gps.encode(a)) 
    {
      if (compt == 0)
        {
          init_GPS_pos(gps);
          compt+=1;
        }
        else
        {
          affichage_gps(gps); // grab the data and display it on the LCD
          delay(60000);  // saisir les données et les afficher sur l'écran LCD
        }
    }
  }
}

//--------------------------------------------------------------------------------------------------

void init_GPS_pos(TinyGPS &gps)
{
    gps.f_get_position(&oldLat, &oldLong);
}

//-------------------------------------------------------------------------------------------------

float calcule_distance(float old_lat, float old_lon, float new_lat, float new_lon)
{
  float latRad, lonRad;
  float tlatRad, tlonRad;
  float midLat, midLon;
  float dist = 0.0;
  
  
  //convert decimal degree into radian
  latRad = old_lat* 0.017453293;
  lonRad = old_lon* 0.017453293;
  tlatRad = new_lat * 0.017453293;
  tlonRad = new_lon * 0.017453293;

  midLat = tlatRad - latRad;
  midLon = tlonRad - lonRad;

  //Calculate the distance in KM
  float latSin = sin((latRad - tlatRad)/2);
  float lonSin = sin((lonRad - tlonRad)/2);
  dist = 2 * asin(sqrt((latSin*latSin) + cos(latRad) * cos(tlatRad) * (lonSin * lonSin)));
  dist = dist * 6371;
  
  return dist;
}



void background(){
  tft.fillScreen(ILI9341_WHITE);
  tft.setRotation(1);
  bmpDraw("logo.bmp", 0, 0);

  tft.setFont(&FreeMono12pt7b);
  tft.setTextColor(ILI9341_BLUE);
  tft.setCursor(100,45);
  tft.println("OHA Arduino GPS");
  tft.setCursor(0, 90);
  tft.setTextColor(ILI9341_BLACK);
  tft.setFont(&FreeMono12pt7b);  
  tft.println("Latitude  :");
  
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(0, 130); 
  tft.println("Longitude :");

  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(0, 170);
  tft.println("Distance  :");

  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(0, 210);
  tft.println("Vitesse   :");
}

void affichage_gps(TinyGPS &gps) {
  
  // Define the variables that will be used
  // Définir les variables qui seront utilisées
  float latitude, longitude;

  // call the function that retrieves the latitude and longitude
  // appelle de la fonction qui récupère la latitude et la longitude
  gps.f_get_position(&latitude, &longitude);
    
  tft.fillRect(160,75,160,160,ILI9341_WHITE);

  distance = distance + calcule_distance(oldLat, oldLong, latitude, longitude);
  
  
  tft.setTextColor(ILI9341_RED);
  tft.setCursor(160, 90); 
  tft.println(latitude,5);
  
  tft.setTextColor(ILI9341_RED); 
  tft.setCursor(160, 130);
  tft.println(longitude,5);  

  tft.setTextColor(ILI9341_RED);
  tft.setCursor(160, 170); 
  tft.println(distance);
  
  tft.setTextColor(ILI9341_RED);
  tft.setCursor(160, 210);
  tft.println(gps.f_speed_kmph());
 
  data_GPS = SD.open("Data_GPS.csv",FILE_WRITE);    
  data_GPS.print(latitude,5);
  data_GPS.print(",");
  data_GPS.print(longitude,5);
  data_GPS.print(",");
  data_GPS.print(distance);                           
  data_GPS.print(",");  
  data_GPS.print(gps.f_speed_kmph());
  data_GPS.print(",");
  data_GPS.println(gps.f_altitude());                          // la lecture des données fournie par le capteur puis leurs écriture dans le fichier 
  
  data_GPS.close();     

  oldLat = latitude;
  oldLong = longitude;
   
}

void bmpDraw(char *filename, uint8_t x, uint16_t y) {

  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  if((x >= tft.width()) || (y >= tft.height())) return;

  //Serial.println();
  //Serial.print(F("Loading image '"));
  //Serial.print(filename);
  //Serial.println('\'');

  // Open requested file on SD card
  
  if ((bmpFile = SD.open(filename)) == NULL) {
    //Serial.print(F("File not found"));
    return;
  }

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
    //Serial.print(F("File size: ")); Serial.println
    read32(bmpFile);
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    //Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    //Serial.print(F("Header size: ")); Serial.println
    read32(bmpFile);
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      //Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        //Serial.print(F("Image size: "));
        //Serial.print(bmpWidth);
        //Serial.print('x');
        //Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1) >= tft.width())  w = tft.width()  - x;
        if((y+h-1) >= tft.height()) h = tft.height() - y;

        // Set TFT address window to clipped image bounds
        tft.setAddrWindow(x, y, x+w-1, y+h-1);

        for (row=0; row<h; row++) { // For each scanline...

          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if(bmpFile.position() != pos) { // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col=0; col<w; col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            tft.pushColor(tft.color565(r,g,b));
          } // end pixel
        } // end scanline
        //Serial.print(F("Loaded in "));
        //Serial.print(millis() - startTime);
        //Serial.println(" ms");
      } // end goodBmp
    }
  }

  bmpFile.close();
  if(!goodBmp); //Serial.println(F("BMP format not recognized."));
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}
