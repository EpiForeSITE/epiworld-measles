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

IMAGE=quay.io/gvegayon/measles:latest

help:
	@echo ""
	@echo "SCHOOL QUARANTINE SIMULATION"
	@echo ""
	@echo "Usage: make [target]"
	@echo ""
	@echo "GENERAL TARGETS"
	@echo "  help            : Display this help message"
	@echo "  update_epiworld : Update epiworld model"
	@echo "  run             : Run the simulation with the default parameters"
	@echo "  compress        : Compress the simulation results"
	@echo "  all             : Generate all reports"
	@echo "  replace_html    : Replace '&gt;' with '>' in all .md files"
	@echo ""
	@echo "SCHOOL-SPECIFIC REPORTS"
	@echo "  canyon_grove_58.md     : Canyon Grove 58% vaccination"
	@echo "  canyon_grove_58_npi.md : Canyon Grove 58% vaccination with NPI"
	@echo "  new_bridge_88.md       : New Bridge 88% vaccination"
	@echo "  new_bridge_88_npi.md   : New Bridge 88% vaccination with NPI"
	@echo "  canyon_crest_74.md     : Canyon Crest 74% vaccination"
	@echo "  canyon_crest_74_npi.md : Canyon Crest 74% vaccination with NPI"
	@echo "  edgemont_92.md         : Edgemont 92% vaccination"
	@echo "  edgemont_92_npi.md     : Edgemont 92% vaccination with NPI"
	@echo ""
	@echo "CONTAINER TARGETS"
	@echo "  container_build     : Build container"
	@echo "  container_run       : Run container"
	@echo "  container_build_mac : Build container for Mac"
	@echo "  singularity         : Build singularity image"
	@echo "  singularity_mac     : Build singularity image for Mac"
	@echo "  container-push      : Push container to quay.io"

run: school_quarantine.o highschool-params.yaml
	rm -f highschool-out/* && \
	./school_quarantine.o highschool-params.yaml

school_quarantine.o: school_quarantine.cpp school_quarantine.hpp
	g++ -std=c++14 $(CXX_FLAGS) school_quarantine.cpp -o school_quarantine.o

all: \
	canyon_grove_58.md canyon_grove_58_npi.md \
	new_bridge_88.md new_bridge_88_npi.md \
	canyon_crest_74.md canyon_crest_74_npi.md \
	edgemont_92.md edgemont_92_npi.md 
	
calibration.md: calibration_params.yaml 
	ID=calibration $(MAKE) report.md

canyon_grove_58.md: canyon_grove_58/params.yaml 
	ID=canyon_grove_58 $(MAKE) report.md

canyon_grove_58_npi.md: canyon_grove_58_npi/params.yaml 
	ID=canyon_grove_58_npi $(MAKE) report.md

new_bridge_88.md: new_bridge_88/params.yaml 
	ID=new_bridge_88 $(MAKE) report.md

new_bridge_88_npi.md: new_bridge_88_npi/params.yaml 
	ID=new_bridge_88_npi $(MAKE) report.md

canyon_crest_74.md: canyon_crest_74/params.yaml 
	ID=canyon_crest_74 $(MAKE) report.md

canyon_crest_74_npi.md: canyon_crest_74_npi/params.yaml
	ID=canyon_crest_74_npi $(MAKE) report.md

edgemont_92.md: edgemont_92/params.yaml 
	ID=edgemont_92 $(MAKE) report.md

edgemont_92_npi.md: edgemont_92_npi/params.yaml 
	ID=edgemont_92_npi $(MAKE) report.md

report.html: report.qmd 
	cp report.qmd $(ID)/README.qmd && cd $(ID) && \
	quarto render report.qmd \
		-M embed-resources:true \
		-P params_fn:params.yaml \
		--to html && \
	rm README.qmd

report.md: report.qmd
	cp report.qmd $(ID)/README.qmd && cd $(ID) && \
	quarto render README.qmd \
		-P params_fn:params.yaml && \
	rm README.qmd

compress:
	rm -f simulation_results.zip && \
		zip -9 simulation_results.zip */*.csv

replace_html:
	sed -i 's/&gt;/>/g' *.md


update_epiworld:
	@echo "Updating epiworld..."
	rsync -avz ../epiworld/epiworld.hpp .

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
	$(ENGINE) push quay.io/gvegayon/measles:latest

hpc_alloc:
	@echo "Allocating resources..."
	salloc --partition=vegayon-shared-np --account=vegayon-np -c40 --mem=100GB

.PHONY: update_epiworld container_build container_run container_build_mac singularity singularity_mac container_push

.PHONY: run all compress replace_html help