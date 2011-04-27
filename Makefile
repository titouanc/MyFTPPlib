COMPILER = gcc
FLAGS = -Wall
LIBS = -lpthread

all : doc lib

lib : myftpplib.a

doc : doxygen.conf
	doxygen $<

test : test.exe
	sudo chown root test.exe
	sudo chmod +s test.exe
	
myftpplib.a : myftpplib.o mypplib.o
	ar -r $@ $^

test.exe : myftpplib.o mypplib.o test.o
	${COMPILER} ${FLAGS} ${LIBS} -o $@ $^

%.o : %.c %.h
	${COMPILER} ${FLAGS} -c -o $@ $<

clean : 
	rm *.o

cleaner : clean
	rm -f *.exe
	rm -rf doc
	rm *.a
