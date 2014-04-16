CC = g++
CFLAGS = -Wall -g
##############################################
OPTION = -D_USE_XBEE_ESC=1 -DQUAD_NB=1
#############################################

#DEBUG_MACRO = -D_DEBUG_XBEE_SEND_MSG=1
##########DEBUG OPTION#######################
#DEBUG_MACRO = -D_DEBUG_CHECKSUM=1 
#DEBUG_MACRO = -D_DEBUG_XBEE_parse_XBEE_msg=1 
#DEBUG_MACRO += -D_DEBUG=1
#DEBUG_MACRO += -D_DEBUG_VERIFY_CHECKSUM=1
#DEBUG_MACRO += -D_DEBUG_XBEE_SEND_MSG=1
#############################################


TARGETS= main test test_geo test_sender test_pprz_msg test_GCS test_recv test_GUI test_XBEE_msg

OBJECT = Swarm.o geo.o Ground_Station.o XBEE.o XBEE_msg.o pprz_msg.o Serial.o GUI.o
GROUND_STATION_SOURCES= Ground_Station.cpp #Swarm.cpp Serial.cpp XBEE.cpp XBEE_msg.cpp pprz_msg.cpp

GTK_FLAG = `pkg-config --cflags gtk+-2.0`
GTK_LINK_FLAG = `pkg-config --libs gtk+-2.0`
PTHREAD_FLAG = -lpthread


all: main test
	rm -f *.o	

main: main.o XBEE.o Serial.o 
	$(CC) -o main *.o $(CFLAGS) $(DEBUG_MACRO) $(OPTION)
	rm -f *.o
###############################################################################
###############################################################################
##############################################################################
test: test.o $(OBJECT)
	$(CC) -o test test.o $(OBJECT) $(CFLAGS) $(DEBUG_MACRO) $(OPTION)
	rm -f *.o
#-----------------------------------------------------------------------------#
test_sender: test_sender.o Serial.o XBEE.o XBEE_msg.o pprz_msg.o
	$(CC) -o test_sender test_sender.o Serial.o XBEE.o XBEE_msg.o pprz_msg.o $(CFLAGS) $(DEBUG_MACRO) $(OPTION)
	rm -f *.o
#-----------------------------------------------------------------------------#
test_pprz_msg: test_pprz_msg.o pprz_msg.o XBEE_msg.o
	$(CC) -o test_pprz_msg test_pprz_msg.o pprz_msg.o XBEE_msg.o $(CFLAGS) $(DEBUG_MACRO) $(OPTION)
	rm -f *.o
#-----------------------------------------------------------------------------#
test_XBEE_msg: test_XBEE_msg.o pprz_msg.o XBEE_msg.o
	$(CC) -o test_XBEE_msg test_XBEE_msg.o pprz_msg.o XBEE_msg.o $(CFLAGS) $(DEBUG_MACRO) $(OPTION)
	rm -f *.o
#-----------------------------------------------------------------------------#
test_GUI: test_GUI.o GUI.o
	$(CC) -o test_GUI test_GUI.o GUI.o $(GTK_LINK_FLAG)
#-----------------------------------------------------------------------------#
test_GCS: test_GCS.o $(OBJECT)
	$(CC) -o test_GCS test_GCS.o $(OBJECT) $(CFLAGS) $(DEBUG_MACRO) $(OPTION) $(PTHREAD_FLAG) $(GTK_LINK_FLAG)
	rm -f *.o
test_recv: test_recv.o $(OBJECT)
	$(CC) -o test_recv test_recv.o $(OBJECT) $(CFLAGS) $(DEBUG_MACRO) $(OPTION)
	rm -f *.o
test_geo: test_geo.cpp geo.o
	$(CC) -o test_geo test_geo.cpp geo.o
test_size: test_size.cpp main.h
	$(CC) -o test_size test_size.cpp
##############################################################################
##############################################################################
##############################################################################
test_GUI.o: test_GUI.cpp
	$(CC) -o test_GUI.o -c test_GUI.cpp $(GTK_FLAG) $(OPTION)

GUI.o: GUI.cpp GUI.h
	$(CC) -o GUI.o -c GUI.cpp $(GTK_FLAG) $(OPTION)

Ground_Station.o: Ground_Station.h Ground_Station.cpp
	$(CC) -o Ground_Station.o -c $(GROUND_STATION_SOURCES) $(CFLAGS) $(DEBUGS_MACRO) $(OPTION) $(GTK_FLAG)
test_GCS.o: test_GCS.cpp
	$(CC) -o test_GCS.o -c test_GCS.cpp $(OPTION) $(GTK_FLAG)
test.o: test.cpp
	$(CC) -o test.o -c test.cpp $(CFLAGS) $(DEBUG_MACRO) $(OPTION)

test_sender.o: test_sender.cpp
	$(CC) -o test_sender.o -c test_sender.cpp $(CFLAGS) $(DEBUG_MACRO) $(OPTION)
test_XBEE_msg.o: test_XBEE_msg.cpp
	$(CC) -o test_XBEE_msg.o -c test_XBEE_msg.cpp $(CFLAGS) $(DEBUG_MACRO) $(OPTION)
test_pprz_msg.o: test_pprz_msg.cpp
	$(CC) -o test_pprz_msg.o -c test_pprz_msg.cpp $(CFLAGS) $(DEBUG_MACRO) $(OPTION)

main.o: main.cpp main.h
	$(CC) -o main.o -c main.cpp $(CFLAGS) $(DEBUG_MACRO) $(OPTION)

Swarm.o : Swarm.cpp Swarm.h main.h address.h
	$(CC) -o Swarm.o -c Swarm.cpp $(CFLAGS) $(DEBUG_MACRO) $(OPTION)

XBEE.o: XBEE.h XBEE.cpp XBEE_msg.cpp
	$(CC) -o XBEE.o -c XBEE.cpp $(CFLAGS) $(DEBUG_MACRO) $(OPTION)

XBEE_msg.o: XBEE_msg.h XBEE_msg.cpp
	$(CC) -o XBEE_msg.o -c XBEE_msg.cpp $(CFLAGS) $(DEBUG_MACRO) $(OPTION)

Serial.o: Serial.h Serial.cpp
	$(CC) -o Serial.o -c Serial.cpp $(CFLAGS) $(DEBUG_MACRO) $(OPTION)

pprz_msg.o: pprz_msg.h pprz_msg.cpp
	$(CC) -o pprz_msg.o -c pprz_msg.cpp $(CFLAGS) $(DEBUG_MACRO) $(OPTION)

geo.o: geo.h geo.cpp GCS_math.h
	$(CC) -o geo.o -c geo.cpp $(CFLAGS) $(DEBUG_MACRO) $(OPTION)
###############################################################################
###############################################################################
###############################################################################
.PHONY: clean 
clean:
	rm -f $(TARGETS)
	rm -f *.o
