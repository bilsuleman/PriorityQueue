build:
	rm -f tests.exe
	g++ -g tests.cpp -o tests.exe -lgtest -lgtest_main -lpthread
	
run:
	./tests.exe

valgrind:
	valgrind --tool=memcheck --leak-check=yes ./tests.exe