/*
 (c) Niklas K. Voss
*/
#include "ws.hpp"

#include <unistd.h>
#include <libwebsockets.h>

#include <vector>
#include <iostream>

namespace ivy
{
	namespace ws
	{
		ws_options ws_default;
		
		libwebsocket_context *context;
		
		std::vector<libwebsocket_protocols> protocols;

		bool init(const ws_options &options)
		{
			lws_context_creation_info info;
			char* interface = NULL;

			info.interface = interface;
			info.port = options.port;
			info.protocols = &protocols[0];
		#ifndef LWS_NO_EXTENSIONS
			info.extensions = libwebsocket_internal_extensions;
		#endif
			if (!options.use_ssl)
			{
				info.ssl_cert_filepath = NULL;
				info.ssl_private_key_filepath = NULL;
			}
			else
			{
				// ...
			}
			info.gid = -1;
			info.uid = -1;
			info.options = 0;

			context = libwebsocket_create_context(&info);
			if (context == NULL)
			{
				std::cout << "Error: unable to create context for WS!" << std::endl;
				return false;
			}
			return true;
		}

		int service(int wait_time)
		{
			return libwebsocket_service(context, wait_time);
		}
	}
}