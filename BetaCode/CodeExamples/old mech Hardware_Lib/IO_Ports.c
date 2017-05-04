/*
 * File:   IO_Ports.c
 * Author: Elkaim
 *
 * Created on December 26, 2011, 11:22 PM
 * Modified on August 8,2013, 9:14 AM
 */

#include <xc.h>

#include <BOARD.h>
#include <serial.h>
#include <IO_Ports.h>


/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/

#ifdef IO_DEBUG_VERBOSE
#define dbprintf(...) printf(__VA_ARGS__)
#include <stdio.h>
#else
#define dbprintf(...)
#endif


#define NUMPINS 10
#define ZERO 0
#define OFFSET 3
#define TOPVW (OFFSET + 6)      // there are 6 useable pins on PORTV & PORTW
#define TOPXYZ (OFFSET + 10)    // there are 10 useable pins on PORTX,Y, & Z

#define PORTVWMASK 0x01F8       //0b0000 0001 1111 1000
#define PORTXYZMASK 0x1FF8      //0b0001 1111 1111 1000

// code readability macros
#define IO_PortsSetInput(port,i) *PORTS_TRISSET[port][i-OFFSET] = PortsBits[port][i-OFFSET]
#define IO_PortsSetOutput(port,i) *PORTS_TRISCLR[port][i-OFFSET] = PortsBits[port][i-OFFSET]
#define IO_PortsSetPin(port,i) *PORTS_LATSET[port][i-OFFSET] = PortsBits[port][i-OFFSET]
#define IO_PortsClearPin(port,i) *PORTS_LATCLR[port][i-OFFSET] = PortsBits[port][i-OFFSET]
#define IO_PortsTogglePin(port,i) *PORTS_LATINV[port][i-OFFSET] = PortsBits[port][i-OFFSET]

/*******************************************************************************
 * PRIVATE STRUCTS and TYPEDEFS                                                *
 ******************************************************************************/

typedef union {

    struct {
        unsigned : OFFSET;
        unsigned bit3 : 1;
        unsigned bit4 : 1;
        unsigned bit5 : 1;
        unsigned bit6 : 1;
        unsigned bit7 : 1;
        unsigned bit8 : 1;
        unsigned bit9 : 1;
        unsigned bit10 : 1;
        unsigned bit11 : 1;
        unsigned bit12 : 1;
        unsigned : (6 - OFFSET);
    };
    uint16_t ui;
} portBitField_T;

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/

