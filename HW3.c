#include "types.h"
#include "stat.h"
#include "user.h"
void job(void)
{
    int dummy = 1;
    int a = 0;
    for (; a < 1000000000; a++)
    {
        dummy *= 10;
        dummy /= 10;
        for (int i = 0; i < 1000000000; i++)
        {
            for (int j = 0; j < 1000000000; j++)
            {
                dummy *= 10;
                dummy /= 10;
            }
        }
    }
}
int main(void)
{
    int p1, p2, p3, p4;
    p1 = fork();
    if (p1 == 0)
    {
        for (int i = 0; i < 15; i++)
        {
            int p5 = fork();
            if (p5 == 0)
            {
                ps();
                job();
                exit();
            }
            else
            {
                set_proc_priority(p5, 5);
                wait();
            }
        }

        exit();
    }
    else
    {
        set_proc_priority(p1, 6);
        ps();
    }
    p2 = fork();
    if (p2 == 0)
    {
        ps();
        job();
        exit();
    }
    else
    {
        set_proc_priority(p2, 3);
        ps();
    }
    // P3: 7
    p3 = fork();
    if (p3 == 0)
    {
        ps();
        job();
        exit();
    }
    else
    {
        set_proc_priority(p3, 6);
        ps();
    }

    p4 = fork();
    if (p4 == 0)
    {
        ps();
        job();
        exit();
    }
    else
    {
        set_proc_priority(p4, 6);
        ps();
    }

    wait();
    ps();
    wait();
    ps();
    wait();
    ps();
    wait();
    ps();

    exit();
}
