ifndef CXX_FLAGS
	CXX_FLAGS=-fopenmp
endif

run: school_quarantine.o highschool-params.yaml
	rm -f highschool-out/* && \
	./school_quarantine.o highschool-params.yaml

school_quarantine.o: school_quarantine.cpp school_quarantine.hpp
	g++ -std=c++14 $(CXX_FLAGS) -O3 school_quarantine.cpp -o school_quarantine.o

README.md: README.qmd school_quarantine.o
	quarto render README.qmd

README.html: README.qmd school_quarantine.o
	quarto render README.qmd --to html 