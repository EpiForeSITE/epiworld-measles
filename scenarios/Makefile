ifndef OMP
	CXX_OMP=-fopenmp
endif

ifdef EPI_DEBUG
	CXX_FLAGS=-O0 -g $(CXX_OMP) -DEPI_DEBUG
else
	CXX_FLAGS=-O3 $(CXX_OMP)
endif

help:
	@echo ""
	@echo "SCHOOL QUARANTINE SIMULATION SCENARIOS"
	@echo ""
	@echo "Usage: make [target]"
	@echo ""
	@echo "GENERAL TARGETS"
	@echo "  help                : Display this help message"
	@echo "  compress            : Compress the simulation results"
	@echo "  all                 : Generate all reports"
	@echo "  replace_html        : Replace '&gt;' with '>' in all .md files"
	@echo "  school_quarantine.o : Compile the school quarantine model"
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

school_quarantine.o: ../model/school_quarantine.cpp ../model/school_quarantine.hpp
	cd ../model && \
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
		zip -j -9 simulation_results.zip */*.csv

replace_html:
	sed -i 's/&gt;/>/g' *.md

.PHONY: run all compress replace_html help