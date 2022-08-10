json: jsonCppUsage.cpp
	g++ jsonCppUsage.cpp libjsoncpp.a -I include -o json
	rm share/json
	cp json share/json

run:
	./json

clean:
	rm json


# ///

# https://github.com/1570005763/GuanFu/issues/2
# docker run -it -v /Users/zy/code:/home/test ubuntu:latest
# ///