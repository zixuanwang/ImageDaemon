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
					ImageResizer.o ShapeFeature.o SURFFeature.o \
					
					
INVERTEDINDEX_OBJS = 	InvertedIndexDaemon_server.o InvertedIndex.o InvertedIndexDaemon_constants.o \
						InvertedIndexDaemon_types.o InvertedIndexDaemon.o \
						GlobalConfig.o HbaseAdapter.o Hbase_constants.o Hbase_types.o \
						Hbase.o DBAdapter.o TypeConverter.o Logger.o
						
ANN_OBJS = 	ANNTreeDaemon.o ANNTreeDaemon_constants.o ANNTreeDaemon_server.o \
			ANNTreeDaemon_types.o TypeConverter.o ANNTreeRoot.o ANNTreeSlave.o \
			Hbase_constants.o Hbase_types.o Hbase.o GlobalConfig.o Logger.o DBAdapter.o \
			HbaseAdapter.o ANNTreeHelper.o Feature.o ColorFeature.o ShapeFeature.o \
			SURFFeature.o ImageResizer.o ANNTreeRootPool.o ANNTreeSlavePool.o

LIBS =		-lthrift -lboost_system -lboost_filesystem \
			-lopencv_core -lopencv_imgproc -lopencv_highgui \
			-lopencv_features2d -lopencv_flann -lopencv_objdetect \
			-lboost_thread -lboost_program_options -levent -lthriftnb \
			-llog4cxx -lapr-1 -laprutil-1 -lmysqlclient

IMAGE_TARGET = ImageDaemon
INVERTEDINDEX_TARGET = InvertedIndexDaemon
ANN_TARGET = ANNTreeDaemon

$(IMAGE_TARGET):	$(IMAGE_OBJS)
	g++ -o $(IMAGE_TARGET) $(LIBS) $(IMAGE_OBJS)
	
$(INVERTEDINDEX_TARGET):	$(INVERTEDINDEX_OBJS)
	g++ -o $(INVERTEDINDEX_TARGET) $(LIBS) $(INVERTEDINDEX_OBJS)
	
$(ANN_TARGET):	$(ANN_OBJS)
	g++ -o $(ANN_TARGET) $(LIBS) $(ANN_OBJS)

%.o: %.cpp %.h
	g++ -c -o $@ $< -I$(THRIFT_DIR) $(CFLAGS) 

all:	$(IMAGE_TARGET) $(INVERTEDINDEX_TARGET) $(ANN_TARGET)

clean:
	rm -f $(IMAGE_OBJS) $(INVERTEDINDEX_OBJS) $(ANN_OBJS) $(IMAGE_TARGET) $(INVERTEDINDEX_TARGET) $(ANN_TARGET)
