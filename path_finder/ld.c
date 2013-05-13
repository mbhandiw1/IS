/*
 * Copyright (c) 2009-2012 Petri Lehtinen <petri@digip.org>
 *
 * Jansson is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <jansson.h>
#include <curl/curl.h>

#include "topology.h"
#include "dijkstra.h"
#include "attach.h"

#define BUFFER_SIZE  (256 * 1024)  /* 256 KB */

#define URL_TOPO     "http://localhost:8080/wm/topology/links/json"
#define URL_SIZE     256
#define URL_DEVICE   "http://localhost:8080/wm/device/"

struct write_result
{
	char *data;
	int pos;
};

static size_t write_response(void *ptr, size_t size, size_t nmemb, void *stream)
{
	struct write_result *result = (struct write_result *)stream;

	if(result->pos + size * nmemb >= BUFFER_SIZE - 1)
	{
		fprintf(stderr, "error: too small buffer\n");
		return 0;
	}

	memcpy(result->data + result->pos, ptr, size * nmemb);
	result->pos += size * nmemb;

	return size * nmemb;
}

static char *request(const char *url)
{
	CURL *curl;
	CURLcode status;
	char *data;
	long code;

	curl = curl_easy_init();
	data = malloc(BUFFER_SIZE);
	if(!curl || !data)
		return NULL;

	struct write_result write_result = {
		.data = data,
		.pos = 0
	};

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &write_result);

	status = curl_easy_perform(curl);
	if(status != 0)
	{
		fprintf(stderr, "error: unable to request data from %s:\n", url);
		fprintf(stderr, "%s\n", curl_easy_strerror(status));
		return NULL;
	}

	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
	if(code != 200)
	{
		fprintf(stderr, "error: server responded with code %ld\n", code);
		return NULL;
	}

	curl_easy_cleanup(curl);
	curl_global_cleanup();

	data[write_result.pos] = '\0';

	return data;
}


int parse_topology(char *text){
	int i, result;
	json_t *root;
	json_error_t error;

	root = json_loads(text, 0, &error);

	if(!root)
	{
		fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
		return 1;
	}

	if(!json_is_array(root))
	{
		fprintf(stderr, "error: root is not an array\n");
		return 1;
	}

	for(i = 0; i < json_array_size(root); i++)
	{

		json_t *data, *src_sw, *src_port, *dst_sw, *dst_port;

		data = json_array_get(root, i);
		if(!json_is_object(data))
		{
			fprintf(stderr, "error: commit data %d is not an object\n", i + 1);
			return 1;
		}

		src_sw = json_object_get(data, "src-switch");
		if(!json_is_string(src_sw))
		{
			fprintf(stderr, "error: commit %d: src_sw is not a string\n", i + 1);
			return 1;
		}

		src_port = json_object_get(data, "src-port");
		if(!json_is_integer(src_port))
		{
			fprintf(stderr, "error: commit %d: commit is not an integer\n", i + 1);
			return 1;
		}

		dst_sw = json_object_get(data, "dst-switch");
		if(!json_is_string(dst_sw))
		{
			fprintf(stderr, "error: commit %d: src_sw is not a string\n", i + 1);
			return 1;
		}

		dst_port = json_object_get(data, "dst-port");
		if(!json_is_integer(dst_port))
		{
			fprintf(stderr, "error: commit %d: commit is not an integer\n", i + 1);
			return 1;
		}


		//printf("\nsrc_switch:%s  src_port:%d dst_switch:%s dst_port:%d\n",json_string_value(src_sw),json_integer_value(src_port),json_string_value(dst_sw),json_integer_value(dst_port));
		result = add_edge(json_string_value(src_sw),json_string_value(dst_sw),1);
		if(result != 0){
			printf("\nError in creating an edge in topology\n");
			exit(0);
		}
	}
	json_decref(root);
	return 0;
}


