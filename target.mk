
$(BIN_DIR)/libvrct.a :  $(OBJ_DIR)/libs/sources/vrct/vrct_api.o $(OBJ_DIR)/libs/sources/vrct/vrct_ctx.o $(OBJ_DIR)/libs/sources/vrct/vrct_expire.o $(OBJ_DIR)/libs/sources/vrct/vrct_msq.o $(OBJ_DIR)/libs/sources/vrct/vrct_net.o $(OBJ_DIR)/libs/sources/vrct/vrct_timer.o
	$(AR) cr $@ $^
	@ranlib  $@
