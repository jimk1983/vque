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
SRC_DIR=/mnt/hgfs/vmshare/vque/libs/sources/vos
CFLAG+=-I/mnt/hgfs/vmshare/vque/libs/sources
LDFLAGS+=-ldl -lpthread -lrt -lm
LIBS_BIN=/mnt/hgfs/vmshare/vque/libs/bins/
LIBS_INC=/mnt/hgfs/vmshare/vque/libs/include/vos/

all : prepare $(BIN_DIR)/libvos.a postpare
prepare:
	@mkdir -p $(BIN_DIR) $(OBJ_DIR)  $(OBJ_DIR)/libs/sources/vos $(OBJ_DIR)/libs/sources/vos/network $(LIBS_INC)

.PHONY: clean
clean:
	@rm -fr $(BIN_DIR) $(OBJ_DIR) $(LIBS_INC) 

postpare:
	@cp -fr $(BIN_DIR)/libvos.a $(LIBS_BIN) 
	@rm -fr $(BIN_DIR)/libvos.a 

$(BIN_DIR)/libvos.a :  $(OBJ_DIR)/libs/sources/vos/vos_utils.o $(OBJ_DIR)/libs/sources/vos/vos_time.o $(OBJ_DIR)/libs/sources/vos/vos_shm.o $(OBJ_DIR)/libs/sources/vos/vos_hash.o $(OBJ_DIR)/libs/sources/vos/vos_event.o $(OBJ_DIR)/libs/sources/vos/vos_mutex.o $(OBJ_DIR)/libs/sources/vos/network/vos_route.o $(OBJ_DIR)/libs/sources/vos/network/vos_sock.o $(OBJ_DIR)/libs/sources/vos/vos_dlist.o
	$(AR) cr $@ $^
	@ranlib  $@

$(OBJ_DIR)/libs/sources/vos/vos_utils.o : $(ROOT_DIR)/libs/sources/vos/vos_utils.c
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/vos/vos_utils.c

$(OBJ_DIR)/libs/sources/vos/vos_time.o : $(ROOT_DIR)/libs/sources/vos/vos_time.c
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/vos/vos_time.c

$(OBJ_DIR)/libs/sources/vos/vos_shm.o : $(ROOT_DIR)/libs/sources/vos/vos_shm.c
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/vos/vos_shm.c

$(OBJ_DIR)/libs/sources/vos/vos_hash.o : $(ROOT_DIR)/libs/sources/vos/vos_hash.c
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/vos/vos_hash.c

$(OBJ_DIR)/libs/sources/vos/vos_event.o : $(ROOT_DIR)/libs/sources/vos/vos_event.c
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/vos/vos_event.c

$(OBJ_DIR)/libs/sources/vos/vos_mutex.o : $(ROOT_DIR)/libs/sources/vos/vos_mutex.c
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/vos/vos_mutex.c

$(OBJ_DIR)/libs/sources/vos/network/vos_route.o : $(ROOT_DIR)/libs/sources/vos/network/vos_route.c
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/vos/network/vos_route.c

$(OBJ_DIR)/libs/sources/vos/network/vos_sock.o : $(ROOT_DIR)/libs/sources/vos/network/vos_sock.c
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/vos/network/vos_sock.c

$(OBJ_DIR)/libs/sources/vos/vos_dlist.o : $(ROOT_DIR)/libs/sources/vos/vos_dlist.c
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/vos/vos_dlist.c

