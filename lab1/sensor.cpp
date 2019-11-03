#include "sensor.h"

Sensor::Sensor(std::string username, double latitude, double longitude, std::string ipAddress, size_t port) {
    this->username = username;
    this->latitude = latitude;
    this->longitude = longitude;
    this->port = port;
    this->ipAddress = ipAddress;
}

Measurement::Measurement(std::string username, std::string parameter, double averageValue) {
    this->username = username;
    this->parameter = parameter;
    this->averageValue = averageValue;
}

std::string Measurement::getUsername() {
    return this->username;
}
std::string Measurement::getParameter() {
    return this->parameter;
}
float Measurement::getAverageValue() {
    return this->averageValue;
}

void Sensor::addData(std::string parameter, float data) {
    this->measurements.push_back(new Measurement(this->getUsername()
                                    , parameter
                                    , data));
}

float Sensor::getLatitude() {
    return this->latitude;
}
float Sensor::getLongitude() {
    return this->longitude;
}
std::string Sensor::getUsername() {
    return this->username;
}
size_t Sensor::getPort() {
    return this->port;
}
std::string Sensor::getIpAddress() {
    return this->ipAddress;
}

std::string Sensor::to_string() {
    std::string ans = "";
    return  std::string("userName = ") + std::string(this->username) + std::string(" , port: ") + std::to_string(this->port);
    
}

web::json::value Sensor::getJSON() {
    web::json::value value;
    value["username"] = json::value::string(this->getUsername());
    value["latitude"] = json::value::number(this->getLatitude());
    value["longitude"] = json::value::number(this->getLongitude());
    value["port"] = json::value::number(this->getPort());
    value["ipAddress"] = json::value::string(this->getIpAddress());
    return value;
}