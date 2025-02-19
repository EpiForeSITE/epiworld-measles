

run: highschool.o highschool-params.yaml
	rm -f highschool-out/* && \
	./highschool.o highschool-params.yaml

highschool.o: highschool.cpp highschool.hpp
	g++ -std=c++14 $(CXX_FLAGS) -O3 highschool.cpp -o highschool.o

