
#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"
#include "ta_internals.h"



typedef struct TAInvar {
  int           is_used;
  void          *data;
  unsigned int  size;
  unsigned char hash[HASH_SIZE];
#ifdef __SANCUS_MSP430
// struct size has to be a power of 2 so that bit shifting instead of
// multiplication is used to address elements of a struct array.
#if   HASH_SIZE ==  8
  unsigned char padding[2];
#elif HASH_SIZE == 16
  unsigned char padding[10];
#else
  #error "Padding undefined."
#endif
#endif 
} TAInvar;


static TAInvar       SM_DATA(TA_SM_NAME) TAInvars[MAX_INVARS];

#ifdef __DEBUG
static unsigned char hash_declas[HASH_SIZE];
#endif



static int SM_FUNC(TA_SM_NAME)
  TAHash (void *data, unsigned int dsize, unsigned char *hash,
    unsigned int hsize)
{
  if (data == NULL || dsize < 1 ||
      hash == NULL || hsize < HASH_SIZE) {
    return (TA_FAILURE);
  }
  sancus_tag(data, dsize, hash);

  return (TA_SUCCESS);
}

static int SM_FUNC(TA_SM_NAME)
  TACompareHashes (unsigned char *h1, unsigned char *h2)
{
  int i;

  for (i = 0; i < HASH_SIZE; i++) {
    if (h1[i] != h2[i]) {
      return (TA_FAILURE); }
  }

  return (TA_SUCCESS);
}

int SM_FUNC(TA_SM_NAME)
  TAInvarInit (void)
{
  int i;

  for (i = 0; i < MAX_INVARS; i++) {
    TAInvars[i].is_used = 0;
  }

  return (TA_SUCCESS);
}

/*---------------------------------------------------------------------------*/
/* Interface function: TARegisterInvar                                       */

int SM_ENTRY(TA_SM_NAME) TARegisterInvar (void *data, unsigned int size)
{
  int i;

  __TIMING_INIT();

  if (! TAInitialised) {
    goto FAIL;
  }

  for (i = 0; i < MAX_INVARS; i++) {
    if (! TAInvars[i].is_used) {
      TAInvars[i].data = data;
      TAInvars[i].size = size;
      if (TAHash (data, size, TAInvars[i].hash, HASH_SIZE) != TA_SUCCESS)
       { return (TA_FAILURE); }
      TAInvars[i].is_used = 1;

  __TIMING_PRINT("+++ TARegisterInvar(): ");

#ifdef __DEBUG
      TAmemcpy(hash_declas, TAInvars[i].hash, HASH_SIZE);
      char *tmp = TAHashToString(hash_declas);
      if (tmp != NULL) {
        printf3("TA: invar @ %p+%u: %s\n",
          (void *) TAInvars[i].data, (void *) TAInvars[i].size, (void *) tmp);
        free (tmp);
      }
#endif

      return (TA_SUCCESS);
    }
  }

FAIL:
  puts("*** TA: invar not registered.");
  return (TA_FAILURE);  return (TA_SUCCESS);
}


/*---------------------------------------------------------------------------*/
/* TACheckInvars                                                             */

int SM_FUNC(TA_SM_NAME) TACheckInvars (void)
{
  unsigned char hash[HASH_SIZE];
  int i, res = TA_SUCCESS;

  if (! TAInitialised) {
    return (res);
  }

  for (i = 0; i < MAX_INVARS; i++) {
    if (TAInvars[i].is_used) {
#ifdef __DEBUG
      printf2("TA: checking %p+%u...",
        (void *) TAInvars[i].data, (void *) TAInvars[i].size);
#endif
      if (TAHash (TAInvars[i].data, TAInvars[i].size, hash, HASH_SIZE)
          != TA_SUCCESS)
       {
#ifdef __DEBUG
         puts ("failed.");
#endif
         res = TA_FAILURE;
         continue; }
      if (TACompareHashes(hash, TAInvars[i].hash) == TA_SUCCESS) {
#ifdef __DEBUG
        puts ("ok.");
#endif
      } else {
#ifdef __DEBUG
        puts ("failed.");
#endif
        res = TA_FAILURE;
      }
    }
  }

  return (res);
}

/*---------------------------------------------------------------------------*/
/* TAInvarsStatus                                                            */

unsigned char* SM_ENTRY(TA_SM_NAME) TAInvarsStatus (unsigned char *nonce,
  int nonce_len, int* status_len)
{
  static unsigned char *InvarStatus = NULL;
  static int sl;

  __TIMING_INIT();

  sl = (sizeof(struct TAInvar) * MAX_INVARS) + nonce_len;
  *status_len = sl + HASH_SIZE;
  InvarStatus = malloc (*status_len);
  if (!InvarStatus) { goto FAIL; }

  sancus_wrap(nonce, nonce_len, TAInvars, *status_len, InvarStatus,
    InvarStatus + sl);

  __TIMING_PRINT("+++ TAInvarsStatus(): ");
  return (InvarStatus);

FAIL:
  *status_len = 0;
  return (NULL); 
}


