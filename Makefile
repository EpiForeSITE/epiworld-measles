ifndef ENGINE
	ENGINE=podman
endif

ifdef OMP
	CXX_FLAGS=CXX_FLAGS=-fopenmp
endif

IMAGE=quay.io/gvegayon/measles:latest

help:
	@echo "Available targets:"
	@echo "  update_epiworld: Update epiworld model"
	@echo "  container_build: Build container"
	@echo "  container_run: Run container"
	@echo "  container_build_mac: Build container for Mac"
	@echo "  singularity: Build singularity image"
	@echo "  singularity_mac: Build singularity image for Mac"
	@echo "  container-push: Push container to quay.io"

update_epiworld:
	@echo "Updating epiworld..."
	rsync -avz ../epiworld/epiworld.hpp .

models/README.md: models/README.qmd
	cd models && \
		$(CXX_FLAGS) $(MAKE) highschool.o && \
		quarto render README.qmd

container_build:
	@echo "Building container..."
	$(ENGINE) build $(CONT_ARGS) -t epiworld-measles -f ContainerFile . && \
	$(ENGINE) tag epiworld-measles $(IMAGE)

container_run:
	@echo "Running container..."
	$(ENGINE) run -it --rm --mount \
		type=bind,source=$(PWD),target=/measles \
		--workdir /measles/ epiworld-measles bash

container_build_mac:
	CONT_ARGS=--platform=linux/amd64 $(MAKE) container_build

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
	$(ENGINE) push quay.io/gvegayon/measles:latest

hpc_alloc:
	@echo "Allocating resources..."
	salloc --partition=vegayon-shared-np --account=vegayon-np -c40 --mem=100GB

.PHONY: update_epiworld container_build container_run container_build_mac singularity singularity_mac container_push

