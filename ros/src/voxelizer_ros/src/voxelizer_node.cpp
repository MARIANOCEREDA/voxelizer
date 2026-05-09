#include "voxelizer_ros/voxelizer_node.hpp"

#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include "voxel.hpp"

namespace voxelizer_ros
{
    VoxelizerNode::VoxelizerNode(const rclcpp::NodeOptions &options)
        : Node("voxelizer_node", options)
    {
        this->declare_parameter("voxel_size", 0.5);
        voxel_size_ = this->get_parameter("voxel_size").as_double();

        pcl_sub_ = this->create_subscription<sensor_msgs::msg::PointCloud2>(
            "/points", 10,
            std::bind(&VoxelizerNode::PointCloudCallback, this, std::placeholders::_1));

        downsampled_pcl_pub_ = this->create_publisher<sensor_msgs::msg::PointCloud2>("downsampled_cloud", 10);

        RCLCPP_INFO(this->get_logger(), "Voxelizer_node has been initialized with voxel_size=%.3f", voxel_size_);
    }

    const sensor_msgs::msg::PointCloud2 VoxelizerNode::DownsamplePointCloud(
        const sensor_msgs::msg::PointCloud2::SharedPtr input_cloud)
    {
        pcl::PointCloud<pcl::PointXYZ> pcl_cloud;
        pcl::fromROSMsg(*input_cloud, pcl_cloud);

        voxelizer::VoxelPoints points;
        points.reserve(pcl_cloud.size());
        for (const auto &pt : pcl_cloud.points)
            points.emplace_back(pt.x, pt.y, pt.z);

        const auto downsampled = voxelizer::Voxel::DownsampleVoxel(points, voxel_size_);

        RCLCPP_INFO(this->get_logger(), "Downsampled from %zu to %zu points. Voxel Size [%f]", pcl_cloud.size(), num_points, voxel_size_);

        pcl::PointCloud<pcl::PointXYZ> output_cloud;
        output_cloud.reserve(downsampled.size());
        for (const auto &pt : downsampled)
            output_cloud.emplace_back(
                static_cast<float>(pt.x()),
                static_cast<float>(pt.y()),
                static_cast<float>(pt.z()));

        sensor_msgs::msg::PointCloud2 output_msg;
        pcl::toROSMsg(output_cloud, output_msg);
        output_msg.header = input_cloud->header;
        return output_msg;
    }

    void VoxelizerNode::PointCloudCallback(const sensor_msgs::msg::PointCloud2::SharedPtr msg)
    {
        const auto downsampled = DownsamplePointCloud(msg);
        downsampled_pcl_pub_->publish(downsampled);
    }
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<voxelizer_ros::VoxelizerNode>(rclcpp::NodeOptions());
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}