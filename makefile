full:	
	nvcc *.h conc_uf.cpp tester_full.cpp -o tester_full.exe -Wall -g

seq:	
	g++ *.h conc_uf.cpp tester_seq.cpp -o tester_seq.exe -Wall -g

clean:
	rm -f tester_seq.exe tester_full.exe tester_seq.o tester_full.o conc_uf.o helpers.o