PWD_DIR  = $(shell pwd)
ROBOT_DIR= $(PWD_DIR)/robot
APP_DIR  = $(PWD_DIR)/app
INC_DIR  = $(PWD_DIR)/robot
OBJ_DIR  = $(PWD_DIR)/obj
OUT      = $(PWD_DIR)/out
MAKE     = make

CC       = gcc
CFLAGS   = -Wall -O -I $(INC_DIR)

export PWD_DIR ROBOT_DIR APP_DIR OBJ_DIR INC_DIR CC CFLAGS OUT
#导出以上变量给子目录下的Makefile使用
#SHELL和MAKEFLAGS这两个变量，默认会自动传递给子Makefile

all:
	@mkdir -p $(OUT)  # -p表示如果存在out文件夹,不会报错
	@$(MAKE) -C $(ROBOT_DIR)  # -C表示指定子目录
	@$(MAKE) -C $(APP_DIR)  # -C表示指定子目录
	@$(MAKE) -C $(OBJ_DIR)
#(make -C 目录)代表执行指定目录下的makefile，依次执行上面三个目录下的makefile

.PHONY:clean
clean:
	@rm -rf $(OBJ_DIR)/*.o
	@rm -rf $(OUT)

