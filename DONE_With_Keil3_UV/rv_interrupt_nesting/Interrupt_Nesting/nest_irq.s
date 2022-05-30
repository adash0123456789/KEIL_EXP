;/*****************************************************************************/
;/* NEST_IRQ.S: Wrapper for Nested Interrupt Functions                        */
;/*****************************************************************************/

                PRESERVE8
                AREA    NEST_IRQ, CODE, READONLY
                ARM
                IMPORT  eint1_srv
                EXPORT  eint1_irq
eint1_irq
                PUSH    {R0-R3,R12,LR}          ; save register context
                MRS     LR, SPSR                ; Copy SPSR_irq to LR
                PUSH    {LR}                    ; Save SPSR_irq
                MSR     CPSR_c, #0x1F           ; Enable IRQ (Sys Mode)
                PUSH    {LR}                    ; Save LR

                BL      eint1_srv

                POP     {LR}                    ; Restore LR
                MSR     CPSR_c, #0x92           ; Disable IRQ (IRQ Mode)
                POP     {LR}                    ; Restore SPSR_irq to LR
                MSR     SPSR_cxsf, LR           ; Copy LR to SPSR_irq

;               VICVectAddr = 0;                ; Acknowledge Interrupt
                MOV     R0,#0
                STR     R0,[R0,#-0xFD0]

                POP     {R0-R3,R12,LR}          ; restore register context
                SUBS    R15,R14,#0x0004         ; return from interrupt


                END
