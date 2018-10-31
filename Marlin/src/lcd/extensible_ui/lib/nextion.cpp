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

#if ENABLED(SDSUPPORT)
  #include "../../../sd/cardreader.h"
#endif

namespace UI {
  const char* printerKilledObj = "sPrintedKilled";
  const char* messageObj = "sMessage";    
  float _feedRate=0;
  bool _connected=false;

  NexVariable fr = NexVariable(1, 57, "fr");
  NexVariable showSdPage = NexVariable(1, 5, "e2");

  NexVariable t0 = NexVariable(3, 1, "t0");
  NexVariable t1 = NexVariable(3, 2, "t1");
  NexVariable t2 = NexVariable(3, 3, "t2");
  NexVariable t3 = NexVariable(3, 4, "t3");
  NexVariable t4 = NexVariable(3, 5, "t4");
  NexVariable t5 = NexVariable(3, 6, "t5");
  NexVariable t6 = NexVariable(3, 7, "t6");
  NexVariable t7 = NexVariable(3, 8, "t7");
  NexVariable t8 = NexVariable(3, 9, "t8");
  NexVariable t9 = NexVariable(3, 10, "t9");

  NexVariable down = NexVariable(3, 11, "b0");
  NexVariable up = NexVariable(3, 12, "b1");

  NexTouch *nex_listen_list[] = 
  {
        &showSdPage,
        &up,
        &down,
  //    &b0,
    //  &b1,
      NULL
  };


  int curPage = 0;
  int maxFiles = 10;

  void sendFilename(NexVariable obj, bool hasFile, const char *filename)
  {
    if(hasFile) {
      obj.setText(filename);
      obj.show();
    }
    else {
      obj.hide();
    }     
  }

  void sendFileList()
  {
    FileList fileList;    
    int startPos = curPage * 9;
    int count = fileList.count();    
    
    for (uint16_t i = 0; i < maxFiles; i++) {
      
      bool hasFile = fileList.seek(i + startPos);   
               
      switch(i)
      {
        case 0: sendFilename(t0, hasFile, fileList.filename()); break;        
        case 1: sendFilename(t1, hasFile, fileList.filename()); break;        
        case 2: sendFilename(t2, hasFile, fileList.filename()); break;        
        case 3: sendFilename(t3, hasFile, fileList.filename()); break;        
        case 4: sendFilename(t4, hasFile, fileList.filename()); break;        
        case 5: sendFilename(t5, hasFile, fileList.filename()); break;        
        case 6: sendFilename(t6, hasFile, fileList.filename()); break;        
        case 7: sendFilename(t7, hasFile, fileList.filename()); break;        
        case 8: sendFilename(t8, hasFile, fileList.filename()); break;        
        case 9: sendFilename(t9, hasFile, fileList.filename()); break;        
      }
    }
  }
  
  void showSdPageCallback(void *ptr)
  {
    curPage = 0;
    sendFileList();
    up.hide();
  }

  void upCallback(void *ptr)
  {
    curPage -= 1;
    if(curPage == 0) up.hide();
    sendFileList();
  }

  void downCallback(void *ptr)
  {
    curPage += 1;
    up.show();
    sendFileList();
  }

  void onStartup() {            
    nexInit();

    showSdPage.attachPop(showSdPageCallback, &showSdPage);

    up.attachPop(upCallback, &up);
    down.attachPop(downCallback, &down);
  }

  void onIdle() { 
      nexLoop(nex_listen_list);      

      if(_feedRate != getFeedRate_percent())
      {
        _feedRate = getFeedRate_percent();


        fr.setText(itostr3(_feedRate));
      }
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
}
#endif // EXTENSIBLE_UI
