/*
 (c) Niklas K. Voss
*/
namespace ivy
{
	namespace ws
	{
		struct ws_options
		{
			ws_options(void) : port(3000), use_ssl(false) {}
			int port;
			bool use_ssl;
		};
		extern ws_options ws_default;

		bool init(const struct ws_options &options);
		int service(int wait_time);
	}
}