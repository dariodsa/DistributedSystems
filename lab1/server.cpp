#include "server.h"
#include <math.h>
#include <stdexcept>

#define EARTH_RADUIS 6371

bool Server::registerNewSensor(Sensor &s1) {
    for(int i=0, len = sensors.size(); i < len; ++i) {
        if(strcmp(sensors[i]->getUsername(), s1.getUsername) == 0) {
            return false;
        }
    }
    sensors.push_back(&s1);
    return true;
}

float Server::dist(Sensor &s1, Sensor &s2) {
    float deltaLon = abs(s1.getLongitude() - s2.getLongitude());
    float deltaLat = abs(s1.getLatitude() - s2.getLatitude());
    float a = (sin(deltaLat/2) * sin(deltaLat/2.0) + cos(s1.getLatitude()) * cos(s2.getLatitude()) * sin(deltaLon/2.0)*sin(deltaLon/2.0));
    float c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return EARTH_RADUIS * c;
}
Sensor* Server::closest(Sensor &s1) {
    if(sensors.size() == 0) {
        throw invalid_argument( "list of sensors is empty");
    }
    float dul = dist(s1, *sensors[0]);
    int id = 0;
    for(int i=1, len = sensors.size(); i < len; ++i) {
        float new_dul = dist(s1, *sensors[i]);
        if(new_dul < dul) {
            dul = new_dul;
            id = i;
        }
    }
    return *sensors[id];
}

int main(int argc, char* argv) {

}