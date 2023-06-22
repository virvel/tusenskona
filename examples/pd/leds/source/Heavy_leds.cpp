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

#include "Heavy_leds.hpp"

#define Context(_c) reinterpret_cast<Heavy_leds *>(_c)



/*
 * C Functions
 */

extern "C" {
  HV_EXPORT HeavyContextInterface *hv_leds_new(double sampleRate) {
    return new Heavy_leds(sampleRate);
  }

  HV_EXPORT HeavyContextInterface *hv_leds_new_with_options(double sampleRate,
      int poolKb, int inQueueKb, int outQueueKb) {
    return new Heavy_leds(sampleRate, poolKb, inQueueKb, outQueueKb);
  }
} // extern "C"







/*
 * Class Functions
 */

Heavy_leds::Heavy_leds(double sampleRate, int poolKb, int inQueueKb, int outQueueKb)
    : HeavyContext(sampleRate, poolKb, inQueueKb, outQueueKb) {
  numBytes += cVar_init_f(&cVar_mZ1fDWv5, 1.0f);
  numBytes += cDelay_init(this, &cDelay_6ddKhIj1, 0.0f);
  numBytes += cVar_init_f(&cVar_2OjdSTwY, 500.0f);
  numBytes += cBinop_init(&cBinop_09h5mAeJ, 0.0f); // __mul
  numBytes += cVar_init_f(&cVar_hanZOzEu, 1.0f);
  
  // schedule a message to trigger all loadbangs via the __hv_init receiver
  scheduleMessageForReceiver(0xCE5CC65B, msg_initWithBang(HV_MESSAGE_ON_STACK(1), 0));
}

Heavy_leds::~Heavy_leds() {
  // nothing to free
}

HvTable *Heavy_leds::getTableForHash(hv_uint32_t tableHash) {
  return nullptr;
}

void Heavy_leds::scheduleMessageForReceiver(hv_uint32_t receiverHash, HvMessage *m) {
  switch (receiverHash) {
    case 0xCE5CC65B: { // __hv_init
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_BA6fBhTn_sendMessage);
      break;
    }
    default: return;
  }
}

int Heavy_leds::getParameterInfo(int index, HvParameterInfo *info) {
  if (info != nullptr) {
    switch (index) {
      default: {
        info->name = "invalid parameter index";
        info->hash = 0;
        info->type = HvParameterType::HV_PARAM_TYPE_PARAMETER_IN;
        info->minVal = 0.0f;
        info->maxVal = 0.0f;
        info->defaultVal = 0.0f;
        break;
      }
    }
  }
  return 0;
}



/*
 * Send Function Implementations
 */


void Heavy_leds::cVar_mZ1fDWv5_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_IQxoD5nh_sendMessage);
  cSwitchcase_pQB0wH8w_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_leds::cBinop_IQxoD5nh_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_mZ1fDWv5, 1, m, &cVar_mZ1fDWv5_sendMessage);
}

void Heavy_leds::cSwitchcase_pQB0wH8w_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  switch (msg_getHash(m, 0)) {
    case 0x0: { // "0.0"
      cMsg_rEb8q9Rx_sendMessage(_c, 0, m);
      break;
    }
    case 0x7A5B032D: { // "stop"
      cMsg_rEb8q9Rx_sendMessage(_c, 0, m);
      break;
    }
    default: {
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_npMAvWzb_sendMessage);
      break;
    }
  }
}

void Heavy_leds::cDelay_6ddKhIj1_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const m) {
  cDelay_clearExecutingMessage(&Context(_c)->cDelay_6ddKhIj1, m);
  cDelay_onMessage(_c, &Context(_c)->cDelay_6ddKhIj1, 0, m, &cDelay_6ddKhIj1_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_hanZOzEu, 0, m, &cVar_hanZOzEu_sendMessage);
}

void Heavy_leds::cCast_npMAvWzb_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_rEb8q9Rx_sendMessage(_c, 0, m);
  cDelay_onMessage(_c, &Context(_c)->cDelay_6ddKhIj1, 0, m, &cDelay_6ddKhIj1_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_hanZOzEu, 0, m, &cVar_hanZOzEu_sendMessage);
}

