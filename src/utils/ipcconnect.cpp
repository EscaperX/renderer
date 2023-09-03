#include "ipcconnect.hpp"

#include <cstring>
#include <iostream>

#ifdef PLATFORM_LINUX
#include "ipcimpl_linux.inl"
#else
#ifdef PLATFORM_WINDOWS
#include "ipcimpl_win.inl"
#endif
#endif

namespace nanami::util
{

    IPCConnect::IPCConnect(const std::string &host)
        : pimpl(std::make_unique<impl>(host))
    {
    }

    IPCConnect::~IPCConnect() {}

    auto IPCConnect::connected() const -> bool { return pimpl->connected(); }
    auto IPCConnect::send(const uint8_t *message, int len) const -> bool { return pimpl->send(message, len); }

    // std::unique_ptr<IPCConnect> IPCConnect::inst;

}
