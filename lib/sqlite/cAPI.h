#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  char callsign[11];
  char radio_service_code[3];
  char grant_date[11];
  char expired_date[11];
  char cancellation_date[11];
  char operator_class[2];
  char previous_callsign[11];
  char trustee_callsign[11];
  char trustee_name[51];
  char applicant_type_code[2];
  char entity_name[201];
  char first_name[21];
  char mi[2];
  char last_name[21];
  char suffix[4];
  char street_address[61];
  char city[21];
  char state[3];
  char zip_code[10];
  char po_box[21];
  char attention_line[36];
  char frn[11];
} LOOKUP;

typedef struct sqliteReader sqliteReader;


sqliteReader *sql_open(const char *filename);
LOOKUP *sql_find_call(sqliteReader *c, const char *call);
void sql_close(sqliteReader *c);

#ifdef __cplusplus
}
#endif