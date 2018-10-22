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
namespace UI {
  char inputBuffer[32];
  bool stringEnd = false;

  void sendCommand(const char* cmd);
  void readCommand();
  void setValue(const char *name, uint32_t number);
  void setText(const char *name, const char *buffer);

  const char* printerKilledObj = "sPrintedKilled";
  const char* messageObj = "sMessage";    

  void onStartup() {        
    NXTSERIAL.begin(9600);
    sendCommand("");
    sendCommand("bkcmd=1");    
    sendCommand("page 0");
  }
  void onIdle() { readCommand(); }
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

  /*
  * Send command to Nextion.
  *
  * @param cmd - the string of command.
  */
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

  void readCommand() {
    volatile char serialChar;
    volatile uint8_t stringLength = 0;
    stringEnd = false;
    memset(inputBuffer, 0, 32);
    while (NXTSERIAL.available()) {
      serialChar = (char)NXTSERIAL.read();

      // have we finished
      if (serialChar == '\n' || serialChar == '\r') {
          inputBuffer[stringLength] = 0;
          enqueue_and_echo_commands_P(inputBuffer);
          stringEnd = true;
      }
      else {
        stringEnd = false;

        inputBuffer[stringLength] = serialChar;
        stringLength++;
      }
    }
	}

  void setValue(const char *name, uint32_t number)
  {
      char buf[10] = {0};
      String cmd;
      
      utoa(number, buf, 10);
      cmd += name;
      cmd += ".val=";
      cmd += buf;

      sendCommand(cmd.c_str());
  }

  void setText(const char *name, const char *buffer)
  {
      String cmd;
      cmd += name;
      cmd += ".txt=\"";
      cmd += buffer;
      cmd += "\"";
      sendCommand(cmd.c_str());      
  }
}
#endif // EXTENSIBLE_UI
