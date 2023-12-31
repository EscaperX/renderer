#include "ipcconnect.hpp"
#include <vector>
#include <thread>
namespace cc::util
{
    /**
     * @brief TevDisplay is supposed to be a singleton class.
     * It contains a IPCConnect object which holds a connection with
     * Tev Server, and a consuming thread sending data to Tev Server
     */
    struct TevDisplay
    {
    public:
        /**
         * @brief           Initialize an instance of TevDisplay.
         *
         * @param host      Host of Tev should be provided at the first time when calling this function
         * @return TevDisplay&
         */
        static auto instance(const std::string &host = "") -> TevDisplay &;

        /**
         * @brief           Simply forward the message to ipc connection.
         *
         * @param message   Message in format [uint32_t total_length_in_bytes][byte operation_type][byte[] operation_specific_payload]
         * @param len
         * @return true
         * @return false
         */
        auto send(const uint8_t *message, int len) -> bool;

    private:
        TevDisplay(const std::string &host);

        TevDisplay(const TevDisplay &) = delete;
        TevDisplay &operator=(const TevDisplay &) = delete;

        std::thread thread;
        IPCConnect ipc;
    };
    /**
     * @brief           Send command of opening image to Tev server.
     *
     * @param name      File path of target image. Absolute path is preferred.
     * @return true     Successful communication
     * @return false    Failed communication
     */
    auto open_image(const std::string &name) -> bool;

    /**
     * @brief           Send command of closing image to Tev server.
     *
     * @param name      The name of target image.
     * @return true     Successful communication
     * @return false    Failed communication
     */
    auto close_image(const std::string &name) -> bool;

    /**
     * @brief           Send command of reloading the image of providing path
     *
     * @param name      File path of target image. Absolute path is preferred.
     * @return true     Successful communication
     * @return false    Failed communication
     */
    auto reload_image(const std::string &name) -> bool;

    /**
     * @brief           Send command of creating a image object
     *
     * @param name      Image title.
     * @param width     Width of image.
     * @param height    Height of image.
     * @param channel   A vector contains channel names
     * @return true     Successful communication
     * @return false    Failed communication
     */
    auto create_image(const std::string &name, int width, int height, const std::vector<std::string> &channel) -> bool;

    // auto update_image(const std::string &name, )
}
