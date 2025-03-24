ifndef OMP
	CXX_OMP=-fopenmp
endif

ifdef EPI_DEBUG
	CXX_FLAGS=-O0 -g $(CXX_OMP) -DEPI_DEBUG
else
	CXX_FLAGS=-O3 $(CXX_OMP)
endif

ifndef ENGINE
	ENGINE=podman
endif

ifndef IMAGE
	IMAGE=quay.io/gvegayon/measles:latest
endif

help:
	@echo ""
	@echo "SCHOOL QUARANTINE SIMULATION"
	@echo ""
	@echo "Usage: make [target]"
	@echo ""
	@echo "GENERAL TARGETS"
	@echo "  help            : Display this help message"
	@echo ""
	@echo "CONTAINER TARGETS"
	@echo "  container_build     : Build container"
	@echo "  container_run       : Run container"
	@echo "  container_build_mac : Build container for Mac"
	@echo "  singularity         : Build singularity image"
	@echo "  singularity_mac     : Build singularity image for Mac"
	@echo "  container-push      : Push container to quay.io"


container_build:
	@echo "Building container..." && \
	cd .devcontainer && \
	$(ENGINE) build $(CONT_ARGS) -t epiworld-measles -f ContainerFile . && \
	$(ENGINE) tag epiworld-measles $(IMAGE)

container_run:
	@echo "Running container..."
	$(ENGINE) run -it --rm  \
		--userns=keep-id --mount \
		type=bind,source=$(PWD),target=/measles \
		--workdir /measles/ epiworld-measles bash

container_build_mac:
	CONT_ARGS=--platform=linux/arm64 $(MAKE) container_build

singularity:
	$(ENGINE) run $(CONT_ARGS) -it --rm \
		--mount type=bind,source=$(PWD),target=/measles \
		--workdir /measles/ \
		quay.io/singularity/singularity:v4.1.0 build measles.sif docker://quay.io/gvegayon/measles:latest

singularity_mac:
	CONT_ARGS=--platform=linux/amd64 $(MAKE) singularity

singularity_render_chpc:
	singularity exec --bind=$(PWD):/measles \
		--pwd=/measles/models measles.sif make README.md

container_push: container_build
	$(ENGINE) push $(IMAGE)

container_push_gh:
	$(MAKE) container_build IMAGE=ghcr.io/epiforesite/epiworld-measles:latest
	$(MAKE) container_push IMAGE=ghcr.io/epiforesite/epiworld-measles:latest

hpc_alloc:
	@echo "Allocating resources..."
	salloc --partition=vegayon-shared-np --account=vegayon-np -c40 --mem=100GB

.PHONY: update_epiworld container_build container_run container_build_mac singularity singularity_mac container_push

.PHONY: run all compress replace_html help