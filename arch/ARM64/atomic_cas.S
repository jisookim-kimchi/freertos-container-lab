
.global atomic_cas


//CAS w1 : ptr value, w2 : new val , [x0] : ptr
atomic_cas:
    CAS w1, w2, [x0]
    mov w0, w1
    ret
    