#include "client.h"
#include <stdio.h>
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

int main(int argc, char* argv) {
    

    return 0;
}