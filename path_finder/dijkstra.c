#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"topology.h"
#include"dijkstra.h"

#define DPID 48
#define ZERO 0
#define INFINITY 9999
#define FALSE -1

list_array *done, *dist;
prev_path *prev;
int count;

int num_switches(topo_graph *graph) {
	topo_graph *temp;
	temp = graph;
	while(temp != NULL) {
		count++;
		temp = temp->next_element;
	}
	return count;
}


list_array *init_list_array(int init_value){
	list_array *a;
	topo_graph *temp;
	int i = 0;
	a = (list_array *)malloc(count*sizeof(list_array));
	if(!a){
		printf("\n Error in one of list array malloc\n");
                exit(0);
	}
	temp = graph;
	while(temp != NULL){
		a[i].dpid = (char *)malloc(DPID*sizeof(char));
		a[i].dpid = temp->sdpid;
		a[i].w = init_value;
		temp = temp->next_element;
		i++;
	}
	return a;
}

prev_path *init_prev_path(void){
	prev_path *a;
	topo_graph *temp;
	int i = 0;
	a = (prev_path *)malloc(count*sizeof(prev_path));
	if(!a){
                printf("\n Error in one of list array malloc\n");
                exit(0);
        }
	temp = graph;
	while(temp != NULL){
                a[i].dpid = (char *)malloc(DPID*sizeof(char));
		a[i].path_dpid = (char *)malloc(DPID*sizeof(char));
                a[i].dpid = temp->sdpid;
                a[i].path_dpid = NULL;
                temp = temp->next_element;
                i++;
        }
        return a;
}


int set_val_list_array(list_array *a, char *dpid, int value) {
	int i;
	for(i = 0; i < count; i++){
		if(strcmp(a[i].dpid,dpid) == 0){
			a[i].w = value;
			return 0;
		}
	}
	return 0;
}

int get_val_list_array(list_array *a, char *dpid) {
	int i;
	for(i = 0; i < count; i++) {
		if(strcmp(a[i].dpid,dpid) == 0){
			return a[i].w;
		}
	}
	return FALSE;
}

int get_index_prev_path(prev_path *a, char *dpid) {
	int i;
	for(i = 0; i < count; i++) {
		if(strcmp(a[i].dpid,dpid) == 0){
			return i;
		}
	}
	return FALSE;
}

void print_init_djk(void){
	int i;
	printf("\n STRUCT ARRAY VALUES: \n");
	printf("\n Count value in print - %d",count);
	for(i = 0; i < count; i++) {
		printf("PREV dpid - %s	W - %s\n",prev[i].dpid,prev[i].path_dpid);
		printf("DONE dpid - %s  W - %d\n",done[i].dpid,done[i].w);
		printf("DIST dpid - %s  W - %d\n",dist[i].dpid,dist[i].w); 
	}
}

void print_array_list(list_array *a){
	int i = 0;
	for(i = 0; i < count; i++)
		printf("dpid - %s w - %d\n",dist[i].dpid,dist[i].w);
}
void dijkstra(char *dpid){
	
	int cnt,inter_dist,i;
	int result;
	topo_graph *temp;
	char *element = (char *)malloc(DPID*sizeof(char));
	count = num_switches(graph);
	prev = init_prev_path();
	done = init_list_array(ZERO);
	dist = init_list_array(INFINITY);
	int value = 0;
	cnt = 0;
	result = set_val_list_array(dist,dpid,value);
	if(result != 0){
                 printf("\n Error in setting dist list array value to - %d\n",value);
                 exit(0);
         }
	
	while(cnt < count-1){
		inter_dist = INFINITY;
		for(i = 0; i < count; i++){
			if(dist[i].w < inter_dist && !done[i].w){
				inter_dist = dist[i].w;
				element = dist[i].dpid;
			}
		}
		set_val_list_array(done,element,1);
		if(result != 0){
	                  printf("\n Error in setting done list array value to - 1\n");
        	          exit(0);
          	}
		temp = graph;
		while(temp != NULL){
			if(strcmp(temp->sdpid,element) == 0){
				adj_node *iter_adjnodes;
				iter_adjnodes = temp->adjnode;
				while(iter_adjnodes != NULL){
					if(!get_val_list_array(done,iter_adjnodes->ddpid)){
						if((inter_dist + iter_adjnodes->w) < get_val_list_array(dist,iter_adjnodes->ddpid)){
							value = inter_dist + iter_adjnodes->w;
							if(set_val_list_array(dist,iter_adjnodes->ddpid,value)){
								printf("\n Error in setting dist value\n");
								exit(0);	
							}
							iter_adjnodes->min = iter_adjnodes->w;
							int prev_index = get_index_prev_path(prev,iter_adjnodes->ddpid);
							prev[prev_index].path_dpid = element;
							
						}
					}					
					iter_adjnodes = iter_adjnodes->next_adjnode;
				}
			}	
			temp = temp->next_element;		
		}
			
		cnt++;	
	}
}


void print_dijkstra(char *dpid){
	int prev_index = get_index_prev_path(prev,dpid);
	if(prev[prev_index].path_dpid != NULL){
		print_dijkstra(prev[prev_index].path_dpid);
	}
	if(prev[prev_index].path_dpid != NULL){
		printf("%s -> %s",prev[prev_index].path_dpid,dpid);
	}
}
