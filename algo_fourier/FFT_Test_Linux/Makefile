#compilateur 
CXX = gcc

OS = "UNIX"

all: FFT

# for Unix
ifeq ($(OS),"UNIX")
	@echo Build completed
else
# for Windows
	@if exist *.axf echo Build completed
endif

clean:
	rm -f FFT *~ *.o ./test/*~ ./fft/*~


CFLAGS = -c -g -O2 -W
LIBS = -lm

COMMON = t000008.o t000016.o t000032.o t000064.o t000128.o t000256.o t000512.o t001024.o FFTTest.o

FFT: $(COMMON) FFT.o
	$(CXX) $(COMMON) FFT.o \
	-o FFT $(LIBS)

FFT.o: fft/FFT.c
	$(CXX) $(CFLAGS) fft/FFT.c $(LIBS)

t000008.o: test/t000008.c
	$(CXX) $(CFLAGS) test/t000008.c -o t000008.o

t000016.o: test/t000016.c
	$(CXX) $(CFLAGS) test/t000016.c -o t000016.o

t000032.o: test/t000032.c
	$(CXX) $(CFLAGS) test/t000032.c -o t000032.o

t000064.o: test/t000064.c
	$(CXX) $(CFLAGS) test/t000064.c -o t000064.o

t000128.o: test/t000128.c
	$(CXX) $(CFLAGS) test/t000128.c -o t000128.o

t000256.o: test/t000256.c
	$(CXX) $(CFLAGS) test/t000256.c -o t000256.o

t000512.o: test/t000512.c
	$(CXX) $(CFLAGS) test/t000512.c -o t000512.o

t001024.o: test/t001024.c
	$(CXX) $(CFLAGS) test/t001024.c -o t001024.o

FFTTest.o: test/FFTTest.c fft/FFT.h
	$(CXX) $(CFLAGS) test/FFTTest.c -o FFTTest.o 