int parse_device(char *text){
	int i, result;
	json_t *root;
	json_error_t error;

	root = json_loads(text, 0, &error);

	if(!root)
	{
		fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
		return 1;
	}

	if(!json_is_array(root))
	{
		fprintf(stderr, "error: root is not an array\n");
		return 1;
	}

	for(i = 0; i < json_array_size(root); i++)
	{
		json_t *data, *mc, *ip, *attpnt, *port, *sw_dpid, *mc_arr, *ip_arr, *att_arr;

		data = json_array_get(root, i);
		if(!json_is_object(data))
		{
			fprintf(stderr, "error: device data %d is not an object\n", i + 1);
			return 1;
		}
		mc = json_object_get(data, "mac");
		if(!json_is_array(mc))
		{
			fprintf(stderr, "error: MAC array  %d: mac is not a array\n", i + 1);
			return 1;
		}

		mc_arr = json_array_get(mc, 0);
		if(!json_is_string(mc_arr))
		{
			fprintf(stderr, "error: MAC data %d is not an string\n", i + 1);
			return 1;
		}

		ip = json_object_get(data, "ipv4");
		if(!json_is_array(ip))
		{
			fprintf(stderr, "error: ipv4 %d: ipv4  is not array\n", i + 1);
			return 1;
		}
		ip_arr = json_array_get(ip, 0);
		if(!json_is_string(ip_arr))
		{
			fprintf(stderr, "error: IP  data %d is not an string\n", i + 1);
			return 1;
		}
		attpnt = json_object_get(data, "attachmentPoint");
		if(!json_is_array(attpnt))
		{
			fprintf(stderr, "error: Attachment %d: commit is not an Array\n", i + 1);
			return 1;
		}
		att_arr = json_array_get(attpnt, 0);
		if(!json_is_object(att_arr))
		{
			fprintf(stderr, "error: First Object in Attachment data %d is not an object\n", i + 1);
			return 1;
		}
		port = json_object_get(att_arr, "port");
		if(!json_is_integer(port))
		{
			fprintf(stderr, "error: PORT %d: message is not a string\n", i + 1);
			return 1;
		}
		sw_dpid = json_object_get(att_arr, "switchDPID");
		if(!json_is_string(sw_dpid))
		{
			fprintf(stderr, "error: SWITCH DPID  %d: message is not a string\n", i + 1);
			return 1;
		}



		//printf("\nMAC=%s  IPV4=%s port=%d SWITCH=%s\n",json_string_value(mc_arr),json_string_value(ip_arr),json_integer_value(port),json_string_value(sw_dpid));
		result = add_entry(json_string_value(ip_arr),json_string_value(mc_arr),json_string_value(sw_dpid),json_integer_value(port));
		if(result != 0){
			printf("\n Error in adding an entry into attachment_point structure\n");
			exit(0);
		}
	}
	json_decref(root);
	return 0;
}


void topology_discovery(void) {
	int result;
	char *text;
	char url[URL_SIZE];

	snprintf(url, URL_SIZE, URL_TOPO);

	text = request(url);
	if(!text){
		printf("\n Error in CURL request\n");
		exit(0);
	}
	result = parse_topology(text);
	if(result != 0){
		printf("\n Error in parsing topology\n");
		exit(0);
	}
	free(text);

}

void device_discovery(void){

	int result;
	char *text;
	char url[URL_SIZE];

	snprintf(url, URL_SIZE, URL_DEVICE);

	text = request(url);
	if(!text){
		printf("\n Error in CURL request\n");
		exit(0);
	}
	result = parse_device(text);
	if(result != 0){
		printf("\n Error in parsing device attachment point \n");
		exit(0);
	}
	free(text);
}

int main(int argc, char **argv){

	//int i,j;
	//int result = 1;
	char *dp;
	char *ip;
	char *source;
	topology_discovery();
	/*if(i!=result){
		printf("\nERROR in TOPOLOGY DISCOVERY\n");
	}*/
	 device_discovery();
	/*if(j!=result){
		printf("\nERROR in DEVICE DISCOVERY\n");
	}*/
	dijkstra(source);
	dp = find_dpid(ip);
	printf(" DPID for ipv4: %s  dpid - %s",ip,dp);
	print_dijkstra(dp);
	return 0;
}
