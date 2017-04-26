double lat_alg = 36.7596531;
double lon_alg = 3.0665137;

double lat_tam = 23.5891607;
double lon_tam = 4.7637973;

float distance=0.0;

double calcule_distance(double old_lat, double old_lon, double new_lat, double new_lon)
{
  double latRad, lonRad;
  double tlatRad, tlonRad;
  double midLat, midLon;
  double dist = 0.0;
  
  
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
  dist = dist * 6378.137;
  
  return dist;
  
}

void setup()
{
  Serial.begin(9600);
  distance = calcule_distance( lat_alg, lon_alg, lat_tam, lon_tam);
  Serial.println("distance Alger Tamanrasset");
  Serial.print(distance,3);
  Serial.println("Km");
}

void loop()
{
  
}
