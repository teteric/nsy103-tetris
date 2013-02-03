
#include "net.h"

void net_init(void) {
	int flag;
	net_connected = 0;

	net_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (net_socket < 0) {
		error("ERROR opening socket");
	}
	printf("[net_init] net_socket set to %d\n", net_socket);
}

void net_connect(char *ip, int port) {
	memset(&net_socket_address, 0, sizeof(net_socket_address));
	net_socket_address.sin_family = AF_INET;
	net_socket_address.sin_addr.s_addr = inet_addr(ip);
	net_socket_address.sin_port = htons(port);

	if (connect(net_socket, (struct sockaddr *)&net_socket_address, sizeof(net_socket_address)) < 0) {
		error("ERROR connecting");
	}
	printf("[net_connect] connected using port %d\n", port);
	net_connected = 1;
}

void net_disconnect(void) {
	net_connected = 0;
	close(net_socket);
	printf("[net_disconnect] closed socket\n");
}

void net_send(int socket, char *buffer) {
	int total = 0;
	int length = strlen(buffer);
	int bytesleft = length;
	int n;

	while (total < length) {
		n = send(socket, buffer + total, bytesleft, MSG_DONTWAIT);
		if (n == -1) {
			break;
		}
		total += n;
		bytesleft -= n;
	}
}

int net_recieve(int socket, char *buffer, int length) {
	int bytes_in_socket = 0;

	bytes_in_socket = recv(socket, buffer, length, MSG_PEEK);
	if (debug == 1) {
		printf("[net_recieve] received %d bytes: %s", bytes_in_socket, buffer);
	}
	if (bytes_in_socket > 0) {
		memset(buffer, '\0', length);
		return recv(socket, buffer, length, 0);
	}
	return bytes_in_socket;
}

// Le client salue le serveur et lui donne le nom qu'il choisi pour jouer
// 100 HELLO <nom_client>
void net_send_name(char *name) {
	char sendBuffer[NET_BUFFER_LENGTH];
	memset(sendBuffer, '\0', sizeof(sendBuffer));
	sprintf(sendBuffer, "100 HELLO %s", name);

	net_send_command(sendBuffer);

	if (net_current_code != 120) {
		sprintf(sendBuffer, "[net_send_name] ERROR! Server did not accept the name (wrong return code) : %d\n", net_current_code);
		error(sendBuffer);
	}
	if (strcmp("HELLO", net_current_command) != 0) {
		sprintf(sendBuffer, "[net_send_name] ERROR! Server did not accept the name (wrong command) %s\n", net_current_command);
		error(sendBuffer);
	}
	if (strcmp(name, net_current_data) != 0) {
		error("[net_send_name] ERROR! Server did not accept the name (wrong name)\n");
	}
}

// Le client est prêt pour le début de la partie
// 110 READY
void net_send_ready(void) {
	net_send_command("110 READY");
}

// Déplacement de la pièce vers la gauche
// 200 LEFT
void net_send_left(void) {
	net_send(net_socket, "200 LEFT");
}

// Déplacement de la pièce vers la droite
// 210 RIGHT
void net_send_right(void) {
	net_send(net_socket, "210 RIGHT");
}

// Déplacement de la pièce d'une ligne vers le bas
// 220 DOWN
void net_send_down(void) {
	net_send(net_socket, "220 DOWN");
}

// Déplacement de la pièce jusqu'au bas du tableau
// 230 FULLDOWN
void net_send_full_down(void) {
	net_send(net_socket, "230 FULLDOWN");
}

// Rotation de la pièce dans le sens horaire
// 240 ROTATE_R
void net_send_rotate_right(void) {
	net_send(net_socket, "240 ROTATE_R");
}

// Rotation de la pièce dans le sens anti-horaire
// 250 ROTATE_L
void net_send_rotate_left(void) {
	net_send(net_socket, "250 ROTATE_L");
}

// Symétrie horizontale de la pièce
// 260 INVERSE
void net_send_invert(void) {
	net_send(net_socket, "260 INVERSE");
}

// Ne rien faire
// 270 PASS
void net_send_pass() {
	net_send(net_socket, "270 PASS");
}

// Demander au serveur une copie du tableau et des pièces
// 280 DUMP
void net_send_dump_request() {
	net_send(net_socket, "280 DUMP");
}

// Demander au serveur une copie du tableau et des pièces de l'adversaire
// 290 DUMPENEMY
void net_send_enemy_dump_request() {
	net_send(net_socket, "290 DUMPENEMY");
}

void net_send_command(char * send_command) {
	net_send(net_socket, send_command);
	net_wait_for_response();
}

void net_wait_for_response(void) {
	char receiveBuffer[NET_BUFFER_LENGTH];
	bzero(receiveBuffer, sizeof(receiveBuffer));
	recv(net_socket, receiveBuffer, sizeof(receiveBuffer), 0);

	net_current_code = 0;
	bzero(net_current_command, sizeof(net_current_command));
	bzero(net_current_data, sizeof(net_current_data));
	sscanf(receiveBuffer, "%d %s %[^\n]", &net_current_code, net_current_command, net_current_data);
}