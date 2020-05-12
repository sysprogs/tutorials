#include <TinyEmbeddedTest.h>
#include <stdio.h>
#include <math.h>

#include <TestResourceManager.h>

TEST_GROUP(DataProcessingTestGroup)
{
};

uint32_t g_Progress;

#include <algorithm>

TEST(DataProcessingTestGroup, SineTest)
{
    auto hInputs = TRMCreateFile("inputs.dat", sfmOpenReadOnly);
    auto hOutputs = TRMCreateFile("outputs.dat", sfmOpenReadOnly);
    uint32_t total = 0;

    CHECK(hInputs != 0);
    CHECK(hOutputs != 0);

    char workArea1[4096], workArea2[4096];
    auto hInputsBurst = TRMBeginReadBurst(hInputs, workArea1, sizeof(workArea1));
    auto hOutputsBurst = TRMBeginReadBurst(hOutputs, workArea2, sizeof(workArea2));

    for (;;)
    {
        size_t done1, done2;
        
        float *pBuf1 = (float *)TRMBeginReadFileCached(hInputsBurst, &done1, 1);
        float *pBuf2 = (float *)TRMBeginReadFileCached(hOutputsBurst, &done2, 1);

        if (!pBuf1 || !pBuf2)
        {
            CHECK(!pBuf1 && !pBuf2);
            break;
        }

        size_t todo = std::min(done1, done2);
        CHECK(todo > 0);

        for (int i = 0; i < (todo / sizeof(*pBuf1)); i++)
        {
            float error = fabsf(pBuf2[i] - sinf(pBuf1[i]));
            CHECK(error < 0.001F);
        }

        TRMEndReadFileCached(hInputsBurst, pBuf1, todo);
        TRMEndReadFileCached(hOutputsBurst, pBuf2, todo);

        total += todo;
        g_Progress = total;
    }

    CHECK(total >= 1024 * 1024);
    TRMEndReadBurst(hInputsBurst);
    TRMEndReadBurst(hOutputsBurst);
    
    TRMCloseFile(hInputs);
    TRMCloseFile(hOutputs);
}
