#include "voxel_map.hpp"


namespace voxelizer
{
    void VoxelMap::InsertPoints(const std::vector<Eigen::Vector3d>& points)
    {
        std::for_each(points.cbegin(), points.cend(), [&] (const auto& pt) {
            const auto voxel = Voxel::BuildVoxelFromPoint(pt, voxel_size_);
            const bool found = grid_.find(voxel) != grid_.end();
            if (!found){
                VoxelPoints voxel_pts;
                voxel_pts.reserve(max_pts_per_voxel_);
                voxel_pts.emplace_back(pt);
                grid_.insert({voxel, std::move(voxel_pts)});
            } else {
                const auto& pts = grid_[voxel];
                if (pts.size() < max_pts_per_voxel_) {
                    grid_[voxel].emplace_back(pt);
                }
            }
        });
    }

    VoxelPoints VoxelMap::GetPointCloud() const
    {
        VoxelPoints points;
        points.reserve(grid_.size() * max_pts_per_voxel_);
        for (const auto& [voxel, voxel_pts] : grid_) {
            points.insert(points.end(), voxel_pts.begin(), voxel_pts.end());
        }
        return points;
    }
}