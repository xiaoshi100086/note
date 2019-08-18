CC=gcc
AR=ar
ARFLAG=crv

STALIBPATH=-L./
STALIBS=-l-my-a
DYNLIBSPATH=-Wl,-rpath,./
DYNLIBS=-l-my-so
INCLUDE=./

CUR_OBJS=${patsubst %.c, %.o, $(CUR_SOURCE)}


all: lib-my-a.a lib-my-so.so main main1
clean:
	rm -f *.a *.so *.o

lib-my-a.a : lib-my-a.o
	$(AR) $(ARFLAG) $@ $^

lib-my-so.so : lib-my-so.c
	$(CC) -fPIC -shared -o $@ $^

main:main.o lib-my-a.a
	$(CC) -o $@ $^ $(STALIBPATH) $(STALIBS) $(DYNLIBSPATH) $(DYNLIBS)

main1:main.c lib-my-a.a
	$(CC) -o $@ $^ $(STALIBPATH) $(STALIBS) $(DYNLIBSPATH) $(DYNLIBS)

$(CUR_OBJS):%.o:%.c
	$(CC) -c -o $@ $(INCLUDE) $^

# makefile中
#	$@表示最终文件，$^表示依赖文件
#	$(CUR_OBJS):%.o:%.c表示文件集中匹配%.o的做最终文件，并且最终文件后缀.o换成.c,作为最终文件的依赖文件。
# 	CC变量中，值为gcc表示编译c文件，值为g++表示编译cpp文件
#
# gcc的标志位
#	-o 指明输出文件名，如果不使用该参数，则输出文件名为默认值
#	-c 指明输出目标文件，而不知二进制文件
# 	-g 指明需要调试信息
# 		1. 创建符号表，符号表包含了程序中使用的变量名称的列表。
# 		2. 关闭所有的优化机制，以便程序执行过程中严格按照原来的C代码进行
# 	-ggdb 指明gdb专门使用的调试信息
#
# 静态库
# 静态库只能通过.o文件和ar命令生成
# ARFLAG=crv中，c表示如果需要生成新的库文件，不要警告
# 				r表示代替库中现有的文件或者插入新的文件
# 				v表示输出详细信息
# 指定静态库所在路径如果有多个，使用格式'-L'+path1+空格+'-L'+path2
#
# 动态库
# 动态库只能通过.c文件和gcc命令生成
# 指定动态库所在路径如果有多个，使用格式-Wl,-rpath,path1:path
#
# 库文件名
# 库名称建议是lib+库名称+.[a|so],这样可以使用-l+库名称代替lib+名称+.[a|so]，简洁。
#
# 头文件
# 指定多个头文件路径，可以使用path1+空格+path2