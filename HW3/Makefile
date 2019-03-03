cpp = g++
options = -o ram1 
options1 = -o ram3_128 
options2 = -o ram3_256
options3 = -o ram3_128_32
options4 = -o ram3_128_4
c11 = -std=c++11
ram1: ram1.cpp
	$(cpp) -g ram1.cpp $(c11) $(options) 

ram3_128: ram3_128
	$(cpp) -g ram3_128.cpp $(c11) $(options1)

ram3_256: ram3_256
	$(cpp) -g ram3_256.cpp $(c11) $(options2)

ram3_128_32: ram3_128_32
	$(cpp) -g ram3_128_32.cpp $(c11) $(options3)

ram3_128_4: ram3_128_4
	$(cpp) -g ram3_128_32.cpp $(c11) $(options4)

clean:
	rm ram ram1 ram3 ram3_128 ram3_256 ram3_128_32 ram3_128_4
	rm -rf *.dSYM
