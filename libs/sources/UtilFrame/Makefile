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
SRC_DIR=/mnt/hgfs/vmshare/vque/libs/sources/UtilFrame
CFLAG+=-I. -I/mnt/hgfs/vmshare/vque/libs/sources
LDFLAGS+=-ldl -lpthread -lrt -lm
LIBS_BIN=/mnt/hgfs/vmshare/vque/libs/bins/
LIBS_INC=/mnt/hgfs/vmshare/vque/libs/include/UtilFrame/

all : prepare $(BIN_DIR)/libUtilFrame.a postpare
prepare:
	@mkdir -p $(BIN_DIR) $(OBJ_DIR)  $(OBJ_DIR)/libs/sources/UtilFrame/opensrc/tinyxml $(OBJ_DIR)/libs/sources/UtilFrame/UtilsFrame $(LIBS_INC)

.PHONY: clean
clean:
	@rm -fr $(BIN_DIR) $(OBJ_DIR) $(LIBS_INC) 

postpare:
	@cp -fr $(BIN_DIR)/libUtilFrame.a $(LIBS_BIN) 
	@rm -fr $(BIN_DIR)/libUtilFrame.a 

$(BIN_DIR)/libUtilFrame.a :  $(OBJ_DIR)/libs/sources/UtilFrame/opensrc/tinyxml/tinystr.o $(OBJ_DIR)/libs/sources/UtilFrame/opensrc/tinyxml/tinyxml.o $(OBJ_DIR)/libs/sources/UtilFrame/opensrc/tinyxml/tinyxmlerror.o $(OBJ_DIR)/libs/sources/UtilFrame/opensrc/tinyxml/tinyxmlparser.o $(OBJ_DIR)/libs/sources/UtilFrame/UtilsFrame/UtilsToolJson.o $(OBJ_DIR)/libs/sources/UtilFrame/UtilsFrame/UtilsToolMap.o $(OBJ_DIR)/libs/sources/UtilFrame/UtilsFrame/UtilsToolStd.o $(OBJ_DIR)/libs/sources/UtilFrame/UtilsFrame/UtilsToolXml.o
	$(AR) cr $@ $^
	@ranlib  $@

$(OBJ_DIR)/libs/sources/UtilFrame/opensrc/tinyxml/tinystr.o : $(ROOT_DIR)/libs/sources/UtilFrame/opensrc/tinyxml/tinystr.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/UtilFrame/opensrc/tinyxml/tinystr.cpp

$(OBJ_DIR)/libs/sources/UtilFrame/opensrc/tinyxml/tinyxml.o : $(ROOT_DIR)/libs/sources/UtilFrame/opensrc/tinyxml/tinyxml.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/UtilFrame/opensrc/tinyxml/tinyxml.cpp

$(OBJ_DIR)/libs/sources/UtilFrame/opensrc/tinyxml/tinyxmlerror.o : $(ROOT_DIR)/libs/sources/UtilFrame/opensrc/tinyxml/tinyxmlerror.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/UtilFrame/opensrc/tinyxml/tinyxmlerror.cpp

$(OBJ_DIR)/libs/sources/UtilFrame/opensrc/tinyxml/tinyxmlparser.o : $(ROOT_DIR)/libs/sources/UtilFrame/opensrc/tinyxml/tinyxmlparser.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/UtilFrame/opensrc/tinyxml/tinyxmlparser.cpp

$(OBJ_DIR)/libs/sources/UtilFrame/UtilsFrame/UtilsToolJson.o : $(ROOT_DIR)/libs/sources/UtilFrame/UtilsFrame/UtilsToolJson.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/UtilFrame/UtilsFrame/UtilsToolJson.cpp

$(OBJ_DIR)/libs/sources/UtilFrame/UtilsFrame/UtilsToolMap.o : $(ROOT_DIR)/libs/sources/UtilFrame/UtilsFrame/UtilsToolMap.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/UtilFrame/UtilsFrame/UtilsToolMap.cpp

$(OBJ_DIR)/libs/sources/UtilFrame/UtilsFrame/UtilsToolStd.o : $(ROOT_DIR)/libs/sources/UtilFrame/UtilsFrame/UtilsToolStd.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/UtilFrame/UtilsFrame/UtilsToolStd.cpp

$(OBJ_DIR)/libs/sources/UtilFrame/UtilsFrame/UtilsToolXml.o : $(ROOT_DIR)/libs/sources/UtilFrame/UtilsFrame/UtilsToolXml.cpp
	g++ $(COPTM) $(CFLAG) -c -o $@ $(ROOT_DIR)/libs/sources/UtilFrame/UtilsFrame/UtilsToolXml.cpp

