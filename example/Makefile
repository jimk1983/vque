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
SRC_DIR=/mnt/hgfs/vmshare/vque/example
CFLAG+=-I. -I/mnt/hgfs/vmshare/vque/libs/sources -I-I/mnt/hgfs/vmshare/vque/deps/include
LDFLAGS+=-L/mnt/hgfs/vmshare/vque/deps/lib -I/mnt/hgfs/vmshare/vque/libs/include -L/mnt/hgfs/vmshare/vque/libs/bins -lUtilFrame -lvrct -lvos -ldl -lpthread -lrt -lm
LIBS_BIN=/mnt/hgfs/vmshare/vque/libs/bins/
LIBS_INC=/mnt/hgfs/vmshare/vque/libs/include/example/

all : prepare $(BIN_DIR)/example
prepare:
	@mkdir -p $(BIN_DIR) $(OBJ_DIR)  $(OBJ_DIR)/example/main $(OBJ_DIR)/example/client $(OBJ_DIR)/example/msque $(OBJ_DIR)/example/server $(OBJ_DIR)/example/timer $(OBJ_DIR)/example/crct $(OBJ_DIR)/example/config 

.PHONY: clean
clean:
	@rm -fr $(BIN_DIR) $(OBJ_DIR) 

$(BIN_DIR)/example :  $(OBJ_DIR)/example/main/main.o $(OBJ_DIR)/example/client/client.o $(OBJ_DIR)/example/msque/msque.o $(OBJ_DIR)/example/server/server.o $(OBJ_DIR)/example/timer/timer.o $(OBJ_DIR)/example/crct/cevtrctor_cfg.o $(OBJ_DIR)/example/crct/cevtrctor_net_conn.o $(OBJ_DIR)/example/crct/cevtrctor_net_dispatch.o $(OBJ_DIR)/example/crct/cevtrctor_net_forward.o $(OBJ_DIR)/example/crct/cevtrctor_net_packet.o $(OBJ_DIR)/example/crct/cevtrctor_net_pairs.o $(OBJ_DIR)/example/config/config.o
	g++ $(OPTM) -Wl,--as-needed,--sort-common,-rpath=$(LIBS_BIN) -L$(LIBS_BIN) -o $@  $(OBJ_DIR)/example/main/main.o $(OBJ_DIR)/example/client/client.o $(OBJ_DIR)/example/msque/msque.o $(OBJ_DIR)/example/server/server.o $(OBJ_DIR)/example/timer/timer.o $(OBJ_DIR)/example/crct/cevtrctor_cfg.o $(OBJ_DIR)/example/crct/cevtrctor_net_conn.o $(OBJ_DIR)/example/crct/cevtrctor_net_dispatch.o $(OBJ_DIR)/example/crct/cevtrctor_net_forward.o $(OBJ_DIR)/example/crct/cevtrctor_net_packet.o $(OBJ_DIR)/example/crct/cevtrctor_net_pairs.o $(OBJ_DIR)/example/config/config.o $(LDFLAGS)
ifeq ($(BUILDS),release)
	@strip $@
endif

$(OBJ_DIR)/example/main/main.o : $(ROOT_DIR)/example/main/main.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/example/main/main.cpp

$(OBJ_DIR)/example/client/client.o : $(ROOT_DIR)/example/client/client.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/example/client/client.cpp

$(OBJ_DIR)/example/msque/msque.o : $(ROOT_DIR)/example/msque/msque.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/example/msque/msque.cpp

$(OBJ_DIR)/example/server/server.o : $(ROOT_DIR)/example/server/server.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/example/server/server.cpp

$(OBJ_DIR)/example/timer/timer.o : $(ROOT_DIR)/example/timer/timer.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/example/timer/timer.cpp

$(OBJ_DIR)/example/crct/cevtrctor_cfg.o : $(ROOT_DIR)/example/crct/cevtrctor_cfg.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/example/crct/cevtrctor_cfg.cpp

$(OBJ_DIR)/example/crct/cevtrctor_net_conn.o : $(ROOT_DIR)/example/crct/cevtrctor_net_conn.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/example/crct/cevtrctor_net_conn.cpp

$(OBJ_DIR)/example/crct/cevtrctor_net_dispatch.o : $(ROOT_DIR)/example/crct/cevtrctor_net_dispatch.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/example/crct/cevtrctor_net_dispatch.cpp

$(OBJ_DIR)/example/crct/cevtrctor_net_forward.o : $(ROOT_DIR)/example/crct/cevtrctor_net_forward.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/example/crct/cevtrctor_net_forward.cpp

$(OBJ_DIR)/example/crct/cevtrctor_net_packet.o : $(ROOT_DIR)/example/crct/cevtrctor_net_packet.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/example/crct/cevtrctor_net_packet.cpp

$(OBJ_DIR)/example/crct/cevtrctor_net_pairs.o : $(ROOT_DIR)/example/crct/cevtrctor_net_pairs.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/example/crct/cevtrctor_net_pairs.cpp

$(OBJ_DIR)/example/config/config.o : $(ROOT_DIR)/example/config/config.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/example/config/config.cpp

