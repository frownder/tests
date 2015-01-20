
CC	=gcc
TARGET	=fstest
OBJS	= main.o
SRCS    = main.c


#OPTION
LFLAGS	= -lccp_fs_api -lnacc -lpthread -lstdc++

#DEPENDENCY
all:$(TARGET) 

$(TARGET):$(OBJS) 
	$(CC) -o $@ $(OBJS) -L. $(LFLAGS) 

$(OBJS):$(SRCS)
	$(CC) -g -c -o $@ $(SRCS)

clean:
	rm -rf $(OBJS) $(TARGET)
