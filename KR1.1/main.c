//
//  main.c
//  KR1.1
//
//  Created by Nikita-Mac on 21.05.17.
//  Copyright © 2017 Nikita-Mac. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>

int IsPi(char buf[], int len){
    int Index = 0;
    
    for (int i = len; i > 0; i--) {
        if (buf[i] == 'P') {
            Index = 1;
            break;
        }
    }
    
    return Index;
}

int IsExp(char buf[], int len){
    int Index = 0;
    
    for (int i = len; i > 0; i--) {
        if (buf[i] == 'E') {
            Index = 1;
            break;
        }
    }
    
    return Index;
}



int main(int argc, const char * argv[]) {
    umask(0);
    
    int fd;
    char tempfile[] = "/Users/nikita-mac/Desktop/UNIX/KR1.1/KR1.1/XXXXXX\0";
    
    if (mktemp(tempfile) == NULL) {
        printf("cannot create temporary file name\n");
        exit(1);
    }
    
    if ((fd = open(tempfile, O_RDWR | O_CREAT)) == -1) {
        perror(argv[0]);
        exit(0);
    }
    
    pid_t pidPI;
    pid_t pidEXP;
    int x = 1;
    
    if ((pidPI = fork()) == 0) {
        double const Const = 2 * sqrt(3);
        double result = 0;
        for (int i = 0; i < 10000; i++) {
            double temp = pow(-1, i) / pow(3, i);
            temp /= 2 * i + 1;
            result += temp * Const;
        }
        char buf[32];
        memset(buf, '\0', 27);
        sprintf(buf,"%1.24f", result);
        strcat(buf, "P");
        write(fd, &buf, 32);
        exit(0);
    }
    
    if ((pidEXP = fork()) == 0) {
        char buf[32];
        double parameter = -(double)(x * x)/2;
        double result = 1;
        int n = 1;
        for (int i = 1; i < 30; i++) {
            n *= i;
            result += pow(parameter, i) / n;
        }
        memset(buf, '\0', 27);
        sprintf(buf,"%12.12f", result);
        strcat(buf, "E");
        write(fd, &buf, 32);
        exit(0);
    }
    
    
    char buf[32] = "";
    double Pi = 0;
    double Exp = 0;
    
    int flag = 0;
    int count = 0;
    while (flag < 2) {
        lseek(fd, count, SEEK_SET);
        long r = read(fd, buf, 32);
        if (r > 0){
            flag++;
            printf("%s\n",buf);
            count += 32;
            
            if (IsPi(buf, 32)) {
                Pi = atof(buf);
            }
            
            if (IsExp(buf, 32)) {
                Exp = atof(buf);
            }
        }
    }
    
    printf("%f\n", Exp / sqrt(2 * Pi));
    
    close(fd);
    unlink(tempfile);
    return 0;
}
