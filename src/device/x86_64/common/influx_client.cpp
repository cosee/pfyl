//
// Created by mohamad on 25.01.20.
//

#include <InfluxDBFactory.h>
#include "InfluxDB.h"
#include <string>
#include <sink_entity.h>
#include <cstring>
#include "influx_client.h"

struct rtos_tasks_t {
    char name[16];
    uint64_t task_handle;
} rtos_tasks[100];
uint32_t rtos_task_index = 0;
typedef struct rtos_tasks_t Task_t;

struct rtos_tasks_t *findTask(uint64_t handle) {
    if (handle == 0)
        return 0;
    for (int i = 0; i <= rtos_task_index; i++) {
        if (rtos_tasks[i].task_handle == handle)
            return &rtos_tasks[i];
    }
    return NULL;
}

void CInfluxClient::initDb(std::string host, std::string database) {
    influxdb = influxdb::InfluxDBFactory::Get(host);
}

CInfluxClient::CInfluxClient(std::string host, std::string database) {
    initDb(host, database);
}

void CInfluxClient::sendToDB(sink_entity *se) {
    if (se->type == PFYL_ENTITY_TYPE_MALLOC) {
        influxdb->write(influxdb::Point{"malloc"}
                          .addField("value", "1")
                          .addTag("tick", std::to_string(se->tick))
                          .addTag("address", std::to_string(se->data))
                          .addTag("size", std::to_string(se->data2))
                          .addTag("caller", std::to_string(se->data3))
                          .addTag("type", std::to_string(se->type))
        );
    } else if (se->type == PFYL_ENTITY_TYPE_FREE) {
        influxdb->write(influxdb::Point{"malloc"}
                          .addField("value", "0")
                          .addTag("tick", std::to_string(se->tick))
                          .addTag("address", std::to_string(se->data))
                          .addTag("size", std::to_string(se->data2))
                          .addTag("caller", std::to_string(se->data3))
                          .addTag("type", std::to_string(se->type))
        );
    } else if (se->type == PFYL_ENTITY_TYPE_TASK_CREATE) {
        uint32_t cur_index = rtos_task_index++;
        memcpy(rtos_tasks[cur_index].name, &se->data2, 8);
        memcpy(rtos_tasks[cur_index].name + 8, &se->data3, 8);
        rtos_tasks[cur_index].task_handle = se->data;
        printf("\n PFYL_ENTITY_TYPE_TASK_CREATE");
        printf("\n name: %s", rtos_tasks[cur_index].name);
        printf("\n handle: %x", rtos_tasks[cur_index].task_handle);
    } else if (se->type == PFYL_ENTITY_TYPE_TASK_RDY) {
        Task_t *task = findTask(se->data);
        if (task == NULL) {
            return;
        }
        std::string s(task->name);
        if (s.empty()) {
            s = "empty";
        }
        influxdb->write(influxdb::Point{"task_state"}
                          .addField("value", "1")
                          .addTag("name", s)
                          .addTag("handle", std::to_string(task->task_handle))
                          .addTag("tick", std::to_string(se->tick))
        );
    } else if (se->type == PFYL_ENTITY_TYPE_TASK_DELAY) {
        Task_t *task = findTask(se->data);
        if (task == NULL) {
            return;
        }
        std::string s(task->name);
        if (s.empty()) {
            s = "empty";
        }
        influxdb->write(influxdb::Point{"task_state"}
                          .addField("value", "0")
                          .addTag("name", s)
                          .addTag("handle", std::to_string(task->task_handle))
                          .addTag("tick", std::to_string(se->tick))
        );
    } else if (se->type == PFYL_ENTITY_TYPE_FUNC_ENTER) {
        influxdb->write(influxdb::Point{"func_trace"}
                          .addField("value", "1")
                          .addTag("current_func", std::to_string(se->data))
                          .addTag("callsite", std::to_string(se->data2))
                          .addTag("stack_pointer", std::to_string(se->data3))
                          .addTag("tick", std::to_string(se->tick))
        );
    } else if (se->type == PFYL_ENTITY_TYPE_FUNC_EXIT) {
        influxdb->write(influxdb::Point{"func_trace"}
                          .addField("value", "0")
                          .addTag("current_func", std::to_string(se->data))
                          .addTag("callsite", std::to_string(se->data2))
                          .addTag("stack_pointer", std::to_string(se->data3))
                          .addTag("tick", std::to_string(se->tick))
        );
    }
}
