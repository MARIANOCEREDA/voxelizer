#pragma once

#include <vector>

#include <Eigen/Dense>

#include "voxel.hpp"

namespace voxelizer
{
    /**
     * @brief A sparse 3D voxel map that stores point clouds in a hash map keyed by voxel.
     *
     * Each voxel can hold up to @p max_pts_per_voxel points. Points are only inserted
     * into a voxel if it has not yet reached its capacity, effectively downsampling
     * dense point clouds.
     */
    class VoxelMap
    {
        public:
            /**
             * @brief Constructs a VoxelMap with the given spatial parameters.
             * @param voxel_size Side length of each voxel in meters.
             * @param max_pts_per_voxel Maximum number of points stored per voxel.
             */
            explicit VoxelMap(
                double voxel_size,
                const int max_pts_per_voxel
            ) : 
                voxel_size_(voxel_size),
                max_pts_per_voxel_(max_pts_per_voxel)
            {};

            /**
             * @brief Checks whether the voxel map contains any points.
             * @return true if the map is empty, false otherwise.
             */
            inline bool isEmpty() const
            {
                return grid_.empty();
            }

            /**
             * @brief Removes all voxels and their associated points from the map.
             */
            inline void Clear()
            {
                grid_.clear();
            }

            /**
             * @brief Inserts a set of 3D points into the voxel map.
             *
             * Each point is assigned to its corresponding voxel. If the voxel does not
             * exist yet, it is created. Points are discarded once the voxel reaches
             * @p max_pts_per_voxel capacity.
             *
             * @param points Input point cloud to insert.
             */
            void InsertPoints(const std::vector<Eigen::Vector3d>& points);

            /**
             * @brief Collects all points from every voxel into a single point cloud.
             * @return A flat vector containing all points stored in the map.
             */
            VoxelPoints GetPointCloud() const;

        private:
            double voxel_size_;       ///< Side length of each voxel in meters.
            int max_pts_per_voxel_;   ///< Maximum number of points allowed per voxel.
            std::unordered_map<Voxel, VoxelPoints> grid_; ///< Sparse hash map of voxels to their points.
    };
}