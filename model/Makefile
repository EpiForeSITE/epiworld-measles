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
	@echo "SCHOOL QUARANTINE SIMULATION"
	@echo ""
	@echo "Usage: make [target]"
	@echo ""
	@echo "GENERAL TARGETS"
	@echo "  help                : Display this help message"
	@echo "  update_epiworld     : Update epiworld model"
	@echo "  school_quarantine.o : Compile the school quarantine model"
	@echo "  run                 : Run the simulation with the default parameters"

school_quarantine.o: school_quarantine.cpp school_quarantine.hpp
	g++ -std=c++14 $(CXX_FLAGS) school_quarantine.cpp -o school_quarantine.o

update_epiworld:
	@echo "Updating epiworld..."
	rsync -avz ../../epiworld/epiworld.hpp .