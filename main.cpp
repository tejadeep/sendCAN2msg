#include <iostream>
#include <string.h>
#include <syslog.h>
#include <signal.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <wiringPiI2C.h>
#include "MCP2515.h"
#include "readIni.h"
#include "sllist.h"
#include "canfoncts.h"

using namespace std;

MCP2515* myCAN=nullptr;

sig_atomic_t volatile siguser1=0;
sig_atomic_t volatile siguser2=0;

sig_atomic_t volatile rx0BuffFull=0;
sig_atomic_t volatile rx1BuffFull=0;

Frame lire0;
Frame lire1;
Frame* plire0=&lire0;
Frame* plire1=&lire1;

bool DEBUG_CONFIG;
bool DEBUG_RUN;
bool DEBUG_INMSG;
bool DEBUG_OUTMSG;
uint16_t MCP2515_LDELAY;
uint8_t MCP2515_MRETRY;
int CSPIN;
char readIniBuf[40];
char iniFile[20];
uint8_t startMM;
uint8_t caninte;

int main(int argc, char *argv[])
{
    char* logIdent;
    logIdent=basename(argv[0]);

    openlog(logIdent, LOG_PID, LOG_DAEMON);

    struct sigaction signalAction;
    signalAction.sa_handler=&signalHandler;
    sigaction(SIGHUP,&signalAction,NULL);
    sigaction(SIGINT,&signalAction,NULL);
    sigaction(SIGQUIT,&signalAction,NULL);
    sigaction(SIGTERM,&signalAction,NULL);
    sigaction(SIGUSR1,&signalAction,NULL);
    sigaction(SIGUSR2,&signalAction,NULL);

    if(argc>1)
    {
        strcpy(iniFile,argv[1]);
    }
    else
    {
        strcpy(iniFile,"canBus.ini");
    }

    syslog(LOG_INFO,"%s starting with %s config file",logIdent,iniFile);

    syslog(LOG_INFO, "canBus initializing");

    ini_gets("DEBUG","CONFIG","0",readIniBuf,20,iniFile);
    DEBUG_CONFIG=bool(strtol(readIniBuf,NULL,2)&1);
    ini_gets("DEBUG","RUN","0",readIniBuf,20,iniFile);
    DEBUG_RUN=bool(strtol(readIniBuf,NULL,2)&1);
    ini_gets("DEBUG","INMSG","0",readIniBuf,20,iniFile);
    DEBUG_INMSG=bool(strtol(readIniBuf,NULL,10)&1);
    ini_gets("DEBUG","OUTMSG","0",readIniBuf,20,iniFile);
    DEBUG_OUTMSG=bool(strtol(readIniBuf,NULL,10)&1);

    ini_gets("MCP2515","CSPin","0",readIniBuf,20,iniFile);
    CSPIN=int(strtol(readIniBuf,NULL,10));
    ini_gets("MCP2515","IntPin","0",readIniBuf,20,iniFile);
    uint8_t Int_Pin=uint8_t(strtol(readIniBuf,NULL,10));
    ini_gets("MCP2515","IntLed","0",readIniBuf,20,iniFile);
    uint8_t Int_Led=uint8_t(strtol(readIniBuf,NULL,10));
    uint8_t Led_Pin = Int_Led;
    ini_gets("MCP2515","fSPI","0",readIniBuf,20,iniFile);
    uint32_t MCP2515_fSPI=uint8_t(strtol(readIniBuf,NULL,10))*1000000;

    wiringPiSetup();

    wiringPiISR(Int_Pin, INT_EDGE_FALLING, &MCP2515Int );

    if(wiringPiSPISetup (CSPIN, MCP2515_fSPI))
    {
        if(DEBUG_CONFIG)
            syslog(LOG_INFO,"SPI Init OK - Speed : %d MHz - CS pin : %d",MCP2515_fSPI/1000000, CSPIN);
    }
    else
    {
        syslog(LOG_ERR, "SPI Init Fail");
        return 1;
    }

    myCAN = new MCP2515();

    if(myCAN->Init())
    {
        if(DEBUG_CONFIG)
        {
            syslog(LOG_INFO,"CAN Init OK");
        }
    }
    else
    {
        syslog(LOG_ERR,"CAN Init Fail");
        return 2;
    }

    Frame ecrire0;
    Frame ecrire1;
    uint32_t timer01=millis();
    uint32_t timer02=timer01+200;

    uint8_t prio0 = 5;
    uint32_t pgn0 = 127488;     /** Engine parameters rapid update */
    uint8_t source0 = 28;
    uint32_t id0 = (prio0<<26) + (pgn0<<8) + source0;
    ecrire0.id = id0;
    ecrire0.dlc = 8;

    uint8_t prio1 = 6;
    uint32_t pgn1 = 127489;     /** Engine parameters dynamic */
    uint8_t source1 = 29;
    uint32_t id1 = (prio1<<26) + (pgn1<<8) + source1;
    ecrire1.id = id1;

    /**
    *   For rapid update
    *   Variables declaration
    */
    uint16_t erpm;
    uint16_t ebp;
    uint8_t ett;

    /**
    *   For dynamic
    *   Variables declaration
    */
    uint16_t eop;
    uint16_t eot;
    uint16_t ect;
    uint16_t eap;
    uint16_t efr;
    uint32_t ehr;
    uint16_t ecp;
    uint16_t efp;
    uint8_t elo;
    uint8_t eto;


    while(1)
    {
        if(millis()-timer01>=200)
        {
            /** Put your stuff here */
            erpm = 1876.75*4;   /** 1876.75 RPM     Engine Round Per 4 minutes (resolution 0.25 RPM) */
            ebp = 1.543*1000;   /** 1.54 B          Engine Boost presure in HPa*/
            ett = 12;           /** 12 %            Engine trim / tilt in %. Attn ! Signed value */

            ecrire0.data[0] = 0;                                /** Instance */
            ecrire0.data[1] = erpm & 0b0000000011111111;        /** Less significant part */
            ecrire0.data[2] = (erpm & 0b1111111100000000)>>8;   /** Most significant part */
            ecrire0.data[3] = ebp & 0b0000000011111111;
            ecrire0.data[4] = (ebp & 0b1111111100000000)>>8;
            ecrire0.data[5] = ett;
            ecrire0.data[6] = 255;                              /** Reserved */
            ecrire0.data[7] = 255;
            /** End of your stuff */

            digitalWrite(Led_Pin, HIGH); // Firing Trigger for SPI bus debugging purpose

            if(sendSingleMsg(myCAN, &ecrire0))
            {
                printf("%d - Message Engine parameters rapid update sent\n\r",millis());
            }
            else
            {
                printf("%d - Error sending message Engine parameters rapid update\n\r",millis());
            }

            digitalWrite(Led_Pin, LOW); // Reseting Trigger
            timer01=millis();
        }

        if(millis()-timer02>=1000)
        {
            struct sllist* myidx = sllist_create(); /** Creating index of elements */
            struct maillon* mydata=nullptr;         /** Declaring an element structure */

            /** Put your stuff here
             *
             * Remember ...
             * 1 Bar (B) = 1000 Hecto Pascal (HPa)
             *           =  100  Kilo Pascal (KPa)
             * 0 Celsius (C) = 273.15 Kelvin (K)
             */
            eop=34.567*1000;        /** 34.567 B    Oil presure                 HPa     */
            eot=(95.2+273.15)*10;   /** 95.2 °C     Oil temp                 0.1 K      */
            ect=(90.34+273.15)*100; /** 90.34 °C    Coolant temp            0.01 K      */
            eap=12.34*100;          /** 12.34 V     Alternator potential    0.01 V      */
            efr=56.7*10;            /** 56.7 L/H    Fuel rate                0.1 L/H    */
            ehr=10.45*3600;         /** 10.45 H     Engine hours                 s      */
            ecp=2.345*1000;         /** 2.345 B     Coolant presure             HPa     */
            efp=3.45*100;           /** 3.45 B      Fuel presure                KPa     */
            elo=56;                 /** %           Engine load %               %       */
            eto=78;                 /** %           Engine torque %             %       */

            addByte(mydata, myidx, 0);                              /** Engine instance */
            addByte(mydata, myidx, eop & 0b0000000011111111);
            addByte(mydata, myidx, (eop & 0b1111111100000000)>>8);
            addByte(mydata, myidx, eot & 0b0000000011111111);
            addByte(mydata, myidx, (eot & 0b1111111100000000)>>8);
            addByte(mydata, myidx, ect & 0b0000000011111111);
            addByte(mydata, myidx, (ect & 0b1111111100000000)>>8);
            addByte(mydata, myidx, eap & 0b0000000011111111);
            addByte(mydata, myidx, (eap & 0b1111111100000000)>>8);
            addByte(mydata, myidx, efr & 0b0000000011111111);
            addByte(mydata, myidx, (efr & 0b1111111100000000)>>8);
            addByte(mydata, myidx,  ehr & 0b00000000000000000000000011111111);
            addByte(mydata, myidx, (ehr & 0b00000000000000001111111100000000)>>8);
            addByte(mydata, myidx, (ehr & 0b00000000111111110000000000000000)>>16);
            addByte(mydata, myidx, (ehr & 0b11111110000000000000000000000000)>>24);
            addByte(mydata, myidx, ecp & 0b0000000011111111);
            addByte(mydata, myidx, (ecp & 0b1111111100000000)>>8);
            addByte(mydata, myidx, efp & 0b0000000011111111);
            addByte(mydata, myidx, (efp & 0b1111111100000000)>>8);
            addByte(mydata, myidx, 0);                              /** Reserved */
            addByte(mydata, myidx, 0);                              /** Discret status 1 ??? */
            addByte(mydata, myidx, 0);
            addByte(mydata, myidx, 0);                              /** Discret status 2 ??? */
            addByte(mydata, myidx, 0);
            addByte(mydata, myidx, elo);
            addByte(mydata, myidx, eto);
            /** End of your stuff */
            /**********************/

            uint8_t nbBytes=myidx->size;

            if(sendMultiMsg(myCAN, myidx, nbBytes, &ecrire1))       /** Building CAN message */
            {
                printf("%d - Message Engine parameters dynamic sent\n\r",millis());
            }
            else
            {
                printf("%d - Error sending message Engine parameters dynamic\n\r",millis());
            }

            timer02=millis();
            sllist_destroy(myidx);                                  /** Clearing index and elements */
            free(mydata);                                           /** Not sure but secure */
        }

        if(rx0BuffFull)
        {
            rx0BuffFull=0;

            if(DEBUG_INMSG) logRecvMsg(0, &lire0);

            /**
             *  Put your stuff here
             *  For example :
             */
            dispSingleMsg(0,&lire0);
            /** End of your stuff **/
        }

        if(rx1BuffFull)
        {
            rx1BuffFull=0;

            if(DEBUG_INMSG) logRecvMsg(1, &lire1);

            /**
             *  Put your stuff here
             *  For example :
             */
            dispSingleMsg(1,&lire1);
            /** End of your stuff **/
        }
    }

    closelog();

    return 0;
}
