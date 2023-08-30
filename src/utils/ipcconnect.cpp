#include "ipcconnect.hpp"

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <netinet/in.h>

namespace nanami::util
{
    IPCConnect::IPCConnect(const std::string& host)
        : pimpl(std::make_unique<impl>(host))
    {
    }

    IPCConnect::~IPCConnect() {}

    auto IPCConnect::connected() const -> bool { return pimpl->connected(); }

    struct IPCConnect::impl
    {
        impl(const std::string& host)
        {
            size_t split = host.find_last_of(':');
            if (split == std::string::npos) {
                exit(0);
            }
            url = std::string(host.begin(), host.begin() + split);
            port = std::string(host.begin() + split + 1, host.end());
            connect();
        }
        auto connected() const -> bool {
            return socketfd != -1;
        }
        auto connect() -> void {

            addrinfo hints = {}, *addrinfo;
            hints.ai_family = PF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;
            int err = getaddrinfo(url.c_str(), port.c_str(), &hints, &addrinfo);
            if (err) {
                exit(0);
            }
            socketfd = -1;
            for (struct addrinfo *ptr = addrinfo; ptr; ptr = ptr->ai_next) {
                socketfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
                if (socketfd == -1) {
                    continue;
                }

                if (::connect(socketfd, ptr->ai_addr, ptr->ai_socktype) == -1) {
                    ::close(socketfd);
                    socketfd = -1;
                    continue;
                }

                break;
            }

            freeaddrinfo(addrinfo);

        }
    private:
        std::string url, port;
        int socketfd;
    };

}
