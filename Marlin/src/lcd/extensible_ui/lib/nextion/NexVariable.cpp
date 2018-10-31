/**
 * @file NexVariable.cpp
 *
 * The implementation of class NexText. 
 *
 * @author  huang xiaoming (email:<xiaoming.huang@itead.cc>)
 * @date    2016/9/13
 * @copyright 
 * Copyright (C) 2014-2015 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */
#include "NexVariable.h"

NexVariable::NexVariable(uint8_t pid, uint8_t cid, const char *name)
    :NexTouch(pid, cid, name)
{
}

uint32_t NexVariable::getValue(uint32_t *number)
{    
    int cmdLen = 9 + strlen(getObjName());
    char cmd[cmdLen];

    strcpy(cmd, "get ");
    strcpy(cmd, getObjName());
    strcat(cmd, ".val");
    sendCommand(cmd);

    return recvRetNumber(number);
}

bool NexVariable::setValue(uint32_t number)
{
    char buf[10] = {0};
    utoa(number, buf, 10);

    int cmdLen = 6 + strlen(getObjName()) + strlen(buf);
    char cmd[cmdLen];
    strcat(cmd, getObjName());
    strcat(cmd, ".val=");
    strcat(cmd, buf);
    sendCommand(cmd);
    return recvRetCommandFinished();
}

uint32_t NexVariable::getText(char *buffer, uint32_t len)
{
    int cmdLen = 12 + strlen(getObjName()) + strlen(buffer);
    char cmd[cmdLen];

    strcpy(cmd, "get ");
    strcat(cmd, getObjName());
    strcat(cmd, ".txt=\"");
    strcat(cmd, buffer);
    strcat(cmd, "\"");
    sendCommand(cmd);

    return recvRetString(buffer,len);
}

bool NexVariable::setText(const char *buffer)
{
    int cmdLen = 8 + strlen(getObjName()) + strlen(buffer);
    char cmd[cmdLen];

    strcpy(cmd, getObjName());
    strcat(cmd, ".txt=\"");
    strcat(cmd, buffer);
    strcat(cmd, "\"");
    sendCommand(cmd);
    return recvRetCommandFinished();    
}

bool NexVariable::show(void) 
{
    int cmdLen = 7 + strlen(getObjName());
    char cmd[cmdLen];

    strcpy(cmd, "vis ");
    strcat(cmd, getObjName());
    strcat(cmd, ",1");
    
    sendCommand(cmd);

    return recvRetCommandFinished();
}


bool NexVariable::hide(void) 
{
    int cmdLen = 7 + strlen(getObjName());
    char cmd[cmdLen];

    strcpy(cmd, "vis ");
    strcat(cmd, getObjName());
    strcat(cmd, ",0");
    
    sendCommand(cmd);

    return recvRetCommandFinished();
}
