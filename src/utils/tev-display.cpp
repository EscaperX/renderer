#include "tev-display.hpp"
#include <cstddef>
#include <cstring>

namespace cc::util
{

    enum struct OperationType : uint8_t
    {
        OpenImage = 0,
        ReloadImage = 1,
        CloseImage = 2,
        UpdateImage = 3,
        CreateImage = 4,
    };

    TevDisplay::TevDisplay(const std::string &host) : ipc(host)
    {
    }
    auto TevDisplay::instance(const std::string &host) -> TevDisplay &
    {
        static TevDisplay inst(host);
        return inst;
    }

    auto TevDisplay::send(const uint8_t *message, int len) -> bool
    {
        return ipc.send(message, len);
    }

    void Serialize(uint8_t **ptr, const std::string &s)
    {
        for (size_t i = 0; i < s.size(); ++i, *ptr += 1)
            **ptr = s[i];
        **ptr = '\0';
        *ptr += 1;
    }

    template <typename T>
    void Serialize(uint8_t **ptr, T value)
    {
        memcpy(*ptr, &value, sizeof(T));
        *ptr += sizeof(T);
    }

    bool open_image(const std::string &name)
    {
        int buffer_len = 4 + 1 + 1 + name.length() + 1 + 1;
        uint8_t *buffer = new uint8_t[buffer_len];
        uint8_t *buffer_head = buffer;
        Serialize(&buffer, buffer_len); // reserve space for message length
        Serialize(&buffer, OperationType::OpenImage);
        uint8_t grabFocus = 1;
        Serialize(&buffer, grabFocus);
        Serialize(&buffer, name);
        Serialize(&buffer, '\0'); // channel end

        bool flag = TevDisplay::instance().send(buffer_head, buffer_len);
        delete[] buffer_head;
        return flag;
    }

    auto close_image(const std::string &name) -> bool
    {
        int buffer_len = 4 + 1 + name.length() + 1;
        uint8_t *buffer = new uint8_t[buffer_len];
        uint8_t *buffer_head = buffer;
        Serialize(&buffer, buffer_len); // reserve space for message length
        Serialize(&buffer, OperationType::CloseImage);
        Serialize(&buffer, name);

        bool flag = TevDisplay::instance().send(buffer_head, buffer_len);
        delete[] buffer_head;
        return flag;
    }

    bool reload_image(const std::string &name)
    {
        int buffer_len = 4 + 1 + 1 + name.length() + 1;
        uint8_t *buffer = new uint8_t[buffer_len];
        uint8_t *buffer_head = buffer;
        Serialize(&buffer, buffer_len); // reserve space for message length
        Serialize(&buffer, OperationType::ReloadImage);
        uint8_t grabFocus = 1;
        Serialize(&buffer, grabFocus);
        Serialize(&buffer, name);

        bool flag = TevDisplay::instance().send(buffer_head, buffer_len);
        delete[] buffer_head;
        return flag;
    }

    auto create_image(const std::string &name, int width, int height, const std::vector<std::string> &channel) -> bool
    {
        int channel_len = 0;
        for (auto &str : channel)
            channel_len += (str.length() + 1);
        // buffer len + operation type + grabfocus + name + 1 + w + h + channel sz + channel
        int buffer_len = 4 + 1 + 1 + name.length() + 1 + 4 + 4 + 4 + channel_len;
        uint8_t *buffer = new uint8_t[buffer_len];
        uint8_t *buffer_head = buffer;
        Serialize(&buffer, buffer_len); // reserve space for message length
        Serialize(&buffer, OperationType::CreateImage);
        uint8_t grabFocus = 1;
        Serialize(&buffer, grabFocus);
        Serialize(&buffer, name);
        Serialize(&buffer, width);
        Serialize(&buffer, height);
        int channel_num = channel.size();
        Serialize(&buffer, channel_num);
        for (auto &str : channel)
            Serialize(&buffer, str);

        bool flag = TevDisplay::instance().send(buffer_head, buffer_len);
        delete[] buffer_head;
        return flag;
    }
}
