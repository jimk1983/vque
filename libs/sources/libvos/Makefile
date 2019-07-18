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
SRC_DIR=/mnt/hgfs/VMWareShare/openvque/libs/sources/libvos
CFLAG+=-I/mnt/hgfs/VMWareShare/openvque/deps/include
LDFLAGS+=-L/mnt/hgfs/VMWareShare/openvque/deps/lib -I/mnt/hgfs/VMWareShare/openvque/libs/include -L/mnt/hgfs/VMWareShare/openvque/libs/bins -ldl -lpthread -lrt -lm

all : prepare $(BIN_DIR)/libvos.a
prepare:
	@mkdir -p $(BIN_DIR) $(OBJ_DIR)  $(OBJ_DIR)/libs/sources/libvos $(OBJ_DIR)/libs/sources/libvos/network

.PHONY: clean
clean:
	@rm -fr $(BIN_DIR)

$(BIN_DIR)/libvos.a :  $(OBJ_DIR)/libs/sources/libvos/vos_rwlock.o $(OBJ_DIR)/libs/sources/libvos/network/vos_sock.o $(OBJ_DIR)/libs/sources/libvos/network/vos_route.o $(OBJ_DIR)/libs/sources/libvos/vos_mutex.o $(OBJ_DIR)/libs/sources/libvos/vos_spin.o $(OBJ_DIR)/libs/sources/libvos/vos_time.o $(OBJ_DIR)/libs/sources/libvos/vos_dlist.o $(OBJ_DIR)/libs/sources/libvos/vos_event.o
	$(AR) cr $@ $^
	@ranlib  $@

$(OBJ_DIR)/libs/sources/libvos/vos_rwlock.o : $(ROOT_DIR)/libs/sources/libvos/vos_rwlock.c
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/libvos/vos_rwlock.c

$(OBJ_DIR)/libs/sources/libvos/network/vos_sock.o : $(ROOT_DIR)/libs/sources/libvos/network/vos_sock.c
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/libvos/network/vos_sock.c

$(OBJ_DIR)/libs/sources/libvos/network/vos_route.o : $(ROOT_DIR)/libs/sources/libvos/network/vos_route.c
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/libvos/network/vos_route.c

$(OBJ_DIR)/libs/sources/libvos/vos_mutex.o : $(ROOT_DIR)/libs/sources/libvos/vos_mutex.c
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/libvos/vos_mutex.c

$(OBJ_DIR)/libs/sources/libvos/vos_spin.o : $(ROOT_DIR)/libs/sources/libvos/vos_spin.c
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/libvos/vos_spin.c

$(OBJ_DIR)/libs/sources/libvos/vos_time.o : $(ROOT_DIR)/libs/sources/libvos/vos_time.c
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/libvos/vos_time.c

$(OBJ_DIR)/libs/sources/libvos/vos_dlist.o : $(ROOT_DIR)/libs/sources/libvos/vos_dlist.c
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/libvos/vos_dlist.c

$(OBJ_DIR)/libs/sources/libvos/vos_event.o : $(ROOT_DIR)/libs/sources/libvos/vos_event.c
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/libvos/vos_event.c

