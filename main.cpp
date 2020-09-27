#include "memory.h"
#include "cpu.h"

int main() {
    Memory mem;
    CPU cpu(&mem);
    cpu.reset();
    cpu.execute();
}
