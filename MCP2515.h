/**
 *
 * MCP2515.h
 *
 * Headers for MCP2515.cpp
 *
 */
#ifndef MCP2515_H_INCLUDED
#define MCP2515_H_INCLUDED

#include <iostream>
#include <syslog.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include "MCP2515_regs.h"
#include "readIni.h"

extern char readIniBuf[40];
extern char iniFile[20];

extern int CSPIN;

extern bool DEBUG_CONFIG;
extern bool DEBUG_RUN;
extern bool DEBUG_INMSG;
extern bool DEBUG_OUTMSG;

extern uint16_t MCP2515_LDELAY;
extern uint8_t MCP2515_MRETRY;

class MCP2515
{
public:
    /**
     * Constructor for class MCP2515
     *
     * After a new MCP2515, method Init() is mandatory
     */
    MCP2515();

    /**
     * Initialize chipset MCP2515
     *
     * No parameters nided, all are in ini file
     * Return true if success otherwise false
     */
    bool Init();

    /**
     * Write one byte to a register
     *
     * register : uint8_t address
     * byte : uint8_t data
     * Return true if success otherwise false
     */
    bool Write(uint8_t address, uint8_t data);

    /**
     * Modify fron 1 to 8 bit of a register according to mask value
     *
     * register : uint8_t address
     * Mask : int8_t mask
     * Bits to be modified : uint8_t data
     * Return true if success otherwise false
     */
    bool BitModify(uint8_t address, uint8_t mask, uint8_t data);

    /**
     * Change running mode of chipset
     *
     * Available modes are Normal, Loopback, Listen Only, Config and Sleep
     * Return true if success otherwise false
     */
    bool Mode(uint8_t mode);

    /**
     * Read one byte from a register
     *
     * register : uint8_t address
     * Return one byte
     */
    uint8_t Read(uint8_t address);

    /**
     * Reset the CAN controler
     * Return true if success otherwise false
     */
    bool Reset();

    /**
     * Read CAN message from a buffer
     *
     * Buffer is RBX0 or RBX1
     * Return a Frame structure defined in MCP2515_regs.h
     */
    Frame ReadBuffer(uint8_t buffer);

    /**
     * Load a CAN massage into buffer
     *
     * CAN message is a Frame structure
     * Buffer is TXB0 or TXB1 or TXB2
     * Return true if success otherwise false
     */
    bool LoadBuffer(uint8_t buffer, Frame message);

    /**
     * Request to send an already loaded buffer
     *
     * Buffer is TXB0 or TXB1 or TXB2 or ALL
     * Return true if success otherwise false
     */
    bool SendBuffer(uint8_t buffers);

private:
    uint8_t _CSPIN;
    uint8_t _INTPIN;
    uint8_t _INTLED;
    uint8_t _FOSC;
    uint16_t _NBR;
    uint8_t _SPPos;

    bool _DEBUG_RUN;
    bool _DEBUG_CONFIG;
    bool _DEBUG_INMSG;
    bool _DEBUG_OUTMSG;

    uint16_t _LDELAY;
    uint8_t _MRETRY;

    uint8_t _SJW;   // CNF1         Bits 7-6
    bool _BLTMODE;  // CNF2         Bit 7
    bool _SAMP;     //              Bit 6
    bool _SOF;      // CNF3         Bit 7
    bool _WAKFIL;   //              Bit 6
    bool _B0BFM;    // BFPCTRL      Bit 1
    bool _B1BFM;    //              Bit 2
    bool _B0BFE;    //              Bit 3
    bool _B1BFE;    //              Bit 4
    bool _RX0IE;    // CANINTE      Bit 0
    bool _RX1IE;    //              Bit 1
    bool _TX0IE;    //              Bit 2
    bool _TX1IE;    //              Bit 3
    bool _TX2IE;    //              Bit 4
    bool _ERRIE;    //              Bit 5
    bool _WAKIE;    //              Bit 6
    bool _MERRE;    //              Bit 7
    bool _RX0IF;    // CANINTF      Bit 0
    bool _RX1IF;    //              Bit 1
    bool _TX0IF;    //              Bit 2
    bool _TX1IF;    //              Bit 3
    bool _TX2IF;    //              Bit 4
    bool _ERRIF;    //              Bit 5
    bool _WAKIF;    //              Bit 6
    bool _MERRF;    //              Bit 7
    bool _B0RTSM;   // TXRTSCTRL    Bit 0
    bool _B1RTSM;   //              Bit 1
    bool _B2RTSM;   //              Bit 2
    uint8_t _RXM0;  // RXB0CTRL     Bits 6-5
    bool _BUKT;     //              Bit 2
    uint8_t _RXM1;  // RXB1CTRL     Bits 6-5
    uint8_t _REQOP; // CANCTRL      Bits 7-5
    bool _ABAT;     //              Bit 4
    bool _OSM;      //              Bit 3
    bool _CLKEN;    //              Bit 2
    uint8_t _CLKPRE;//              Bits 1-0
    uint32_t _MASK0;// RXB0 Mask
    uint32_t _MASK1;// RXB1 Mask
    uint32_t _FILT0;// RXB0 Filters
    uint32_t _FILT1;
    uint32_t _FILT2;// RXB1 Filters
    uint32_t _FILT3;
    uint32_t _FILT4;
    uint32_t _FILT5;
};

#endif // MCP2515_H_INCLUDED
