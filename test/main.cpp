#include <QtTest>

#include "loadtest.h"
#include "storetest.h"
#include "cputest.h"
#include "addcarrytest.h"
#include "subtracttest.h"
#include "transfertest.h"
#include "stacktest.h"
#include "logicaltest.h"
#include "branchtest.h"
#include "incdectest.h"
#include "comparetest.h"
#include "shiftstest.h"
#include "flagstest.h"
#include "misctest.h"

#define RUN_TEST(test) {test t; QTest::qExec(&t, argc, argv);}

int main(int argc, char *argv[]) {
    RUN_TEST(LoadTest);
    RUN_TEST(StoreTest);
    RUN_TEST(AddCarryTest);
    RUN_TEST(SubtractTest);
    RUN_TEST(CpuTest);
    RUN_TEST(TransferTest);
    RUN_TEST(StackTest);
    RUN_TEST(LogicalTest);
    RUN_TEST(BranchTest);
    RUN_TEST(IncDecTest);
    RUN_TEST(CompareTest);
    RUN_TEST(ShiftsTest);
    RUN_TEST(FlagsTest);
    RUN_TEST(MiscTest);
}

