#include "progmem_jump.h"

__attribute__((always_inline)) __STATIC_INLINE void __set_PC(uint32_t ProgramCounter)
{
    __ASM volatile("MOV pc, %0"
                   :
                   : "r"(ProgramCounter)
                   :);
}

void ProgMem_Jump(uint32_t jumpLocation)
{
    if (jumpLocation != 0xFFFFFFFF)
    {
        __set_MSP(*(uint32_t *)jumpLocation);
        __set_PC(*(uint32_t *)(jumpLocation + 4));

        __NOP();
        __NOP();
        __NOP();

        vPortEndScheduler();
    }
}

void ProgMem_JumpToApp(uint32_t address)
{

    if (CONTROL_nPRIV_Msk & __get_CONTROL())
    { /* not in privileged mode */
        EnablePrivilegedMode();
    }

    memset((uint32_t *)NVIC->ICER, 0xFF, sizeof(NVIC->ICER));

    memset((uint32_t *)NVIC->ICPR, 0xFF, sizeof(NVIC->ICPR));

    SysTick->CTRL = 0;

    SCB->ICSR |= SCB_ICSR_PENDSTCLR_Msk;

    SCB->SHCSR &= ~(SCB_SHCSR_USGFAULTENA_Msk |
                    SCB_SHCSR_BUSFAULTENA_Msk |
                    SCB_SHCSR_MEMFAULTENA_Msk);
#ifdef PROGMEM_R7FA4E1
    __NOP();
#elif PROGMEM_RA7FA6M2
    /* Disable the HW Stack monitor for MSP and PSP */
    // R_MPU_SPMON->SP[MSP_SPMPU].PT = 0xA500;
    // R_MPU_SPMON->SP[PSP_SPMPU].PT = 0xA500;

    // R_MPU_SPMON->SP[MSP_SPMPU].CTL = 0x0000;
    // R_MPU_SPMON->SP[PSP_SPMPU].CTL = 0x0000;
#else

#endif
    SCB->VTOR = address;

    if (CONTROL_SPSEL_Msk & __get_CONTROL())
    { /* MSP is not active */
        __set_MSP(__get_PSP());
        __set_CONTROL(__get_CONTROL() & ~CONTROL_SPSEL_Msk);
    }

    // Jump to our application entry point
    ProgMem_Jump(address);
}
