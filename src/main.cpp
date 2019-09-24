#include "main.hpp"

static const char* url = "https://superheroapi.com/api.php/10206518162748484/search/";
static const char* s_http_port = "8000";
static int s_exit_flag = 0;
string result = "";
static void server_handler(struct mg_connection* nc, int ev, void* ev_data) {
	struct http_message* hm = (struct http_message*) ev_data;
	switch (ev) {
	case MG_EV_ACCEPT: {
		char addr[32];
		mg_sock_addr_to_str(&nc->sa, addr, sizeof(addr),
			MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
		printf("server handler %p: Connection from %s\r\n", nc, addr);
		break;
	}
	case MG_EV_CONNECT: {
		cout << "connection established  " << endl;
	}
	case MG_EV_HTTP_REQUEST: {
		char addr[32];
		mg_sock_addr_to_str(&nc->sa, addr, sizeof(addr),
			MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);

		char superHeroName[100];
		int c = mg_get_http_var(&hm->query_string, "name", superHeroName, sizeof(superHeroName));

		char finalUrl[300] = { 0 };

		strcat(finalUrl, url);

		strcat(finalUrl, superHeroName);

		struct mg_mgr client_mgr;
		struct mg_connection* client_nc;

		mg_mgr_init(&client_mgr, NULL);
		client_nc = mg_connect_http(&client_mgr, ev_handler, finalUrl, NULL, NULL);

		mg_set_protocol_http_websocket(client_nc);

		while (s_exit_flag == 0) {
			mg_mgr_poll(&client_mgr, 1000);
		}

		mg_mgr_free(&client_mgr);

		s_exit_flag = 0;

		mg_send_response_line(nc, 200,
			"Content-Type: text/html; charset=utf-8\r\n"
			"Access-Control-Allow-Origin: *");
		mg_printf(nc,
			"\r\n{\"result\" :%s}\r\n",
			result.c_str());

		nc->flags |= MG_F_SEND_AND_CLOSE;

		result.clear();
		break;
	}
	case MG_EV_HTTP_REPLY: {
		cout << "Server HTTP Reply received " << endl;
		break;
	}
	
	case MG_EV_TIMER:
		cout << "Server Timer expired" << endl;
		nc->flags |= MG_F_CLOSE_IMMEDIATELY;
		break;
	case MG_EV_CLOSE: {
		printf("server handler %p: Connection closed\r4\n", nc);
		break;
	}
	}
}
static void  ev_handler(struct mg_connection* c, int ev, void* p) {
	struct http_message* ev_data = (struct http_message*)p;
	switch (ev) {
	case MG_EV_CONNECT:
		if (*(int*)p != 0) {
			fprintf(stderr, "connect() failed: %s\n", strerror(*(int*)p));
			s_exit_flag = 1;
		}
		break;
	case MG_EV_ACCEPT:
		char addr[32];
		mg_sock_addr_to_str(&c->sa, addr, sizeof(addr),
			MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
		printf("RESTAPI %p: Connection from %s\r\n", c, addr);
		break;
	case MG_EV_HTTP_REPLY:
		if (s_exit_flag == 0) {
			printf("RESTAPI Server closed connection\n");
			s_exit_flag = 1;
		}
		c->flags |= MG_F_SEND_AND_CLOSE;
		putchar('\n');
		break;
	case MG_EV_HTTP_CHUNK:
		result = string(ev_data->body.p, ev_data->body.len);
		break;
	case MG_EV_TIMER:
		cout << "REST API Connection timeout " << endl;
		if (s_exit_flag == 0) {
			printf("RESTAPI Server closed connection\n");
			s_exit_flag = 1;
		}
		c->flags |= MG_F_CLOSE_IMMEDIATELY;
		break;
	case MG_EV_CLOSE:
		if (s_exit_flag == 0) {
			printf("RESTAPI Server closed connection\n");
			s_exit_flag = 1;
		}
		c->flags |= MG_F_SEND_AND_CLOSE;
		break;
	default:
		break;
	}
}

int main(void) {
	struct mg_mgr mgr;
	struct mg_connection* nc;

	mg_mgr_init(&mgr, NULL);
	cout << "starting web server on port " << s_http_port << endl;
	nc = mg_bind(&mgr, s_http_port, server_handler);
	if (nc == NULL) {
		cout << "failed to create listener " << endl;
		return 0;
	}
	mg_set_protocol_http_websocket(nc);

	for (;;) {
		mg_mgr_poll(&mgr, 1000);
	}
	mg_mgr_free(&mgr);

	return 0;
}
