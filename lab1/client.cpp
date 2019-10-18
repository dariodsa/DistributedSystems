#include "client.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#define LONGITUDE_MIN 15.87
#define LONGITUDE_MAX 16
#define LATITUDE_MIN 45.75
#define LATITUDE_MAX 45.85

char q[1000];

float getValue(char* fileName, char* parameterName, int time) {
    time = (time % 100 ) + 2;
    FILE *pfile = fopen(fileName, "w");
    int br = 0;
    int pok = 0;
    while( fscanf(pfile, "%s", q) != 0) {
        if(br == 0) {
            int cnt = 0 ;
            char *token = strtok(q, ",");
            while (token != NULL) {
                if(strcmp(token, parameterName) == 0) {
                    pok = cnt;
                    break;
                }
                ++cnt;
                token = strtok(NULL, ",");
            }
        } else {
            if(br - 1 == time) {
                bool found = false;
                int cnt = 0;
                int num = 0;
                for(int i = 0, len = strlen(q); i < len; ++i){
                    if(cnt == pok && !found) {
                        found = true;
                        num = num * 10 + '0' + q[i];
                    }
                    else if(found && cnt > pok) {
                        found = false;
                        return num;
                    }
                    else if(q[i] == ',')  ++cnt;
                }
            }
        }
    }
    return 0;
}

void usage() {
    printf("Usage:\n");
    printf(" ./client [OPTION] [FILE]\n");
    printf(" Reads the FILE's data and reports it to the main server.\n");
    printf("Opitions:\n");
    printf("  -x sensor's latitude\n");
    printf("  -y sensor's longitude\n");
    printf("  -t sensor's parameter\n");
    printf("  -p port\n");
    printf("  -n sensor's name\n");
    printf("  -h usage\n");
}

float latitude, longitude;
size_t PORT;
char name[LEN_USERNAME];
char file[LEN_USERNAME];
char parameter[LEN_PARAMETER];
char ipAddress[LEN_IP_ADDRESS];

int main(int argc, char* argv[]) {
    
    int opt;
    int br = 1;
    while( (opt = getopt(argc, argv, "x:y:t:n:p:h")) != -1) {
        ++br;
        switch(opt) {
            case 'x':
                ++br;
                if(optarg) latitude = atof(optarg);
                break;
            case 'y':
                if(optarg) longitude = atof(optarg);
                ++br;
                break;
            case 't':
                if(optarg) strcpy(parameter, optarg);
                ++br;
                break;
            case 'n':
                if(optarg) strcpy(name, optarg);
                ++br;
                break;
            case 'p':
                PORT = atoi(optarg);
                ++br;
                break;
            case 'h':
                usage();
                break;
            default:
                printf("Parameter %c is not acceptable.\n");
                usage();
                return -1;
        }
    }
    if(argc > br) {
        strcpy(file, argv[br]);
    } else {
        printf("File parameter is missing.\n");
        return -1;
    }
    

    if(!(LATITUDE_MIN <= latitude && latitude <= LATITUDE_MAX &&
       LONGITUDE_MIN <= longitude && longitude <= LONGITUDE_MAX)) {
           printf("Location is not in the acceptable range.\n");
           printf("Longitude range is [%f, %f] and latitude range is [%f, %f].\n", LONGITUDE_MIN, LONGITUDE_MAX, LATITUDE_MIN, LATITUDE_MAX);
           printf("Your longitude and latitude is %f %f\n", longitude, latitude);
           return -1;
    }
    Sensor* s = new Sensor(name, latitude, longitude, ipAddress, PORT);

    return 0;
}