cpp = g++
options = -o ram1 
options1 = -o ram3
options2 = -o ram2 
c11 = -std=c++11 -w
ram1: ram1.cpp
	$(cpp) -g ram1.cpp $(c11) $(options) 

ram3: ram2.cpp
	$(cpp) -g ram2.cpp $(c11) $(options2)

ram3: ram3.cpp
	$(cpp) -g ram3.cpp $(c11) $(options1)

sysm:
	rm -rf *.dSYM
