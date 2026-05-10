#include "voxel.hpp"

#include "tbb/concurrent_unordered_map.h"
#include "tbb/parallel_for_each.h"

namespace voxelizer
{
    Voxel Voxel::BuildVoxelFromPoint(const VoxelPoint& point, double voxel_size)
    {
        return Voxel(static_cast<int>(std::floor(point.x() / voxel_size)),
                     static_cast<int>(std::floor(point.y() / voxel_size)),
                     static_cast<int>(std::floor(point.z() / voxel_size)),
                     voxel_size);
    }

    VoxelPoints Voxel::DownsampleVoxel(const VoxelPoints& points, const double voxel_size)
    {
        std::unordered_map<Voxel, VoxelPoint> grid;
        std::for_each(points.cbegin(),
                      points.cend(),
                      [&](const auto& pt)
                      {
                          const auto voxel = Voxel::BuildVoxelFromPoint(pt, voxel_size);
                          if (grid.find(voxel) == grid.end())
                          {
                              grid.insert({voxel, pt});
                          }
                      });
        VoxelPoints downsampled_points;
        downsampled_points.reserve(grid.size());
        std::for_each(grid.cbegin(),
                      grid.cend(),
                      [&](const auto& voxel)
                      {
                          const auto& voxel_pt = voxel.second;
                          downsampled_points.emplace_back(voxel_pt);
                      });
        return downsampled_points;
    }

    VoxelPoints Voxel::DownsampleVoxelParallel(const VoxelPoints& points, const double voxel_size)
    {
        tbb::concurrent_unordered_map<Voxel, VoxelPoint> grid;
        tbb::parallel_for_each(points.cbegin(),
                               points.cend(),
                               [&](const auto& pt)
                               {
                                   const auto voxel = Voxel::BuildVoxelFromPoint(pt, voxel_size);
                                   grid.insert({voxel, pt});
                               });
        VoxelPoints downsampled_points;
        downsampled_points.reserve(grid.size());
        std::for_each(grid.cbegin(),
                      grid.cend(),
                      [&](const auto& voxel)
                      {
                          const auto& voxel_pt = voxel.second;
                          downsampled_points.emplace_back(voxel_pt);
                      });
        return downsampled_points;
    }

}  // namespace voxelizer