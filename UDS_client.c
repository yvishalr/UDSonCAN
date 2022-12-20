#include "UDS.h"

/* general definitions */
uint8_t PCI;

/* data parameter definitions for with DID and with subfunction respectively */
uint64_t DATA;
uint8_t DATA_ARRAY[5] = {0x23, 0x48, 0x55, 0x47, 0x22};

/* sub function definitions */
uint8_t suppressPosRspMsgIndicationBit = 0;
uint8_t SUBFUNCTION;

/* definitions for flow control frames */
uint8_t MODE;
uint8_t BLOCKSIZE = 0;
uint8_t ST = 0;

/* MAIN */
int main(void)
{
    // Initialising frame;
    f[0] = f[1] = f[2] = f[3] = f[4] = f[5] = f[6] = f[7] = 0x00;

    // // example single frame with DID
    // PCI = 0x6;
    // single_frame_did(f, PCI, SID->WDBI, DID->KILOMETRAGE, DATA);
    // display_frame(f);

    // example single frame with a subfunction
    PCI = 0x7;
    SUBFUNCTION = SID->subf.sub_1 | (suppressPosRspMsgIndicationBit << 7);
    single_frame_subf(f, PCI, SID->TP, SUBFUNCTION, DATA_ARRAY);
    display_frame(f);

    return 0;
}
