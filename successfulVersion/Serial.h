#ifndef __SERIAL__
#define __SERIAL__

#include <stdio.h>
#include <stdlib.h>
//#include <iostream>

#define uint8_t unsigned int

typedef struct {
  //  uint8_t * dst_buf;              //指向接收缓存
  uint8_t sfd;                        //帧首标志,为0xFF或者0xEE  
  uint8_t sfd_flag;                   //找到帧首,一般是3~5个FF或EE  
  uint8_t sfd_count;                  //帧首的个数,一般3~5个  
  float x;
  float y;                            //坐标值
  uint8_t received_len;               //已经接收的字节数  
  uint8_t find_fram_flag;             //找到完整帧后,置1  
}Frame;

#endif
