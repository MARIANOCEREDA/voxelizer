#pragma once

#include "rclcpp/rclcpp.hpp"
#include <sensor_msgs/msg/point_cloud2.hpp>

namespace voxelizer_ros
{
    class VoxelizerNode : public rclcpp::Node
    {
    public:
        VoxelizerNode(const rclcpp::NodeOptions &options);

        const sensor_msgs::msg::PointCloud2 DownsamplePointCloud(const sensor_msgs::msg::PointCloud2::SharedPtr input_cloud);

    private:
        void PointCloudCallback(const sensor_msgs::msg::PointCloud2::SharedPtr msg);
        
        double voxel_size_;
        bool use_parallel_;
        bool profile_;

        rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr pcl_sub_;
        rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pcl_pub_;
        rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr downsampled_pcl_pub_;
    };
}