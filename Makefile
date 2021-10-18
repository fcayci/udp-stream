TARGET1 = udpserver
TARGET2 = udpclient
CC = g++

SRC_DIR := src
OBJ_DIR := obj


# SRC := $(wildcard $(SRC_DIR)/*.cpp)
# OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)


SRC1 = $(SRC_DIR)/server.cpp $(SRC_DIR)/socket.cpp
SRC2 = $(SRC_DIR)/client.cpp $(SRC_DIR)/socket.cpp
OBJ1 = $(OBJ_DIR)/server.o $(OBJ_DIR)/socket.o
OBJ2 = $(OBJ_DIR)/client.o $(OBJ_DIR)/socket.o


# different library locations
uname_m := $(shell uname -m)
OPENCV_LIB.x86_64  = /usr/lib
OPENCV_LIB.arm64  = /opt/conda/lib
OPENCV_LIB.aarch64 = /usr/lib/aarch64-linux-gnu
OPENCV_LIB = $(OPENCV_LIB.$(uname_m))


CXXFLAGS += -Iinclude
CXXFLAGS += -I/usr/include/opencv4
CXXFLAGS += -I/opt/conda/include/opencv4
CXXFLAGS += -I$(OPENCV_LIB)
CXXFLAGS += -std=c++14

CFLAGS += -Wall -Wextra
CFLAGS += -O2

LDFLAGS := -L$(OPENCV_LIB)

LDLIBS :=

LDFLAGS += -rdynamic
LDFLAGS += -lpthread
LDFLAGS += $(OPENCV_LIB)/libopencv_highgui.dylib
LDFLAGS += $(OPENCV_LIB)/libopencv_video.dylib
LDFLAGS += $(OPENCV_LIB)/libopencv_videoio.dylib
LDFLAGS += $(OPENCV_LIB)/libopencv_imgproc.dylib
LDFLAGS += $(OPENCV_LIB)/libopencv_imgcodecs.dylib
LDFLAGS += $(OPENCV_LIB)/libopencv_core.dylib

all: $(TARGET1) $(TARGET2)

$(TARGET1): $(OBJ1)
	$(CC) $^ $(LDFLAGS) $(LDLIBS) -o $(TARGET1)

$(TARGET2): $(OBJ2)
	$(CC) $^ $(LDFLAGS) $(LDLIBS) -o $(TARGET2)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CXXFLAGS) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $@

clean:
	@$(RM) -rv $(OBJ_DIR) $(TARGET1) $(TARGET2)

.PHONY: all clean

-include $(OBJ:.o=.d)

