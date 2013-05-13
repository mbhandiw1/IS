#include<stdio.h>
#include<stdlib.h>
#include<string.h>


typedef struct attachment_point attachment_point;

struct attachment_point {
        char *ipv4;
        char *mac;
        char *dpid;
        int port;
         struct attachment_point *next_attachment_point;
};

attachment_point *create_att_point_element(char *ipv4, char *mac, char *dpid, int port);
int add_entry(char *ipv4, char *mac, char *dpid, int port);
char *find_dpid(char *ipv4);
int print_att_point(void);
