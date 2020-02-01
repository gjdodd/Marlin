/*********************
 * extui_example.cpp *
 *********************/

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

#include "../inc/MarlinConfigPre.h"

#if BOTH(EXTUI_NEXTION, EXTENSIBLE_UI)

#include "extensible_ui/ui_api.h"
#include "extensible_ui/lib/nextion/nextion.h"


// To implement a new UI, complete the functions below and
// read or update Marlin's state using the methods in the
// ExtUI methods in "../ui_api.h"
//
// Although it may be possible to access other state
// variables from Marlin, using the API here possibly
// helps ensure future compatibility.

namespace ExtUI {
  void t0PopCallback(void *ptr);
void b0PopCallback(void *ptr);
void b1PopCallback(void *ptr);

/*
 * Declare a text object [page id:0,component id:1, component name: "t0"]. 
 */
NexText t0 = NexText(0, 1, "t0");

/*
 * Declare a button object [page id:0,component id:2, component name: "b0"]. 
 */
NexButton b0 = NexButton(0, 2, "b0");

/*
 * Declare a button object [page id:0,component id:3, component name: "b1"]. 
 */
NexButton b1 = NexButton(0, 3, "b1");

char buffer[100] = {0};

/*
 * Register object t0, b0, b1, to the touch event list.  
 */
NexTouch *nex_listen_list[] = 
{
    &t0,
    &b0,
    &b1,
    NULL
};

/*
 * Text component pop callback function. 
 */
void t0PopCallback(void *ptr)
{
    dbSerialPrintln("t0PopCallback");
    t0.setText("50");
}

/*
 * Button0 component pop callback function.
 * In this example,the value of the text component will plus one every time when button0 is released.
 */
void b0PopCallback(void *ptr)
{
    //uint16_t len;
    uint16_t number;
    
    dbSerialPrintln("b0PopCallback");

    memset(buffer, 0, sizeof(buffer));
    t0.getText(buffer, sizeof(buffer));
    
    number = atoi(buffer);
    number += 1;

    memset(buffer, 0, sizeof(buffer));
    itoa(number, buffer, 10);
    
    t0.setText(buffer);
}

/*
 * Button1 component pop callback function.
 * In this example,the value of the text component will minus one every time when button1 is released.
 */
void b1PopCallback(void *ptr)
{
    //uint16_t len;
    uint16_t number;
    
    dbSerialPrintln("b1PopCallback");

    memset(buffer, 0, sizeof(buffer));
    t0.getText(buffer, sizeof(buffer));
    
    number = atoi(buffer);
    number -= 1;

    memset(buffer, 0, sizeof(buffer));
    itoa(number, buffer, 10);
    
    t0.setText(buffer);
}

void setup(void)
{
    /* Set the baudrate which is for debug and communicate with Nextion screen. */
    nexInit();

    /* Register the pop event callback function of the current text component. */
    t0.attachPop(t0PopCallback);

    /* Register the pop event callback function of the current button0 component. */
    b0.attachPop(b0PopCallback);

    /* Register the pop event callback function of the current button1 component. */
    b1.attachPop(b1PopCallback);

    dbSerialPrintln("setup done");
}

  void onStartup() {
    /* Initialize the display module here. The following
     * routines are available for access to the GPIO pins:
     *
     *   SET_OUTPUT(pin)
     *   SET_INPUT_PULLUP(pin)
     *   SET_INPUT(pin)
     *   WRITE(pin,value)
     *   READ(pin)
     */

     setup();
  }
  void onIdle() {
    nexLoop(nex_listen_list);
  }
  void onPrinterKilled(PGM_P const error, PGM_P const component) {}
  void onMediaInserted() {};
  void onMediaError() {};
  void onMediaRemoved() {};
  void onPlayTone(const uint16_t frequency, const uint16_t duration) {}
  void onPrintTimerStarted() {}
  void onPrintTimerPaused() {}
  void onPrintTimerStopped() {}
  void onFilamentRunout(const extruder_t extruder) {}
  void onUserConfirmRequired(const char * const msg) {}
  void onStatusChanged(const char * const msg) {}
  void onFactoryReset() {}

  void onStoreSettings(char *buff) {
    // Called when saving to EEPROM (i.e. M500). If the ExtUI needs
    // permanent data to be stored, it can write up to eeprom_data_size bytes
    // into buff.

    // Example:
    //  static_assert(sizeof(myDataStruct) <= ExtUI::eeprom_data_size);
    //  memcpy(buff, &myDataStruct, sizeof(myDataStruct));
  }

  void onLoadSettings(const char *buff) {
    // Called while loading settings from EEPROM. If the ExtUI
    // needs to retrieve data, it should copy up to eeprom_data_size bytes
    // from buff

    // Example:
    //  static_assert(sizeof(myDataStruct) <= ExtUI::eeprom_data_size);
    //  memcpy(&myDataStruct, buff, sizeof(myDataStruct));
  }

  void onConfigurationStoreWritten(bool success) {
    // Called after the entire EEPROM has been written,
    // whether successful or not.
  }

  void onConfigurationStoreRead(bool success) {
    // Called after the entire EEPROM has been read,
    // whether successful or not.
  }

  void onMeshUpdate(const int8_t xpos, const int8_t ypos, const float zval) {
    // Called when any mesh points are updated
  }

  #if ENABLED(POWER_LOSS_RECOVERY)
    void OnPowerLossResume() {
      // Called on resume from power-loss
    }
  #endif

  #if HAS_PID_HEATING
    void OnPidTuning(const result_t rst) {
      // Called for temperature PID tuning result
    }
  #endif
}

#endif // EXTUI_EXAMPLE && EXTENSIBLE_UI
