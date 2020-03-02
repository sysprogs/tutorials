#include <TinyEmbeddedTest.h>
#include <stdio.h>
#include <math.h>

#include <TestResourceManager.h>

TEST_GROUP(DataProcessingTestGroup)
{
};

uint32_t g_Progress;

TEST(DataProcessingTestGroup, SineTest)
{
    auto hInputs = TRMCreateFile("inputs.dat", sfmOpenReadOnly);
    auto hOutputs = TRMCreateFile("outputs.dat", sfmOpenReadOnly);
    uint32_t total = 0;

    CHECK(hInputs != 0);
    CHECK(hOutputs != 0);

    float buf1[1024], buf2[1024];

    for (;;)
    {
        ssize_t done = TRMReadFile(hInputs, buf1, sizeof(buf1));
        CHECK(done >= 0);
        if (!done)
            break;

        ssize_t done2 = TRMReadFile(hOutputs, buf2, done);
        CHECK_EQUAL(done, done2);

        for (int i = 0; i < (done / sizeof(buf1[0])); i++)
            CHECK_EQUAL(buf2[i], sinf(buf1[i]));

        total += done;
        g_Progress = total;
    }

    CHECK(total >= 1024 * 1024);
    TRMCloseFile(hInputs);
    TRMCloseFile(hOutputs);
}