static volatile unsigned int * const PORTS_TRISCLR[][NUMPINS] = {
    {&TRISBCLR, &TRISBCLR, &TRISBCLR, &TRISBCLR, &TRISBCLR, &TRISBCLR, NULL, NULL, NULL, NULL},
    {&TRISBCLR, &TRISBCLR, &TRISBCLR, &TRISBCLR, &TRISBCLR, &TRISBCLR, NULL, NULL, NULL, NULL},
    {&TRISFCLR, &TRISBCLR, &TRISGCLR, &TRISFCLR, &TRISGCLR, &TRISFCLR, &TRISGCLR, &TRISDCLR, &TRISDCLR, &TRISDCLR},
    {&TRISDCLR, &TRISDCLR, &TRISDCLR, &TRISDCLR, &TRISECLR, &TRISDCLR, &TRISECLR, &TRISDCLR, &TRISECLR, &TRISDCLR},
    {&TRISECLR, &TRISFCLR, &TRISECLR, &TRISDCLR, &TRISECLR, &TRISDCLR, &TRISECLR, &TRISFCLR, &TRISECLR, &TRISFCLR}
};
static volatile unsigned int * const PORTS_TRISSET[][NUMPINS] = {
    {&TRISBSET, &TRISBSET, &TRISBSET, &TRISBSET, &TRISBSET, &TRISBSET, NULL, NULL, NULL, NULL},
    {&TRISBSET, &TRISBSET, &TRISBSET, &TRISBSET, &TRISBSET, &TRISBSET, NULL, NULL, NULL, NULL},
    {&TRISFSET, &TRISBSET, &TRISGSET, &TRISFSET, &TRISGSET, &TRISFSET, &TRISGSET, &TRISDSET, &TRISDSET, &TRISDSET},
    {&TRISDSET, &TRISDSET, &TRISDSET, &TRISDSET, &TRISESET, &TRISDSET, &TRISESET, &TRISDSET, &TRISESET, &TRISDSET},
    {&TRISESET, &TRISFSET, &TRISESET, &TRISDSET, &TRISESET, &TRISDSET, &TRISESET, &TRISFSET, &TRISESET, &TRISFSET}
};
static volatile unsigned int * const PORTS_LATCLR[][NUMPINS] = {
    {&LATBCLR, &LATBCLR, &LATBCLR, &LATBCLR, &LATBCLR, &LATBCLR, NULL, NULL, NULL, NULL},
    {&LATBCLR, &LATBCLR, &LATBCLR, &LATBCLR, &LATBCLR, &LATBCLR, NULL, NULL, NULL, NULL},
    {&LATFCLR, &LATBCLR, &LATGCLR, &LATFCLR, &LATGCLR, &LATFCLR, &LATGCLR, &LATDCLR, &LATDCLR, &LATDCLR},
    {&LATDCLR, &LATDCLR, &LATDCLR, &LATDCLR, &LATECLR, &LATDCLR, &LATECLR, &LATDCLR, &LATECLR, &LATDCLR},
    {&LATECLR, &LATFCLR, &LATECLR, &LATDCLR, &LATECLR, &LATDCLR, &LATECLR, &LATFCLR, &LATECLR, &LATFCLR}
};
static volatile unsigned int * const PORTS_LATSET[][NUMPINS] = {
    {&LATBSET, &LATBSET, &LATBSET, &LATBSET, &LATBSET, &LATBSET, NULL, NULL, NULL, NULL},
    {&LATBSET, &LATBSET, &LATBSET, &LATBSET, &LATBSET, &LATBSET, NULL, NULL, NULL, NULL},
    {&LATFSET, &LATBSET, &LATGSET, &LATFSET, &LATGSET, &LATFSET, &LATGSET, &LATDSET, &LATDSET, &LATDSET},
    {&LATDSET, &LATDSET, &LATDSET, &LATDSET, &LATESET, &LATDSET, &LATESET, &LATDSET, &LATESET, &LATDSET},
    {&LATESET, &LATFSET, &LATESET, &LATDSET, &LATESET, &LATDSET, &LATESET, &LATFSET, &LATESET, &LATFSET}
};
static volatile unsigned int * const PORTS_LATINV[][NUMPINS] = {
    {&LATBINV, &LATBINV, &LATBINV, &LATBINV, &LATBINV, &LATBINV, NULL, NULL, NULL, NULL},
    {&LATBINV, &LATBINV, &LATBINV, &LATBINV, &LATBINV, &LATBINV, NULL, NULL, NULL, NULL},
    {&LATFINV, &LATBINV, &LATGINV, &LATFINV, &LATGINV, &LATFINV, &LATGINV, &LATDINV, &LATDINV, &LATDINV},
    {&LATDINV, &LATDINV, &LATDINV, &LATDINV, &LATEINV, &LATDINV, &LATEINV, &LATDINV, &LATEINV, &LATDINV},
    {&LATEINV, &LATFINV, &LATEINV, &LATDINV, &LATEINV, &LATDINV, &LATEINV, &LATFINV, &LATEINV, &LATFINV}
};

#ifdef JP_SPI_MASTER
static const uint16_t PortsBits[][NUMPINS] = {
    {BIT_2, BIT_3, BIT_4, BIT_5, BIT_8, BIT_9, ZERO, ZERO, ZERO, ZERO},
    {BIT_11, BIT_10, BIT_13, BIT_12, BIT_15, BIT_14, ZERO, ZERO, ZERO, ZERO},
    {BIT_5, BIT_0, BIT_6, BIT_4, BIT_7, BIT_6, BIT_8, BIT_7, BIT_4, BIT_6},
    {BIT_11, BIT_3, BIT_5, BIT_10, BIT_7, BIT_9, BIT_6, BIT_2, BIT_5, BIT_1},
    {BIT_4, BIT_1, BIT_3, BIT_0, BIT_2, BIT_8, BIT_1, BIT_3, BIT_0, BIT_2}
};
#else
static const uint16_t PortBits[][NUMPINS] = {
    {BIT_2, BIT_3, BIT_4, BIT_5, BIT_8, BIT_9, ZERO, ZERO, ZERO, ZERO},
    {BIT_11, BIT_10, BIT_13, BIT_12, BIT_15, BIT_14, ZERO, ZERO, ZERO, ZERO},
    {BIT_5, BIT_0, BIT_6, BIT_4, BIT_8, BIT_6, BIT_7, BIT_7, BIT_4, BIT_6},
    {BIT_11, BIT_3, BIT_5, BIT_10, BIT_7, BIT_9, BIT_6, BIT_2, BIT_5, BIT_1},
    {BIT_4, BIT_1, BIT_3, BIT_0, BIT_2, BIT_8, BIT_1, BIT_3, BIT_0, BIT_2}
};
#endif
/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                *
 ******************************************************************************/

