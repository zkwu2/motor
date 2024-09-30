PWD_DIR   =  $(shell pwd)
ROBOT_DIR =  $(PWD_DIR)/robot
APP_DIR   =  $(PWD_DIR)/app
HW_DIR    =  $(PWD_DIR)/hardware
OBJ_DIR   =  $(PWD_DIR)/obj

ROBOT_INC_DIR   += $(PWD_DIR)/robot
HW_INC_DIR      += $(PWD_DIR)/hardware

OUT       =  $(PWD_DIR)/out
MAKE      =  make

CC       = gcc
CFLAGS   += -Wall
CFLAGS   += -O1
CFLAGS   += -I$(ROBOT_INC_DIR) -I$(HW_INC_DIR)   #-I 用于指定头文件的搜索路径

#导出以下变量给子目录下的Makefile使用
#SHELL和MAKEFLAGS这两个变量，默认会自动传递给子Makefile
export PWD_DIR ROBOT_DIR APP_DIR HW_DIR OBJ_DIR INC_DIR CC CFLAGS OUT

#(make -C 目录)代表执行指定目录下的makefile，依次执行上面三个目录下的makefile
all:
	@mkdir -p $(OUT)  # -p表示如果存在out文件夹,不会报错
	@$(MAKE) -C $(ROBOT_DIR)  # -C表示指定子目录
	@$(MAKE) -C $(HW_DIR)
	@$(MAKE) -C $(APP_DIR)
	@$(MAKE) -C $(OBJ_DIR)

.PHONY:clean
clean:
	@rm -rf $(OBJ_DIR)/*.o
	@rm -rf $(OUT)

