#include "UDS.h"

/* general definitions */
uint8_t f[8] = {0, 0, 0, 0, 0, 0, 0, 0};
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
    // example single frame with DID
    PCI = 0x6;
    single_frame_did(f, PCI, SID->RDBI, DID->KILOMETRAGE, DATA);
    display_frame(f);

    // example single frame with a subfunction
    PCI = 0x7;
    SUBFUNCTION = SID->subf.sub_0 | (suppressPosRspMsgIndicationBit << 7);
    single_frame_subf(f, PCI, SID->TP, SUBFUNCTION, DATA_ARRAY);
    display_frame(f);

    // example FC frame
    MODE = 1;
    BLOCKSIZE = 1;
    ST = 1;
    flow_control_frame(f, MODE, BLOCKSIZE, ST);
    display_frame(f);

    return 0;
}
