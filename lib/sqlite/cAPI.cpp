#include "cAPI.h"
#include "sqliteReader.h"
#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif


sqliteReader *sql_open(const char *filename) {
  return new sqliteReader(filename);
}

LOOKUP *sql_find_call(sqliteReader *c, const char *call) {
  if (c->lookup_call(call)) {
    return (&c->lookup);
  } else {
    return (NULL);
  }
}

void sql_close(sqliteReader *c) {
  delete c;  
}

#ifdef __cplusplus
}
#endif