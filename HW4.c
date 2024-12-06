#include "types.h"
#include "stat.h"
#include "user.h"

int main()
{
    uint x = getnumfreepages();
    printf(1, "%d\n", x);
    exit();
}
