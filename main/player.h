

#define PLAYER_SAMPLE_RATE 30 // Hz
#define PLAYER_RECORDING_LENGTH 10 // Seconds

#include <stdint.h>

class LEDPlayer {
  public:
    LEDPlayer();
    ~LEDPlayer();
    void recordSample(uint16_t val);
    uint16_t getSample(uint16_t idx);
    void reset(void);
    bool isRecording(void);


  private:
    uint16_t recSpace[PLAYER_SAMPLE_RATE * PLAYER_RECORDING_LENGTH];
    uint16_t recSpaceHeadIdx;
    uint16_t currentLength;
    bool recording;

};
