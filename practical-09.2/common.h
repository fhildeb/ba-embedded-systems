#ifndef PRAK9_COMMON_H
#define PRAK9_COMMON_H

/* Wir definieren die Nachrichtengröße als PATH_MAX,
weil wir Dateipfade übermitteln wollen.
wir werden Datenblöcke derselben größe übermitteln */

#include <limits.h>

#define MSGSIZE PATH_MAX /* 4096 */
#define BLKSIZE 1024

/* name, länge, blockdaten, success, error, ende */
/* Type MUSS GRÖSSER ALS 0 sein */
enum
{
  MSGT_NAM = 1,
  MSGT_LEN,
  MSGT_BLK,
  MSGT_SUC,
  MSGT_ERR,
  MSGT_BYE
};

struct msgbuf
{
  long type;
  char text[MSGSIZE];
};

#endif /* PRAK9_COMMON_H */