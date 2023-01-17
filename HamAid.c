#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "button.h"

#include "config.h"
#include "DEV_Config.h"
#include "display.h"
#include "lcd.h"
#include "liberationmono.h"
#include "ff.h"

FATFS microSDFatFs;
bool looping = true;

void setup() {
    // static struct repeating_timer timer;

    set_sys_clock_48mhz();
    // add_repeating_timer_ms(125, heartbeat, NULL, &timer);
    System_Init();

    // const char * DirName = "/";
    // f_mount(&microSDFatFs, DirName, 1);

    InitPWM();
    
    /* LCD Init */
    InitLCD(HORIZONTAL);

    #if 0
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
      DisplayOn(25);
    #endif
    DisplayOn(25);
    Clear(BLACK);
    for(uint8_t y = 1; y < 14; y += 2) {
      for (uint8_t x = 0; x < 20; x += 4) {
        if (x == 4) {
          button(x, y, 4, 2, "ABCDEF", "IJKLMN", WHITE, BLACK, BRED, true);
        } else {
          button(x, y, 4, 2, "ABC", NULL, WHITE, BLACK, BRED, false);
        }
      }
    }
}


typedef struct {
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
} fcc_rec;

const char *dataName = "FCC.db";
const char *indexName = "FCC.ndx";
#define node_count 171
#define key_size 10

typedef struct {
  uint16_t keySize;
  uint16_t nodeSize;
  uint32_t root;
} index_hdr;

typedef struct {
  char key[node_count * 2 - 1][key_size];
  uint32_t record[node_count * 2 - 1];
  uint16_t child[node_count * 2];
  bool leaf;
  uint16_t active;
} index_rec;

int binSearch(const char* targetCall) {
  index_hdr hdr;
  index_rec rec;

  uint bytes_read;
  FRESULT res;
  FIL f;
  res = f_open(&f, indexName, FA_READ);
  res = f_read(&f, &hdr, sizeof(index_hdr), &bytes_read);

  bool searching = true;
  int curIdx = hdr.root;
  int retval = -1;
  while (searching) {
    res = f_lseek(&f, sizeof(index_hdr) + sizeof(index_rec) * curIdx);
    res = f_read(&f, &rec, sizeof(index_rec), &bytes_read);

    uint16_t high = rec.active;
    uint16_t low = 0;
    uint16_t mid = (high + low) / 2;
    while (low < high) {
      int c = strcmp(targetCall, rec.key[mid]);
      if (!c) {
        break;
      } else if (c > 0) {
        low = mid + 1;
      } else {
        high = mid;
      }
      mid = (high + low) / 2;
    }
    if (mid == rec.active) {
      if (rec.leaf) {
        searching = false;
      } else {
        curIdx = rec.child[rec.active];
      }
    } else if (strcmp(targetCall, rec.key[mid])) {
      if (rec.leaf) {
        searching = false;
      } else {
        curIdx = rec.child[mid];
      } 
    } else {
      searching = false;
      retval = rec.record[mid];
    }
  }
  f_close(&f);
  return retval;
}

void lookup(const char *targetCall) {
  int r = binSearch(targetCall);
  if (r > -1) {
    FIL f;
    fcc_rec fcc;
    uint bytes_read;
    f_open(&f, dataName, FA_READ);
    f_lseek(&f, r * sizeof(fcc_rec));
    f_read(&f, &fcc, sizeof(fcc_rec), &bytes_read);
    f_close(&f);
    printf("%s %s\r\n", fcc.callsign, fcc.entity_name);
  } else {
    printf("%s Not found.\r\n", targetCall);
  }
}

int testBTree(void) {
  const char *test_calls[] = {
    "AA0A", "AA0AA", "AA0AB", "N8ME", "W8NX", "WA8KKN", "W8CR", "N8CWU", "WZ9Y", "WZ9Z", "WZ9ZZZ", 
    "-", "N8ME-", "Z"
  };
  
  for (int i = 0; i<sizeof(test_calls) / sizeof(test_calls[0]); i++) {
    lookup(test_calls[i]);
  }
  return 0;
}

void loop() {
  // testBTree();
  looping = false;
}

int main(void)
{
    setup();
    sleep_ms(15 * 1000);

    while(looping) {
      loop();
    }
    DisplayOff();
    return 0;
}