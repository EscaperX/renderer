#include <tev-display.hpp>

int main()
{
    std::string host = "127.0.0.1:14158";
    std::string img_path = "/mnt/c//Users//Stellaris//Downloads//1.png";

    auto &tev = cc::util::TevDisplay::instance(host);

    // cc::util::open_image(img_path);
    //  cc::util::reload_image(img_path);
    // cc::util::create_image("test.png", 128, 128, {"R", "G", "B"});
    cc::util::close_image("/mnt/c//Users//Stellaris//Downloads//1.png");
}