/**
 * @Function PortReadV
 * @param none
 * @return Bits on the port
 * @brief Samples to appropriate _BIT register to assemble to port value into the
 *        returned variable.
 * @note Bit positions match the PINS on the header, thus the
 *       first possible non-zero is bit 3. Last is bit 8 for Port V & W.
 * @author Gabriel H Elkaim, 2013.10.20 12:23 */
uint16_t PortReadV(void);

/**
 * @Function PortReadW
 * @param none
 * @return Bits on the port
 * @brief Samples to appropriate _BIT register to assemble to port value into the
 *        returned variable.
 * @note Bit positions match the PINS on the header, thus the
 *       first possible non-zero is bit 3. Last is bit 8 for Port V & W.
 * @author Gabriel H Elkaim, 2013.10.20 12:23 */
uint16_t PortReadW(void);

/**
 * @Function PortReadX
 * @param none
 * @return Bits on the port
 * @brief Samples to appropriate _BIT register to assemble to port value into the
 *        returned variable.
 * @note Bit positions match the PINS on the header, thus the
 *       first possible non-zero is bit 3. Last is bit 12 for Port X Y & Z.
 * @author Gabriel H Elkaim, 2013.10.20 12:23 */
uint16_t PortReadX(void);

/**
 * @Function PortReadY
 * @param none
 * @return Bits on the port
 * @brief Samples to appropriate _BIT register to assemble to port value into the
 *        returned variable.
 * @note Bit positions match the PINS on the header, thus the
 *       first possible non-zero is bit 3. Last is bit 12 for Port X Y & Z.
 * @author Gabriel H Elkaim, 2013.10.20 12:23 */
uint16_t PortReadY(void);

/**
 * @Function PortReadZ
 * @param none
 * @return Bits on the port
 * @brief Samples to appropriate _BIT register to assemble to port value into the
 *        returned variable.
 * @note Bit positions match the PINS on the header, thus the
 *       first possible non-zero is bit 3. Last is bit 12 for Port X Y & Z.
 * @author Gabriel H Elkaim, 2013.10.20 12:23 */
uint16_t PortReadZ(void);

/**
 * @Function: PortHandleHardwareIndirection(char port, unsigned short pattern,
    static volatile unsigned int * const portregister, const char * fname)
 * @param port - #defined as PORTx [V, W, X, Y, or Z]
 * @param pattern - pattern to be inserted into special registers, valid from
 *                  bits 3-8 [V,W] or 3-12 [X,Y,Z]
 * @param portregister - pointer to PORT_TRIS, _LAT, etc.
 * @param altregister - pointer to PORT_TRIS, _LAT, etc.
 * @return SUCCESS or ERROR
 * @brief Handles the hardware indirection and port checking functions to make
 *         the ports on the IO_Board of the Uno32 stack appear to be contiguous
 *         in hardware. Avoids some code replication by having it all handled in
 *         one function.
 * @note if the altregister pointer is NULL, then only the 1's on the pattern are
 *       used. Use this to handle functions where both 1's and 0's are important.
 * @author Gabriel H Elkaim, 2013.10.19 21:49 */
int8_t PortHandleHardwareIndirection(int8_t port, uint16_t pattern,
    volatile unsigned int * const portregister[][NUMPINS],
    volatile unsigned int * const altregister[][NUMPINS]);

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                           *
 ******************************************************************************/

int8_t IO_PortsSetPortDirection(int8_t port, uint16_t pattern)
{
    if (PortHandleHardwareIndirection(port, pattern,
        PORTS_TRISSET, PORTS_TRISCLR) == ERROR) {
        dbprintf("\nIO_Ports: %s failed, must be called with a single PORTx", __FUNCTION__);
    } else {
        return SUCCESS;
    }
    //    unsigned char i, topbit;
    //    if ((port < PORTV) || (port > PORTZ)) { // test if port is in range
    //        dbprintf("\nIO_Ports: Set Direction failed, must be called with a single PORTx");
    //        return ERROR;
    //    }
    //    if (port <= PORTW) { // covers both PORTV and PORTW
    //        topbit = TOPVW;
    //        pattern &= PORTVWMASK;
    //    } else { // covers ports PORTX, PORTY, and PORTZ
    //        topbit = TOPXYZ;
    //        pattern &= PORTXYZMASK;
    //    }
    //    for (i = OFFSET; i < topbit; i++) {
    //        if (pattern & (1 << i)) { // set as input for "1" in position
    //            IO_PortsSetInput(port, i);
    //        } else {
    //            IO_PortsSetOutput(port, i);
    //        }
    //    }
    //    return SUCCESS;
}

