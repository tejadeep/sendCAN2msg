/**
 *
 * MCP2515.cpp
 *
 * Classes Chipset Microchip MCP2515
 *
 */
#include "MCP2515.h"
#include "canfoncts.h"

MCP2515::MCP2515()
{
    _DEBUG_CONFIG=DEBUG_CONFIG;
    _DEBUG_RUN=DEBUG_RUN;
    _DEBUG_INMSG=DEBUG_INMSG;
    _DEBUG_OUTMSG=DEBUG_OUTMSG;

    _CSPIN=CSPIN;

    ini_gets("MCP2515","IntPin","6",readIniBuf,20,iniFile);
    _INTPIN=uint8_t(strtol(readIniBuf,NULL,10));
    ini_gets("MCP2515","IntLed","6",readIniBuf,20,iniFile);
    _INTLED=uint8_t(strtol(readIniBuf,NULL,10));
    ini_gets("MCP2515","fOsc","16",readIniBuf,20,iniFile);
    _FOSC=uint8_t(strtol(readIniBuf,NULL,10));
    ini_gets("MCP2515","lDelay","10",readIniBuf,20,iniFile);
    uint16_t _LDELAY=uint16_t(strtol(readIniBuf,NULL,10));
    ini_gets("MCP2515","mRetry","5",readIniBuf,20,iniFile);
    uint8_t _MRETRY=uint8_t(strtol(readIniBuf,NULL,10));

    MCP2515_LDELAY=_LDELAY;
    MCP2515_MRETRY=_MRETRY;

    ini_gets("CANBUS","MBR","250",readIniBuf,20,iniFile);
    _NBR=uint16_t(strtol(readIniBuf,NULL,10));
    ini_gets("CANBUS","SPPos","75",readIniBuf,20,iniFile);
    _SPPos=uint8_t(strtol(readIniBuf,NULL,10));

    ini_gets("CNF1","SJW","0",readIniBuf,20,iniFile);
    _SJW=uint8_t(strtol(readIniBuf,NULL,10));

    ini_gets("CNF2","BLTMODE","0",readIniBuf,20,iniFile);
    _BLTMODE=bool(strtol(readIniBuf,NULL,10));
    ini_gets("CNF2","SAMP","0",readIniBuf,20,iniFile);
    _SAMP=bool(strtol(readIniBuf,NULL,10));

    ini_gets("CNF3","SOF","0",readIniBuf,20,iniFile);
    _SOF=bool(strtol(readIniBuf,NULL,10));
    ini_gets("CNF3","WAKFIL","0",readIniBuf,20,iniFile);
    _WAKFIL=bool(strtol(readIniBuf,NULL,10));

    ini_gets("RXB0CTRL","RXM","0",readIniBuf,20,iniFile);
    _RXM0=uint8_t(strtol(readIniBuf,NULL,10));
    ini_gets("RXB0CTRL","BUKT","0",readIniBuf,20,iniFile);
    _BUKT=bool(strtol(readIniBuf,NULL,10));
    ini_gets("RXB1CTRL","RXM","0",readIniBuf,20,iniFile);
    _RXM1=uint8_t(strtol(readIniBuf,NULL,10));

    ini_gets("BFPCTRL","B0BFM","0",readIniBuf,20,iniFile);
    _B0BFM=bool(strtol(readIniBuf,NULL,10));
    ini_gets("BFPCTRL","B0BFM","0",readIniBuf,20,iniFile);
    _B1BFM=bool(strtol(readIniBuf,NULL,10));
    ini_gets("BFPCTRL","B0BFM","0",readIniBuf,20,iniFile);
    _B0BFE=bool(strtol(readIniBuf,NULL,10));
    ini_gets("BFPCTRL","B0BFM","0",readIniBuf,20,iniFile);
    _B1BFE=bool(strtol(readIniBuf,NULL,10));

    ini_gets("CANINTE","RX0IE","0",readIniBuf,20,iniFile);
    _RX0IE=bool(strtol(readIniBuf,NULL,10));
    ini_gets("CANINTE","RX1IE","0",readIniBuf,20,iniFile);
    _RX1IE=bool(strtol(readIniBuf,NULL,10));
    ini_gets("CANINTE","TX0IE","0",readIniBuf,20,iniFile);
    _TX0IE=bool(strtol(readIniBuf,NULL,10));
    ini_gets("CANINTE","TX1IE","0",readIniBuf,20,iniFile);
    _TX1IE=bool(strtol(readIniBuf,NULL,10));
    ini_gets("CANINTE","TX2IE","0",readIniBuf,20,iniFile);
    _TX2IE=bool(strtol(readIniBuf,NULL,10));
    ini_gets("CANINTE","ERRIE","0",readIniBuf,20,iniFile);
    _ERRIE=bool(strtol(readIniBuf,NULL,10));
    ini_gets("CANINTE","WAKIE","0",readIniBuf,20,iniFile);
    _WAKIE=bool(strtol(readIniBuf,NULL,10));
    ini_gets("CANINTE","MERRE","0",readIniBuf,20,iniFile);
    _MERRE=bool(strtol(readIniBuf,NULL,10));

    ini_gets("CANINTF","RX0IF","0",readIniBuf,20,iniFile);
    _RX0IF=bool(strtol(readIniBuf,NULL,10));
    ini_gets("CANINTF","RX1IF","0",readIniBuf,20,iniFile);
    _RX1IF=bool(strtol(readIniBuf,NULL,10));
    ini_gets("CANINTF","TX0IF","0",readIniBuf,20,iniFile);
    _TX0IF=bool(strtol(readIniBuf,NULL,10));
    ini_gets("CANINTF","TX1IF","0",readIniBuf,20,iniFile);
    _TX1IF=bool(strtol(readIniBuf,NULL,10));
    ini_gets("CANINTF","TX2IF","0",readIniBuf,20,iniFile);
    _TX2IF=bool(strtol(readIniBuf,NULL,10));
    ini_gets("CANINTF","ERRIF","0",readIniBuf,20,iniFile);
    _ERRIF=bool(strtol(readIniBuf,NULL,10));
    ini_gets("CANINTF","WAKIF","0",readIniBuf,20,iniFile);
    _WAKIF=bool(strtol(readIniBuf,NULL,10));
    ini_gets("CANINTF","MERRF","0",readIniBuf,20,iniFile);
    _MERRF=bool(strtol(readIniBuf,NULL,10));

    ini_gets("TXRTSCTRL","B0RTSM","0",readIniBuf,20,iniFile);
    _B0RTSM=bool(strtol(readIniBuf,NULL,10));
    ini_gets("TXRTSCTRL","B1RTSM","0",readIniBuf,20,iniFile);
    _B1RTSM=bool(strtol(readIniBuf,NULL,10));
    ini_gets("TXRTSCTRL","B2RTSM","0",readIniBuf,20,iniFile);
    _B2RTSM=bool(strtol(readIniBuf,NULL,10));

    ini_gets("CANCTRL","REQOP","0",readIniBuf,20,iniFile);
    _REQOP=uint8_t(strtol(readIniBuf,NULL,10));
     ini_gets("CANCTRL","ABAT","0",readIniBuf,20,iniFile);
    _ABAT=bool(strtol(readIniBuf,NULL,10));
     ini_gets("CANCTRL","OSM","0",readIniBuf,20,iniFile);
    _OSM=bool(strtol(readIniBuf,NULL,10));
    ini_gets("CANCTRL","CLKEN","0",readIniBuf,20,iniFile);
    _CLKEN=bool(strtol(readIniBuf,NULL,10));
    ini_gets("CANCTRL","CLKPRE","0",readIniBuf,20,iniFile);
    _CLKPRE=uint8_t(strtol(readIniBuf,NULL,10));

    ini_gets("FILTERS","MASK0","0",readIniBuf,40,iniFile);
    _MASK0=uint32_t(strtol(readIniBuf,NULL,2));
    ini_gets("FILTERS","MASK1","0",readIniBuf,40,iniFile);
    _MASK1=uint32_t(strtol(readIniBuf,NULL,2));
    ini_gets("FILTERS","FILT0","0",readIniBuf,40,iniFile);
    _FILT0=uint32_t(strtol(readIniBuf,NULL,2));
    ini_gets("FILTERS","FILT1","0",readIniBuf,40,iniFile);
    _FILT1=uint32_t(strtol(readIniBuf,NULL,2));
    ini_gets("FILTERS","FILT2","0",readIniBuf,40,iniFile);
    _FILT2=uint32_t(strtol(readIniBuf,NULL,2));
    ini_gets("FILTERS","FILT3","0",readIniBuf,40,iniFile);
    _FILT3=uint32_t(strtol(readIniBuf,NULL,2));
    ini_gets("FILTERS","FILT4","0",readIniBuf,40,iniFile);
    _FILT4=uint32_t(strtol(readIniBuf,NULL,2));
    ini_gets("FILTERS","FILT5","0",readIniBuf,40,iniFile);
    _FILT5=uint32_t(strtol(readIniBuf,NULL,2));
}

