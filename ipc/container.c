#include "container.h"

static int container_id = 0;

BaseType_t MakeContainer(struct Container *c, TaskFunction_t task_func, const char *name, configSTACK_DEPTH_TYPE stack_depth, void *task_func_param, UBaseType_t priority, StackType_t *stack,
    StaticTask_t *tcb)
{
    if (c == NULL || task_func == NULL)
    {
        return pdFAIL;
    }

    c->id = container_id++;
    c->priority = priority;

    c->task = xTaskCreateStatic
    (
        task_func,
        name,
        stack_depth,
        task_func_param,
        priority,
        stack,
        tcb
    );
    c->task_func = task_func;
    c->name = name;
    c->stack_depth = stack_depth;
    c->task_func_param = task_func_param;
    c->priority = priority;
    c->stack = stack;
    c->tcb = tcb;
    c->root_page_table = PageTableAllocate(0);
    if (c->task == NULL)
    {
        c->state = ERROR;
        return pdFAIL;
    }
    vTaskSetThreadLocalStoragePointer(c->task, 0, (void *)c);
    c->state = READY;
    return pdPASS;
}

BaseType_t ContainerAddMemoryArea(struct Container *c, void *virt_base, void *phy_base, size_t size, uint8_t permission)
{
    if (c == NULL || c->root_page_table == NULL)
    {
        return pdFAIL;
    }
    for (int i = 0; i < CONTAINER_MEMORY_LAYERS; i++)
    {
        if (c->mem_areas[i].size == 0)
        {
            MemoryAreaInit(&c->mem_areas[i], virt_base, phy_base, size, permission);
            page_table_map(c->root_page_table, 0, &c->mem_areas[i]);
            return pdPASS;
        }
    }
    return pdFAIL;
}

BaseType_t SetTask(struct Container *c, TaskHandle_t new_task)
{
    if (c == NULL || new_task == NULL)
    {
        return pdFAIL;
    }
    if (c->state != READY && c->state != SLEEP)
        return pdFAIL;
    c->task = new_task;

    return pdPASS;
}

BaseType_t StartContainer(struct Container* c)
{
    if (c == NULL || c->task == NULL)
    {
        return pdFAIL;
    }

    if (c->state != READY)
    {
        return pdFAIL;
    }

    c->state = RUNNING;

    return pdPASS;
}

BaseType_t StopContainer(struct Container *c)
{
    if (c == NULL || c->state != RUNNING)
    {
        return pdFAIL;
    }

    vTaskSuspend(c->task);
    c->state = SLEEP;
    
    return pdPASS;
}

BaseType_t RestartContainer(struct Container *c)
{
    if (c == NULL || c->task == NULL)
    {
        return pdFAIL;
    }

    if (c->state != SLEEP)
    {
        return pdFAIL;
    }

    vTaskDelete(c->task);

    c->task = xTaskCreateStatic
    (
        c->task_func,
        c->name,
        c->stack_depth,
        c->task_func_param,
        c->priority,
        c->stack,
        c->tcb
    );

    if (c->task == NULL)
    {
        c->state = ERROR;
        return pdFAIL;
    }

    c->state = READY;

    return pdPASS;
}

BaseType_t DeleteContainer(struct Container* c)
{
    if (c == NULL || c->task == NULL || c->state == RUNNING)
    {
        return pdFAIL;
    }
    
    vTaskDelete(c->task);
    c->task = NULL;
    c->state = DEAD;

    return pdPASS;
}
    

enum State ContainerStatus(struct Container* c)
{
    if (c == NULL)
    {
        return ERROR;
    }
    return c->state;
}

void SwitchContainer(struct Container *c)
{
    if (c != NULL && c->root_page_table != NULL)
    {
        uint64_t ttbr0 = ((uint64_t)c->id << 48) | (uintptr_t)c->root_page_table;
        switch_mmu_table(ttbr0);
    }
}

void ContainerTaskSwitchHook()
{
    TaskHandle_t cur = xTaskGetCurrentTaskHandle();
    if (cur != NULL)
    {
        struct Container *c =pvTaskGetThreadLocalStoragePointer(cur, 0);
        SwitchContainer(c);
    } 
}