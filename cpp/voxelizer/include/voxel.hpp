#pragma once

#include <vector>
#include <cmath>
#include <functional>

#include <Eigen/Dense>

#include <tsl/robin_map.h>

namespace voxelizer
{
    /// @brief Alias for a single 3D point in double precision.
    typedef Eigen::Vector3d VoxelPoint;

    /// @brief Alias for a collection of 3D points.
    typedef std::vector<VoxelPoint> VoxelPoints;

    /**
     * @brief Represents a single voxel identified by its discrete 3D grid coordinates.
     *
     * A voxel is a unit of space in a 3D grid. Each voxel is uniquely identified
     * by integer coordinates derived from a point in continuous space and a voxel size.
     */
    class Voxel
    {
    public:
        /**
         * @brief Constructs a voxel from discrete grid coordinates and a voxel size.
         * @param x Grid coordinate along the X axis.
         * @param y Grid coordinate along the Y axis.
         * @param z Grid coordinate along the Z axis.
         * @param voxel_size Side length of the voxel in meters.
         */
        Voxel(int x, int y, int z, const double voxel_size) : position_(x, y, z), voxel_size_(voxel_size) {};
        Voxel(const Voxel &other) = default;
        Voxel(Voxel &&other) noexcept = default;
        Voxel &operator=(const Voxel &other) = default;
        Voxel &operator=(Voxel &&other) noexcept = default;

        /**
         * @brief Equality operator based on grid position.
         * @param other The voxel to compare against.
         * @return true if both voxels share the same grid coordinates.
         */
        bool operator==(const Voxel &other) const
        {
            return position_ == other.position_;
        }

        /**
         * @brief Returns the discrete grid position of this voxel.
         * @return Const reference to the integer 3D grid coordinates.
         */
        const Eigen::Vector3i &GetPosition() const { return position_; }

        /**
         * @brief Constructs a voxel from a continuous 3D point and a voxel size.
         * @param point The 3D point to discretize.
         * @param voxel_size Side length of each voxel in meters.
         * @return A Voxel whose grid coordinates correspond to the given point.
         */
        static Voxel BuildVoxelFromPoint(const VoxelPoint &point, double voxel_size);

        /**
         * @brief Downsamples a set of points by retaining one representative point per voxel.
         * @param points Input point cloud to downsample.
         * @param voxel_size Side length of each voxel in meters.
         * @return Downsampled point cloud with at most one point per voxel.
         */
        static VoxelPoints DownsampleVoxel(const VoxelPoints &points, const double voxel_size);

    private:
        Eigen::Vector3i position_; ///< Discrete grid coordinates of the voxel.
        double voxel_size_;        ///< Side length of the voxel in meters.
    };

} // namespace voxelizer

namespace std
{
    template <>
    struct hash<voxelizer::Voxel>
    {
        size_t operator()(const voxelizer::Voxel &voxel) const
        {
            const auto &pos = voxel.GetPosition();
            size_t seed = 0;
            seed ^= std::hash<int>()(pos.x()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= std::hash<int>()(pos.y()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= std::hash<int>()(pos.z()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            return seed;
        }
    };
} // namespace std