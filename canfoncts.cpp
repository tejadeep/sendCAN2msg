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
    uint8_t canstat = myCAN->Read(CANSTAT);
    uint8_t icod = ( canstat & 0b00001110) >> 1;
    uint8_t eflg = myCAN->Read(EFLG);
    uint8_t tec = myCAN->Read(TEC);
    uint8_t rec = myCAN->Read(REC);

    switch (icod)
    {
    case 1:
        syslog(LOG_WARNING,"Error. EFLG register : 0x%x",eflg);

        if ( eflg & 0b10000000 )
        {
            syslog(LOG_WARNING,"Receive Buffer 1 Overflow");
        }

        if ( eflg & 0b01000000 )
        {
            syslog(LOG_WARNING,"Receive Buffer 0 Overflow");
        }

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

    case 6:
        rx0BuffFull=1;
        break;

    case 7:
        rx1BuffFull=1;
        break;

    default:
        syslog(LOG_ERR,"Unknown Error !!!");
    }
}

/**
 *
 * Load message into free buffer and request to send it
 *
 */
bool sendMsg(MCP2515* objCAN, Frame* msg)
{

    if((objCAN->Read(CANINTF) & TX0IF)>>2)
    {
        if(objCAN->LoadBuffer(TXB0, *msg))
        {
            if(objCAN->BitModify(CANINTF, TX0IF, 0))
            {
                if(objCAN->SendBuffer(TXB0))
                {
                    if(DEBUG_OUTMSG)
                    {
                        char tmpbuff[10];
                        char syslogMsg[200];
                        sprintf(syslogMsg, "TX0 Buffer send Msg ID %lu DLC %u DATA ", msg->id, msg->dlc);

                        for(int i=0; i<msg->dlc; i++)
                        {
                            sprintf(tmpbuff,"0x%x ",msg->data[i]);
                            strcat(syslogMsg, tmpbuff);
                        }

                        syslog(LOG_INFO,syslogMsg);
                    }

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
                syslog(LOG_ERR,"Unable to reset TX0 interrupt bit");
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
        if(objCAN->LoadBuffer(TXB1, *msg))
        {
            if(objCAN->BitModify(CANINTF, TX1IF, 0))
            {
                if(objCAN->SendBuffer(TXB1))
                {
                    if(DEBUG_OUTMSG)
                    {
                        char tmpbuff[10];
                        char syslogMsg[200];
                        sprintf(syslogMsg, "TX1 Buffer send Msg ID %lu DLC %u DATA ", msg->id, msg->dlc);

                        for(int i=0; i<msg->dlc; i++)
                        {
                            sprintf(tmpbuff,"0x%x ",msg->data[i]);
                            strcat(syslogMsg, tmpbuff);
                        }

                        syslog(LOG_INFO,syslogMsg);

                    }
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
                syslog(LOG_ERR,"Unable to reset TX1 interrupt bit");
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
        if(objCAN->LoadBuffer(TXB2, *msg))
        {
            if(objCAN->BitModify(CANINTF, TX2IF, 0))
            {
                if(objCAN->SendBuffer(TXB2))
                {
                    if(DEBUG_OUTMSG)
                    {
                        char tmpbuff[10];
                        char syslogMsg[200];
                        sprintf(syslogMsg, "TX2 Buffer send Msg ID %lu DLC %u DATA ", msg->id, msg->dlc);

                        for(int i=0; i<msg->dlc; i++)
                        {
                            sprintf(tmpbuff,"0x%x ",msg->data[i]);
                            strcat(syslogMsg, tmpbuff);
                        }

                        syslog(LOG_INFO,syslogMsg);
                    }
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
                syslog(LOG_ERR,"Unable to reset TX2 interrupt bit");
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
        return false;   /** All TXn buffers full **/
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
    printf("Received data in buffer %d\n\r",rxBuff);
    printf("ID : %ld\n\r",oneMsg->id);
    printf("DLC : %d\n\r",oneMsg->dlc);
    printf("Data");
    for(int i=0;i<oneMsg->dlc;i++)
    {
        printf(" : %d",oneMsg->data[i]);
    }
    printf("\n\rFilter : %d\n\r",oneMsg->filt);
}
