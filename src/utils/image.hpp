#pragma once
#include<vector>
#include "math-header.hpp"

namespace nanami::util
{
    /**
     * @brief A single channel of image pixels. Image starts from bottom-left, where the index is (0, 0).
     * Image ends at top-right, where the index is (width, height)
     *
     */
    struct Channel
    {
        /**
         * @brief           Construct a new Channel object
         *
         * @param name      Channel's name, like "R", "G", "B", "Depth"..
         * @param h         Image height
         * @param w         Image width
         */
        Channel(const std::string& name, int h, int w):name(name), height(h), width(w) {
            data.reserve(h * w);
        }
        /**
         * @brief           Get the name of the channel.
         *
         * @return const std::string&
         */
        auto name() const -> const std::string & {
            return name;
        }
        /**
         * @brief           Get the raw data
         *
         * @return const std::vector<float>&
         */
        auto data()  const -> const std::vector<float> & {
            return data;
        }
        /**
         * @brief           Get the constant pixel at index x.
         *
         * @param x         Index of the pixel.
         * @return float
         */
        auto operator()(int x) const -> float {
            return data[x];
        }
        /**
         * @brief           Get the mutable pixel at index x.
         *
         * @param x         Index of the pixel.
         * @return float&
         */
        auto operator()(int x) -> float& {
            return data[x];
        }
        /**
         * @brief           Get the constant pixel locating at (wx, hy)
         *
         * @param x         Width
         * @param y         Height
         * @return float
         */
        auto operator()(int x, int y) const -> float {
            if (x < 0 || x >= width || y < 0 || y >= height) {
                exit(-1);
            }
            return data[y * width + x];
        }
        /**
         * @brief           Get the mutable pixel locating at (wx, hy)
         *
         * @param x         Width
         * @param y         Height
         * @return float&
         */
        auto operator()(int x, int y) -> float& {
            if (x < 0 || x >= width || y < 0 || y >= height) {
                exit(-1);
            }
            return data[y * width + x];
        }
        /**
         * @brief           Update a tile of the image.
         *
         * @param x         The x index of the start point in the target image.
         * @param y         The y index of the start point in the target image.
         * @param w         The width of the coming image.
         * @param h         The height of the coming image.
         * @param update_data The data of coming image.
         */
        auto update_tile(int x, int y, int w, int h, const std::vector<float>& update_data) -> void {
            if (x < 0 || y < 0 || x + w >= width || y + h >= height) {
                return;
            }
            for (int i = 0; i < h; i++)
                for (int j = 0; j < w; j++)
                    this(x + j, y + i) = update_data(i * w + j);
        }
    private:
        std::string name;
        std::vector<float> data;
        int height, width;
    };
    struct Image
    {
        Image() {}
        Image(int w, int h):width(w), height(h)
        {
            data.resize(w * h);
            memset(data.data(), 0, sizeof(T) * w * h);
        }
        // Image(const Image& img) {
        //     width = img.width;
        //     height = img.height;
        //     data = img.data;
        // }

        int width, height;
        std::vector<T> data;
    };



}

