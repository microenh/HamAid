#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#include "config.h"
#include "DEV_Config.h"
#include "display.h"
#include "lcd.h"
#include "liberationmono.h"
#include "ff.h"

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments

void setup() {
    // static struct repeating_timer timer;

    // set_sys_clock_48mhz();
    // add_repeating_timer_ms(125, heartbeat, NULL, &timer);
    System_Init();
    InitPWM();
    
    BacklightLevel(0);
    /* LCD Init */
    InitLCD(HORIZONTAL);
    Invert(true);
    Clear(BACKGROUND);


    BacklightLevel(25);

    uint y = 0;

    const short unsigned fgs[] = {TOP_FOREGROUND, BOTTOM_FOREGROUND};
    uint index = 0;

    const sFONT* fonts[] = {
        &Liberation6,
        &Liberation7,
        &Liberation8,
        &Liberation9,
        &Liberation10,
        &Liberation11,
        &Liberation12,
        &Liberation14,
        &Liberation16,
        &Liberation18,
        &Liberation20,
        &Liberation22,
        #if 0
        &Liberation24,
        &Liberation26,
        &Liberation28,
        &Liberation36,
        &Liberation48,
        &Liberation72   
        #endif 
    };

    uint points[] = {
        6,7,8,9,10,11,12,14,16,18,20,22,24,26,28,36,48,72
    };

    char disp_buf[6];

    for (int i=0; i<12; i++) {
        sprintf(disp_buf, "%d pt", points[i]);
        DrawString(0, y, disp_buf, fonts[i], fgs[index], BACKGROUND);
        index = !index;
        y += fonts[i]->height + 1;
    }

}


struct fcc_rec {
  char callsign[10];
  char radio_service_code[2];
  char grant_date[10];
  char expired_date[10];
  char cancellation_date[10];
  char operator_class;
  char previous_callsign[10];
  char trustee_callsign[10];
  char trustee_name[50];
  char applicant_type_code;
  char entity_name[200];
  char first_name[20];
  char mi;
  char last_name[20];
  char suffix[3];
  char street_address[60];
  char city[20];
  char state[2];
  char zip_code[9];
  char po_box[20];
  char attention_line[35];
  char frn[10];
};

const unsigned rec_size = sizeof(struct fcc_rec);

struct fcc_rec fcc;

const char *fileName = "FCCSER.DAT";


void lookup(const char *target) {
  // printf("records: %lu\r\n", records);
  FIL f;
  FRESULT res;
  res = f_open(&f, fileName, FA_READ);
  // printf("open: %d\r\n", res);
  long records = f_size(&f) / rec_size;
  int c;
  long low = 0;
  long high = records;
  long mid;
  uint bytes_read;
  f_read(&f, &fcc, rec_size, &bytes_read);
  // printf("bytes_read: %u\r\n", bytes_read);
  c = strcmp(target, fcc.callsign);
  if (c > 0) {
    while (high - low > 1) {
      mid = (low + high) / 2;
      // printf("%s %s %ld %ld %ld\r\n", fcc.callsign, target, low, mid, high);
      f_lseek(&f, mid * rec_size);
      f_read(&f, &fcc, rec_size, &bytes_read);
      c = strcmp(target, fcc.callsign);
      if (!c) {
        break;
      } else if (c > 0) {
        low = mid;
      } else {
        high = mid;
      }
    }
  }
  f_close(&f);
  if (strcmp(fcc.callsign, target)) {
    printf("%s - Not found.\r\n", target);
  } else {
    printf("%s %s %s\r\n", fcc.callsign, fcc.first_name, fcc.last_name);
  }
}

const char *calls[] = {
  "_",
  "AA0A",
  "AA0AA",
  "AA0AB",
  "N8ME",
  "N8ME-",
  "W8NX",
  "WA8KKN",
  "W8CR",
  "N8CWU",
  "WZ9Y",
  "WZ9Z",
  "WZ9ZZZ",
  "Z",
};

int mainFlat(void) {

  for (int i = 0; i<sizeof(calls) / sizeof(calls[0]); i++) {
    lookup(calls[i]);
  }
  return 0;
}


void loop() {

}

const char * DirName = "/";

int main(void)
{
    // const uint MAX_BMP_FILES = 25;
    DIR MyDirectory;
    FILINFO MyFileInfo;
    FATFS microSDFatFs;

    setup();
    // sleep_ms(5 * 1000);
    DisplayOff();
    printf("Display Off\r\n");


#if 0
    FRESULT res;
    res = f_mount(&microSDFatFs, (TCHAR const*) "/", 1);
    printf("res: %d\r\n", res);
    if((res != FR_OK)){
		if(res == FR_NO_FILESYSTEM){
			/* Display message: SD card not FAT formated */
			printf("SD_CARD_NOT_FORMATTED\r\n");  
		}else{
			/* Display message: Fail to open directory */
			printf("SD_CARD_OPEN_FAIL\r\n");          
		}
    } else {
        res = f_opendir(&MyDirectory, DirName);
        // printf("res %d\r\n", res);
        if(res == FR_OK){
            for (;;){
                res = f_readdir(&MyDirectory, &MyFileInfo);
                if (res != FR_OK || MyFileInfo.fname[0] == 0) break;
                // if(MyFileInfo.fname[0] == '.') continue;
                if(!(MyFileInfo.fattrib & AM_DIR)){
                    printf("%s\r\n", MyFileInfo.fname);
                }
            }
        }
 	}
#endif

    FRESULT res;
    res = f_mount(&microSDFatFs, (TCHAR const*) "/", 1);
    printf("mount: %d\r\n", res);
    mainFlat();

    return 0;
}