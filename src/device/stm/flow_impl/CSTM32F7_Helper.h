//
// Created by mohamad on 09.02.20.
//

#ifndef F7_DEVICE_CSTM32F7_HELPER_H
#define F7_DEVICE_CSTM32F7_HELPER_H

class CSTM32F7_Helper : public CFlowHelper {
public:
    uint8_t push_sink_entity(const sink_entity *entity) override;

    uint32_t flush_buffer() override;

    void pfyl_begin_transfer() override;

    uint32_t pfyl_transfer(uint8_t *buf, size_t size) override;

    void pfyl_end_transfer() override;

    void setUART(UART_HandleTypeDef *);

private:
    UART_HandleTypeDef *uart;
    UBaseType_t tempUxSavedInterruptStatus;
};


#endif //F7_DEVICE_CSTM32F7_HELPER_H
