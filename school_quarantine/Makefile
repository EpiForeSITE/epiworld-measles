ifndef CXX_FLAGS
	CXX_FLAGS=-fopenmp
endif

run: school_quarantine.o highschool-params.yaml
	rm -f highschool-out/* && \
	./school_quarantine.o highschool-params.yaml

school_quarantine.o: school_quarantine.cpp school_quarantine.hpp
	g++ -std=c++14 $(CXX_FLAGS) -O3 school_quarantine.cpp -o school_quarantine.o

texas.md: texas_params.yaml school_quarantine.o
	ID=texas $(MAKE) report.md

davis_90.md: davis_90_params.yaml school_quarantine.o
	ID=davis_90 $(MAKE) report.md

davis_95.md: davis_95_params.yaml school_quarantine.o
	ID=davis_95 $(MAKE) report.md

davis_85.md: davis_85_params.yaml school_quarantine.o
	ID=davis_85 $(MAKE) report.md

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


.PHONY: texas/README.md compress