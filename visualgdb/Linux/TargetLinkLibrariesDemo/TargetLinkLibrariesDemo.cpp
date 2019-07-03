#include <iostream>
#include <sqlite3.h>

using namespace std;

int main(int argc, char *argv[])
{
    int rc = sqlite3_initialize();
    if (rc != SQLITE_OK)
    {
        printf("sqlite3_initialize() failed: error %d\n", rc);
        return 1;
    }
    sqlite3 *db = nullptr;
    rc = sqlite3_open("test.db", &db);
    if (rc != SQLITE_OK)
    {
        printf("sqlite3_open() failed: error %d\n", rc);
        return 1;
    }

    sqlite3_close(db);
    return 0;
}