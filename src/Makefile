ifeq ($(BUILDS), debug)
	CFLAG+=-g -O0
	BUILDS=debug
else ifeq ($(BUILDS), release)
	CFLAG+=-O3
	BUILDS=release
else
	CFLAG+=-O3
	BUILDS=release
endif

LDFLAGS=

# dependent include
DEP_DIR=/mnt/hgfs/VMWareShare/openvque/deps
OUT_DIR=/mnt/hgfs/VMWareShare/openvque/outs
ROOT_DIR=/mnt/hgfs/VMWareShare/openvque

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
BIN_DIR=/mnt/hgfs/VMWareShare/openvque/outs/$(BUILDS)
OBJ_DIR=/mnt/hgfs/VMWareShare/openvque/outs/$(BUILDS)_obj
SRC_DIR=/mnt/hgfs/VMWareShare/openvque/src
CFLAG+=-I/mnt/hgfs/VMWareShare/openvque/deps/include
LDFLAGS+=-L/mnt/hgfs/VMWareShare/openvque/deps/lib -I/mnt/hgfs/VMWareShare/openvque/libs/include -L/mnt/hgfs/VMWareShare/openvque/libs/bins -lvque -lvos -ldl -lpthread -lrt -lm
LIBS_BIN=/mnt/hgfs/VMWareShare/openvque/libs/bins/
LIBS_INC=/mnt/hgfs/VMWareShare/openvque/libs/include/proxygw/

all : prepare $(BIN_DIR)/proxygw
prepare:
	@mkdir -p $(BIN_DIR) $(OBJ_DIR)  $(OBJ_DIR)/src/share/core $(OBJ_DIR)/src/main 

.PHONY: clean
clean:
	@rm -fr $(BIN_DIR) $(OBJ_DIR) $(LIBS_INC) 

$(BIN_DIR)/proxygw :  $(OBJ_DIR)/src/share/core/core_hash.o $(OBJ_DIR)/src/main/main.o
	g++ $(OPTM) -Wl,--as-needed,--sort-common,-rpath=$(LIBS_BIN) -L$(LIBS_BIN) -o $@  $(OBJ_DIR)/src/share/core/core_hash.o $(OBJ_DIR)/src/main/main.o $(LDFLAGS)
ifeq ($(BUILDS),release)
	@strip $@
endif

$(OBJ_DIR)/src/share/core/core_hash.o : $(ROOT_DIR)/src/share/core/core_hash.c
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/src/share/core/core_hash.c

$(OBJ_DIR)/src/main/main.o : $(ROOT_DIR)/src/main/main.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/src/main/main.cpp

