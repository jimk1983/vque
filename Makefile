ifeq ($(BUILDS), debug)
	CFLAGS=-g
else ifeq ($(BUILDS), release)
	CFLAGS=-O3
else
    	BUILDS=release
	CFLAGS=-O3
endif

BUILDS_DIR=$(ROOT_BIN)/$(BUILDS)
OBJ_DIR=$(ROOT_BIN)/$(BUILDS)_obj

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
