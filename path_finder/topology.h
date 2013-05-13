#include<stdio.h>
#include<stdlib.h>
#include<string.h>



typedef struct adj_node adj_node;
typedef struct topo_graph topo_graph;

struct adj_node {
        char *ddpid;
        int w;
        int min;
        struct adj_node *next_adjnode;
};

struct topo_graph{
        char *sdpid;
        struct topo_graph *next_element;
        struct adj_node *adjnode;
};

topo_graph *graph;
int count;
topo_graph* create_topograph(char *src_dpid);
adj_node* create_adjnode(char *dst_dpid, int w);
topo_graph* graph_instance(void);
int add_edge(char *src_dpid, char *dst_dpid, int w);
int print_graph(topo_graph *graph);
int num_switches(topo_graph *graph);
void free_graph(void);


