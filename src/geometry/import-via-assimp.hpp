#include "geometry.hpp"
#include <string>
#include <vector>
namespace nanami::geometry
{
    auto import_mesh_only(const std::string &path) -> std::vector<TriangleMesh>;

} // namespace nanami::geometry
