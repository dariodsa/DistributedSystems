#include <string.h>
#define LEN_USERNAME 100
#define LEN_PARAMETER 100
#define LEN_IP_ADDRESS 20

class Sensor{
    private:
        char username[LEN_USERNAME];
        double latitude;
        double longitude;
        char ipAddress[LEN_IP_ADDRESS];
        size_t port;
    public:
    Sensor(char *username, double latitude, double longitude, char *ipAddress, size_t port) {
        strcpy(this->username, username);
        this->latitude = latitude;
        this->longitude = longitude;
        this->port = port;
        strcpy(this->ipAddress, ipAddress);
    }
    float getLatitude() {
        return this->latitude;
    }
    float getLongitude() {
        return this->longitude;
    }
    char* getUsername() {
        return this->username;
    }
    size_t getPort() {
        return this->port;
    }
    char* getIpAddress() {
        return this->ipAddress;
    }
};

class Measurement{
    private:
        char username[LEN_USERNAME];
        char parameter[LEN_PARAMETER];
        float averageValue;
    Measurement(char *username, char *parameter, float averageValue) {
        strcpy(this->username, username);
        strcpy(this->parameter, parameter);
        this->averageValue = averageValue;
    }
    float getValue(int time, char* parameter);


};
