#include "uWebSockets/src/App.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <boost/process.hpp>

void startSessionSpawning();

struct PortFinder {
	uint16_t nextPort = 10000;
	
	uint16_t findPort();
	bool portAvailable(uint16_t port);
};

struct Spawner {
	void 
	startSessionSpawning() 
	{
		PortFinder pf;
		uWS::SSLApp({
			.key_file_name = "",
			.cert_file_name = "",
			.passphrase = ""
		}).get("/*", [this](auto *res, auto *req) {
			auto hostline = req->getHeader("host");
			std::string redirectLocation(hostline);
			std::cout << "hostline:" << hostline << std::endl;
			auto colonIdx = hostline.find(':');
			if (colonIdx != std::string_view::npos) {
				redirectLocation = redirectLocation.substr(0, colonIdx);
			}
			redirectLocation += ":" + std::to_string(pfinder.findPort());
			std::cout << "rl:" << redirectLocation << std::endl;
	//		for (auto itr = req->begin(); itr != req->end(); ++itr) {
	//			std::cout << (*itr).first << ":" << (*itr).second << std::endl;
	//		}
			res->writeStatus("302 Found ")->writeHeader("Location", redirectLocation)->end();
		}).listen(9001, [](auto *listen_socket) {
			printf("listening on 9001\n");
		}).run();
	}
private:
	PortFinder pfinder;
};

int 
main(int argc, char** argv) 
{
	Spawner spawns;
	spawns.startSessionSpawning();
}

uint16_t 
PortFinder::findPort() {
	while (true) {
		std::cout << "loop " << nextPort << std::endl;
		if (nextPort > 40000) {
			nextPort = 10000;
			continue;
		}
		if (portAvailable(nextPort)) {
			return nextPort++;
		}
		++nextPort;
	}
}

bool PortFinder::portAvailable ( uint16_t port ) {
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		std::cout << "socket failed with " << errno << std::endl;
		return false;
	}
	struct sockaddr_in testAddr;
	testAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	testAddr.sin_family = AF_INET;
	testAddr.sin_port = htons(port);
	
	auto res = connect(fd, (struct sockaddr *)&testAddr, sizeof(testAddr));
	close(fd);
	std::cout << "portAvailable(" << port << ") = " << res << " errno=" << errno << std::endl;
	return res == -1;
}








