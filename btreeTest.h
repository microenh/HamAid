#pragma once

#include "ff.h"
#include <stdbool.h>

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

int testBTree(void);
bool findCall(fcc_rec *fcc, char *call);

extern FATFS microSDFatFs;