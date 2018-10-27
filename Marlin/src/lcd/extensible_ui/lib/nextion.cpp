/*************
 * dummy.cpp *
 *************/

/****************************************************************************
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/
#include "../../../inc/MarlinConfigPre.h"
#include "../ui_api.h"
#include "../../../gcode/queue.h"

#if ENABLED(EXTENSIBLE_UI) && ENABLED(EXTENSIBLE_UI_NEXTION)
#include "nextion/Nextion.h"

#define NEX_RET_CMD_FINISHED            (0x01)
namespace UI {
  void sendCommand(const char* cmd);
  void readCommand();
  void setValue(const char *name, uint32_t number);
  void setText(const char *name, const char *buffer);
  bool recvRetCommandFinished(uint32_t timeout);

  const char* printerKilledObj = "sPrintedKilled";
  const char* messageObj = "sMessage";    
  float _feedRate=0;
  bool _connected=false;

  NexTouch *nex_listen_list[] = 
  {
//      &t0,
  //    &b0,
    //  &b1,
      NULL
  };

  void onStartup() {            
    NXTSERIAL.begin(9600);
    _connected=true;
    sendCommand("");
    sendCommand("bkcmd=0");    
  }
  void onIdle() { 
      nexLoop(nex_listen_list);
      /*
      if(_connected)
      {
        readCommand();

        if(_feedRate != getFeedRate_percent())
        {
          _feedRate = getFeedRate_percent();
          setText("fr", itostr3(_feedRate));       
        }
      }         */
    }
  void onPrinterKilled(const char* msg) {}
  void onMediaInserted(){};
  void onMediaError(){};
  void onMediaRemoved(){};
  void onPlayTone(const uint16_t frequency, const uint16_t duration) {}
  void onPrintTimerStarted() {}
  void onPrintTimerPaused() {}
  void onPrintTimerStopped() {}
  void onFilamentRunout() {}
  void onStatusChanged(const char* msg) {}
  void onStatusChanged(progmem_str msg) {}
  void onFactoryReset() {}
  void onLoadSettings() {}
  void onStoreSettings() {}

  void sendCommand(const char* cmd)
  {
      while (NXTSERIAL.available())
      {
          NXTSERIAL.read();
      }
      
      NXTSERIAL.print(cmd);
      NXTSERIAL.write(0xFF);
      NXTSERIAL.write(0xFF);
      NXTSERIAL.write(0xFF);    
  }

  static char inputBuffer[32];
  static int stringLength = 0;

  void readCommand() {  
    char serialChar;    

    while (NXTSERIAL.available()) {
      serialChar = (char)NXTSERIAL.read();      
    
      // have we finished
      if (serialChar == '\r') {
          enqueue_and_echo_command(inputBuffer);               
          inputBuffer[0] = '\0';
          stringLength = 0;
      }
      else if(stringLength == 0 && serialChar != 'G' && serialChar != 'M')
      {
        continue;
      }
      else {
        inputBuffer[stringLength] = serialChar;
        inputBuffer[stringLength+1] = '\0';
        stringLength += 1;
      }
    }
	}

  void setText(const char *name, const char *buffer)
  {
      NXTSERIAL.print(name);
      NXTSERIAL.print(".txt=\"");
      NXTSERIAL.print(buffer);
      NXTSERIAL.print("\"");
      NXTSERIAL.write(0xFF);
      NXTSERIAL.write(0xFF);
      NXTSERIAL.write(0xFF);
  }

  bool recvRetCommandFinished(uint32_t timeout)
  {    
    bool ret = false;
    uint8_t temp[4] = {0};
    
    NXTSERIAL.setTimeout(timeout);
    if (sizeof(temp) != NXTSERIAL.readBytes((char *)temp, sizeof(temp)))
    {
        ret = false;
    }

    if (temp[0] == NEX_RET_CMD_FINISHED
        && temp[1] == 0xFF
        && temp[2] == 0xFF
        && temp[3] == 0xFF
        )
    {
        ret = true;
    }    
    
    return ret;
  }
}
#endif // EXTENSIBLE_UI