int8_t IO_PortsSetPortInputs(int8_t port, uint16_t pattern)
{
    if (PortHandleHardwareIndirection(port, pattern, PORTS_TRISSET, NULL) == ERROR) {
        dbprintf("\nIO_Ports: %s failed, must be called with a single PORTx", __FUNCTION__);
    } else {
        return SUCCESS;
    }
    //    unsigned char i, topbit;
    //    if ((port < PORTV) || (port > PORTZ)) { // test if port is in range
    //        dbprintf("\nIO_Ports: Set Inputs failed, must be called with a single PORTx");
    //        return ERROR;
    //    }
    //    if (port <= PORTW) {
    //        topbit = TOPVW;
    //        pattern &= PORTVWMASK;
    //    } else {
    //        topbit = TOPXYZ;
    //        pattern &= PORTXYZMASK;
    //    }
    //    for (i = OFFSET; i < topbit; i++) {
    //        if (pattern & (1 << i)) { // set as input for "1" in position
    //            IO_PortsSetInput(port, i);
    //        }
    //    }
    //    return SUCCESS;
}

int8_t IO_PortsSetPortOutputs(int8_t port, uint16_t pattern)
{
    if (PortHandleHardwareIndirection(port, pattern, PORTS_TRISCLR, NULL) == ERROR) {
        dbprintf("\nIO_Ports: %s failed, must be called with a single PORTx", __FUNCTION__);
    } else {
        return SUCCESS;
    }
    //    unsigned char i, topbit;
    //    if ((port < PORTV) || (port > PORTZ)) { // test if port is in range
    //        dbprintf("\nIO_Ports: Set Output failed, must be called with a single PORTx");
    //        return ERROR;
    //    }
    //    if (port <= PORTW) {
    //        topbit = TOPVW;
    //        pattern &= PORTVWMASK;
    //    } else {
    //        topbit = TOPXYZ;
    //        pattern &= PORTXYZMASK;
    //    }
    //    for (i = OFFSET; i < topbit; i++) {
    //        if (pattern & (1 << i)) { // set as output for "1" in position
    //            IO_PortsSetOutput(port, i);
    //        }
    //    }
    //    return SUCCESS;
}

int16_t IO_PortsReadPort(int8_t port)
{
    if ((port < PORTV) || (port > PORTZ)) {
        dbprintf("\nIO_Ports: %s failed, must be called with a single PORTx", __FUNCTION__);
        return ERROR;
    }
    switch (port) {
    case PORTV:
        return PortReadV();
        break;
    case PORTW:
        return PortReadW();
        break;
    case PORTX:
        return PortReadX();
        break;
    case PORTY:
        return PortReadY();
        break;
    case PORTZ:
        return PortReadZ();
    default:
        dbprintf("\nSwitch error not trapped in %s, very bad", __FUNCTION__);
        return ERROR;
        break;
    }
}

int8_t IO_PortsWritePort(int8_t port, uint16_t pattern)
{
    if (PortHandleHardwareIndirection(port, pattern, PORTS_LATSET, PORTS_LATCLR) == ERROR) {
        dbprintf("\nIO_Ports: %s failed, must be called with a single PORTx", __FUNCTION__);
    } else {
        return SUCCESS;
    }
    //    unsigned char i, topbit;
    //    if ((port < PORTV) || (port > PORTZ)) { // test if port has only one bit active
    //        dbprintf("\nIO_Ports: Write failed, must be called with a single PORTx");
    //        return ERROR;
    //    }
    //    if (port <= PORTW) {
    //        topbit = TOPVW;
    //        pattern &= PORTVWMASK;
    //    } else {
    //        topbit = TOPXYZ;
    //        pattern &= PORTXYZMASK;
    //    }
    //    for (i = OFFSET; i < topbit; i++) {
    //        if (pattern & (1 << i)) { // set for "1" in position
    //            IO_PortsSetPin(port, i);
    //        } else {
    //            IO_PortsClearPin(port, i);
    //        }
    //    }
    //    return SUCCESS;
}

int8_t IO_PortsSetPortBits(int8_t port, uint16_t pattern)
{
    if (PortHandleHardwareIndirection(port, pattern, PORTS_LATSET, NULL) == ERROR) {
        dbprintf("\nIO_Ports: %s failed, must be called with a single PORTx", __FUNCTION__);
    } else {
        return SUCCESS;
    }
    //    unsigned char i, topbit;
    //    if ((port < PORTV) || (port > PORTZ)) { // test if port has only one bit active
    //        dbprintf("\nIO_Ports: Set Bits failed, must be called with a single PORTx");
    //        return ERROR;
    //    }
    //    if (port <= PORTW) {
    //        topbit = TOPVW;
    //        pattern &= PORTVWMASK;
    //    } else {
    //        topbit = TOPXYZ;
    //        pattern &= PORTXYZMASK;
    //    }
    //    for (i = OFFSET; i < topbit; i++) {
    //        if (pattern & (1 << i)) { // set for "1" in position
    //            IO_PortsSetPin(port, i);
    //        }
    //    }
    //    return SUCCESS;
}

