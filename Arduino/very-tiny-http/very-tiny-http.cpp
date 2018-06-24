#include <stdint.h>
#include <string.h>

#include <avr/pgmspace.h>

#include "fixed-length-accumulator.h"
#include "very-tiny-http.h"

static char s_response[256];

static FixedLengthAccumulator s_current_response(s_response, 256);

static void get_url(char * url, char const * req)
{
	while (*req != ' ') { req++; }
	req++;

	while (*req != ' ') { *url++ = *req++; }
	*url = '\0';
}

static http_get_handler * match_handler_url(char const * const url, http_get_handler * handlers)
{
	uint8_t i = 0;
	while (handlers[i].fn)
	{
		if (strncmp(url, handlers[i].url, min(strlen(url), strlen(handlers[i].url))) == 0)
		{
			return &handlers[i];
		}
		i++;
	}
	return NULL;
}

void http_server_set_response_code(char const * const code)
{
	s_current_response.writeStringP(PSTR("HTTP/1.1 "));
	s_current_response.writeString(code);
	s_current_response.writeStringP(PSTR("\r\n"));
}

void http_server_set_header(char const * const field, char const * const value)
{
	s_current_response.writeString(field);
	s_current_response.writeStringP(PSTR(": "));
	s_current_response.writeString(value);
	s_current_response.writeStringP(PSTR("\r\n"));
}

void http_server_finish_headers()
{
	s_current_response.writeStringP(PSTR("\r\n"));	
}

void http_server_add_body(char const * const body)
{
	s_current_response.writeString(body);
}

void http_server_handle_req(http_get_handler * handlers, char const * const recvd)
{
	char url[64];

	http_get_handler * handler;
	s_current_response.attach(s_response, 256);

	if ((recvd[0] == 'G') && (recvd[1] == 'E') && (recvd[2] == 'T'))
	{
		get_url(url, recvd);
		if ((handler = match_handler_url(url, handlers)))
		{
			handler->fn(url);
		}
	}

	s_current_response.detach();
}

char * http_server_get_response()
{
	return s_response;
}
