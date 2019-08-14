BUILDS=debug

ifeq ($(BUILDS), debug)
	CFLAG+=-g -O0
	BUILDS=debug
else ifeq ($(BUILDS), release)
	CFLAG+=-O3
	BUILDS=release
else
	CFLAG+=-g -O0
endif

CFLAG+=-Wall -Werror -std=c++11  

#CFLAG+=-fgnu89-inline 

LDFLAGS=

# dependent include
DEP_DIR=/mnt/hgfs/vmshare/vque/deps
OUT_DIR=/mnt/hgfs/vmshare/vque/outs
ROOT_DIR=/mnt/hgfs/vmshare/vque

# installation directory
INST_BIN=/usr/local/bin
INST_LIB=/usr/local/lib
INST_INC=/usr/local/include

# version control
VERSION=1.0.0
VER_MAJOR=1
VER_MINOR=0
VER_REVISION=0

# all : prepare targets...
BIN_DIR=/mnt/hgfs/vmshare/vque/outs/$(BUILDS)
OBJ_DIR=/mnt/hgfs/vmshare/vque/outs/$(BUILDS)_obj
SRC_DIR=/mnt/hgfs/vmshare/vque/example
CFLAG+=-I. -I/mnt/hgfs/vmshare/vque/libs/sources -I-I/mnt/hgfs/vmshare/vque/deps/include
LDFLAGS+=-L/mnt/hgfs/vmshare/vque/deps/lib -I/mnt/hgfs/vmshare/vque/libs/include -L/mnt/hgfs/vmshare/vque/libs/bins -lvos -ldl -lpthread -lrt -lm
LIBS_BIN=/mnt/hgfs/vmshare/vque/libs/bins/
LIBS_INC=/mnt/hgfs/vmshare/vque/libs/include/example/

all : prepare $(BIN_DIR)/example
prepare:
	@mkdir -p $(BIN_DIR) $(OBJ_DIR)  $(OBJ_DIR)/example/main $(OBJ_DIR)/example/client $(OBJ_DIR)/example/msque $(OBJ_DIR)/example/server $(OBJ_DIR)/example/timer $(OBJ_DIR)/example/crct 

.PHONY: clean
clean:
	@rm -fr $(BIN_DIR) $(OBJ_DIR) 

$(BIN_DIR)/example :  $(OBJ_DIR)/example/main/main.o $(OBJ_DIR)/example/client/client.o $(OBJ_DIR)/example/msque/msque.o $(OBJ_DIR)/example/server/server.o $(OBJ_DIR)/example/timer/timer.o $(OBJ_DIR)/example/crct/CEvtRctor.o
	g++ $(OPTM) -Wl,--as-needed,--sort-common,-rpath=$(LIBS_BIN) -L$(LIBS_BIN) -o $@  $(OBJ_DIR)/example/main/main.o $(OBJ_DIR)/example/client/client.o $(OBJ_DIR)/example/msque/msque.o $(OBJ_DIR)/example/server/server.o $(OBJ_DIR)/example/timer/timer.o $(OBJ_DIR)/example/crct/CEvtRctor.o $(LDFLAGS)
ifeq ($(BUILDS),release)
	@strip $@
endif

$(OBJ_DIR)/example/main/main.o : $(ROOT_DIR)/example/main/main.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/example/main/main.cpp

$(OBJ_DIR)/example/client/client.o : $(ROOT_DIR)/example/client/client.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/example/client/client.cpp

$(OBJ_DIR)/example/msque/msque.o : $(ROOT_DIR)/example/msque/msque.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/example/msque/msque.cpp

$(OBJ_DIR)/example/server/server.o : $(ROOT_DIR)/example/server/server.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/example/server/server.cpp

$(OBJ_DIR)/example/timer/timer.o : $(ROOT_DIR)/example/timer/timer.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/example/timer/timer.cpp

$(OBJ_DIR)/example/crct/CEvtRctor.o : $(ROOT_DIR)/example/crct/CEvtRctor.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/example/crct/CEvtRctor.cpp

