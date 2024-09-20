.PHONY:all

APP_SRC_C := $(wildcard $(APP_DIR)/*.c)
APP_SRC_O := $(patsubst %.c, %.o, $(APP_SRC_C))

# 所有文件编译完成以后将其移到obj目录下
all : $(APP_SRC_O)
	@mv $(APP_SRC_O) $(OBJ_DIR)

# $@表示目标文件，$<表示第一个依赖的文件名
%.o : %.c
	$(CC) -o $@ -c $(CFLAGS) $<