bool MCP2515::Init()
{
    if(!Reset())
    {
        return false;
    }

    pinMode(_INTPIN, INPUT);
    pinMode(_INTLED, OUTPUT);

    for(int i=0; i<3; i++)
    {
        digitalWrite (_INTLED, HIGH) ;
        delay (500) ;
        digitalWrite (_INTLED, LOW) ;
        delay (500) ;
    }

    int mcpBRP;               // BRP for MicroChip manufacturer
    float tQ;                 // Time Quantum in seconds
    int nTQ;                  // Number of TQ
    float tmpNTQ;             // Temporary number of TQ
    float nBT;                // Nominal Bit Time in seconds
    int sPt;                  // Sample Point
    int phSeg1;               // Phase Segment 1
    int phSeg2;               // Phase Segment 2
    int prSeg;                // Propagation Segment

    nBT = 1.0 / ((float)_NBR * 1000.0);
    for(mcpBRP=0; mcpBRP<15; mcpBRP++)          // BRP from 1 to 32
    {
        tQ = 2.0 * ((float)mcpBRP + 1.0) / ((float)_FOSC * 1000000.0);
        tmpNTQ = nBT / tQ;
        nTQ = (int)tmpNTQ;
        if(tmpNTQ-nTQ==0)   // nTQ is an entire value ?
        {
            if(nTQ>=8 && nTQ<=25)
            {
                sPt = (_SPPos * nTQ / 100 + 0.5); // Sample point rounded
                phSeg2 = nTQ - sPt;
                phSeg1 = phSeg2;
                prSeg = nTQ - 1 - phSeg1 - phSeg2;
                if((phSeg1>=2 && phSeg1<=8) && (prSeg>=1 && prSeg<=8) && (phSeg2>=_SJW+1 && phSeg2<=8) )
                {
                    break;
                }
            }
        }
    }

    uint8_t confReg1 = (((_SJW) << 6) | mcpBRP);
    uint8_t confReg2 = ((_BLTMODE << 7) | (_SAMP << 6) | ((phSeg1-1) << 3) | (prSeg-1));
    uint8_t confReg3 = ((_SOF << 7) | (_WAKFIL << 6) | (phSeg2-1));

    if(_DEBUG_CONFIG)
    {
        syslog(LOG_INFO,"RPi Board revision : %d",piBoardRev());
        syslog(LOG_INFO,"Micro Chip BRP : %d",mcpBRP);
        syslog(LOG_INFO,"Real BRP : %d",2*(mcpBRP+1));
        syslog(LOG_INFO,"Number of TQ : %d",nTQ);
        syslog(LOG_INFO,"Sample Point : %d (%d %%)",sPt,100*sPt/nTQ);
        syslog(LOG_INFO,"Propagation Segment : %d",prSeg);
        syslog(LOG_INFO,"Phase Segment 1 : %d",phSeg1);
        syslog(LOG_INFO,"Phase Segment 2 : %d",phSeg2);
        syslog(LOG_INFO,"SJW : %d",_SJW+1);
        syslog(LOG_INFO,"Latency Delay %d", MCP2515_LDELAY);
        syslog(LOG_INFO,"Number Of Retry %d", MCP2515_MRETRY);
    }

    if(!Write(CNF1, confReg1))
    {
        syslog(LOG_ERR, "Writing CNF1 Fail");
        return false;
    }
    else if(_DEBUG_CONFIG)
    {
        syslog(LOG_INFO,"CNF1 OK : 0x%x",Read(CNF1));
    }

    if(!Write(CNF2, confReg2))
    {
        syslog(LOG_ERR, "Writing CNF2 Fail");
        return false;
    }
    else if(_DEBUG_CONFIG)
    {
        syslog(LOG_INFO,"CNF2 OK : 0x%x",Read(CNF2));
    }


    if(!Write(CNF3, confReg3))
    {
        syslog(LOG_ERR, "Writing CNF2 Fail");
        return false;
    }
    else if(_DEBUG_CONFIG)
    {
        syslog(LOG_INFO,"CNF3 OK : 0x%x",Read(CNF3));
    }


    if(!BitModify(TXRTSCTRL, 0b00000111, (_B2RTSM<<2) | (_B1RTSM<<1) | _B0RTSM))
    {
        syslog(LOG_ERR, "Writing TXRTSCTRL Fail");
        return false;
    }
    else if(_DEBUG_CONFIG)
    {
        syslog(LOG_INFO,"TXRTSCTRL OK : 0x%x",Read(TXRTSCTRL));
    }

    if(!BitModify(RXB0CTRL,0b01100100,(_RXM0 << 5) | (_BUKT<<2)))
    {
        syslog(LOG_ERR, "Writing RXB0CTRL Fail");
        return false;
    }
    else if(_DEBUG_CONFIG)
    {
        syslog(LOG_INFO,"RXB0CTRL OK : 0x%x",Read(RXB0CTRL));
    }

    if(!BitModify(RXB1CTRL,0b01100000,(_RXM1 << 5)))
    {
        syslog(LOG_ERR, "Writing RXB1CTRL Fail");
        return false;
    }
    else if(_DEBUG_CONFIG)
    {
        syslog(LOG_INFO,"RXB1CTRL OK : 0x%x",Read(RXB1CTRL));
    }

    if(!BitModify(BFPCTRL, 0b00001111, (_B1BFE<<3) | (_B0BFE<<2) | (_B1BFM<<1) | _B0BFM))
    {
        syslog(LOG_ERR, "Writing BFPCTRL Fail");
        return false;
    }
    else if(_DEBUG_CONFIG)
    {
        syslog(LOG_INFO,"BFPCTRL OK : 0x%x",Read(BFPCTRL));
    }

    caninte=(_MERRE<<7) | (_WAKIE<<6) | (_ERRIE<<5) | (_TX2IE<<4) | (_TX1IE<<3) | (_TX0IE<<2) | (_RX1IE<<1) | _RX0IE;

    if(!Write(CANINTE, caninte))
    {
        syslog(LOG_ERR, "Writing CANINTE Fail");
        return false;
    }
    else if(_DEBUG_CONFIG)
    {
        syslog(LOG_INFO,"CANINTE OK : 0x%x",Read(CANINTE));
    }

    if(!Write(CANCTRL, (_REQOP<<5) | (_ABAT<<4) | (_OSM<<3) | (_CLKEN<<2) | _CLKPRE ))
    {
        syslog(LOG_ERR, "Writing CANCTRL Fail");
        return false;
    }
    else if(_DEBUG_CONFIG)
    {
        syslog(LOG_INFO,"CANCTRL OK : 0x%x",Read(CANCTRL));
    }

    if(!Write(CANINTF, (_MERRF<<7) | (_WAKIF<<6) | (_ERRIF<<5) | (_TX2IF<<4) | (_TX1IF<<3) | (_TX0IF<<2) | (_RX1IF<<1) | _RX0IF))
    {
        syslog(LOG_ERR, "Writing CANINTF Fail");
        return false;
    }
    else if(_DEBUG_CONFIG)
    {
        syslog(LOG_INFO,"CANINTF OK : 0x%x",Read(CANINTF));
    }

    if(_DEBUG_CONFIG)
    {
        syslog(LOG_INFO,"CANSTAT OK : 0x%x",Read(CANSTAT));
    }

    if(_DEBUG_CONFIG) syslog(LOG_INFO,"Setting masks and filters");
    uint8_t sidh;
    uint8_t sidl;
    uint8_t eid8;
    uint8_t eid0;
    Mode(MODE_CONFIG);
    // RXB0
    // Mask 0
    buildFilt(&_MASK0, &sidh, &sidl, &eid8, &eid0);
    Write(RXM0SIDH,sidh);
    Write(RXM0SIDL,sidl);
    Write(RXM0EID8,eid8);
    Write(RXM0EID0,eid0);
    // Filter 0
    buildFilt(&_FILT0, &sidh, &sidl, &eid8, &eid0);
    Write(RXF0SIDH,sidh);
    Write(RXF0SIDL,sidl);
    Write(RXF0EID8,eid8);
    Write(RXF0EID0,eid0);
    // Filter 1
    buildFilt(&_FILT1, &sidh, &sidl, &eid8, &eid0);
    Write(RXF1SIDH,sidh);
    Write(RXF1SIDL,sidl);
    Write(RXF1EID8,eid8);
    Write(RXF1EID0,eid0);
    // RXB1
    // Mask 1
    buildFilt(&_MASK1, &sidh, &sidl, &eid8, &eid0);
    Write(RXM1SIDH,sidh);
    Write(RXM1SIDL,sidl);
    Write(RXM1EID8,eid8);
    Write(RXM1EID0,eid0);
    // Filter 2
    buildFilt(&_FILT2, &sidh, &sidl, &eid8, &eid0);
    Write(RXF2SIDH,sidh);
    Write(RXF2SIDL,sidl);
    Write(RXF2EID8,eid8);
    Write(RXF2EID0,eid0);
    // Filter 3
    buildFilt(&_FILT3, &sidh, &sidl, &eid8, &eid0);
    Write(RXF3SIDH,sidh);
    Write(RXF3SIDL,sidl);
    Write(RXF3EID8,eid8);
    Write(RXF3EID0,eid0);
    // Filter 4
    buildFilt(&_FILT4, &sidh, &sidl, &eid8, &eid0);
    Write(RXF4SIDH,sidh);
    Write(RXF4SIDL,sidl);
    Write(RXF4EID8,eid8);
    Write(RXF4EID0,eid0);
    // Filter 5
    buildFilt(&_FILT5, &sidh, &sidl, &eid8, &eid0);
    Write(RXF5SIDH,sidh);
    Write(RXF5SIDL,sidl);
    Write(RXF5EID8,eid8);
    Write(RXF5EID0,eid0);

    switch(_REQOP)
    {
        case 0:
        syslog(LOG_INFO,"Starting Mode : Normal");
            Mode(MODE_NORMAL);
            break;
        case 1:
        syslog(LOG_INFO,"Starting Mode : Sleep");
            Mode(MODE_SLEEP);
            break;
        case 2:
        syslog(LOG_INFO,"Starting Mode : Loopback");
            Mode(MODE_LOOPBACK);
            break;
        case 3:
        syslog(LOG_INFO,"Starting Mode : Listen Only");
            Mode(MODE_LISTEN);
            break;
        case 4:
        syslog(LOG_INFO,"Starting Mode : Config");
            Mode(MODE_CONFIG);
            break;
        default:
        syslog(LOG_INFO,"Starting Default Mode : Normal");
            Mode(MODE_NORMAL);
    }

    return true;
}

