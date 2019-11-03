#include "client.h"
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>


#define LONGITUDE_MIN 15.87
#define LONGITUDE_MAX 16.0
#define LATITUDE_MIN 45.75
#define LATITUDE_MAX 45.85

#define SERVER_PORT 8080

char q[1000];
float latitude, longitude;
int PORT;
time_t start_time;
std::string name;
char file[100];
std::string parameter;
std::string ipAddress= "127.0.0.1";
std::vector<std::string> PARAMETERS; 

std::vector<double>values;


double getValue(char* fileName, std::string parameterName, int time) {
    time = (time % 100 );
    
    FILE *pfile = fopen(fileName, "r");
    int br = 0;
    int pok = 0;
    while( fscanf(pfile, "%s", q) != 0) {
        if(br == 0) {
            int cnt = 0 ;
            char *token = strtok(q, ",");
            while (token != NULL) {
                if(parameterName.compare(std::string(token)) == 0) {
                    pok = cnt;
                    break;
                }
                ++cnt;
                token = strtok(NULL, ",");
            }
        } else if(br - 1 == time) {
                bool found = false;
                int cnt = 0;
                int num = 0;
                for(int i = 0, len = strlen(q); i < len; ++i){
                    if(cnt == pok && !found) {
                        found = true;
                        
                    }
                    else if(found && cnt > pok) {
                        found = false;
                        fclose(pfile);
                        return (double)num;
                    } 
                    if(found && q[i] >= '0' && q[i] <= '9') {
                        num = num * 10 + (q[i] - '0');
                    }
                    if(q[i] == ',')  ++cnt;
                }
                fclose(pfile);
                return (double)num;
        }
        ++br;
    }
    fclose(pfile);
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

pplx::task<void> register_to_the_server(Sensor* s) {
    http_client client(U("http://localhost:8080"));
    web::http::http_request request(methods::POST);
    json::value value = s->getJSON();
    request.set_body(value);
    request.set_request_uri("/register");
    
    return client.request(request).then([=](http_response response)
    {
        if(response.status_code() == status_codes::OK)
        {
            cout << response.extract_json().get()<< endl;

        }

        // Handle error cases, for now return empty json value... 
        return pplx::task_from_result(json::value());
    }).then([](json::value v) {

    });
}
json::value get_closest() {
    http_client client(U("http://localhost:8080"));
    web::http::http_request request(methods::GET);
    request.set_request_uri("/closest/" + name);
    cout << "/closest/" + name << endl;
    return client.request(request).get().extract_json().get();
    
}

pplx::task<void> sendData(std::string parameter, double avgValue) {
    http_client client(U("http://localhost:8080"));
    web::http::http_request request(methods::POST);
    json::value value;
    value["username"] = json::value::string(name);
    value["parameter"] = json::value::string(name);
    value["averageValue"] = json::value::number(avgValue);

    request.set_body(value);
    request.set_request_uri("/store");
    
    return client.request(request).then([=](http_response response)
    {
        if(response.status_code() == status_codes::OK)
        {
            cout << "OK" << endl;

        }

        // Handle error cases, for now return empty json value... 
        return pplx::task_from_result(json::value());
    }).then([](json::value v) {

    });
}

int sock;
struct sockaddr_in my_addr;

void *process(void* pok) {
    printf("Processing\n");
    int* pok2 = (int*) pok;
    int my_sock = pok2[0];
    char* buff = (char*) malloc(sizeof(char));
    int r = read(my_sock, (void*) buff, sizeof(char));
    char _time = buff[0];
    int time = _time;
    double* arr = (double*) malloc(sizeof(double) * PARAMETERS.size());
    time_t t1 = std::time(NULL) - (start_time);
    cout << "Asking to compare " << t1 << " and " << time << " values"<< endl;
    for(int i=0, len = PARAMETERS.size(); i < len; ++i) {
        
        arr[i] = getValue(file, PARAMETERS[i], t1);
    }
    write(my_sock, (void*)arr, sizeof(double) * PARAMETERS.size());
    free(arr);
    close(my_sock);
    return 0;
}

void *listen(void *p) {
    listen(sock, 10);
    printf("SLUŠAM\n");
    while(true) {
        struct sockaddr_in their_addr;
        socklen_t sin_size = sizeof their_addr;
        int newfd;
        if((newfd=accept(sock,(struct sockaddr *)&their_addr,&sin_size)) == -1) {
            printf("Error in accept.\n");
            exit(-1);
        }
        pthread_t process_thread;
        int* p = (int*)malloc(sizeof(int));
        p[0] = newfd;
        if(pthread_create(&process_thread, NULL, process, (void*)p)) {
            printf("Error in creating thread\n");
            exit(-1);
        }
    }
    return 0;
}

int main(int argc, char* argv[]) {
    
    PARAMETERS.push_back("Temperature");
    PARAMETERS.push_back("Pressure");
    PARAMETERS.push_back("Humidity");
    PARAMETERS.push_back("CO");
    PARAMETERS.push_back("NO2");
    PARAMETERS.push_back("SO2");

    

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
                if(optarg) parameter = std::string(optarg);
                ++br;
                break;
            case 'n':
                if(optarg) name = std::string(optarg);
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
                printf("Parameter %s is not acceptable.\n", optarg);
                usage();
                return -1;
        }
    }
    if(argc > br) {
        strcpy(file, argv[br]);
    } else {
        printf("File parameter is missing.\n");
        usage();
        return -1;
    }
    
    /*if(!(LATITUDE_MIN <= latitude && latitude <= LATITUDE_MAX &&
       LONGITUDE_MIN <= longitude && longitude <= LONGITUDE_MAX)) {
           printf("Location is not in the acceptable range.\n");
           printf("Longitude range is [%f, %f] and latitude range is [%f, %f].\n", LONGITUDE_MIN, LONGITUDE_MAX, LATITUDE_MIN, LATITUDE_MAX);
           printf("Your longitude and latitude is %f %f\n", longitude, latitude);
           return -1;
    }*/
    Sensor* s = new Sensor(name, latitude, longitude, ipAddress, PORT);
    //register to the server
    cout << s->getJSON().serialize()<< endl;
    register_to_the_server(s).wait();
    cout << "Registracija uspješna." << endl;
    char tmp[50];
    int time = 0;
    start_time = std::time(NULL);
    
    if( (sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        return -1;
    }
    
    my_addr.sin_family = AF_INET;
    cout << PORT << endl;
    my_addr.sin_port = htons(PORT);
    my_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    bind(sock, (struct sockaddr *)&my_addr, sizeof my_addr);
    
    pthread_t listen_thread;
    if(pthread_create(&listen_thread, NULL, listen, NULL)) {
        printf("Error in creating thread\n");
        return -1;
    }

    while(true) {
        printf("Pritisni s sa novo dohvacanje podataka ili q za gašanja");
        scanf("%s", tmp);
        if((strlen(tmp) == 1 && tmp[0] == 'q')) {
            close(sock);
            printf("I am done. :-) \n");
            return 0;
        }
        if(!(strlen(tmp) == 1 && tmp[0] == 's')) continue;
        time_t t1 = std::time(NULL) - (start_time);
        cout << "Vrijednost za vrijeme " << t1 << "\n";
        
        values.clear();
        for(int i=0, len = PARAMETERS.size(); i < len; ++i) {
            double value = getValue(file, PARAMETERS[i], t1);
            cout << "Value " << value << endl;
            values.push_back(value);
        }
        Sensor *s1 = new Sensor(get_closest());
        cout << "Najbliži " << s1->getUsername() << endl;
        
        struct addrinfo hints, *res;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_flags = AI_CANONNAME;
        if((getaddrinfo(s1->getIpAddress().c_str(), std::to_string(s1->getPort()).c_str(), &hints, &res))) {
            return -1;
        }

        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if(sock < 0) {
            printf("sock error\n");
            exit(-1);
        }
        struct sockaddr_in server;
        server.sin_port = htons(s1->getPort());
        server.sin_family = AF_INET;
        server.sin_addr = ((struct sockaddr_in *) res->ai_addr)->sin_addr;
        if(connect(sock, (struct sockaddr*)&server, sizeof server) == -1) {
            printf("connect error\n");
            exit(-1);
        }
        char *messTime = (char*) malloc(sizeof(char));
        messTime[0] = t1;
        int r = write(sock, (void*) messTime, sizeof(char));
        printf("r %d\n", r);
        char *arr = (char*) malloc(sizeof(double) * PARAMETERS.size());
        int pok = 0;
        int len = sizeof(double) * PARAMETERS.size();
        while(pok < len) {
            char *temparr = (char*) malloc(sizeof(double) * PARAMETERS.size());
            int k = read(sock, temparr, len);
            for(int i=0; i < k; ++i) {
                arr[i + pok] = temparr[i];
            }
            pok += k;
        }
        double *p = (double*)arr;
        for(size_t i=0;i<PARAMETERS.size(); ++i) {
            printf("My value: %lf, neighbour's value: %lf\n", values[i], p[i]);
            if(p[0] == 0) {
                continue;
            } else if(values[i] == 0) {
                values[i] = p[i];
            } else {
                values[i] = (values[i] + p[i]) / 2;
            }
        }
        close(sock);
        for(size_t i=0;i<PARAMETERS.size(); ++i) {
            sendData(PARAMETERS[i], values[i]).wait();
        }

        ++time;
    }


    return 0;
}