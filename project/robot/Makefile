.PHONY:all

SRC_C := $(wildcard $(ROBOT_DIR)/*.c)
SRC_O := $(patsubst %.c, %.o, $(SRC_C))

all : $(SRC_O)
	@mv $(SRC_O) $(OBJ_DIR)  #所有文件编译完成以后将其移到obj目录下

%.o : %.c
	$(CC) -o $@ -c $(CFLAGS) $<
# $@表示目标文件，$<表示第一个依赖的文件名