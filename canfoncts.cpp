/**
 *
 * canfoncts.cpp
 *
 * CAN Functions
 *
 */
#include "canfoncts.h"

/**
 *
 * System interrupt handler
 *
 */
void signalHandler(int sigNum)
{
    switch(sigNum)
    {
    case SIGUSR1:
        siguser1=1;
        syslog(LOG_WARNING, "Received signal SIGUSR1");
        break;
    case SIGUSR2:
        siguser2=1;
        syslog(LOG_WARNING, "Received signal SIGUSR2");
        break;
    default:
        syslog(LOG_WARNING, "Exiting on signal %d", sigNum);
        exit(sigNum);
    }
}

/**
 *
 * Pin interrupt handler
 *
 */
void MCP2515Int(void)
{
    uint8_t canintf = myCAN->Read(CANINTF);

    if((canintf&0b11)==3)
    {
        *plire0 = myCAN->ReadBuffer(RXB0);
        rx0BuffFull=1;
        *plire1 = myCAN->ReadBuffer(RXB1);
        rx1BuffFull=1;
    }

    if(canintf&0b01)
    {
        *plire0 = myCAN->ReadBuffer(RXB0);
        rx0BuffFull=1;
    }

    if(canintf&0b10)
    {
        *plire1 = myCAN->ReadBuffer(RXB1);
        rx1BuffFull=1;
    }

    if( (canintf & 0b11111100) & caninte )
    {
        uint8_t canstat = myCAN->Read(CANSTAT);
        uint8_t icod = ( canstat & 0b00001110) >> 1;
        uint8_t eflg;
        uint8_t tec;
        uint8_t rec;

        switch (icod)
        {
        case 1:
            eflg = myCAN->Read(EFLG);
            syslog(LOG_WARNING,"Error. EFLG register : 0x%x",eflg);

            if ( eflg & 0b10000000 )
            {
                syslog(LOG_WARNING,"Receive Buffer 1 Overflow");
            }

            if ( eflg & 0b01000000 )
            {
                syslog(LOG_WARNING,"Receive Buffer 0 Overflow");
            }

            if( eflg & 0b00111111 )
            {
                tec = myCAN->Read(TEC);
                rec = myCAN->Read(REC);

                if ( eflg & 0b00100000 )
                {
                    syslog(LOG_ERR,"Bus-Off Error. TEC or REC = 255 : TEC = %u REC = %u", tec, rec);
                }

                if ( eflg & 0b00010000 )
                {
                    syslog(LOG_WARNING,"Transmit Error-Passive. TEC >= 128 : %u",tec);
                }

                if ( eflg & 0b00001000 )
                {
                    syslog(LOG_WARNING,"Receive Error-Passive. REC >= 128 : %u",rec);
                }

                if ( eflg & 0b00000100 )
                {
                    syslog(LOG_WARNING,"Transmit Error-Warning. TEC >= 96 : %u",tec);
                }

                if ( eflg & 0b00000010 )
                {
                    syslog(LOG_WARNING,"Receive Error-Warning. REC >= 96 : %u",rec);
                }

                if ( eflg & 0b00000001 )
                {
                    syslog(LOG_WARNING,"Error-Warning. TEC or REC >= 96. TEC = %u - REC = %u",tec, rec);
                }
            }
            break;

        case 2:
            if(DEBUG_RUN)
            {
                syslog(LOG_INFO,"Wake-up");
            }
            break;

        case 3:
            if(DEBUG_RUN)
            {
                syslog(LOG_INFO,"Transmit Buffer 0 Empty : message sent");
            }
            break;

        case 4:
            if(DEBUG_RUN)
            {
                syslog(LOG_INFO,"Transmit Buffer 1 Empty : message sent");
            }
            break;

        case 5:
            if(DEBUG_RUN)
            {
                syslog(LOG_INFO,"Transmit Buffer 2 Empty : message sent");
            }
            break;

        /*case 6:
            rx0BuffFull=1;
            *plire0 = myCAN->ReadBuffer(RXB0);
            break;

        case 7:
            rx1BuffFull=1;
            *plire1 = myCAN->ReadBuffer(RXB1);
            break;*/

        default:
            syslog(LOG_ERR,"Unknown Error code CANSTAT.ICOD %u", icod);
        }
    }
}