void Heavy_leds::cMsg_difxIUCa_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_SCOeetjQ_sendMessage);
}

void Heavy_leds::cSystem_SCOeetjQ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 1000.0f, 0, m, &cBinop_DpBedamn_sendMessage);
}

void Heavy_leds::cVar_2OjdSTwY_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_09h5mAeJ, HV_BINOP_MULTIPLY, 0, m, &cBinop_09h5mAeJ_sendMessage);
}

void Heavy_leds::cMsg_rEb8q9Rx_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "clear");
  cDelay_onMessage(_c, &Context(_c)->cDelay_6ddKhIj1, 0, m, &cDelay_6ddKhIj1_sendMessage);
}

void Heavy_leds::cBinop_CbYNCrwO_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cDelay_onMessage(_c, &Context(_c)->cDelay_6ddKhIj1, 2, m, &cDelay_6ddKhIj1_sendMessage);
}

void Heavy_leds::cBinop_DpBedamn_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_09h5mAeJ, HV_BINOP_MULTIPLY, 1, m, &cBinop_09h5mAeJ_sendMessage);
}

void Heavy_leds::cBinop_09h5mAeJ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 1.0f, 0, m, &cBinop_CbYNCrwO_sendMessage);
}

void Heavy_leds::cVar_hanZOzEu_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_EQ, 0.0f, 0, m, &cBinop_lLdR2WE3_sendMessage);
  cSend_1ocFJAoh_sendMessage(_c, 0, m);
}

void Heavy_leds::cBinop_lLdR2WE3_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_hanZOzEu, 1, m, &cVar_hanZOzEu_sendMessage);
}

void Heavy_leds::cSend_1ocFJAoh_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  if (_c->getSendHook() != nullptr) _c->getSendHook()(_c, "led", 0x25E27FF3, m);
}

void Heavy_leds::cReceive_BA6fBhTn_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_difxIUCa_sendMessage(_c, 0, m);
  cVar_onMessage(_c, &Context(_c)->cVar_2OjdSTwY, 0, m, &cVar_2OjdSTwY_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_mZ1fDWv5, 0, m, &cVar_mZ1fDWv5_sendMessage);
}




/*
 * Context Process Implementation
 */

int Heavy_leds::process(float **inputBuffers, float **outputBuffers, int n) {
  while (hLp_hasData(&inQueue)) {
    hv_uint32_t numBytes = 0;
    ReceiverMessagePair *p = reinterpret_cast<ReceiverMessagePair *>(hLp_getReadBuffer(&inQueue, &numBytes));
    hv_assert(numBytes >= sizeof(ReceiverMessagePair));
    scheduleMessageForReceiver(p->receiverHash, &p->msg);
    hLp_consume(&inQueue);
  }
  
  hv_uint32_t nextBlock = blockStartTimestamp + n;
  while (mq_hasMessageBefore(&mq, nextBlock)) {
    MessageNode *const node = mq_peek(&mq);
    node->sendMessage(this, node->let, node->m);
    mq_pop(&mq);
  }

  blockStartTimestamp = nextBlock;
  return n;
}

int Heavy_leds::processInline(float *inputBuffers, float *outputBuffers, int n4) {
  hv_assert(!(n4 & HV_N_SIMD_MASK)); // ensure that n4 is a multiple of HV_N_SIMD

  // define the heavy input buffer for 0 channel(s)
  float **const bIn = NULL;

  // define the heavy output buffer for 0 channel(s)
  float **const bOut = NULL;

  int n = process(bIn, bOut, n4);
  return n;
}

int Heavy_leds::processInlineInterleaved(float *inputBuffers, float *outputBuffers, int n4) {
  hv_assert(n4 & ~HV_N_SIMD_MASK); // ensure that n4 is a multiple of HV_N_SIMD

  // define the heavy input buffer for 0 channel(s), uninterleave
  float *const bIn = NULL;

  // define the heavy output buffer for 0 channel(s)
  float *const bOut = NULL;

  int n = processInline(bIn, bOut, n4);

  

  return n;
}
