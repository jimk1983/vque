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
OBJ_DIR=/mnt/hgfs/VMWareShare/openvque/outs/proxygw/$(BUILDS)_obj
SRC_DIR=/mnt/hgfs/VMWareShare/openvque/src
CFLAG+=-I/mnt/hgfs/VMWareShare/openvque/deps/include
LDFLAGS+=-L/mnt/hgfs/VMWareShare/openvque/deps/lib -L/mnt/hgfs/VMWareShare/openvque/libs/linux -lvque -lvos -ldl -lpthread -lrt -lm

all : prepare $(BIN_DIR)/proxygw
prepare:
	@mkdir -p $(BIN_DIR) $(OBJ_DIR)  $(BIN_DIR)/src/share/core $(BIN_DIR)/src/main

.PHONY: clean
clean:
	@rm -fr $(BIN_DIR)

$(BIN_DIR)/proxygw :  $(BIN_DIR)/src/share/core/core_hash.o $(BIN_DIR)/src/main/main.o
	gcc $(OPTM) -Wl,--as-needed,--sort-common,-rpath=.,-rpath=$(BUILDS_DIR) -L$(BUILDS_DIR) -o $@  $(BIN_DIR)/src/share/core/core_hash.o $(BIN_DIR)/src/main/main.o $(LDFLAGS)
ifeq ($(BUILDS),release)
	@strip $@
endif

$(BIN_DIR)/src/share/core/core_hash.o : $(ROOT_DIR)/src/share/core/core_hash.c
	gcc $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/src/share/core/core_hash.c

$(BIN_DIR)/src/main/main.o : $(ROOT_DIR)/src/main/main.cpp
	gcc $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/src/main/main.cpp

