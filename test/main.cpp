#include <QtTest>

#include "loadtest.h"
#include "storetest.h"
#include "cputest.h"
#include "addcarrytest.h"

#define RUN_TEST(test) {test t; QTest::qExec(&t, argc, argv);}

int main(int argc, char *argv[]) {
    RUN_TEST(LoadTest);
    RUN_TEST(StoreTest);
    RUN_TEST(AddCarryTest);
    RUN_TEST(CpuTest);
}

