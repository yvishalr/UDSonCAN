#include "UDS.h"
#include <fstream>

/* Define all valid SIDs for example script */
uint8_t stored_SID[] = {0x22, 0x3E, 0x2E, 0x10, 0x11};

void readFile();

/* MAIN */
int main(void)
{
    // imitate reading bus data
    readFile();

    short flag = 0;
    for (short i = 0; i < 5; i++)
    {
        if (stored_SID[i] == f[1])
            flag = 1;
    }

    if (flag)
    {
        printf("\nPositive Response :- \n");

        f[0] = 0x4; // specify pci length here, assumed 4
        f[1] = f[1] + 0x40;
        f[3] = 0x12;
        f[4] = 0x34;
    }
    else
    {
        printf("\nNegative Response :- \n");

        f[0] = 0x3;
        f[1] = 0x7F;
        f[3] = 0x31; // assumed NRC
        f[4] = f[5] = f[6] = f[7] = 0;
    }

    display_frame(f);

    return 0;
}

void readFile()
{
    std::ifstream fin;
    fin.open("message.txt", std::ios::in);

    for (short i = 0; i < 8; i++)
        fin >> f[i];

    fin.close();
}