#include "main.hpp"

static const char* url = "https://akabab.github.io/superhero-api/api/id/1.json";
static int s_exit_flag = 0;
static void server_handler(struct mg_connection* c, int ev, void* p) {

}
static void ev_handler(struct mg_connection* c, int ev, void* p) {
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
			fwrite(ev_data->body.p, 1, ev_data->body.len, stdout);
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
	nc=mg_connect_http(&mgr, ev_handler, url, NULL, NULL);
	mg_set_protocol_http_websocket(nc);

	while (s_exit_flag == 0) {
		mg_mgr_poll(&mgr, 1000);
	}
	mg_mgr_free(&mgr);

	return 0;
}
