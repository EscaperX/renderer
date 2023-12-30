#pragma once
#include <string>
#include <vector>

namespace cc::resource
{
    /**
     * @brief A single channel of image pixels. Image starts from bottom-left, where the index is (0, 0).
     * Image ends at top-right, where the index is (width, height)
     *
     */
    class Channel
    {
    public:
        /**
         * @brief           Construct a new Channel object
         *
         * @param name      Channel's name, like "R", "G", "B", "Depth"..
         * @param h         Image height
         * @param w         Image width
         */
        Channel(const std::string &name, int h, int w);
        /**
         * @brief           Get the name of the channel.
         *
         * @return const std::string&
         */
        auto get_name() const -> const std::string &;
        /**
         * @brief           Get the raw data
         *
         * @return const std::vector<float>&
         */
        auto get_data() const -> const std::vector<float> &;
        /**
         * @brief           Get the constant pixel at index x.
         *
         * @param x         Index of the pixel.
         * @return float
         */
        auto operator()(int x) const -> float;
        /**
         * @brief           Get the mutable pixel at index x.
         *
         * @param x         Index of the pixel.
         * @return float&
         */
        auto operator()(int x) -> float &;
        /**
         * @brief           Get the constant pixel locating at (wx, hy)
         *
         * @param x         Width
         * @param y         Height
         * @return float
         */
        auto operator()(int x, int y) const -> float;
        /**
         * @brief           Get the mutable pixel locating at (wx, hy)
         *
         * @param x         Width
         * @param y         Height
         * @return float&
         */
        auto operator()(int x, int y) -> float &;
        /**
         * @brief           Update a tile of the image.
         *
         * @param x         The x index of the start point in the target image.
         * @param y         The y index of the start point in the target image.
         * @param w         The width of the coming image.
         * @param h         The height of the coming image.
         * @param update_data The data of coming image.
         */
        auto update_tile(int x, int y, int w, int h, const std::vector<float> &update_data) -> void;

    private:
        std::string name;
        std::vector<float> data;
        int height, width;
    };
    /**
     * @brief A simple image metadata container. The data is arranged in RGBARGBA...
     *
     */
    template<typename T>
    class Image
    {
    public:
        Image() {}
        Image(int w = 1920, int h = 1080): width(w), height(h)
        {
            data.resize(width * height);
        }
        Image(const Image &img):width(img.width), height(img.height), data(img.data)
        {

        }
        // Image &operator=(const Image &);
        // ~Image();

        /**
         * @brief Resizing on image metadatas will simply drop the original data and resize the container.
         *
         * @param nw Target width.
         * @param nh Target height.
         */
        void resize(int nw, int nh)
        {
            data.clear();
            width = nw;
            height = nh;
            data.resize((width * height));
        }

        int width, height;
        std::vector<T> data;
    };

}
