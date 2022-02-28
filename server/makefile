SOURCES = main.cpp

TARGET = server

CC = g++

CXXFLAGS = -g -std=c++11

OBJS:= ${SOURCES:.cpp=.o}

$(TARGET) : $(OBJS)
	$(CC) -o $@ $(OBJS)

.cpp.o:
	$(CC) -c $(CXXFLAGS) $(INCDIR) $<

clean:
	rm -f $(OBJS) $(TARGET) core
