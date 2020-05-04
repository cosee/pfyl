//
// Created by mohamad on 09.02.20.
//

#ifndef F7_DEVICE_SINK_ENTITY_H
#define F7_DEVICE_SINK_ENTITY_H

#include <cstdint>
#include <cstddef>

extern void *__real_malloc(size_t size);

enum PFYL_ENTITY_TYPE {
    PFYL_ENTITY_TYPE_MALLOC = 0,
    PFYL_ENTITY_TYPE_FREE = 6,
    PFYL_ENTITY_TYPE_TASK_CREATE = 1,
    PFYL_ENTITY_TYPE_TASK_RDY = 2,
    PFYL_ENTITY_TYPE_TASK_DELAY = 3,
    PFYL_ENTITY_TYPE_FUNC_ENTER = 4,
    PFYL_ENTITY_TYPE_FUNC_EXIT = 5
};

struct __attribute__((packed)) sink_entity_struct {
    uint8_t type;
    uint64_t tick;
    uint64_t data;
    uint64_t data2;
    uint64_t data3;
};

typedef struct sink_entity_struct sink_entity;
class SinkEntityHelper{
        public:
        inline void update_sink_entity_tick(sink_entity *se) {
            se->tick = reference_tick;

        }

        inline void increase_tick() {
            reference_tick++;
        }

        inline void updateSinkEntity(sink_entity *se, enum PFYL_ENTITY_TYPE type, uint64_t data, uint64_t data2, uint64_t data3) {
            se->type = type;
            se->tick = reference_tick;
            se->data = data;
            se->data2 = data2;
            se->data3 = data3;
        }
        inline sink_entity *
        create_sink_entity(enum PFYL_ENTITY_TYPE type, uint64_t data, uint64_t data2, uint64_t data3) {
            sink_entity *se = (sink_entity *) __real_malloc(sizeof(sink_entity));
            se->type = type;
            se->tick = reference_tick;
            se->data = data;
            se->data2 = data2;
            se->data3 = data3;
            return se;
        }

        private:
        uint64_t reference_tick;
};
#endif //F7_DEVICE_SINK_ENTITY_H
