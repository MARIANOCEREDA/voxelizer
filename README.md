# Voxelizer

A C++ library and ROS 2 node that demonstrates **voxel-based point cloud downsampling** and **voxel map construction**. The core idea is straightforward: space is divided into a regular 3D grid of cubes (voxels), and the voxel size directly controls the resolution of the resulting point cloud — larger voxels mean more aggressive downsampling.

---

## Concepts

### Voxel & Downsampling

A **voxel** is a discrete unit of 3D space identified by integer grid coordinates $(i, j, k)$ derived from a continuous point $(x, y, z)$ and a voxel size $s$:

$$i = \lfloor x / s \rfloor, \quad j = \lfloor y / s \rfloor, \quad k = \lfloor z / s \rfloor$$

Downsampling works by keeping **at most one point per voxel**: every incoming point is mapped to its voxel, and if that voxel is already occupied the point is discarded. The result is a uniformly thinned point cloud where the spatial resolution is bounded by $s$.

- **Small voxel size** → fine grid → many voxels occupied → high-resolution output (little downsampling).
- **Large voxel size** → coarse grid → few voxels occupied → low-resolution output (aggressive downsampling).

### Voxel Map

A **voxel map** is a sparse hash map from voxel keys to lists of points. Unlike the single-point downsampler, each voxel can hold up to `max_pts_per_voxel` points before it stops accepting new ones. This lets you retain local density information while still bounding memory usage. The full point cloud can be retrieved by flattening all per-voxel lists.

---

## C++ Library (`cpp/voxelizer`)

### Build

```bash
make build-voxelizer
# or manually:
cd cpp/voxelizer && mkdir -p build && cd build && cmake .. && make
```

---

## ROS 2 Node (`ros/src/voxelizer_ros`)

The `voxelizer_node` wraps the C++ library into a ROS 2 node that subscribes to a raw LiDAR point cloud, downsamples it, and republishes the result.

### Build (colcon)

```bash
# From workspace root
source /opt/ros/<distro>/setup.bash
colcon build --packages-select voxelizer_ros
source install/setup.bash
```

### Run

```bash
ros2 run voxelizer_ros voxelizer_node --ros-args -p voxel_size:=0.5
```

To observe the downsampling effect, visualise both `/points` and `downsampled_cloud` in **RViz2** and vary `voxel_size` at runtime:

```bash
ros2 param set /voxelizer_node voxel_size 1.0   # coarser
ros2 param set /voxelizer_node voxel_size 0.1   # finer
```
