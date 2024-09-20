.PHONY:all

ROBOT_SRC_C := $(wildcard $(ROBOT_DIR)/*.c)
ROBOT_SRC_O := $(patsubst %.c, %.o, $(ROBOT_SRC_C))

# 所有文件编译完成以后将其移到obj目录下
all : $(ROBOT_SRC_O)
	@mv $(ROBOT_SRC_O) $(OBJ_DIR) 

# $@表示目标文件，$<表示第一个依赖的文件名
%.o : %.c
	$(CC) -o $@ -c $(CFLAGS) $<