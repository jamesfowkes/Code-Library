#ifndef _VERY_TINY_HTTP_H_
#define _VERY_TINY_HTTP_H_

typedef void (*http_get_response_fn)(char const * const url);

typedef struct
{
	char const * const url;
	http_get_response_fn fn;
} http_get_handler;

void http_server_handle_req(http_get_handler * handlers, char const * const recvd);
void http_server_set_response_code(char const * const code);
void http_server_set_header(char const * const field, char const * const value);
void http_server_finish_headers();
void http_server_add_body(char const * const body);
http_get_handler * http_server_match_handler_url(char const * const url, http_get_handler * handlers);
char * http_server_get_response();

#ifndef max
#define max(a,b) ((a > b) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) ((a < b) ? (a) : (b))
#endif

#endif