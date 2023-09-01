#include <tev-display.hpp>

int main()
{
    std::string host = "127.0.0.1:14158";
    std::string img_path = "C://Users//Stellaris//Downloads//1.png";

    nanami::util::IPCConnect::create(host);

    nanami::util::open_image(img_path);
    // nanami::util::reload_image(img_path);
}
