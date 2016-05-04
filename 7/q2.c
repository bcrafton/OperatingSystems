#include <stdio.h>
#include <time.h>
#include <pthread.h>

typedef unsigned char uint8_t;

typedef struct network_t
{
	pthread_mutex_t nodes[4][4];
} network_t;

typedef struct packet_t
{
	uint8_t current_x;
	uint8_t current_y;
	uint8_t dest_x;
	uint8_t dest_y;
	
} packet_t;

// the network
static network_t network;

// functions
void move_packet(network_t* network, packet_t* packet);
void init_packet(packet_t* packet);
void init_network(network_t* network);
void *send_packet(void *arg);

void move_packet(network_t* network, packet_t* packet)
{
	if(packet->current_x < packet->dest_x)
	{
		pthread_mutex_lock(&network->nodes[packet->current_x+1][packet->current_y]);
		pthread_mutex_unlock(&network->nodes[packet->current_x][packet->current_y]);
		packet->current_x++;
	}
	else if(packet->current_x > packet->dest_x)
	{
		pthread_mutex_lock(&network->nodes[packet->current_x-1][packet->current_y]);
		pthread_mutex_unlock(&network->nodes[packet->current_x][packet->current_y]);
		packet->current_x--;
	}
	else if(packet->current_y < packet->dest_y)
	{
		pthread_mutex_lock(&network->nodes[packet->current_x][packet->current_y+1]);
		pthread_mutex_unlock(&network->nodes[packet->current_x][packet->current_y]);
		packet->current_y++;
	}
	else if(packet->current_y > packet->dest_y)
	{
		pthread_mutex_lock(&network->nodes[packet->current_x][packet->current_y-1]);
		pthread_mutex_unlock(&network->nodes[packet->current_x][packet->current_y]);
		packet->current_y--;
	}
	else
	{
		printf("Should not be able to reach this %u %u %u %u\n", packet->current_x,
			packet->current_y, packet->dest_x, packet->dest_y);
	}
}

void init_packet(packet_t* packet)
{
	packet->current_x = rand() % 4;
	packet->current_y = rand() % 4;
	do
	{
		packet->dest_x = rand() % 4;
		packet->dest_y = rand() % 4;
	}
	while(packet->current_x == packet->dest_x && packet->current_y == packet->dest_y);
}

void init_network(network_t* network)
{
	int i, j;
	for(i=0; i<4; i++)
	{
		for(j=0; j<4; j++)
		{
			pthread_mutex_init(&network->nodes[i][j], NULL);
		}
	}
}

void *send_packet(void *arg)
{
	srand(time(NULL));

	uint8_t thread_id = *((uint8_t*)arg);

	packet_t packet;
	init_packet(&packet);
	while(1)
	{
		move_packet(&network, &packet);

		printf("thread #%u current location: %u %u, destination: %u %u\n", thread_id, packet.current_x,
			packet.current_y, packet.dest_x, packet.dest_y);


		if(packet.current_x == packet.dest_x && packet.current_y == packet.dest_y)
		{
			//printf("Packet reached destination!\n");
			pthread_mutex_unlock(&network.nodes[packet.current_x][packet.current_y]);
			init_packet(&packet);
		}
	}
}

int main()
{
	init_network(&network);

	pthread_t p1, p2;
	uint8_t thread1_id = 1;
	uint8_t thread2_id = 2;
	pthread_create(&p1, NULL, send_packet, (void *)&thread1_id);
	pthread_create(&p2, NULL, send_packet, (void *)&thread2_id);
	
	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	
	return 0;
}