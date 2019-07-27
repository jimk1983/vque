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

CFLAG+=-Wall -Werror  

#CFLAG+=-fgnu89-inline 

LDFLAGS=

# dependent include
DEP_DIR=/mnt/hgfs/wmshare/vque/deps
OUT_DIR=/mnt/hgfs/wmshare/vque/outs
ROOT_DIR=/mnt/hgfs/wmshare/vque

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
BIN_DIR=/mnt/hgfs/wmshare/vque/outs/$(BUILDS)
OBJ_DIR=/mnt/hgfs/wmshare/vque/outs/$(BUILDS)_obj
SRC_DIR=/mnt/hgfs/wmshare/vque/examplevque
CFLAG+=-I. -I/mnt/hgfs/wmshare/vque/libs/sources -I-I/mnt/hgfs/wmshare/vque/deps/include
LDFLAGS+=-L/mnt/hgfs/wmshare/vque/deps/lib -I/mnt/hgfs/wmshare/vque/libs/include -L/mnt/hgfs/wmshare/vque/libs/bins -lUtilFrame -lvrct -lvos -ldl -lpthread -lrt -lm
LIBS_BIN=/mnt/hgfs/wmshare/vque/libs/bins/
LIBS_INC=/mnt/hgfs/wmshare/vque/libs/include/examplevque/

all : prepare $(BIN_DIR)/examplevque
prepare:
	@mkdir -p $(BIN_DIR) $(OBJ_DIR)  $(OBJ_DIR)/examplevque 

.PHONY: clean
clean:
	@rm -fr $(BIN_DIR) $(OBJ_DIR) 

$(BIN_DIR)/examplevque :  $(OBJ_DIR)/examplevque/main.o
	g++ $(OPTM) -Wl,--as-needed,--sort-common,-rpath=$(LIBS_BIN) -L$(LIBS_BIN) -o $@  $(OBJ_DIR)/examplevque/main.o $(LDFLAGS)
ifeq ($(BUILDS),release)
	@strip $@
endif

$(OBJ_DIR)/examplevque/main.o : $(ROOT_DIR)/examplevque/main.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/examplevque/main.cpp

