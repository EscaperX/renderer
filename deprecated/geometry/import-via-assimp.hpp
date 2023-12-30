#include "geometry.hpp"
#include <string>
#include <vector>
namespace cc::geometry
{
    auto import_mesh_only(const std::string &path) -> std::vector<TriangleMesh>;

} // namespace cc::geometry