bool MCP2515::Reset()
{
    uint8_t buff[1];
    buff[0] = CAN_RESET;

    if(wiringPiSPIDataRW (_CSPIN, buff, 1))
    {
        if(_DEBUG_RUN) syslog(LOG_INFO, "Reset OK");
        return true;
    }
    else
    {
        syslog(LOG_ERR, "Reset Fail");
        return false;
    }
}

bool MCP2515::Write(uint8_t address, uint8_t data)
{
    uint8_t buff[3];
    buff[0] = CAN_WRITE;
    buff[1] = address;
    buff[2] = data;

    if(wiringPiSPIDataRW (_CSPIN, buff, 3))
    {
        if(_DEBUG_RUN) syslog(LOG_INFO,"Write to %x OK",address);
        return true;
    }
    else
    {
        syslog(LOG_ERR,"Write to %x Fail",address);
        return false;
    }
}

bool MCP2515::BitModify(uint8_t address, uint8_t mask, uint8_t data)
{
    uint8_t buff[4];
    buff[0] = CAN_BIT_MODIFY;
    buff[1] = address;
    buff[2] = mask;
    buff[3] = data;

    if(wiringPiSPIDataRW (_CSPIN, buff, 4))
    {
        if(_DEBUG_RUN) syslog(LOG_INFO,"Modify %x OK",address);
        return true;
    }
    else
    {
        syslog(LOG_ERR,"Modify %x Fail",address);
        return false;
    }
}

