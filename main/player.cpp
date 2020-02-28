
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

bool
LEDPlayer::isFull(void)
{
  return (recSpaceHeadIdx == PLAYER_RECORDING_CAPACITY);
}

void
LEDPlayer::recordSample(uint16_t val)
{
  if (recSpaceHeadIdx < sizeof(recSpace))
  {
    recSpace[recSpaceHeadIdx] = val;
    recSpaceHeadIdx++;
    if (recSpaceHeadIdx == sizeof(recSpace))
    {
      recording = false;
    }
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

uint16_t
LEDPlayer::getCurrentLength(void)
{
  return recSpaceHeadIdx;
}

void
LEDPlayer::reset(void)
{
  memset(&recSpace, 0x00, sizeof(recSpace));
  recSpaceHeadIdx = 0;
}
