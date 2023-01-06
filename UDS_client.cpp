#include "UDS.h"
#include <fstream>

/* general definitions */
uint8_t PCI;

/* data parameter definitions for with DID and with subfunction respectively */
uint64_t DATA;
uint8_t DATA_ARRAY[5] = {0xAA, 0xBB, 0xCC, 0xDD};

/* sub function definitions */
uint8_t suppressPosRspMsgIndicationBit = 0;
uint8_t SUBFUNCTION;

/* definitions for flow control frames */
uint8_t MODE;
uint8_t BLOCKSIZE = 0;
uint8_t ST = 0;

/* Method prototypes */
void writeFrame();

/* MAIN */
int main(void)
{
    // Initialising frame;
    f[0] = f[1] = f[2] = f[3] = f[4] = f[5] = f[6] = f[7] = 0x00;

    // example for session control
    PCI = 0x2;
    DATA_ARRAY[0] = DATA_ARRAY[1] = DATA_ARRAY[2] = DATA_ARRAY[3] = DATA_ARRAY[4] = 0x00;
    SUBFUNCTION = SID->subf.sub_1;
    // single_frame_subf(f, PCI, SID->DSC, SUBFUNCTION, DATA_ARRAY);

    // example single frame with a subfunction
    PCI = 0x3;
    SUBFUNCTION = SID->subf.sub_4 | (suppressPosRspMsgIndicationBit << 7);
    single_frame_did(f, PCI, SID->RDTCI, 0x1408, 0x00);

    // imitate sending frame into bus
    writeFrame();

    display_frame(f);

    return 0;
}

void writeFrame()
{
    std::ofstream fout;
    fout.open("message.txt", std::ios::out | std::ios::trunc);

    for (short i = 0; i < 8; i++)
        fout << f[i];

    fout.close();
}
