#include<mongoose.h>
#include<iostream>

using namespace std;
static void ev_handler(struct mg_connection* c, int ev, void* p);
//#include<iostream>
//#include <cpprest/http_client.h>
//#include <cpprest/filestream.h>
//#include<cpprest/>
//using namespace std;
//using namespace utility;                    // Common utilities like string conversions
//using namespace web;                        // Common features like URIs.
//using namespace web::http;                  // Common HTTP functionality
//using namespace web::http::client;          // HTTP client features
//using namespace concurrency::streams;       // Asynchronous streams