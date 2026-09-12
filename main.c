
#include "../container/memory.h"

TaskHandle_t task_handler_1;
TaskHandle_t task_handler_2;

struct Container container1;
struct Container container2;

void vResumeTimerCallback(TimerHandle_t xTimer)
{
    printf("[Timer] 시간이 다 되어 Task 1을 자동으로 깨웁니다!\n");
    vTaskResume(task_handler_1);
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    printf("[Stack Overflow] Task: %s\n", pcTaskName);
    while
    (
        1
    );
}

void myTask(void *pvParameters)
{
    while(1)
    {
        printf("[Task 1] Hello World\n");
        vTaskDelay(100);
    }
}

void myTask2(void *pvParameters)
{
    TimerHandle_t timer = xTimerCreate("ResumeTimer", 3000, pdFALSE, 0, vResumeTimerCallback);
    xTimerStart(timer, 0);
    while(1)
    {
        printf("[Task 2] 난 한국인이란다\n");
        vTaskSuspend(task_handler_1); // task_1을 멈춘다
        xTimerStart(timer, 0);

        for(int i = 0; i < 5; i++)
        {
            printf("[Task 2] 김지수는 calm down 이런 기분 처음봐 \n");
            vTaskDelay(100);
        }
    }
}
    /*
        @brief  create TaskHandle_t(Task Handler)
        @param 1 : task function.
        @param 2 : task name.
        @param 3 : stack size. 
        @param 4 : datas to pass.
        @param 5 : priority.
        @param 6 : stack memory
        @param 7 : TCB memory
    */
    // TaskHandle_t xTaskCreateStatic( 1,2,3,4,5,6,7);

int main()
{   
    /* Task 1 설정 및 정적 메모리 */
    static StackType_t stack_buffer1[256];
    static StaticTask_t task_buffer1;
 
    // MakeContainer(&container1, myTask, "kimchi", 256, NULL, 1, stack_buffer1, &task_buffer1);
    // StartContainer(&container1);
    
    /* Task 2 설정 및 정적 메모리 */
    // static StackType_t stack_buffer2[256];
    // static StaticTask_t task_buffer2;

    // MakeContainer(&container2, myTask2, "kimbap", 256, NULL, 2, stack_buffer2, &task_buffer2);
    // StartContainer(&container2);

    MakeContainer(&container1, myTask, "kimchi", 256, NULL, 1, stack_buffer1, &task_buffer1);
    /* Map Container 1's isolated RAM and UART */
    ContainerAddMemoryArea(&container1, (void *)0x40000000, (void *)0x40000000, 0x100000, MEM_READ | MEM_WRITE | MEM_EXEC);
    ContainerAddMemoryArea(&container1, (void *)0x09000000, (void *)0x09000000, 0x1000,   MEM_READ | MEM_WRITE);
    StartContainer(&container1);
    
    /* FreeRTOS 스케줄러 시작 */
    vTaskStartScheduler();

    
    return 0;
}