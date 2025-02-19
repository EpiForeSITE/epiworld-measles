update-epiworld:
	@echo "Updating epiworld..."
	rsync -avz ../epiworld/epiworld.hpp models/

container-build:
	@echo "Building container..."
	podman build $(BUILD_ARGS) -t epiworld-measles -f ContainerFile .

container-run:
	@echo "Running container..."
	podman run -it --rm --mount \
		type=bind,source=$(shell pwd)/models,target=/models \
		--workdir /models epiworld-measles bash

container-build-mac:
	BUILD_ARGS=--platform=linux/amd64 $(MAKE) container-build

.PHONY: update-epiworld

