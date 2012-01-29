CFLAGS = 	-O3 -Wall

THRIFT_DIR = /usr/local/include/thrift

OBJS =		ANNVocabulary.o BoWHistogram.o DBAdapter.o \
			GlobalConfig.o Hbase_constants.o Hbase_types.o \
			Hbase.o Image.o ImageDaemon_constants.o \
			ImageDaemon_types.o ImageDaemon.o InvertedIndex.o \
			LocalFeature.o LocalFeatureExtractor.o MongoDBAdapter.o \
			RankItem.o TypeConverter.o ImageDaemon_server.o \
			ColorFeature.o Feature.o

LIBS =		-lthrift -lboost_system -lboost_filesystem \
			-lopencv_core -lopencv_imgproc -lopencv_highgui \
			-lopencv_features2d -lopencv_flann -lmongoclient -lboost_thread -lboost_program_options

TARGET =	ImageDaemon

$(TARGET):	$(OBJS)
	g++ -o $(TARGET) $(LIBS) $(CFLAGS) $(OBJS)

%.o: %.cpp
	g++ -c -o $@ $< -I$(THRIFT_DIR) $(CFLAGS) 

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
