#include "UDS.h"
#include <fstream>

/*General Definitions */
uint8_t currentSession = 0x01U;

/* Define all valid SIDs for example script */
uint8_t stored_SID[] = {0x22, 0x3E, 0x2E, 0x10, 0x11};

/* All valid DIDs with corresponding virtual register values for example script */
uint16_t stored_DID[] = {0x1401, 0x1402, 0x1403, 0x1404, 0x1405};
uint8_t register_data[] = {0x1F, 0x2F, 0x3F, 0x4F, 0x5F};

/* Method prototypes */
// Cases
void service_not_supported();
void service_present();
void session_check_fail();
void session_change_pass();

// File
void readFrame();
void writeSession();
void readSession();

/* MAIN */
int main(void)
{
    // imitate reading bus data
    readFrame();

    // Congigures active session as last used session
    readSession();

    short flagS = 0;
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
        if (f[1] == 0x10)
        {
            session_change_pass();
            writeSession();
        }
        else if (currentSession == 0x03)
            service_present();
        else
            session_check_fail();
    }
    else
    {
        service_not_supported();
    }

    display_frame(f);

    return 0;
}

/* Test cases */

void service_present()
{
    short flagD = 0;
    static uint16_t searchDID = (f[2] << 8) | f[3];
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

        f[0] = 0x4U; // specify pci length here, assumed 4
        f[1] = f[1] + 0x40U;
        f[4] = register_data[flagD - 1];
        f[5] = f[6] = f[7] = 0x00;
    }
    else
    {
        printf("\nNegative Response :- \n");

        // NRC : Request Out of Range
        negative_response_frame(f, 0x31U);
    }
}

void service_not_supported()
{
    printf("\nNegative Response :- \n");

    // NRC : Service Not Supported
    negative_response_frame(f, 0x11U);
}

void session_check_fail()
{
    printf("\nNegative Response :- \n");

    // NRC : Request Unavailable in Active Session
    negative_response_frame(f, 0x7FU);
}

void session_change_pass()
{
    printf("\nPositive Response :- \n");

    f[0] = 0x4U; // specify pci length here, assumed 4
    f[1] = f[1] + 0x40U;
    f[3] = 0xAA;
    f[4] = 0xAA;
    f[5] = 0xAA;
    f[6] = 0xAA;
}

/*-----------------------------------------------------------------------*/
/* File operation methods */

void readFrame()
{
    std::ifstream fin;
    fin.open("message.txt", std::ios::in);

    for (short i = 0; i < 8; i++)
        fin >> f[i];

    fin.close();
}

void writeSession()
{
    std::ofstream fout;
    fout.open("session.txt", std::ios::out | std::ios::trunc);

    fout << f[2];

    fout.close();
}

void readSession()
{
    std::ifstream fin;
    fin.open("session.txt", std::ios::in);

    fin >> currentSession;

    fin.close();
}