int8_t IO_PortsClearPortBits(int8_t port, uint16_t pattern)
{
    if (PortHandleHardwareIndirection(port, pattern, PORTS_LATCLR, NULL) == ERROR) {
        dbprintf("\nIO_Ports: %s failed, must be called with a single PORTx", __FUNCTION__);
    } else {
        return SUCCESS;
    }
    //    unsigned char i, topbit;
    //    if ((port < PORTV) || (port > PORTZ)) { // test if port has only one bit active
    //        dbprintf("\nIO_Ports: Clear Bits failed, must be called with a single PORTx");
    //        return ERROR;
    //    }
    //    if (port <= PORTW) {
    //        topbit = TOPVW;
    //        pattern &= PORTVWMASK;
    //    } else {
    //        topbit = TOPXYZ;
    //        pattern &= PORTXYZMASK;
    //    }
    //    for (i = OFFSET; i < topbit; i++) {
    //        if (pattern & (1 << i)) { // set for "1" in position
    //            IO_PortsClearPin(port, i);
    //        }
    //    }
    //    return SUCCESS;
}

int8_t IO_PortsTogglePortBits(int8_t port, uint16_t pattern)
{
    if (PortHandleHardwareIndirection(port, pattern, PORTS_LATINV, NULL) == ERROR) {
        dbprintf("\nIO_Ports: %s failed, must be called with a single PORTx", __FUNCTION__);
    } else {
        return SUCCESS;
    }
    //    unsigned char i, topbit;
    //    if ((port < PORTV) || (port > PORTZ)) { // test if port has only one bit active
    //        dbprintf("\nIO_Ports: Toggle Bits failed, must be called with a single PORTx");
    //        return ERROR;
    //    }
    //    if (port <= PORTW) {
    //        topbit = TOPVW;
    //        pattern &= PORTVWMASK;
    //    } else {
    //        topbit = TOPXYZ;
    //        pattern &= PORTXYZMASK;
    //    }
    //    for (i = OFFSET; i < topbit; i++) {
    //        if (pattern & (1 << i)) { // set for "1" in position
    //            IO_PortsTogglePin(port, i);
    //        }
    //    }
    //    return SUCCESS;
}

/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/

uint16_t PortReadV(void)
{
    portBitField_T portV = {.ui = 0x0000};
    portV.bit3 = PORTV03_BIT;
    portV.bit4 = PORTV04_BIT;
    portV.bit5 = PORTV05_BIT;
    portV.bit6 = PORTV06_BIT;
    portV.bit7 = PORTV07_BIT;
    portV.bit8 = PORTV08_BIT;
    return (portV.ui);
}

uint16_t PortReadW(void)
{
    portBitField_T portW = {.ui = 0x0000};
    portW.bit3 = PORTW03_BIT;
    portW.bit4 = PORTW04_BIT;
    portW.bit5 = PORTW05_BIT;
    portW.bit6 = PORTW06_BIT;
    portW.bit7 = PORTW07_BIT;
    portW.bit8 = PORTW08_BIT;
    return (portW.ui);
}

uint16_t PortReadX(void)
{
    portBitField_T portX = {.ui = 0x0000};
    portX.bit3 = PORTX03_BIT;
    portX.bit4 = PORTX04_BIT;
    portX.bit5 = PORTX05_BIT;
    portX.bit6 = PORTX06_BIT;
    portX.bit7 = PORTX07_BIT;
    portX.bit8 = PORTX08_BIT;
    portX.bit9 = PORTX09_BIT;
    portX.bit10 = PORTX10_BIT;
    portX.bit11 = PORTX11_BIT;
    portX.bit12 = PORTX12_BIT;
    return (portX.ui);
}

uint16_t PortReadY(void)
{
    portBitField_T portY = {.ui = 0x0000};
    portY.bit3 = PORTY03_BIT;
    portY.bit4 = PORTY04_BIT;
    portY.bit5 = PORTY05_BIT;
    portY.bit6 = PORTY06_BIT;
    portY.bit7 = PORTY07_BIT;
    portY.bit8 = PORTY08_BIT;
    portY.bit9 = PORTY09_BIT;
    portY.bit10 = PORTY10_BIT;
    portY.bit11 = PORTY11_BIT;
    portY.bit12 = PORTY12_BIT;
    return (portY.ui);
}

