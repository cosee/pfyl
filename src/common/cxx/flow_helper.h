//
// Created by mohamad on 09.02.20.
//

#ifndef F7_DEVICE_FLOW_HELPER_H
#define F7_DEVICE_FLOW_HELPER_H
#include "sink_entity.h"
class CFlowHelper {
    /* required */
    virtual uint8_t push_sink_entity(const sink_entity *) = 0;
    virtual uint32_t flush_buffer() = 0;
    virtual void pfyl_begin_transfer() = 0;
    virtual uint32_t pfyl_transfer(uint8_t *buf, size_t size) = 0;
    virtual void pfyl_end_transfer() = 0;

    /* optional */
    virtual uint8_t pop_sink_entity(const sink_entity *) {
        return 0;
    };

};

#endif //F7_DEVICE_FLOW_HELPER_H
