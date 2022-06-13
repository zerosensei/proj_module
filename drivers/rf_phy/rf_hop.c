/**
 * @brief rf frequency hopping, hopping when receive data successfully.
 * 
 * @copyright Copyright (c) 2022 zerosensei
 * 
 */
#include "rf_hop.h"
#include "sys/util.h"

const uint8_t rf_channel_map[] = {0, 8, 15};

void rf_hop(void)
{ 
    static uint8_t idx = 0;

    idx++;
    rf_set_channel(rf_channel_map[idx%sizeof(rf_channel_map)]);
}   