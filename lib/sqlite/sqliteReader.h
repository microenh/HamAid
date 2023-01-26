// #pragma once

#include <stdint.h>
#include "../fatfs/ff.h"
#include "cAPI.h"

union DB {
  int64_t i;
  double f;
  uint8_t *ptr;
};

struct SCHEMA {
  char type[11];
  char name[21];
  char tbl_name[21];
  uint32_t rootpage;
};



class sqliteReader {
  private:
    char *filename;
    uint16_t page_size;
    uint8_t *page_buffer;
    uint16_t U;
    uint16_t M;
    uint16_t X;
    uint8_t type;
    uint16_t max_number_cells;
    uint16_t number_cells;
    uint32_t right_child;
    uint32_t left_child;
    uint16_t *cell_offset;
    uint32_t row_id;
    uint32_t overflow;
    SCHEMA *schema;
    void *data_dest;

    void parse_page(void);
    void parse_payload(uint16_t offset, uint16_t max=0xffff);
    void parse_record(uint16_t offset, uint16_t max=0xffff);
    void (sqliteReader::*store)(uint16_t field_no, uint32_t field_type, DB data);
    void store_header(uint16_t field_no, uint32_t field_type, DB data);
    void store_data(uint16_t field_no, uint32_t field_type, DB data);
    void save_string(uint16_t field_len, uint16_t target_len, char *dest, char *src);
    void read_page(FIL *f, uint32_t page);
  
    uint32_t varint(uint16_t &ofs);


  public:
    sqliteReader(const char *);
    ~sqliteReader();
    bool lookup_call(const char *);
    LOOKUP lookup;
};