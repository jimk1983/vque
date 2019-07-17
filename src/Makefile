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
OBJ_DIR=/mnt/hgfs/VMWareShare/openvque/outs/proxygw/$(BUILDS)_obj
SRC_DIR=/mnt/hgfs/VMWareShare/openvque/src
CFLAG+=-I/mnt/hgfs/VMWareShare/openvque/deps/include

all : prepare $(BIN_DIR)/proxygw
prepare:
	@mkdir -p $(BIN_DIR) $(OBJ_DIR)  $(BIN_DIR)/src/main

.PHONY: clean
clean:
	@rm -fr $(BIN_DIR)

 :  $(BIN_DIR)/src/main/main.o
	 $(OPTM) -shared -Wl,--sort-common,-soname,.$(VER_MAJOR) -o $@  $(BIN_DIR)/src/main/main.o
	@rm -f  .$(VER_MAJOR)
	@cd $(BUILDS_DIR) && ldconfig -l .$(VERSION)  && ln -s .$(VER_MAJOR) 
ifeq ($(BUILDS),release)
	@strip $@
endif

$(BIN_DIR)/src/main/main.o : $(ROOT_DIR)/src/main/main.cpp
	gcc $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/src/main/main.cpp

