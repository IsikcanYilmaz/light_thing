
#include "player.h"
#include "string.h"


LEDPlayer::LEDPlayer() 
{
  recSpaceHeadIdx = 0;
  currentLength = 0;
  recording = false;
}

LEDPlayer::~LEDPlayer(){};

bool
LEDPlayer::isRecording(void)
{
  return recording;
}

void
LEDPlayer::setRecording(bool r)
{
  recording = r;
}

void
LEDPlayer::recordSample(uint16_t val)
{
  if (recSpaceHeadIdx < sizeof(recSpace))
  {
    recSpace[recSpaceHeadIdx] = val;
    recSpaceHeadIdx++;
  }
}

uint16_t
LEDPlayer::getSample(uint16_t idx)
{
  if (idx < sizeof(recSpace))
  {
    return recSpace[idx];
  }
  else
  {
    return 0;
  }
}

void
LEDPlayer::reset(void)
{
  memset(&recSpace, 0x00, sizeof(recSpace));
  recSpaceHeadIdx = 0;
}
