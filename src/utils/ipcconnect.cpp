#include "ipcconnect.hpp"

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <netinet/in.h>

#include <cstring>
#include <iostream>

namespace nanami::util
{
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
        ~impl()
        {
            if (connected()) {
                disconnect();
            }
        }
        auto connected() const -> bool {
            return socketfd != -1;
        }
        auto connect() -> void {

            socketfd = socket(AF_INET, SOCK_STREAM, 0);
            struct sockaddr_in serv_addr;
            memset(&serv_addr, 0, sizeof(serv_addr));
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_addr.s_addr = inet_addr(url.c_str());
            serv_addr.sin_port = htons(std::atoi(port.c_str()));
            if (::connect(socketfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
                socketfd = -1;
                std::cout << "Connect failed" << std::endl;
                exit(-1);
            }

        }
        auto disconnect() -> void {
            ::close(socketfd);
            socketfd = -1;
        };
        auto send(const uint8_t* message, int len) const -> bool {
            if (!connected()) {
                return false;
            }
            int bytesSent =
                ::send(socketfd, message, len, 0);
            if (bytesSent == len)
                return true;
            return false;
        }
    private:
        std::string url, port;
        int socketfd;
    };

    IPCConnect::IPCConnect(const std::string& host)
        : pimpl(std::make_unique<impl>(host))
    {
    }

    IPCConnect::~IPCConnect() {}

    auto IPCConnect::connected() const -> bool { return pimpl->connected(); }
    auto IPCConnect::send(const uint8_t* message, int len) const -> bool { return pimpl->send(message, len); }

    std::unique_ptr<IPCConnect> IPCConnect::inst;


}
