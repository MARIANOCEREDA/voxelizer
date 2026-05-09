# Build any cmake project in the current directory
build:
	mkdir -p build && cd build && cmake .. && make

# Build the voxelizer library
build-voxelizer:
	@echo "Building cpp voxelizer library..."
	@cd cpp/voxelizer && mkdir -p build && cd build && cmake .. && make

# Clean and rebuild the voxelizer library
build-voxelizer-clean:
	@echo "Cleaning cpp voxelizer build..."
	@rm -rf cpp/voxelizer/build
	@$(MAKE) build-voxelizer
