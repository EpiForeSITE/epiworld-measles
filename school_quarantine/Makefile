ifndef CXX_FLAGS
	CXX_FLAGS=-fopenmp
endif

help:
	@echo "Usage: make [target]"
	@echo "Targets:"
	@echo "  run: Run the simulation with the default parameters"
	@echo "  compress: Compress the simulation results"
	@echo "  texas.md: Run the simulation with the Texas parameters"
	@echo "  davis_90.md: Run the simulation with the Davis 90% parameters"
	@echo "  davis_95.md: Run the simulation with the Davis 95% parameters"
	@echo "  davis_85.md: Run the simulation with the Davis 85% parameters"

run: school_quarantine.o highschool-params.yaml
	rm -f highschool-out/* && \
	./school_quarantine.o highschool-params.yaml

school_quarantine.o: school_quarantine.cpp school_quarantine.hpp
	g++ -std=c++14 $(CXX_FLAGS) -O3 school_quarantine.cpp -o school_quarantine.o

all: \
	canyon_grove_58.md canyon_grove_58_npi.md \
	new_bridge_88.md new_bridge_88_npi.md \
	canyon_crest_74.md canyon_crest_74_npi.md \
	edgemont_92.md edgemont_92_npi.md

calibration.md: calibration_params.yaml 
	ID=calibration $(MAKE) report.md

canyon_grove_58.md: canyon_grove_58_params.yaml 
	ID=canyon_grove_58 $(MAKE) report.md

canyon_grove_58_npi.md: canyon_grove_58_npi_params.yaml 
	ID=canyon_grove_58_npi $(MAKE) report.md

new_bridge_88.md: new_bridge_88_params.yaml 
	ID=new_bridge_88 $(MAKE) report.md

new_bridge_88_npi.md: new_bridge_88_npi_params.yaml 
	ID=new_bridge_88_npi $(MAKE) report.md

canyon_crest_74.md: canyon_crest_74_params.yaml 
	ID=canyon_crest_74 $(MAKE) report.md

canyon_crest_74_npi.md: canyon_crest_74_npi_params.yaml
	ID=canyon_crest_74_npi $(MAKE) report.md

edgemont_92.md: edgemont_92_params.yaml 
	ID=edgemont_92 $(MAKE) report.md

edgemont_92_npi.md: edgemont_92_npi_params.yaml 
	ID=edgemont_92_npi $(MAKE) report.md

report.html: report.qmd 
	quarto render report.qmd \
		-M embed-resources:true \
		-P params_fn:$(ID)_params.yaml \
		-P out_dir:$(ID) \
		--to html && \
	mv report.html $(ID).html

report.md: report.qmd 
	quarto render report.qmd \
		-P params_fn:$(ID)_params.yaml \
		-P out_dir:$(ID) && \
	mv report.md $(ID).md && \
	rm -rf $(ID)_files && mv report_files $(ID)_files && \
	sed -i 's/report_files/$(ID)_files/g' $(ID).md

compress:
	zip -9 simulation_results.zip *.csv


.PHONY: texas/README.md compress all