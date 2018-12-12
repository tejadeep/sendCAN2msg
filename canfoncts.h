/**
 *
 * canfoncts.h
 *
 * Headers for canfoncts.cpp
 *
 */
#ifndef CANFONCTS_H_INCLUDED
#define CANFONCTS_H_INCLUDED

#include <iostream>
#include <syslog.h>
#include <string.h>
#include <wiringPi.h>
#include <signal.h>
#include "sllist.h"
#include "MCP2515.h"

struct maillon
{
    int vala;
};

extern sig_atomic_t volatile siguser1;
extern sig_atomic_t volatile siguser2;

extern sig_atomic_t volatile rx0BuffFull;
extern sig_atomic_t volatile rx1BuffFull;

extern uint8_t startMM;
extern uint8_t caninte;

extern Frame* plire0;
extern Frame* plire1;

extern MCP2515* myCAN;

bool sendMsg(MCP2515* objCAN, Frame* oneMsg);

bool sendSingleMsg(MCP2515* objCAN, Frame* oneMsg);

bool sendMultiMsg(MCP2515* objCAN, struct sllist* oneIdx, uint8_t oneSize, Frame* oneMsg);

void dispSingleMsg(uint8_t rxBuff, Frame* oneMsg);

void logRecvMsg(uint8_t rxBuff, Frame* oneMsg);

void logSentMsg(uint8_t txBuff, Frame* oneMsg);

void MCP2515Int(void);

void buildFilt(uint32_t* pFilt, uint8_t* sidh, uint8_t* sidl, uint8_t* eid8, uint8_t* eid0);

void signalHandler(int sigNum);

bool addByte(struct maillon* onedata, struct sllist* oneIdx, uint8_t oneByte);

#endif // CANFONCTS_H_INCLUDED
