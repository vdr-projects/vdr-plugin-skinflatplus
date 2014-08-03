/*
 * Frontend Status Monitor plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __FEMONRECEIVER_H
#define __FEMONRECEIVER_H

#include <vdr/thread.h>
#include <vdr/receiver.h>
#include "baserender.h"

class cFemonReceiver : public cFlatBaseRender, public cReceiver, public cThread {
private:
  cMutex            m_Mutex;
  cCondWait         m_Sleep;
  bool              m_Active;


  cRingBufferLinear m_VideoBuffer; // Y
  cTsToPes          m_VideoAssembler;
  int               m_VideoType;
  int               m_VideoPid; // Y
  int               m_VideoPacketCount; // Y
  double            m_VideoBitrate;

  cRingBufferLinear m_AudioBuffer; // Y
  cTsToPes          m_AudioAssembler;
  int               m_AudioPid; // Y
  int               m_AudioPacketCount; // Y
  double            m_AudioBitrate;
  bool              m_AudioValid;

  cRingBufferLinear m_AC3Buffer; // Y
  cTsToPes          m_AC3Assembler;
  int               m_AC3Pid; // Y
  int               m_AC3PacketCount; // Y
  double            m_AC3Bitrate; // Y
  bool              m_AC3Valid;

protected:
  virtual void Activate(bool On);
  virtual void Receive(uchar *Data, int Length);
  virtual void Action(void);

public:
  cFemonReceiver(const cChannel* Channel, int ATrack, int DTrack);
  virtual ~cFemonReceiver();
  void Deactivate(void);

  double VideoBitrate(void)         { cMutexLock MutexLock(&m_Mutex);
                                      return m_VideoBitrate; };                // bit/s
  double AudioBitrate(void)         { cMutexLock MutexLock(&m_Mutex);
                                      return m_AudioBitrate; };                // bit/s
  double AC3Bitrate(void)           { cMutexLock MutexLock(&m_Mutex);
                                      return m_AC3Bitrate; };                  // bit/s
  };

#endif //__FEMONRECEIVER_H

