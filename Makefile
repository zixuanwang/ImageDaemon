CFLAGS = 	-O3 -Wall -DHAVE_INTTYPES_H -DHAVE_NETINET_IN_H

THRIFT_DIR = /usr/local/include/thrift

IMAGE_OBJS =		ANNVocabulary.o BoWHistogram.o DBAdapter.o \
					GlobalConfig.o Hbase_constants.o Hbase_types.o \
					Hbase.o Image.o ImageDaemon_constants.o \
					ImageDaemon_types.o ImageDaemon.o Logger.o \
					LocalFeature.o LocalFeatureExtractor.o HbaseAdapter.o \
					RankItem.o TypeConverter.o ImageDaemon_server.o \
					ColorFeature.o Feature.o Ticker.o RobustMatcher.o \
					ImageRecognizer.o InvertedIndexClient.o InvertedIndexDaemon.o \
					InvertedIndexDaemon_constants.o InvertedIndexDaemon_types.o \
					ImageResizer.o ShapeFeature.o\
					
					
INVERTEDINDEX_OBJS = 	InvertedIndexDaemon_server.o InvertedIndex.o InvertedIndexDaemon_constants.o \
						InvertedIndexDaemon_types.o InvertedIndexDaemon.o \
						GlobalConfig.o HbaseAdapter.o Hbase_constants.o Hbase_types.o \
						Hbase.o DBAdapter.o TypeConverter.o Logger.o

LIBS =		-lthrift -lboost_system -lboost_filesystem \
			-lopencv_core -lopencv_imgproc -lopencv_highgui \
			-lopencv_features2d -lopencv_flann -lopencv_objdetect \
			-lboost_thread -lboost_program_options -levent -lthriftnb \
			-llog4cxx -lapr-1 -laprutil-1

IMAGE_TARGET = ImageDaemon
INVERTEDINDEX_TARGET = InvertedIndexDaemon

$(IMAGE_TARGET):	$(IMAGE_OBJS)
	g++ -o $(IMAGE_TARGET) $(LIBS) $(IMAGE_OBJS)
	
$(INVERTEDINDEX_TARGET):	$(INVERTEDINDEX_OBJS)
	g++ -o $(INVERTEDINDEX_TARGET) $(LIBS) $(INVERTEDINDEX_OBJS)

%.o: %.cpp %.h
	g++ -c -o $@ $< -I$(THRIFT_DIR) $(CFLAGS) 

all:	$(IMAGE_TARGET) $(INVERTEDINDEX_TARGET)

clean:
	rm -f $(IMAGE_OBJS) $(INVERTEDINDEX_OBJS) $(IMAGE_TARGET) $(INVERTEDINDEX_TARGET)
