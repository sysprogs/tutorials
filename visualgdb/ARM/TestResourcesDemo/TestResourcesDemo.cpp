#include <SysprogsTestHooks.h>
#include <TinyEmbeddedTest.h>
#include <stm32f4xx_hal.h>

#ifdef __cplusplus
extern "C"
#endif
    void
    SysTick_Handler(void)
{
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}

#include <TestResourceManager.h>

extern uint32_t g_Progress;

void GenerateTestData()
{
    auto hInputs = TRMCreateFile("inputs.dat", sfmCreateOrTruncateWriteOnly);
    auto hOutputs = TRMCreateFile("outputs.dat", sfmCreateOrTruncateWriteOnly);
    uint32_t total = 0;

    float buf1[1024], buf2[1024];
    float nextValue = 0;

    for (int block = 0; block < 256; block++)
    {
        for (int i = 0; i < (sizeof(buf1) / sizeof(buf1[0])); i++)
        {
            buf1[i] = nextValue += 0.01F;
            buf2[i] = sinf(buf1[i]);
        }

        ssize_t done = TRMWriteFile(hInputs, buf1, sizeof(buf1));
        done = TRMWriteFile(hOutputs, buf2, sizeof(buf2));

        g_Progress = block;
    }

    TRMCloseFile(hInputs);
    TRMCloseFile(hOutputs);
}

int main(void)
{
    HAL_Init();

    if (IsRunningUnitTests())
    {
        RunAllTests();
    }
    else
    {
        GenerateTestData();
    }
    return 0;
}
