#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"attach.h"

#define IPV4 32
#define MAC 48
#define DPID 48


attachment_point *att_point;


attachment_point *create_att_point_element(char *ipv4, char *mac, char *dpid, int port){
	attachment_point *temp;
	temp = (attachment_point *)malloc(sizeof(attachment_point));
	temp->ipv4 = (char *)malloc(IPV4*sizeof(char));
	temp->mac = (char *)malloc(MAC*sizeof(char));
	temp->dpid = (char *)malloc(DPID*sizeof(char));
	strcpy(temp->ipv4,ipv4);
	strcpy(temp->mac,mac);
	strcpy(temp->dpid,dpid);
	temp->port = port;
	temp->next_attachment_point = NULL;
	return temp;
}


int add_entry(char *ipv4, char *mac, char *dpid, int port){
	if(att_point == NULL){
		attachment_point *temp = create_att_point_element(ipv4,mac,dpid,port);
		att_point = temp;
		return 0;
	}	
	else {
		attachment_point *current, *temp1;
		current = att_point;
		temp1 = create_att_point_element(ipv4,mac,dpid,port);
		while(current->next_attachment_point != NULL){
			current = current->next_attachment_point;
		}
		current->next_attachment_point = temp1;
		return 0;
	}
}

char *find_dpid(char *ipv4){
	attachment_point *temp;
	temp = att_point;
	while(temp != NULL){
		if(strcmp(temp->ipv4,ipv4) == 0){
			return temp->dpid;
		}
		temp = temp->next_attachment_point;
	}
	return NULL;
}

int print_att_point(void){

	attachment_point *temp;
	temp = att_point;
	while(temp != NULL){
		printf("IPv4 = %s, MAC = %s, DPID = %s, Port = %d\n",temp->ipv4,temp->mac,temp->dpid,temp->port);
		temp = temp->next_attachment_point;
	}
	return 0;
}

/*int main(){
	int result;
	char ip[32], mac[48], dpid[48];
	int port;
	char *dp;
	
	FILE *fp = fopen("att.txt", "r");
        if(fp == NULL){
                printf("\nERROR:Input File open error\n");
                exit(0);
        }
	
	while((fscanf(fp, "%s %s %s %d", ip, mac, dpid, &port))!=EOF){
		printf("\n MAIN IPV4 = %s",ip);
         result = add_entry(ip,mac,dpid,port);
         if(result != 0){
                printf("\nError in Adding an edge\n");
                return 0;
        	}
	}
	result = print_att_point();
	if(result != 0){
                printf("\n GRAPH PRINTING ERROR\n");
                return 0;
        }
	char *fip = "10.2.2.3";
	dp = find_dpid(fip);
	printf(" DPID for ipv4: %s  dpid - %s",fip,dp);
	return 0;	
}*/
