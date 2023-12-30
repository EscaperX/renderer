#pragma once
#include <string>
#include <memory>
namespace cc::util
{
    /**
     * @brief IPC socket communication to the provided host
     *
     */
    struct IPCConnect
    {
        ~IPCConnect();
        /**
         * @brief           Construct a new IPCConnect object
         *
         * @param host      Url in format "x.x.x.x:xxxx"
         */
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
        /**
         * @brief           Send message to another client
         *
         * @param message   Array of chars to send to client
         * @param len       Length of the message
         * @return true
         * @return false
         */
        auto send(const uint8_t *message, int len) const -> bool;

        /**
         * @brief           Connect to remote server. Only called at constructor.
         *
         * @return true
         * @return false
         */
        auto connected() const -> bool;

    private:
        struct impl;
        // static std::unique_ptr<IPCConnect> inst;
        std::unique_ptr<impl> pimpl;
    };
}