/**
 *
 * Load message into free buffer and request to send it
 *
 */
bool sendMsg(MCP2515* objCAN, Frame* oneMsg)
{

    if((objCAN->Read(CANINTF) & TX0IF)>>2)
    {
        if(objCAN->LoadBuffer(TXB0, *oneMsg))
        {
            objCAN->BitModify(CANINTF, TX0IF, 0);

            if(objCAN->SendBuffer(TXB0))
            {
                if(DEBUG_OUTMSG)
                    logSentMsg(0, oneMsg);

                return true;
            }
            else
            {
                syslog(LOG_ERR,"Unable to send buffer TX0");
                return false;
            }

        }
        else
        {
            syslog(LOG_ERR,"Unable to load buffer TX0");
            return false;
        }
    }
    else if((objCAN->Read(CANINTF) & TX1IF)>>3)
    {
        if(objCAN->LoadBuffer(TXB1, *oneMsg))
        {
            objCAN->BitModify(CANINTF, TX1IF, 0);

            if(objCAN->SendBuffer(TXB1))
            {
                if(DEBUG_OUTMSG)
                    logSentMsg(1, oneMsg);

                return true;
            }
            else
            {
                syslog(LOG_ERR,"Unable to send buffer TX1");
                return false;
            }
        }
        else
        {
            syslog(LOG_ERR,"Unable to load buffer TX1");
            return false;
        }
    }
    else if((objCAN->Read(CANINTF) & TX2IF)>>4)
    {
        if(objCAN->LoadBuffer(TXB2, *oneMsg))
        {
            objCAN->BitModify(CANINTF, TX2IF, 0);

            if(objCAN->SendBuffer(TXB2))
            {
                if(DEBUG_OUTMSG)
                    logSentMsg(2, oneMsg);

                return true;
            }
            else
            {
                syslog(LOG_ERR,"Unable to send buffer TX2");
                return false;
            }
        }
        else
        {
            syslog(LOG_ERR,"Unable to load buffer TX2");
            return false;
        }
    }
    else
    {
        syslog(LOG_ERR, "All TXn Buffers Full");
        return false;
    }

    syslog(LOG_ERR,"Unable to send message. Unkown error ...");
    return false;
}

/**
 *
 * Build masks and filters
 *
 */
void buildFilt(uint32_t* filt, uint8_t* sidh, uint8_t* sidl, uint8_t* eid8, uint8_t* eid0)
{
    *sidh = uint8_t((*filt << 3)>>24);
    *sidl = uint8_t(((*filt << 11)>>24) & 0b11100000);
    *sidl = *sidl | uint8_t((*filt <<14)>>30);
    *sidl = *sidl | 0b00001000;
    *eid8 = uint8_t((*filt<<16)>>24);
    *eid0 = uint8_t((*filt<<24)>>24);
}

/**
 *
 * Send a single message
 *
 */
bool sendSingleMsg(MCP2515* objCAN, Frame* oneMsg)
{
    uint8_t retry=0;

    while(!sendMsg(objCAN, oneMsg))
    {
        if(DEBUG_OUTMSG)
            syslog(LOG_WARNING,"Retrying single message");

        delayMicroseconds(MCP2515_LDELAY);
        retry++;

        if(retry > MCP2515_MRETRY)
        {
            syslog(LOG_ERR,"Error sending single message. Retry exeeded");
            break;
        }
    }

    if(retry > MCP2515_MRETRY)
    {
        return false;
    }
    else
    {
        return true;
    }
}

/**
 *
 * Send a multi frame message
 *
 */
