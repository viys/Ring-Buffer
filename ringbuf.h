/*
 * @Author: yuzy 2628587386@qq.com
 * @Date: 2023-07-11 20:55:54
 * @LastEditors: yuzy 2628587386@qq.com
 * @LastEditTime: 2023-07-11 23:40:38
 * @FilePath: \Ring-Buffer\ringbuf.h
 */

#ifndef __RINGBUFF_H
#define __RINGBUFF_H

#include "stdarg.h"
#include "stdio.h"
#include "string.h"

/* 屏蔽数据类型差异 */
typedef unsigned          char u8;
typedef unsigned short     int u16;
typedef unsigned           int u32;

/* 宏定义 */
#define U1_RX_SIZE  2048
#define U1_RX_MAX   256

#define URxNUM      10

/* RingBuf帧地址结构体 */
typedef struct{
    /* s用于标记起始位置 */
    uint8_t *start;
    /* e用于标记结束位置 */
    uint8_t *end;
}RingBuf_Ptr;

/* RingBuf控制区指针结构体 */
typedef struct{
    /* 积累接受数据量 */
    u16 URxCounter;
    /* se指针对于结构体数组 */
    RingBuf_Ptr URxDataPtr[URxNUM];
    /* IN指针用于标记接收数据 */
    RingBuf_Ptr *URxDataIN;
    /* OUT指针用于提取接受的数据 */
    RingBuf_Ptr *URxDataOUT;
    /* IN 和 OUT 指针的结尾标志 */
    RingBuf_Ptr *URxDataEND;
}RingBuf_CB;

/* 用户配置结构体 */
typedef struct
{
    /* 环形缓冲区大小 */
    u32 Ringbuf_size;
    /* 单帧接受量 */
    u32 Ringbuf_rx_max;
    /* 保存数据帧数的最大数量 */
    u32 Ringbuf_ptr_num;
    /* 环形缓冲区数据起始地址 */
    u8* Ringbuf_addr;
    /* 数据帧处理函数 data:数据帧起始地址 datalen:数据帧长度 */
    void (*Buf_handle)(u8 *data,u16 datalen);
    /* 数据帧转存函数 */
    void (*Data_save)(u8 *data,u16 datalen);
    /* 数据 */
    RingBuf_CB* rb_cb;
}RingBuf_cfg;

/**
 * @brief 环形缓冲区初始化函数
 * 
 * @param rb_cfg 环形缓冲区API结构体
 * @param cfg 用户配置
 */
void ringbuf_cfg(RingBuf_cfg *rb_cfg,RingBuf_cfg cfg);

/**
 * @brief 环形缓冲区数据帧检测
 * 
 */
void ringbuf_resive_detection(void);

/**
 * @brief 帧数据直接转存函数
 * 
 * @param ringbuf 环形缓冲区API结构体
 */
void ringbuf_direct_storage(RingBuf_cfg ringbuf);

/**
 * @brief 环形缓冲区数据直接转存函数(数据处理函数)
 * 
 * @param ringbuf 环形缓冲区API结构体
 */
void ringbuf_direct_storage(RingBuf_cfg ringbuf);

/**
 * @brief 环形缓冲区配合DMA数据存储函数A
 * 
 * @param ringbuf 环形缓冲区API结构体
 * @param datalen 接收的数据帧的数据长度
 * @return u32 待压栈的数据帧数据存储首地址
 */
u32 ringbuf_resive_saddr(RingBuf_cfg ringbuf,u32 datalen);

/**
 * @brief 环形缓冲区配合DMA数据存储函数B
 * 
 * @param ringbuf 环形缓冲区API结构体
 * @return u32 压栈后的新数据帧数据存储首地址
 */
u32 ringbuf_resive_newsaddr(RingBuf_cfg ringbuf);

/**
 * @brief 环形缓冲区单字节接收接口函数
 * 
 * @param ringbuf 环形缓冲区API结构体
 * @param data 要接收的的单字节
 */
void ringbuf_data_resive(RingBuf_cfg ringbuf,u8 data);

#endif /* __RINGBUFF_H */
