
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
