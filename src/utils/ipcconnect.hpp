#pragma once
#include <string>
#include <memory>
namespace nanami::util
{
    struct IPCConnect
    {
        ~IPCConnect();
        IPCConnect(const std::string &host);
        IPCConnect(const IPCConnect &) = delete;
        IPCConnect &operator=(const IPCConnect &) = delete;

        // static void create(const std::string &host) {
        //     inst = std::make_unique<IPCConnect>(host);
        // }
        // static IPCConnect& instance() {
        //     if (!inst)
        //         exit(-1);
        //     return *inst;
        // }

        auto send(const uint8_t *message, int len) const -> bool;
        auto connected() const -> bool;

    private:
        struct impl;
        // static std::unique_ptr<IPCConnect> inst;
        std::unique_ptr<impl> pimpl;
    };
}
