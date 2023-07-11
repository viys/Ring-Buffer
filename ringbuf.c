/*
 * @Author: yuzy 2628587386@qq.com
 * @Date: 2023-07-11 20:55:54
 * @LastEditors: yuzy 2628587386@qq.com
 * @LastEditTime: 2023-07-11 23:40:50
 * @FilePath: \Ring-Buffer\ringbuf.c
 */

#include "ringbuf.h"

/**
 * @brief 环形缓冲区指针初始化
 * 
 */
static void ringbuf_rx_ptr_init(RingBuf_cfg rb_cfg)
{
    /* IN指针指向SE指针对结构体数组的0号成员 */
    rb_cfg.rb_cb->URxDataIN = &rb_cfg.rb_cb->URxDataPtr[0];
    /* OUT指针指向SE指针对结构体数组的0号成员 */
    rb_cfg.rb_cb->URxDataOUT = &rb_cfg.rb_cb->URxDataPtr[0];
    /* END指针指向SE指针对结构体数组的最后一个成员 */
    rb_cfg.rb_cb->URxDataEND = &rb_cfg.rb_cb->URxDataPtr[rb_cfg.Ringbuf_ptr_num-1];
    /* 使用 IN 指针指向的SE指针对中的S指针标记第一次接收的起始位置 */
    rb_cfg.rb_cb->URxDataIN->start = rb_cfg.Ringbuf_addr;
    /* 积累接收数量清零 */
    rb_cfg.rb_cb->URxCounter = 0;
}

void ringbuf_cfg(RingBuf_cfg *rb_cfg,RingBuf_cfg cfg)
{
    /* 数据帧处理函数API */
    rb_cfg->Buf_handle = cfg.Buf_handle;
    /* 缓冲区起始地址 */
    rb_cfg->Ringbuf_addr = cfg.Ringbuf_addr;
    /* 缓冲区,可存最大帧数 */
    rb_cfg->Ringbuf_ptr_num = cfg.Ringbuf_ptr_num;
    /* 单帧接收最大数据量 */
    rb_cfg->Ringbuf_rx_max = cfg.Ringbuf_rx_max;
    /* 缓冲区大小 */
    rb_cfg->Ringbuf_size = cfg.Ringbuf_size;
    /* 接入rb控制块 */
    rb_cfg->rb_cb = cfg.rb_cb;

    /* 初始化环形缓冲区指针 */
    ringbuf_rx_ptr_init(*rb_cfg); //fix
}

void ringbuf_resive_detection(RingBuf_cfg ringbuf)
{
    /* Bootloader命令行程序 */
    if(ringbuf.rb_cb->URxDataOUT != ringbuf.rb_cb->URxDataIN){
        /* 数据帧处理 */
        (*ringbuf.Buf_handle)(ringbuf.rb_cb->URxDataOUT->start,ringbuf.rb_cb->URxDataOUT->end - ringbuf.rb_cb->URxDataOUT->start + 1);
        ringbuf.rb_cb->URxDataOUT ++;

        /* 回卷 */
        if(ringbuf.rb_cb->URxDataOUT == ringbuf.rb_cb->URxDataEND){
            ringbuf.rb_cb->URxDataOUT = &ringbuf.rb_cb->URxDataPtr[0];
        }
    }
}

/**
 * @brief 环形缓冲区压栈函数
 * 
 * @param ringbuf 环形缓冲区API结构体
 * @return u32 压栈后数据帧数据存储地址
 */
static u32 ringbuf_press_stack(RingBuf_cfg ringbuf)
{
    /* 数据帧输入指针后移 */
    ringbuf.rb_cb->URxDataIN ++;

    /* 后移至END标记的位置 */
    if(ringbuf.rb_cb->URxDataIN == ringbuf.rb_cb->URxDataEND){
        /* 回卷,重新指向0号成员 */
        ringbuf.rb_cb->URxDataIN = &ringbuf.rb_cb->URxDataPtr[0];
    }
    /* 如果剩余空间量大于等于单次接收最大量 */
    if(ringbuf.Ringbuf_size - ringbuf.rb_cb->URxCounter >= ringbuf.Ringbuf_rx_max){
        ringbuf.rb_cb->URxDataIN->start = &(*ringbuf.Ringbuf_addr)[ringbuf.rb_cb->URxCounter];
    }else{
        /* 回卷,下次的接收位置返回缓冲区的起始位置 */
        ringbuf.rb_cb->URxDataIN->start = ringbuf.Ringbuf_addr;
        /* 累计值清零 */
        ringbuf.rb_cb->URxCounter = 0;
    }
    /* 返回压栈后数据帧数据存储地址 */
    return ringbuf.rb_cb->URxDataIN->start;
}

void ringbuf_direct_storage(RingBuf_cfg ringbuf)
{
    u32 datalen;

    /* 将数据帧数据存入数据帧指针指向的数据帧数据的起始地点 */
    (*ringbuf.Data_save)(ringbuf.rb_cb->URxDataIN->start,datalen);

    /* 环形缓冲区计数加上每次接收的数据帧长 */
    ringbuf.rb_cb->URxCounter += datalen;
    /* 保存数据帧的结束地址 */
    ringbuf.rb_cb->URxDataIN->end = &(*ringbuf.Ringbuf_addr)[ringbuf.rb_cb->URxCounter - 1];
    
    /* 压栈 */
    ringbuf_press_stack(ringbuf);
}

u32 ringbuf_resive_saddr(RingBuf_cfg ringbuf,u32 datalen)
{
    ringbuf.rb_cb->URxCounter += datalen;
    return ringbuf.rb_cb->URxDataIN->start;
}

u32 ringbuf_resive_newsaddr(RingBuf_cfg ringbuf)
{
    return ringbuf_press_stack(ringbuf);
}

void ringbuf_data_resive(RingBuf_cfg ringbuf,u8 data)
{
    
}

// void ux_printf(char *format,...)
// {
//     /* 建立一个va_list变量listdata */
//     va_list listdata;
//     /* 向listdata加载,...代表不定长的参数 */
//     va_start(listdata,format);

//     /* 格式化输出到缓冲区 Ux_TxBuff */
//     vsprintf((char *)Ux_TxBuff,format,listdata);
//     /* 释放lostdata */
//     va_end(listdata);

//     /* 等待发送缓冲区为0 */
    
//     /* 填入数据 */
//     UART1_SendString(Ux_TxBuff,strlen((const char*)Ux_TxBuff));

//     /* 等到最后一个字节数据发送完毕 */

// }
