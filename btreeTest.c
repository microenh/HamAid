#include "btreeTest.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "ff.h"

FATFS microSDFatFs;




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
  const char * DirName = "/";
  f_mount(&microSDFatFs, DirName, 1);

  const char *test_calls[] = {
      "AA0A", "AA0AA", "AA0AB", "N8ME", "W8NX", "WA8KKN", "W8CR", "N8CWU", "WZ9Y", "WZ9Z", "WZ9ZZZ", 
      "-", "N8ME-", "Z"
  };
  
  for (int i = 0; i<sizeof(test_calls) / sizeof(test_calls[0]); i++) {
    lookup(test_calls[i]);
  }
  return 0;
}

bool findCall(fcc_rec *fcc, char *call) {
  int r = binSearch(call);
  if (r > -1) {
    FIL f;
    uint bytes_read;
    f_open(&f, dataName, FA_READ);
    f_lseek(&f, r * sizeof(fcc_rec));
    f_read(&f, fcc, sizeof(fcc_rec), &bytes_read);
    f_close(&f);
    return true;
  } else {
    return false;
  }

}