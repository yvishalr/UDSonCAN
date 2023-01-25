#include "../headers/UDS.h"
#include <fstream>

/*General Definitions */
uint8_t currentSession = 0x01U;

/* Define all valid SIDs for example script */
uint8_t stored_SID[] = {0x22, 0x3E, 0x2E, 0x10, 0x11};

/* All valid DIDs with corresponding virtual register values for example script */
const struct stored_messages
{
    uint16_t stored_DID;
    uint8_t data;
    uint8_t session_requirement;
} m[] = {
    {.stored_DID = 0x1401U, .data = 0x1F, .session_requirement = 0x01U},
    {.stored_DID = 0x1402U, .data = 0x2F, .session_requirement = 0x01U},
    {.stored_DID = 0x1403U, .data = 0x3F, .session_requirement = 0x01U},
    {.stored_DID = 0x1404U, .data = 0x4F, .session_requirement = 0x03U},
    {.stored_DID = 0x1405U, .data = 0x5F, .session_requirement = 0x03U}};

/* Method prototypes */
// Cases
void service_not_supported();
void service_present();
void session_check_fail();
void session_change_pass();
void session_change_fail();

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
            if (f[2] == 0x02 && currentSession == 0x01)
                session_change_fail(); // defaultSession to programmingSession is unallowed
            else
            {
                session_change_pass();
                writeSession();
            }
        }
        else
            service_present();
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
        if (m[i].stored_DID == searchDID)
        {
            flagD = i + 1;
            break;
        }
    }

    if (searchDID == 0xF186)
    {
        printf("\nPositive Response :- \n");

        f[0] = 0x4U; // specify pci length here, assumed 4
        f[1] = f[1] + 0x40U;
        f[4] = currentSession;
        f[5] = f[6] = f[7] = 0x00;
    }
    else if (flagD)
    {
        if (m[flagD - 1].session_requirement == currentSession)
        {
            printf("\nPositive Response :- \n");

            f[0] = 0x4U; // specify pci length here, assumed 4
            f[1] = f[1] + 0x40U;
            f[4] = m[flagD - 1].data;
            f[5] = f[6] = f[7] = 0x00;
        }
        else
            session_check_fail();
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
    f[3] = 0xAAU;
    f[4] = 0xAAU;
    f[5] = 0xAAU;
    f[6] = 0xAAU;
}

void session_change_fail()
{
    printf("\nNegative Response :- \n");

    // NRC : Conditions Not Correct
    negative_response_frame(f, 0x22U);
}

/*-----------------------------------------------------------------------*/
/* File operations */

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