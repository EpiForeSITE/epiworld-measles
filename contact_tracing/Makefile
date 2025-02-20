

run: contact_tracing.o highschool-params.yaml
	rm -f highschool-out/* && \
	./contact_tracing.o highschool-params.yaml

contact_tracing.o: contact_tracing.cpp contact_tracing.hpp
	g++ -std=c++14 $(CXX_FLAGS) -O3 contact_tracing.cpp -o contact_tracing.o

