#include "UDS.h"
#include <fstream>

/* Define all valid SIDs for example script */
uint8_t stored_SID[] = {0x22, 0x3E, 0x2E, 0x10, 0x11};

/* All valid DIDs with corresponding virtual register values for example script */
uint16_t stored_DID[] = {0x1401, 0x1402, 0x1403, 0x1404, 0x1405};
uint8_t register_data[] = {0x1F, 0x2F, 0x3F, 0x4F, 0x5F};

/* Method prototypes */
void readFile();

/* MAIN */
int main(void)
{
    // imitate reading bus data
    readFile();

    short flagS = 0, flagD = 0;
    for (short i = 0; i < 5; i++)
    {
        if (stored_SID[i] == f[1])
        {
            flagS = 1;
            break;
        }
    }

    if (flagS)
    {
        uint16_t searchDID = (f[2] << 8) | f[3];
        for (short i = 0; i < 5; i++)
        {
            if (stored_DID[i] == searchDID)
            {
                flagD = i + 1;
                break;
            }
        }

        if (flagD)
        {
            printf("\nPositive Response :- \n");

            f[0] = 0x4; // specify pci length here, assumed 4
            f[1] = f[1] + 0x40;
            f[4] = register_data[flagD - 1];
            f[5] = f[6] = f[7] = 0x00;
        }
        else
        {
            printf("\nNegative Response :- \n");

            f[0] = 0x3;
            f[2] = f[1];
            f[1] = 0x7F;
            f[3] = 0x31; // NRC (Request out of range)
            f[4] = f[5] = f[6] = f[7] = 0x00;
        }
    }
    else
    {
        printf("\nNegative Response :- \n");

        f[0] = 0x3;
        f[2] = f[1];
        f[1] = 0x7F;
        f[3] = 0x11; // NRC (Service not supported)
        f[4] = f[5] = f[6] = f[7] = 0x00;
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