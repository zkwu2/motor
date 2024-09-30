.PHONY:all

HW_SRC_C := $(wildcard $(HW_DIR)/*.c)
HW_SRC_O := $(patsubst %.c, %.o, $(HW_SRC_C))

# 所有文件编译完成以后将其移到obj目录下
all : $(HW_SRC_O)
	@mv $(HW_SRC_O) $(OBJ_DIR)

# $@表示目标文件，$<表示第一个依赖的文件名
%.o : %.c
	$(CC) -o $@ -c $(CFLAGS) $<