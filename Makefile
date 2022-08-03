read: jsonCppUsage.cpp
	g++ jsonCppUsage.cpp libjsoncpp.a -I include -o read

run:
	./read

clean:
	rm read


# ///

# https://github.com/1570005763/GuanFu/issues/2
# docker run -it -v /Users/zy/code:/home/test ubuntu:latest
# ///