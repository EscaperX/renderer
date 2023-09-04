#include <tev-display.hpp>

int main()
{
    std::string host = "127.0.0.1:14158";
    std::string img_path = "/mnt/c//Users//Stellaris//Downloads//1.png";

    auto &tev = nanami::util::TevDisplay::instance(host);

    // nanami::util::open_image(img_path);
    //  nanami::util::reload_image(img_path);
    // nanami::util::create_image("test.png", 128, 128, {"R", "G", "B"});
    nanami::util::close_image("/mnt/c//Users//Stellaris//Downloads//1.png");
}
