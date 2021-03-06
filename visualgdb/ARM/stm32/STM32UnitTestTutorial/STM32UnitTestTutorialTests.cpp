#include <stm32f4xx_hal.h>
#include <TinyEmbeddedTest.h>
#include <MemoryLeakTests.h>
#include <stdio.h>

TEST_GROUP(TimingTests)
{
    void setup()
    {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    }
};

class TimingValidator
{
  private:
    unsigned m_Timeout;

  public:
    TimingValidator(unsigned timeout)
        : m_Timeout(timeout)
    {
        DWT->CYCCNT = 0;
    }

    ~TimingValidator()
    {
        unsigned cycles = DWT->CYCCNT;
        if (cycles < m_Timeout)
            printf("Timing OK: %d < %d\n", cycles, m_Timeout);
        else
        {
            char sz[64];
            sprintf(sz, "Timing violation: %d > %d", cycles, m_Timeout);
            FAIL(sz);
        }
    }
};

TEST(TimingTests, sinf)
{
    volatile double arg = 0.1;
    TimingValidator validator(120);
    volatile float result = sinf(arg);
}