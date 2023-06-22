/**
 * Copyright (c) 2023 Enzien Audio, Ltd.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the phrase "powered by heavy",
 *    the heavy logo, and a hyperlink to https://enzienaudio.com, all in a visible
 *    form.
 * 
 *   2.1 If the Application is distributed in a store system (for example,
 *       the Apple "App Store" or "Google Play"), the phrase "powered by heavy"
 *       shall be included in the app description or the copyright text as well as
 *       the in the app itself. The heavy logo will shall be visible in the app
 *       itself as well.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#ifndef _HEAVY_CONTEXT_LEDS_HPP_
#define _HEAVY_CONTEXT_LEDS_HPP_

// object includes
#include "HeavyContext.hpp"
#include "HvControlBinop.h"
#include "HvControlDelay.h"
#include "HvControlCast.h"
#include "HvControlSystem.h"
#include "HvControlVar.h"

class Heavy_leds : public HeavyContext {

 public:
  Heavy_leds(double sampleRate, int poolKb=10, int inQueueKb=2, int outQueueKb=0);
  ~Heavy_leds();

  const char *getName() override { return "leds"; }
  int getNumInputChannels() override { return 0; }
  int getNumOutputChannels() override { return 0; }

  int process(float **inputBuffers, float **outputBuffer, int n) override;
  int processInline(float *inputBuffers, float *outputBuffer, int n) override;
  int processInlineInterleaved(float *inputBuffers, float *outputBuffer, int n) override;

  int getParameterInfo(int index, HvParameterInfo *info) override;
  struct Parameter {
    
    struct Out {
      enum ParameterOut : hv_uint32_t {
        LED = 0x25E27FF3, // led
      };
    };
  };

 private:
  HvTable *getTableForHash(hv_uint32_t tableHash) override;
  void scheduleMessageForReceiver(hv_uint32_t receiverHash, HvMessage *m) override;

  // static sendMessage functions
  static void cVar_mZ1fDWv5_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_IQxoD5nh_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_pQB0wH8w_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cDelay_6ddKhIj1_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_npMAvWzb_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_difxIUCa_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_SCOeetjQ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_2OjdSTwY_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_rEb8q9Rx_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_CbYNCrwO_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_DpBedamn_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_09h5mAeJ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_hanZOzEu_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_lLdR2WE3_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSend_1ocFJAoh_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_BA6fBhTn_sendMessage(HeavyContextInterface *, int, const HvMessage *);

  // objects
  ControlVar cVar_mZ1fDWv5;
  ControlBinop cBinop_IQxoD5nh;
  ControlDelay cDelay_6ddKhIj1;
  ControlVar cVar_2OjdSTwY;
  ControlBinop cBinop_CbYNCrwO;
  ControlBinop cBinop_DpBedamn;
  ControlBinop cBinop_09h5mAeJ;
  ControlVar cVar_hanZOzEu;
  ControlBinop cBinop_lLdR2WE3;
};

#endif // _HEAVY_CONTEXT_LEDS_HPP_
