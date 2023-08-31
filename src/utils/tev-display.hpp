#include "ipcconnect.hpp"

namespace nanami::util
{
//     struct TevDisplay {
//
//         static void create(const std::string& host) {
//             inst = std::make_unique<TevDisplay>(host);
//         }
//         static TevDisplay& instance() {
//             if (!inst) exit(0);
//             return *inst.get();
//         }
//
//     private:
//         TevDisplay(const std::string& host);
//
//         TevDisplay(const TevDisplay&) = delete;
//         TevDisplay& operator =(const TevDisplay &) = delete;
//
//         static std::unique_ptr<TevDisplay> inst;
//
//         IPCConnect ipc;
//     };

    bool open_image(const std::string& name);
    bool reload_image(const std::string& name);

}
