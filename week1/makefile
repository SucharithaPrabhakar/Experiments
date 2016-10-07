CC = g++
CFLAGS1  = -std=c++11 -pthread
CFLAGS2  = -std=c++11 -c -Wall -I ./libraries/boost_1_61_0
CFLAGS3 = -L /usr/libx32/librt.so

COSCHED1 = cosched_1
COSCHED2 = cosched_2
COSCHED3 = cosched_3
COSCHED4 = cosched_4
COSCHED5 = process1
COSCHED6 = process2
COSCHED7 = process3
all: cosched1 cosched2 cosched3 cosched4 cosched5 cosched6 cosched7

cosched1: $(COSCHED1).cpp
	$(CC) $(CFLAGS2) $(COSCHED1).cpp
	$(CC) $(CFLAGS3) $(COSCHED1).o -o $(COSCHED1) -lrt -lpthread

cosched2: $(COSCHED2).cpp
	$(CC) $(CFLAGS2) $(COSCHED2).cpp
	$(CC) $(CFLAGS3) $(COSCHED2).o -o $(COSCHED2) -lrt -lpthread

cosched3: $(COSCHED3).cpp
	$(CC) $(CFLAGS2) $(COSCHED3).cpp
	$(CC) $(CFLAGS3) $(COSCHED3).o -o $(COSCHED3) -lrt -lpthread

cosched4: $(COSCHED4).cpp
	$(CC) $(CFLAGS2) $(COSCHED4).cpp
	$(CC) $(CFLAGS3) $(COSCHED4).o -o $(COSCHED4) -lrt -lpthread

cosched5: $(COSCHED5).cpp
	$(CC) $(CFLAGS2) $(COSCHED5).cpp
	$(CC) $(CFLAGS3) $(COSCHED5).o -o $(COSCHED5) -lrt -lpthread

cosched6: $(COSCHED6).cpp
	$(CC) $(CFLAGS2) $(COSCHED6).cpp
	$(CC) $(CFLAGS3) $(COSCHED6).o -o $(COSCHED6) -lrt -lpthread

cosched7: $(COSCHED7).cpp
	$(CC) $(CFLAGS2) $(COSCHED7).cpp
	$(CC) $(CFLAGS3) $(COSCHED7).o -o $(COSCHED7) -lrt -lpthread

clean:
	$(RM) $(TARGET) $(COSCHED1) $(COSCHED2) $(COSCHED3) $(COSCHED4) $(COSCHED5) $(COSCHED6) $(COSCHED7)