bool MCP2515::Mode(uint8_t mode)
{
    /*
    mode can be one of the following:
    MODE_CONFIG
    MODE_LISTEN
    MODE_LOOPBACK
    MODE_SLEEP
    MODE_NORMAL
    */
    if(BitModify(CANCTRL, 0b11100000, mode << 5))
    {
        delay(10); // allow for any transmissions to complete

        if(((Read(CANSTAT) & 0b11100000) >> 5) == mode)
        {
            if(_DEBUG_RUN) syslog(LOG_INFO,"Change Mode to %x OK",mode);
            return true;
        }
        else
        {
            syslog(LOG_ERR,"Change Mode to %x Fail",mode);
            return false;
        }
    }
    else
    {
        syslog(LOG_ERR,"Modify Mode to %x Fail",mode);
        return false;
    }
}

uint8_t MCP2515::Read(uint8_t address)
{
    uint8_t buff[3];
    buff[0] = CAN_READ;
    buff[1] = address;
    buff[2] = 0;

    if(wiringPiSPIDataRW (_CSPIN, buff, 3))
    {
        if(_DEBUG_RUN) syslog(LOG_INFO,"Read from %x OK",address);
        return buff[2];
    }
    else
    {
        syslog(LOG_WARNING,"Nothing to Read at %x OK",address);
        return 0;
    }
}

