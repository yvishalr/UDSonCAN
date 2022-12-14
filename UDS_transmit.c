#include "UDS.h"

int main(void)
{
    // example single frame
    uint8_t f[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t PCI = 0x6;
    uint64_t DATA = 0x441122;

    single_frame_did(f, PCI, SID->RDBI, DID->KILOMETRAGE, DATA);
    display_frame(f);

    // example FC frame
    uint8_t MODE = 1;
    uint8_t BLOCKSIZE = 0;
    uint8_t ST = 0;

    flow_control_frame(f, MODE, BLOCKSIZE, ST);
    display_frame(f);

    // printf("\n\n%#06X ", DID->RCD_LINE_STATE);

    return 0;
}
