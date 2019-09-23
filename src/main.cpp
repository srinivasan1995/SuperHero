#include "main.hpp"

static const char* url = "https://akabab.github.io/superhero-api/api/id/1.json";
static const char* s_http_port = "8000";
static int s_exit_flag = 0;
string result = "";
static void server_handler(struct mg_connection* nc, int ev, void* ev_data) {
	switch (ev) {
	case MG_EV_ACCEPT: {
		char addr[32];
		mg_sock_addr_to_str(&nc->sa, addr, sizeof(addr),
			MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
		printf("%p: Connection from %s\r\n", nc, addr);
		break;
	}
	case MG_EV_HTTP_REQUEST: {
		struct http_message* hm = (struct http_message*) ev_data;
		char addr[32];
		mg_sock_addr_to_str(&nc->sa, addr, sizeof(addr),
			MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);

		struct mg_mgr client_mgr;
		struct mg_connection* client_nc;

		mg_mgr_init(&client_mgr, NULL);
		client_nc = mg_connect_http(&client_mgr, ev_handler, url, NULL, NULL);

		mg_set_protocol_http_websocket(client_nc);

		while (s_exit_flag == 0) {
			mg_mgr_poll(&client_mgr, 1000);
		}

		mg_mgr_free(&client_mgr);

		s_exit_flag = 0;

		printf("%p: %.*s %.*s\r\n", nc, (int)hm->method.len, hm->method.p,
			(int)hm->uri.len, hm->uri.p);
		//mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
		mg_send_response_line(nc, 200,
		"Content-Type: text/html charset=utf-8\r\n"
			"Connection: close");
		mg_printf(nc,
			//"\r\n<h1>Hello, %s!</h1>\r\n"
			//"You asked for %.*s\r\n"
			"\r\n<body>actual data = %s</body> \r\n",
			//addr, (int)hm->uri.len, hm->uri.p, 
			result.c_str());
		//mg_printf_http_chunk(nc, "%s",result.c_str());

		nc->flags |= MG_F_SEND_AND_CLOSE;

		result.clear();
		break;
	}
	case MG_EV_CLOSE: {
		printf("%p: Connection closed\r4\n", nc);
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
	case MG_EV_HTTP_REPLY:
		c->flags |= MG_F_SEND_AND_CLOSE;
		//if (s_show_headers) {
		result.append(ev_data->body.p, ev_data->body.len);
			//fwrite(ev_data->body.p, 1, ev_data->body.len, stdout);
		cout <<"Srini only strin value is printing "<< result << endl;
		//}
		putchar('\n');
		s_exit_flag = 1;
		break;
	case MG_EV_HTTP_CHUNK:
		//fwrite(ev_data->body.p, 1, ev_data->body.len, stdout);
		s_exit_flag = 1;
		break;
	case MG_EV_CLOSE:
		if (s_exit_flag == 0) {
			printf("Server closed connection\n");
			s_exit_flag = 1;
		}
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
	//nc=mg_connect_http(&mgr, ev_handler, url, NULL, NULL);
	mg_set_protocol_http_websocket(nc);

	/*while (s_exit_flag == 0) {
		mg_mgr_poll(&mgr, 1000);
	}*/
	for (;;) {
		mg_mgr_poll(&mgr, 1000);
	}
	mg_mgr_free(&mgr);

	return 0;
}
