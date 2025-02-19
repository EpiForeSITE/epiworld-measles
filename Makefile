ifndef ENGINE
	ENGINE=podman
endif

ifdef OMP
	CXX_FLAGS=CXX_FLAGS=-fopenmp
endif

IMAGE=quay.io/gvegayon/measles:latest

help:
	@echo "Available targets:"
	@echo "  update-epiworld: Update epiworld model"
	@echo "  container-build: Build container"
	@echo "  container-run: Run container"
	@echo "  container-build-mac: Build container for Mac"
	@echo "  singularity: Build singularity image"
	@echo "  singularity-mac: Build singularity image for Mac"
	@echo "  container-push: Push container to quay.io"

update-epiworld:
	@echo "Updating epiworld..."
	rsync -avz ../epiworld/epiworld.hpp models/

models/README.md: models/README.qmd
	cd models && \
		$(CXX_FLAGS) $(MAKE) highschool.o && \
		quarto render README.qmd

container-build:
	@echo "Building container..."
	$(ENGINE) build $(CONT_ARGS) -t epiworld-measles -f ContainerFile . && \
	$(ENGINE) tag epiworld-measles $(IMAGE)

container-run:
	@echo "Running container..."
	$(ENGINE) run -it --rm --mount \
		type=bind,source=$(PWD),target=/measles \
		--workdir /measles/ epiworld-measles bash

container-build-mac:
	CONT_ARGS=--platform=linux/amd64 $(MAKE) container-build

singularity:
	$(ENGINE) run $(CONT_ARGS) -it --rm \
		--mount type=bind,source=$(PWD),target=/measles \
		--workdir /measles/ \
		quay.io/singularity/singularity:v4.1.0 build measles.sif docker://quay.io/gvegayon/measles:latest

singularity-mac:
	CONT_ARGS=--platform=linux/amd64 $(MAKE) singularity

singularity-render-chpc:
	singularity exec --bind=$(PWD):/measles \
		--pwd=/measles/models measles.sif make README.md

container-push: container-build
	$(ENGINE) push quay.io/gvegayon/measles:latest

.PHONY: update-epiworld container-build container-run container-build-mac singularity singularity-mac container-push

