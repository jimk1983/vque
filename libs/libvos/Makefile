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
OBJ_DIR=/mnt/hgfs/VMWareShare/openvque/outs/vos/$(BUILDS)_obj
SRC_DIR=/mnt/hgfs/VMWareShare/openvque/libs/libvos
CFLAG+=-I/mnt/hgfs/VMWareShare/openvque/deps/include

all : prepare $(BIN_DIR)/libvos.a
prepare:
	@mkdir -p $(BIN_DIR) $(OBJ_DIR)  $(BIN_DIR)/libs/libvos $(BIN_DIR)/libs/libvos/network

.PHONY: clean
clean:
	@rm -fr $(BIN_DIR)

$(BIN_DIR)/libvos.a :  $(BIN_DIR)/libs/libvos/vos_time.o $(BIN_DIR)/libs/libvos/network/vos_sock.o $(BIN_DIR)/libs/libvos/vos_rwlock.o $(BIN_DIR)/libs/libvos/vos_mutex.o $(BIN_DIR)/libs/libvos/network/vos_route.o $(BIN_DIR)/libs/libvos/vos_event.o $(BIN_DIR)/libs/libvos/vos_dlist.o $(BIN_DIR)/libs/libvos/vos_spin.o
	$(AR) cr $@ $^
	@ranlib  $@

$(BIN_DIR)/libs/libvos/vos_time.o : $(ROOT_DIR)/libs/libvos/vos_time.c
	gcc $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/libvos/vos_time.c

$(BIN_DIR)/libs/libvos/network/vos_sock.o : $(ROOT_DIR)/libs/libvos/network/vos_sock.c
	gcc $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/libvos/network/vos_sock.c

$(BIN_DIR)/libs/libvos/vos_rwlock.o : $(ROOT_DIR)/libs/libvos/vos_rwlock.c
	gcc $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/libvos/vos_rwlock.c

$(BIN_DIR)/libs/libvos/vos_mutex.o : $(ROOT_DIR)/libs/libvos/vos_mutex.c
	gcc $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/libvos/vos_mutex.c

$(BIN_DIR)/libs/libvos/network/vos_route.o : $(ROOT_DIR)/libs/libvos/network/vos_route.c
	gcc $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/libvos/network/vos_route.c

$(BIN_DIR)/libs/libvos/vos_event.o : $(ROOT_DIR)/libs/libvos/vos_event.c
	gcc $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/libvos/vos_event.c

$(BIN_DIR)/libs/libvos/vos_dlist.o : $(ROOT_DIR)/libs/libvos/vos_dlist.c
	gcc $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/libvos/vos_dlist.c

$(BIN_DIR)/libs/libvos/vos_spin.o : $(ROOT_DIR)/libs/libvos/vos_spin.c
	gcc $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/libvos/vos_spin.c

