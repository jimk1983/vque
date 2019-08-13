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
SRC_DIR=/mnt/hgfs/vmshare/vque/src
CFLAG+=-I. -I/mnt/hgfs/vmshare/vque/libs/sources -I/mnt/hgfs/vmshare/vque/deps/include
LDFLAGS+=-L/mnt/hgfs/vmshare/vque/deps/lib -I/mnt/hgfs/vmshare/vque/libs/include -L/mnt/hgfs/vmshare/vque/libs/bins -lvrct -lvos -ldl -lpthread -lrt -lm
LIBS_BIN=/mnt/hgfs/vmshare/vque/libs/bins/
LIBS_INC=/mnt/hgfs/vmshare/vque/libs/include/proxygw/

all : prepare $(BIN_DIR)/proxygw
prepare:
	@mkdir -p $(BIN_DIR) $(OBJ_DIR)  $(OBJ_DIR)/src/main $(OBJ_DIR)/src/share/core 

.PHONY: clean
clean:
	@rm -fr $(BIN_DIR) $(OBJ_DIR) 

$(BIN_DIR)/proxygw :  $(OBJ_DIR)/src/main/main.o $(OBJ_DIR)/src/share/core/core_hash.o
	g++ $(OPTM) -Wl,--as-needed,--sort-common,-rpath=$(LIBS_BIN) -L$(LIBS_BIN) -o $@  $(OBJ_DIR)/src/main/main.o $(OBJ_DIR)/src/share/core/core_hash.o $(LDFLAGS)
ifeq ($(BUILDS),release)
	@strip $@
endif

$(OBJ_DIR)/src/main/main.o : $(ROOT_DIR)/src/main/main.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/src/main/main.cpp

$(OBJ_DIR)/src/share/core/core_hash.o : $(ROOT_DIR)/src/share/core/core_hash.c
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/src/share/core/core_hash.c