bool sendMultiMsg(MCP2515* objCAN, struct sllist* oneIdx, uint8_t oneSize, Frame* oneMsg)
{
    if(startMM>7)
        startMM=0;

    uint8_t myMsg[9];
    uint8_t reste=(oneSize+1)%7;
    uint8_t trames=0;
    uint8_t lastByte=0;

    if(reste==0)
    {
        trames=(oneSize+1)/7;
    }
    else
    {
        trames=(oneSize+1)/7+1;
    }

    for(int i=0; i<trames; i++)
    {
        myMsg[0]=8;
        myMsg[1]=i+32*startMM;

        if(i==0)
        {
            myMsg[2]=oneSize;

            for(int j=3; j<9; j++)
            {
                struct maillon* tmpidx = (struct maillon*)sllist_read_index(oneIdx, j-3);
                myMsg[j]=tmpidx->vala;
            }

            lastByte=5;
        }
        else
        {
            if(i==trames-1)
            {
                if(reste==0)
                {
                    myMsg[0]=8;
                }
                else
                {
                    myMsg[0]=reste+1;
                }
            }

            for(int j=2; j<9; j++)
            {
                lastByte++;

                if(lastByte<oneSize)
                {
                    struct maillon* tmpidx = (struct maillon*)sllist_read_index(oneIdx, lastByte);
                    myMsg[j]=tmpidx->vala;
                }
            }
        }

        oneMsg->dlc=myMsg[0];

        for(int i=0; i<8; i++)
        {
            oneMsg->data[i]=myMsg[i+1];
        }

        uint8_t retry=0;

        while(!sendMsg(objCAN, oneMsg))
        {
            if(DEBUG_OUTMSG)
                syslog(LOG_WARNING,"Retrying multi message");

            delayMicroseconds(MCP2515_LDELAY);
            retry++;

            if(retry > MCP2515_MRETRY)
            {
                syslog(LOG_ERR,"All TX buffers full. Error sending multi message");
                break;
            }
        }

        memset(myMsg,0,sizeof(myMsg));

        if(retry > MCP2515_MRETRY)
            return false;

    }

    startMM++;
    return true;
}

/**
 *
 * Append data byte to multi frame message
 *
 */
bool addByte(struct maillon* onedata, struct sllist* oneIdx, uint8_t oneByte)
{
    onedata = (struct maillon*)malloc(sizeof(maillon));
    onedata->vala=oneByte;

    if(sllist_push_back(oneIdx, onedata))
        return false;

    return true;
}

/**
 *
 * Display single message
 *
 */
void dispSingleMsg(uint8_t rxBuff, Frame* oneMsg)
{
    uint8_t prio=(oneMsg->id&(0b111<<26))>>26;
    uint32_t pgn=(oneMsg->id&(0b111111111111111111<<8))>>8;
    uint8_t src=oneMsg->id&0b11111111;
    uint8_t dlc=oneMsg->dlc;
    uint8_t dp=(pgn&(1<<16))>>16;
    uint8_t pduf=(pgn&(0b11111111<<8))>>8;
    uint8_t pdus=pgn&0b11111111;
    printf("RX%d prio %d PGN %d (dp %d pduf %d pdus %d) src %d DLC %d Data", rxBuff, prio, pgn, dp, pduf, pdus, src, dlc);

    for(int i=0; i<dlc; i++)
    {
        printf(" 0x%x",oneMsg->data[i]);
    }
    printf(" Filter %d\n\r",oneMsg->filt);
}

void logRecvMsg(uint8_t rxBuff, Frame* oneMsg)
{
    char tmpbuff[10];
    char syslogMsg[200];
    sprintf(syslogMsg, "RX%u full : filter %u Msg ID %lu DLC %u DATA ",rxBuff, oneMsg->filt,oneMsg->id, oneMsg->dlc);

    for(int i=0; i<oneMsg->dlc; i++)
    {
        sprintf(tmpbuff,"0x%x ",oneMsg->data[i]);
        strcat(syslogMsg, tmpbuff);
    }

    syslog(LOG_INFO,syslogMsg);
}

void logSentMsg(uint8_t txBuff, Frame* oneMsg)
{
    char tmpbuff[10];
    char syslogMsg[200];
    sprintf(syslogMsg, "TX%u Buffer send Msg ID %lu DLC %u DATA ", txBuff, oneMsg->id, oneMsg->dlc);

    for(int i=0; i<oneMsg->dlc; i++)
    {
        sprintf(tmpbuff,"0x%x ",oneMsg->data[i]);
        strcat(syslogMsg, tmpbuff);
    }

    syslog(LOG_INFO,syslogMsg);
}
