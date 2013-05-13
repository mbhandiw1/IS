#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"topology.h"

#define INFINITY 9999

topo_graph* create_topograph(char *src_dpid){
	topo_graph *temp = (topo_graph *)malloc(sizeof(topo_graph));
	temp->sdpid = (char *)malloc(48 * sizeof(char *));
	strcpy(temp->sdpid,src_dpid);
	temp->adjnode = NULL;
	temp->next_element = NULL;
	return temp;
}
adj_node* create_adjnode(char *dst_dpid, int w){
	adj_node *new_adjnode = (adj_node *)malloc(sizeof(adj_node));
	new_adjnode->ddpid = (char *)malloc(48 * sizeof(char));
	strcpy(new_adjnode->ddpid,dst_dpid);
	new_adjnode->w = w;
	new_adjnode->min = INFINITY;
	new_adjnode->next_adjnode = NULL;
	return new_adjnode;
}

topo_graph* graph_instance(void) {
        topo_graph *temp = (topo_graph *)malloc(sizeof(topo_graph));
        temp->sdpid = NULL;
        temp->next_element = NULL;
        temp->adjnode = NULL;
        return temp;
}


int add_edge(char *src_dpid, char *dst_dpid, int w){
	if(graph->sdpid == NULL){
		topo_graph *temp = create_topograph(src_dpid);
		adj_node *new_adjnode = create_adjnode(dst_dpid,w);
		temp->adjnode = new_adjnode;
		temp->next_element = NULL;
		graph = temp;
		return 0;	
	}
	else {
		topo_graph *current;
		current = graph;
		
		while(current != NULL){
			if(strcmp(current->sdpid,src_dpid) == 0){
				adj_node *new_adjnode = create_adjnode(dst_dpid,w);
				adj_node *temp;
				temp = current->adjnode;
				while(temp->next_adjnode != NULL){
					temp = temp->next_adjnode;
				}
				temp->next_adjnode = new_adjnode;
				return 0;
				
			}
			current = current->next_element;
		}
		topo_graph *temp1;
		temp1 = graph;
		topo_graph *temp = create_topograph(src_dpid);
		adj_node *new_adjnode = create_adjnode(dst_dpid,w);
		temp->adjnode = new_adjnode;
		while(temp1->next_element != NULL)
			temp1 = temp1->next_element;
		temp1->next_element = temp;
		return 0;
	}	
	
}

int print_graph(topo_graph *graph){
	topo_graph *temp1;
	temp1 = graph;

	if(temp1->sdpid == NULL) {
		printf("\n GRAPH is EMPTY\n");
		return 1;
	}
	else {
		while(temp1 != NULL){
			adj_node* temp2 = temp1->adjnode;
			while(temp2 != NULL){
				printf("source - %s	destination - %s	weight - %d\n",temp1->sdpid,temp2->ddpid,temp2->w);
				temp2= temp2->next_adjnode;
			}
			temp1 = temp1->next_element;
		}
	}
	return 0;
}

/*int num_switches(topo_graph *graph) {
	topo_graph *temp;
	temp = graph;
	while(temp != NULL) {
		count++;
		temp = temp->next_element;
	}
	return count;
}*/

void free_graph(void){
	free(graph->adjnode);
	free(graph);
}
