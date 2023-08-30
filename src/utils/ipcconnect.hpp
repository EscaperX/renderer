#pragma once
#include <string>
#include <memory>
#include <vector>
namespace nanami::util
{
    struct IPCConnect
    {
        IPCConnect(const std::string &host);
        ~IPCConnect();
        IPCConnect(const IPCConnect &) = delete;
        IPCConnect &operator=(const IPCConnect &) = delete;

        auto send(std::vector<const uint8_t> message) const -> bool;
        auto connected() const -> bool;
    private:
        struct impl;
        std::unique_ptr<impl> pimpl;
    };
}
