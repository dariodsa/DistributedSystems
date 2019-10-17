#include "server.h"
#include <math.h>

#define EARTH_RADUIS 6371

float Server::dist(Sensor &s1, Sensor &s2) {
    float deltaLon = abs(s1.getLongitude() - s2.getLongitude());
    float deltaLat = abs(s1.getLatitude() - s2.getLatitude());
    float a = (sin(deltaLat/2) * sin(deltaLat/2.0) + cos(s1.getLatitude()) * cos(s2.getLatitude()) * sin(deltaLon/2.0)*sin(deltaLon/2.0));
    float c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return EARTH_RADUIS * c;
}
Sensor Server::closest(Sensor &s1) {
    
}

int main(int argc, char* argv) {

}