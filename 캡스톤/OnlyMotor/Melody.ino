#include "FirebaseESP8266.h"
#include <pitches.h>

//Mario main them tempo
const int melody[] PROGMEM = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7, 0, NOTE_C7, NOTE_E7, 0, NOTE_G7, 0, 0,  0, NOTE_G6, 0, 0, 0, 
  NOTE_C7, 0, 0, NOTE_G6, 0, 0, NOTE_E6, 0, 0, NOTE_A6, 0, NOTE_B6, 0, NOTE_AS6, NOTE_A6, 0, 
  NOTE_G6, NOTE_E7, NOTE_G7, NOTE_A7, 0, NOTE_F7, NOTE_G7, 0, NOTE_E7, 0,NOTE_C7, NOTE_D7, NOTE_B6, 0, 0,
  NOTE_C7, 0, 0, NOTE_G6, 0, 0, NOTE_E6, 0, 0, NOTE_A6, 0, NOTE_B6, 0, NOTE_AS6, NOTE_A6, 0, 
  NOTE_G6, NOTE_E7, NOTE_G7, NOTE_A7, 0, NOTE_F7, NOTE_G7, 0, NOTE_E7, 0,NOTE_C7, NOTE_D7, NOTE_B6, 0, 0
};
const int tempo[] PROGMEM = {
  12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 
  12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 
  9, 9, 9, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
  12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
  9, 9, 9, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
};

// 징글벨 첫 소절 음계 : 알기 쉽게 다장조의 음계로 변환하여 처리
const int song[] PROGMEM = { NOTE_G4, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_G4, 0, // 솔미레도솔(쉼표)
NOTE_G4, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_A4, 0, // 솔미레도라(쉼표)
NOTE_A4, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_B4, 0, // 라파미레시(쉼표)
NOTE_G5, NOTE_G5, NOTE_F5, NOTE_D5, NOTE_E5, NOTE_C5, 0 }; // 솔솔파레미도(쉼표)
// 징글벨 첫 소절 연주시간
const int times[] PROGMEM= { 250, 250, 250, 250, 750, 250, // 8분음표 = 250ms
250, 250, 250, 250, 750, 250,
250, 250, 250, 250, 750, 250,
250, 250, 250, 250, 500, 250, 250 };

// end

// 사운드 모드를 결정하는 함수
void SoundMode()
{
  Firebase.getInt(firebaseData, "DBConnect/Users/10001/Sound/number");
  int Mode =  firebaseData.intData();
  Serial.printf("mode : %d \n", Mode);
  if(Mode==0)
  {
    //음소거모드
  }
  else if(Mode == 1)
  {
    //1번 출력
    if(millis() < prevPlayTime + playDuration)
    {
        // music is playing. do nothing
        return;
    }
    // stop the tone playing:
    noTone(8);
    
    if(currentMelody >= melodySize)
        currentMelody = 0;
    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/tempo[currentMelody];

    tone(d8, melody[currentMelody], noteDuration);
    prevPlayTime = millis();
    playDuration = noteDuration * 1.30;
    
    currentMelody++;
  }
  else if(Mode==2)
  {
    //2번 출력
    for (int i=0; i<25; i++) // 연주 길이만큼 숫자 조정
    {
      tone(d8, song[i], times[i]); // 배열순으로 노래 재생
      delay(times[i]);
    }
  }
  else if(Mode==3)
  {
    //3번 출력
  }
  else
  {
    //잘못된 값 0으로 초기화
    Mode = 0;
  }
}
