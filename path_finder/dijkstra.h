#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct list list_array;
typedef struct prev_path prev_path;

struct list {
	char *dpid;
	int w;
};

struct prev_path{
	char *dpid;
	char *path_dpid;
};

list_array *init_list_array(int init_value);
prev_path *init_prev_path(void);
int set_val_list_array(list_array *a, char *dpid, int value);
int get_val_list_array(list_array *a, char *dpid);
int get_index_prev_path(prev_path *a, char *dpid);
void print_init_djk(void);
void print_array_list(list_array *a);
void dijkstra(char *dpid);
void print_dijkstra(char *dpid);