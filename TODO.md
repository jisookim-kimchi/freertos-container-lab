

① qemu-system-aarch64
        ↓
② ARM64 bare-metal "Hello"
        ↓
③ linker script로 RAM address 결정
        ↓
④ C 코드에서 실제 memory address 확인
        ↓
⑤ PageTable을 특정 PA에 배치
        ↓
⑥ ARM64 descriptor 작성
        ↓
⑦ TTBR0_EL1 설정
        ↓
⑧ MMU enable
        ↓
⑨ FreeRTOS ARM64 port
        ↓
⑩ Container memory isolation


