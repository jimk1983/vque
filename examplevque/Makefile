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
SRC_DIR=/mnt/hgfs/vmshare/vque/examplevque
CFLAG+=-I. -I/mnt/hgfs/vmshare/vque/libs/sources -I-I/mnt/hgfs/vmshare/vque/deps/include
LDFLAGS+=-L/mnt/hgfs/vmshare/vque/deps/lib -I/mnt/hgfs/vmshare/vque/libs/include -L/mnt/hgfs/vmshare/vque/libs/bins -lUtilFrame -lvrct -lvos -ldl -lpthread -lrt -lm
LIBS_BIN=/mnt/hgfs/vmshare/vque/libs/bins/
LIBS_INC=/mnt/hgfs/vmshare/vque/libs/include/examplevque/

all : prepare $(BIN_DIR)/examplevque
prepare:
	@mkdir -p $(BIN_DIR) $(OBJ_DIR)  $(OBJ_DIR)/examplevque/config $(OBJ_DIR)/examplevque/msque $(OBJ_DIR)/examplevque/server 

.PHONY: clean
clean:
	@rm -fr $(BIN_DIR) $(OBJ_DIR) 

$(BIN_DIR)/examplevque :  $(OBJ_DIR)/examplevque/config/config.o $(OBJ_DIR)/examplevque/main.o $(OBJ_DIR)/examplevque/msque/msque.o $(OBJ_DIR)/examplevque/server/server.o
	g++ $(OPTM) -Wl,--as-needed,--sort-common,-rpath=$(LIBS_BIN) -L$(LIBS_BIN) -o $@  $(OBJ_DIR)/examplevque/config/config.o $(OBJ_DIR)/examplevque/main.o $(OBJ_DIR)/examplevque/msque/msque.o $(OBJ_DIR)/examplevque/server/server.o $(LDFLAGS)
ifeq ($(BUILDS),release)
	@strip $@
endif

$(OBJ_DIR)/examplevque/config/config.o : $(ROOT_DIR)/examplevque/config/config.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/examplevque/config/config.cpp

$(OBJ_DIR)/examplevque/main.o : $(ROOT_DIR)/examplevque/main.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/examplevque/main.cpp

$(OBJ_DIR)/examplevque/msque/msque.o : $(ROOT_DIR)/examplevque/msque/msque.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/examplevque/msque/msque.cpp

$(OBJ_DIR)/examplevque/server/server.o : $(ROOT_DIR)/examplevque/server/server.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/examplevque/server/server.cpp

