#include "ipcconnect.hpp"
#include <vector>
#include <thread>
namespace nanami::util
{
    /**
     * @brief TevDisplay is supposed to be a singleton class.
     * It contains a IPCConnect object which holds a connection with
     * Tev Server, and a consuming thread sending data to Tev Server
     */
    struct TevDisplay
    {
    public:
        static auto instance(const std::string &host = "") -> TevDisplay &;
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
