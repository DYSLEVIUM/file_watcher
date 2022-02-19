cpp_version = c++20
executable = file_watcher.exe

run: $(executable)
	./$(executable)

$(executable): file_watcher.h file_watcher.cc
	g++ -std=$(cpp_version) -g file_watcher.cc -o $(executable) -Ofast

clean:
	rm $(executable)