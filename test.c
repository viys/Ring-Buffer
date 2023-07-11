/*
 * @Author: yuzy 2628587386@qq.com
 * @Date: 2023-07-11 20:55:54
 * @LastEditors: yuzy 2628587386@qq.com
 * @LastEditTime: 2023-07-11 23:56:40
 * @FilePath: \Ring-Buffer\ringbuf.h
 */

#include "ringbuf.h"

RingBuf_cfg UxRingBuf;
u8 RingBuff1[1000];

void input_detection(u8 *data,u16 datalen);

int main()
{
    RingBuf_cfg rb_cfg =
    {
        .Buf_handle = input_detection,
        .Data_save = (*Data_save)(u8 *data,u16 datalen), //数据接收API函数
        .Ringbuf_addr = RingBuff1,
        .Ringbuf_size = 1000,
        .Ringbuf_rx_max = 100,
        .Ringbuf_ptr_num = 10
    };
    ringbuf_cfg(UxRingBuf,rb_cfg);

    whlie(1){
        ringbuf_resive_detection(UxRingBuf);
    }
}

void input_detection(u8 *data,u16 datalen)
{
    /* 数据帧处理函数 */
}
