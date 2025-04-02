#ifndef SRC_IF_H
#define SRC_IF_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#ifdef __cplusplus
extern "C"
{
#endif
#define IIS_SCLK 41//SCK
#define IIS_LCLK 42//WS
#define IIS_DSIN -1
#define IIS_DOUT 2//DATA

  extern QueueHandle_t sndQueue;
  typedef struct
  {
    QueueHandle_t *queue;
    int item_size; //in bytes
  } src_cfg_t;

  void recsrcTask(void *arg);
  void app_speech_wakeup_init();
  void speech_commands_action(int command_id);

  void wake_up_action(void);

  typedef enum
  {
    ID0_TIAODAOERSHIDU = 0,
    ID1_TIAODAOERSHIYIDU = 1,
    ID2_TIAODAOERSHIERDU = 2,
    ID3_TIAODAOERSHISANDU = 3,
    ID4_TIAODAOERSHISIDU = 4,
    ID5_TIAODAOERSHIWUDU = 5,
    ID6_TIAODAOERSHLIUIDU = 6,
    ID7_TIAODAOERSHIQIDU = 7,
    ID8_TIAODAOERSHIBADU = 8,
    ID9_QIDONGKONGTIAOSAOFENG = 9,
    ID10_TINGZHIKONGTIAOSAOFENG = 10,
    ID11_TIAODAOZIDONGFENGSU = 11,
    ID12_TIAODAOYIJIFENGSU = 12,
    ID13_TIAODAOERJIFENGSU = 13,
    ID14_TIAODAOSANJIFENGSU = 14,
    ID15_DAKAIKONGTIAO = 15,
    ID16_GUANBIKONGTIAO = 16,
    ID17_GELIKONGTIAO = 17,
    ID18_MEIDIKONGTIAO = 18,
    ID19_AOKESIKONGTIAO = 19,
    ID20_HAIERKONGTIAO = 20,
    ID21_KAIDENG = 21,
    ID22_GUANDENG = 22,
    ID_MAX,
  } asr_multinet_event_t;



  esp_err_t play_wav(const char *filepath);
  void record_i2s_init(void);
  void play_i2s_init(void);
  void all_i2s_deinit(void);
  void play_sdfile_name(char *file_name);
  void play_spiffs_name(char *file_name);
#ifdef __cplusplus
}
#endif
#endif
