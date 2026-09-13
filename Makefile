run-rtos:
	$(MAKE) -C core3_FreeRTOS run

run-linux:
	$(MAKE) -C core0_linux run

clean:
	$(MAKE) -C core0_linux clean
	$(MAKE) -C core3_FreeRTOS clean

.PHONY: all run-rtos run-linux clean