uint16_t PortReadZ(void)
{
    portBitField_T portZ = {.ui = 0x0000};
    portZ.bit3 = PORTZ03_BIT;
    portZ.bit4 = PORTZ04_BIT;
    portZ.bit5 = PORTZ05_BIT;
    portZ.bit6 = PORTZ06_BIT;
    portZ.bit7 = PORTZ07_BIT;
    portZ.bit8 = PORTZ08_BIT;
    portZ.bit9 = PORTZ09_BIT;
    portZ.bit10 = PORTZ10_BIT;
    portZ.bit11 = PORTZ11_BIT;
    portZ.bit12 = PORTZ12_BIT;
    return (portZ.ui);
}

/**
 * @Function: PortHandleHardwareIndirection(char port, unsigned short pattern, 
    static volatile unsigned int * const portregister, const char * fname)
 * @param port - #defined as PORTx [V, W, X, Y, or Z]
 * @param pattern - pattern to be inserted into special registers, valid from
 *                  bits 3-8 [V,W] or 3-12 [X,Y,Z]
 * @param portregister - pointer to PORT_TRIS, _LAT, etc.
 * @param altregister - pointer to PORT_TRIS, _LAT, etc.
 * @return SUCCESS or ERROR
 * @brief Handles the hardware indirection and port checking functions to make
 *         the ports on the IO_Board of the Uno32 stack appear to be contiguous
 *         in hardware. Avoids some code replication by having it all handled in
 *         one function.
 * @note if the altregister pointer is NULL, then only the 1's on the pattern are
 *       used. Use this to handle functions where both 1's and 0's are important.
 * @author Gabriel H Elkaim, 2013.10.19 21:49 */
int8_t PortHandleHardwareIndirection(int8_t port, uint16_t pattern,
    volatile unsigned int * const portregister[][NUMPINS],
    volatile unsigned int * const altregister[][NUMPINS])
{
    uint8_t i, topbit;
    if ((port < PORTV) || (port > PORTZ)) { // test if port is within range
        return ERROR;
    }
    if (port <= PORTW) {
        topbit = TOPVW;
        pattern &= PORTVWMASK;
    } else {
        topbit = TOPXYZ;
        pattern &= PORTXYZMASK;
    }
    for (i = OFFSET; i < topbit; i++) {
        if (pattern & (1 << i)) { // set for "1" in position
            *portregister[port][i - OFFSET] = PortsBits[port][i - OFFSET];
            // This is where the actual hardware abstraction is handled, along with
            // the offsets for the fact that the pins on the IOBoard are numbered
            // from 1 to 14, with pins 1 & 2 and 13 & 14 always being ground.
            // See code useability macros above for decoding these.
        } else if (altregister != NULL) {
            *altregister[port][i - OFFSET] = PortsBits[port][i - OFFSET];
        }
    }
    return SUCCESS;

}

/*******************************************************************************
 * TEST HARNESS                                                                *
 ******************************************************************************/

#ifdef IO_PORTS_TEST

#include <SERIAL.h>
#include "IO_Ports.h"
#include <stdio.h>

#define DELAY(x)    for (wait = 0; wait <= x; wait++) {asm("nop");}
#define A_WEE_BIT   18300
#define A_BIT       183000
#define A_LOT       1830000
#define NUM2REPEAT  10
#define OUTPUTPORT PORTY
#define INPUTPORT PORTX

/* Testing function is just a pointer to function wrapper to make the testing harness
 * take less space to implement.
 */
void TestFunction(int8_t port, uint16_t pattern, int8_t(*functionPtr)(int8_t, uint16_t),
    char * functionName, int8_t desiredResult)
{
    printf("testing %s: ", functionName);
    if (functionPtr(port, pattern) == desiredResult) {
        printf("PASSED");
    } else {
        printf("FAILED");
    }
}

