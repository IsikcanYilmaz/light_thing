

#define PLAYER_SAMPLE_RATE 15 // Hz
#define PLAYER_RECORDING_LENGTH 10 // Seconds
#define PLAYER_RECORDING_CAPACITY (PLAYER_SAMPLE_RATE * PLAYER_RECORDING_LENGTH) // uint16_ts 

#include <stdint.h>

class LEDPlayer {
  public:
    LEDPlayer();
    ~LEDPlayer();
    void recordSample(uint16_t val);
    uint16_t getSample(uint16_t idx);
    uint16_t getCurrentLength(void);
    void reset(void);
    bool isRecording(void);
    void setRecording(bool r);
    bool isFull(void);


  private:
    uint16_t recSpace[PLAYER_RECORDING_CAPACITY];
    uint16_t recSpaceHeadIdx;
    uint16_t currentLength;
    bool recording;

};
