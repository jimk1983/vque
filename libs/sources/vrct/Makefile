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
SRC_DIR=/mnt/hgfs/wmshare/vque/libs/sources/vrct
CFLAG+=-I/mnt/hgfs/wmshare/vque/libs/sources -I/mnt/hgfs/wmshare/vque/deps/include
LDFLAGS+=-L/mnt/hgfs/wmshare/vque/deps/lib -I/mnt/hgfs/wmshare/vque/libs/include -L/mnt/hgfs/wmshare/vque/libs/bins -lvos -ldl -lpthread -lrt -lm
LIBS_BIN=/mnt/hgfs/wmshare/vque/libs/bins/
LIBS_INC=/mnt/hgfs/wmshare/vque/libs/include/vrct/

all : prepare $(BIN_DIR)/libvrct.a postpare
prepare:
	@mkdir -p $(BIN_DIR) $(OBJ_DIR)  $(OBJ_DIR)/libs/sources/vrct $(LIBS_INC)

.PHONY: clean
clean:
	@rm -fr $(BIN_DIR) $(OBJ_DIR) $(LIBS_INC) 

postpare:
	@cp -fr $(BIN_DIR)/libvrct.a $(LIBS_BIN) 
	@rm -fr $(BIN_DIR)/libvrct.a 

$(BIN_DIR)/libvrct.a :  $(OBJ_DIR)/libs/sources/vrct/vrct_api.o $(OBJ_DIR)/libs/sources/vrct/vrct_ctx.o $(OBJ_DIR)/libs/sources/vrct/vrct_expire.o $(OBJ_DIR)/libs/sources/vrct/vrct_msq.o $(OBJ_DIR)/libs/sources/vrct/vrct_net.o $(OBJ_DIR)/libs/sources/vrct/vrct_timer.o
	$(AR) cr $@ $^
	@ranlib  $@

$(OBJ_DIR)/libs/sources/vrct/vrct_api.o : $(ROOT_DIR)/libs/sources/vrct/vrct_api.c
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/vrct/vrct_api.c

$(OBJ_DIR)/libs/sources/vrct/vrct_ctx.o : $(ROOT_DIR)/libs/sources/vrct/vrct_ctx.c
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/vrct/vrct_ctx.c

$(OBJ_DIR)/libs/sources/vrct/vrct_expire.o : $(ROOT_DIR)/libs/sources/vrct/vrct_expire.c
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/vrct/vrct_expire.c

$(OBJ_DIR)/libs/sources/vrct/vrct_msq.o : $(ROOT_DIR)/libs/sources/vrct/vrct_msq.c
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/vrct/vrct_msq.c

$(OBJ_DIR)/libs/sources/vrct/vrct_net.o : $(ROOT_DIR)/libs/sources/vrct/vrct_net.c
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/vrct/vrct_net.c

$(OBJ_DIR)/libs/sources/vrct/vrct_timer.o : $(ROOT_DIR)/libs/sources/vrct/vrct_timer.c
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/vrct/vrct_timer.c

