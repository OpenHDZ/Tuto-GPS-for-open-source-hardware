/*
Programme qui est utilisé pour communiqué les coordonées a un ordanateur via liaison série.

Ecrit par Areour mohamed Cherif
Date : 12/04/2017
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

#include <SoftwareSerial.h>
#include <TinyGPS.h>
 
float latitude,longitude; 
 
SoftwareSerial gpSerial(2, 3); 
TinyGPS gps; 
 
void setup(){
  Serial.begin(9600);   // port serial de communication avec le pc
  gpSerial.begin(9600); // port de comunication avec le capteur GPS
}
 
void loop(){
  
  while(gpSerial.available()){
     
    
    if(gps.encode(gpSerial.read())){ 
      
      gps.f_get_position(&latitude,&longitude); // get latitude and longitude
    
      Serial.print(latitude);
      Serial.print(",");
      Serial.println(longitude);
     }
  }
    
}
