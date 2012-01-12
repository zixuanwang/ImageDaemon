THRIFT_DIR = /usr/local/include/thrift

LIB_DIR = /usr/local/lib

SRC = 		gen-cpp/*.cpp

LIBS =		-lthrift -lboost_system -lboost_filesystem \
			-lopencv_core -lopencv_imgproc -lopencv_highgui \
			-lopencv_features2d -lopencv_flann

TARGET =	ImageDaemon

$(TARGET):	$(SRC)
	g++ -o $(TARGET) -I$(THRIFT_DIR) -I./gen-cpp -L$(LIB_DIR) $(LIBS) $(SRC) -O3 -Wall

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
