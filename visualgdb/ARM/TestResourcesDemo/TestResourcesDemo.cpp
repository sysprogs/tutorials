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

    auto hInputsBurst = TRMBeginWriteBurst(hInputs);
    auto hOutputsBurst = TRMBeginWriteBurst(hOutputs);

    float buf1[32], buf2[32];
    float nextValue = 0;

    for (int block = 0; block < 1024 * 1024 / sizeof(buf1); block++)
    {
        for (int i = 0; i < (sizeof(buf1) / sizeof(buf1[0])); i++)
        {
            buf1[i] = nextValue += 0.01F;
            buf2[i] = sinf(buf1[i]);
        }

        ssize_t done = TRMWriteFileCached(hInputsBurst, buf1, sizeof(buf1));
        done = TRMWriteFileCached(hOutputsBurst, buf2, sizeof(buf2));

        g_Progress = block;
    }

    TRMEndWriteBurst(hInputsBurst);
    TRMEndWriteBurst(hOutputsBurst);

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
