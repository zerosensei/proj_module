/**
 * @file rf_port.h
 * 
 * @copyright Copyright (c) 2022 zerosensei
 * 
 */
#ifndef __RF_PORT_H
#define __RF_PORT_H

#include "rf_trans.h"

#ifndef CONFIG_RF_DEV_RCV_WINDOW_US
#define CONFIG_RF_DEV_RCV_WINDOW_US     100
#endif


struct rf_port_cb {
    void (*req_mac_cb)(uint8_t *data, uint8_t len);
    void (*req_pic_cb)(uint8_t *data, uint8_t len);
};


static inline uint8_t count_bit1(uint8_t val)
{
    uint8_t temp = val;
    uint8_t count = 0;
    while(temp){
        temp = temp & (temp - 1);
        count++;
    }
    return count;
}

bool rf_port_is_data_loaded(void);
void rf_port_transmit_cancel(void);
int rf_port_load_data(uint8_t type, uint8_t *data, size_t len);
void rf_port_rcv_open(int timeout);
void rf_port_init(struct rf_port_cb *cb);

#endif /* __RF_PORT_H */