Frame MCP2515::ReadBuffer(uint8_t rxBuffer)     // rxBuffer : RXB0 / RXB1
{
    Frame message;
    uint8_t buff[14];
    buff[0] = (CAN_READ_BUFFER | (rxBuffer<<1));

    /*
    buff[1] : RXBnSIDH
    buff[2] : RXBnSIDL
    buff[3] : RXBnEID8
    buff[4] : RXBnEID0
    buff[5] : RXBnDLC
    buff[6]->buff[13] : RXBnDm
    */

    for(int i = 1; i < 14; i++)
    {
        buff[i] = 0;
    }

    wiringPiSPIDataRW(_CSPIN, buff, 14);

    message.id = (buff[1]>>3);
    message.id = (message.id<<8) | ((buff[1]<<5) | ((buff[2]>>5)<<2) | (buff[2] & 0b00000011));
    message.id = (message.id<<8) | buff[3];
    message.id = (message.id<<8) | buff[4];
    message.dlc = (buff[5] & 0b00001111);

    for(int i = 0; i < message.dlc; i++)
    {
        message.data[i] = buff[i+6];
    }

    switch(rxBuffer)
    {
        case RXB0:
        message.filt = Read(RXB0CTRL) & 0b00000001;
        break;
        case RXB1:
        message.filt = Read(RXB1CTRL) & 0b00000111;
        break;
    }

    if(_DEBUG_RUN) syslog(LOG_INFO,"Read Buffer %x OK",rxBuffer);

    return message;
}