int main(void)
{

    int wait;
    unsigned short oldPattern, pattern;
    int16_t j, k;
    char portLabel[] = {'V', 'W', 'X', 'Y', 'Z'};
    unsigned short pinLimit = PIN8;
    //DELAY(A_BIT);
    BOARD_Init();
    //DELAY(A_LOT);

    printf("\nTest Harness for IO_Ports Library");
    printf("\nSet #defined INPUTPORT and OUTPUTPORT for testing");

    //
    // Test all functions with input PORT out of range
    //
    printf("\n\nTesting all functions for input out of bounds...\n");
    printf("\nInput out of range low,  ");
    TestFunction(-1, 0xFFFF, &IO_PortsSetPortDirection, "IO_PortsSetPortDirection", ERROR);
    printf("\nInput out of range high, ");
    TestFunction(6, 0xFFFF, &IO_PortsSetPortDirection, "IO_PortsSetPortDirection", ERROR);
    printf("\nInput out of range low,  ");
    TestFunction(-1, 0xFFFF, &IO_PortsSetPortInputs, "IO_PortsSetPortInputs", ERROR);
    printf("\nInput out of range high, ");
    TestFunction(6, 0xFFFF, &IO_PortsSetPortInputs, "IO_PortsSetPortInputs", ERROR);
    printf("\nInput out of range low,  ");
    TestFunction(-1, 0xFFFF, &IO_PortsSetPortOutputs, "IO_PortsSetPortOutputs", ERROR);
    printf("\nInput out of range high, ");
    TestFunction(6, 0xFFFF, &IO_PortsSetPortOutputs, "IO_PortsSetPortOutputs", ERROR);
    printf("\nInput out of range low,  ");
    TestFunction(-1, 0xFFFF, &IO_PortsWritePort, "IO_PortsWritePort", ERROR);
    printf("\nInput out of range high, ");
    TestFunction(6, 0xFFFF, &IO_PortsWritePort, "IO_PortsWritePort", ERROR);
    printf("\nInput out of range low,  ");
    TestFunction(-1, 0xFFFF, &IO_PortsSetPortBits, "IO_PortsSetPortBits", ERROR);
    printf("\nInput out of range high, ");
    TestFunction(6, 0xFFFF, &IO_PortsSetPortBits, "IO_PortsSetPortBits", ERROR);
    printf("\nInput out of range low,  ");
    TestFunction(-1, 0xFFFF, &IO_PortsClearPortBits, "IO_PortsClearPortBits", ERROR);
    printf("\nInput out of range high, ");
    TestFunction(6, 0xFFFF, &IO_PortsClearPortBits, "IO_PortsClearPortBits", ERROR);
    printf("\nInput out of range low,  ");
    TestFunction(-1, 0xFFFF, &IO_PortsTogglePortBits, "IO_PortsTogglePortBits", ERROR);
    printf("\nInput out of range high, ");
    TestFunction(6, 0xFFFF, &IO_PortsTogglePortBits, "IO_PortsTogglePortBits", ERROR);
    printf("\nInput out of range  low, testing IO_PortsReadPort:");
    if (IO_PortsReadPort(-1) == ERROR) {
        printf("PASSED");
    } else {
        printf("FAILED");
    }
    printf("\nInput out of range high, testing IO_PortsReadPort:");
    if (IO_PortsReadPort(6) == ERROR) {
        printf("PASSED");
    } else {
        printf("FAILED");
    }
    DELAY(A_BIT);

    //
    // Test that all ports can be turned into outputs successfully
    //
    printf("\n\nTesting all ports set to output using IO_PortsSetDirection\n");
    for (k = PORTV; k <= PORTZ; k++) {
        printf("\nSetting PORT%c to output: ", portLabel[k]);
        TestFunction(k, 0x0000, &IO_PortsSetPortDirection, "IO_PortsSetPortDirection", SUCCESS);
    }
    printf("\n\nTesting all ports set to output using IO_PortsSetPortOutputs\n");
    for (k = PORTV; k <= PORTZ; k++) {
        printf("\nSetting PORT%c to output: ", portLabel[k]);
        TestFunction(k, 0xFFFF, &IO_PortsSetPortOutputs, "IO_PortsSetPortOutputs", SUCCESS);
    }
    DELAY(A_BIT);

    //
    // Test that all ports can be turned into inputs successfully
    //
    printf("\n\nTesting all ports set to inputs using IO_PortsSetDirection\n");
    for (k = PORTV; k <= PORTZ; k++) {
        printf("\nSetting PORT%c to input: ", portLabel[k]);
        TestFunction(k, 0xFFFF, &IO_PortsSetPortDirection, "IO_PortsSetPortDirection", SUCCESS);
    }
    printf("\n\nTesting all ports set to input using IO_PortsSetPortInputs\n");
    for (k = PORTV; k <= PORTZ; k++) {
        printf("\nSetting PORT%c to input: ", portLabel[k]);
        TestFunction(k, 0xFFFF, &IO_PortsSetPortInputs, "IO_PortsSetPortInputs", SUCCESS);
    }
    DELAY(A_BIT);

    //
    // Set OUTPUTPORT to output, all others to INPUTS
    //
    printf("\n\nSetting OUTPUTPORT [PORT%c] to output using IO_PortsSetPortDirection: ", portLabel[OUTPUTPORT]);
    if (IO_PortsSetPortDirection(OUTPUTPORT, 0x0000) == SUCCESS) {
        printf("PASSED");
    } else {
        printf("FAILED");
    }
    printf("\nSetting all other ports to inputs");
    for (k = PORTV; k <= PORTZ; k++) {
        if (k != OUTPUTPORT) {
            printf("\nSetting PORT%c to input: ", portLabel[k]);
            TestFunction(k, 0xFFFF, &IO_PortsSetPortInputs, "IO_PortsSetPortInputs", SUCCESS);
        }
    }
    DELAY(A_BIT);

    //
    // Strobe pins on OUTPUT port, leave all others alone
    //
    printf("\n\nTesting IO_PORT output functions");
    printf("\n\nStrobing the pins on OUTPUTPORT [PORT%c] sequentially using IO_PortsWritePort", portLabel[OUTPUTPORT]);
    printf("\nuse a logic analyzer or and LED bank to see outputs walk from PIN3 to PIN8/12");
    printf("\npattern will strobe each pin %u times and then move on to the next one", NUM2REPEAT);
    if (OUTPUTPORT < PORTX) {
        pinLimit = PIN8;
    } else {
        pinLimit = PIN12;
    }
    printf("\nStrobing: ");
    for (k = PIN3; k <= pinLimit; k <<= 1) {
        printf("[0x%04X] ", k);
        for (j = 0; j < NUM2REPEAT; j++) {
            IO_PortsWritePort(OUTPUTPORT, k);
            DELAY(A_WEE_BIT);
            IO_PortsWritePort(OUTPUTPORT, 0x0000);
            DELAY(A_WEE_BIT);
        }
    }
    DELAY(A_BIT);

    printf("\n\nStrobing the pins on OUTPUTPORT [PORT%c] sequentially using IO_PortsTogglePortBits", portLabel[OUTPUTPORT]);
    printf("\nuse a logic analyzer or and LED bank to see outputs walk from PIN8/12 to PIN3");
    printf("\npattern will strobe each pin %u times and then move on to the next one", NUM2REPEAT);
    printf("\nStrobing: ");
    for (k = pinLimit; k >= PIN3; k >>= 1) {
        printf("[0x%04X] ", k);
        for (j = 0; j < NUM2REPEAT * 2; j++) {
            IO_PortsTogglePortBits(OUTPUTPORT, k);
            DELAY(A_WEE_BIT);
        }
    }
    DELAY(A_BIT);


    printf("\n\nStrobing the pins on OUTPUTPORT [PORT%c] adding pins using IO_PortsSetPortBits and IO_PortsClearPortBits", portLabel[OUTPUTPORT]);
    printf("\nuse a logic analyzer or and LED bank to see outputs increase from PIN3 to PIN8/12");
    printf("\npattern will strobe each pin %u times and then move on and add the next one", NUM2REPEAT);
    if (OUTPUTPORT < PORTX) {
        pinLimit = PORTVWMASK;
    } else {
        pinLimit = PORTXYZMASK;
    }
    printf("\nStrobing: ");
    for (k = PIN3; k <= pinLimit; k = (k << 1) + PIN3) {
        printf("[0x%04X] ", k);
        for (j = 0; j < NUM2REPEAT; j++) {
            IO_PortsSetPortBits(OUTPUTPORT, k);
            DELAY(A_WEE_BIT);
            IO_PortsClearPortBits(OUTPUTPORT, k);
            DELAY(A_WEE_BIT);
        }
    }
    DELAY(A_BIT);

    //
    // Testing the input functions for the IO_Ports library
    //
    printf("\n\nSetting INPUTPORT [PORT%c] to INPUT, leave others untouched: ", portLabel[INPUTPORT]);
    if (IO_PortsSetPortDirection(INPUTPORT, 0xFFFF) == SUCCESS) {
        printf("PASSED");
    } else {
        printf("FAILED");
    }


    printf("\nTesting IO_PortsReadPort, all pins on PORT%c will be echo'd onto PORT%c.", portLabel[INPUTPORT], portLabel[OUTPUTPORT]);
    oldPattern = IO_PortsReadPort(INPUTPORT);
    printf("\n[0x%04X] ", oldPattern);
    k = 0;
    for (j = 0; j < 1000; j++) {
        pattern = IO_PortsReadPort(INPUTPORT);
        IO_PortsWritePort(OUTPUTPORT, pattern);
        if (pattern != oldPattern) {
            oldPattern = pattern;
            printf("[0x%04X] ", oldPattern);
            if (++k > 10) {
                k = 0;
                printf("\n");
            }
        }
        DELAY(A_WEE_BIT);
    }
    printf("\n\nSetting all ports to INPUTS as high impedance");
    for (k = PORTV; k <= PORTZ; k++) {
        IO_PortsSetPortInputs(k, 0xFFFF);
    }
    printf("\nEnd of IO_PORTS unit testing");
    DELAY(A_LOT);
    while (1);
}
#endif