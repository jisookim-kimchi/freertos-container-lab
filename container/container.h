#ifndef CONTAINER_H
#define CONTAINER_H

#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include <stdbool.h>
#include <timers.h>
#include "../config.h"
#include "memory.h"
#include <stdint.h>

enum State
{
    ERROR = -1,
    READY = 0,
    RUNNING,
    SLEEP,
    DEAD,
};

// UBaseType_t : unsigned int
struct Container
{
    BaseType_t id;
    TaskHandle_t task;

    TaskFunction_t task_func;
    const char *name;
    configSTACK_DEPTH_TYPE stack_depth;
    void *task_func_param;
    UBaseType_t priority;

    StackType_t *stack;
    StaticTask_t *tcb;

    enum State state;
    struct MemoryArea mem_areas[CONTAINER_MEMORY_LAYERS];
    struct PageTable *root_page_table;
};

BaseType_t MakeContainer(struct Container *c, TaskFunction_t task_func, const char *name, configSTACK_DEPTH_TYPE stack_depth, void *task_func_param, UBaseType_t priority, StackType_t *stack, StaticTask_t *tcb);
BaseType_t ContainerAddMemoryArea(struct Container *c, void *virt_base, void *phy_base, size_t size, uint8_t permission);
BaseType_t SetTask(struct Container *c, TaskHandle_t task);
BaseType_t StartContainer(struct Container* c);
BaseType_t StopContainer(struct Container *c);
BaseType_t RestartContainer(struct Container* c);
BaseType_t DeleteContainer(struct Container* c);
enum State ContainerStatus(struct Container* c);
void SwitchContainer(struct Container *c);
void ContainerTaskSwitchHook(void);

#endif 