bool MCP2515::LoadBuffer(uint8_t txBuffer, Frame message)   // txBuffer : TXB0 / TXB1 / TXB2
{
    if(txBuffer == TXB0) txBuffer = 0;
    uint8_t buff[14];

    /*
    buff[1] : TXBnSIDH
    buff[2] : TXBnSIDL
    buff[3] : TXBnEID8
    buff[4] : TXBnEID0
    buff[5] : TXBnDLC
    buff[6]->buff[13] : TXBnDm
    */

    buff[0] = (CAN_LOAD_BUFFER | txBuffer);
    buff[1] = uint8_t((message.id<<3)>>24);                   // 8 MSBits of SID
    buff[2] = uint8_t(((message.id<<11)>>24) & 0b11100000);   // 3 LSBits of SID
    buff[2] = buff[2] | uint8_t((message.id<<14)>>30);        // 2 MSBits of EI
    buff[2] = buff[2] | 0b00001000;                           // EXIDE
    buff[3] = uint8_t((message.id<<16)>>24);                  // EID Bits 15-8
    buff[4] = uint8_t((message.id<<24)>>24);                  // EID Bits 7-0
    buff[5] = message.dlc;

    for(int i = 6; i < message.dlc+6; i++)
    {
        buff[i] = message.data[i-6];
    }

    if(wiringPiSPIDataRW(_CSPIN, buff, message.dlc+6))
    {
        if(_DEBUG_RUN) syslog(LOG_INFO,"Load Buffer %x OK",txBuffer);
        return true;
    }
    else
    {
        syslog(LOG_ERR,"Load Buffer %x Fail",txBuffer);
        return false;
    }
}

bool MCP2515::SendBuffer(uint8_t txBuffer)     // txBuffer : any combination ( | ) of TXB0, TXB1, TXB2 or together : TXB_ALL
{
    uint8_t buff[1];
    buff[0] = CAN_RTS | txBuffer;

    if(wiringPiSPIDataRW(_CSPIN, buff, 1))
    {
        if(_DEBUG_RUN) syslog(LOG_INFO,"Send Buffer %x OK",txBuffer);
        return true;
    }
    else
    {
        syslog(LOG_ERR,"Send Buffer %x Fail",txBuffer);
        return false;
    }
}
