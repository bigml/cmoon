#ifndef __MMISC_H__
#define __MMISC_H__

#include "mheads.h"

__BEGIN_DECLS

void exiting(void);
bool mmisc_getdatetime(char *res, int len, const char *fmt, time_t second);
bool mmisc_getdatetime_gmt(char *res, int len, const char *fmt, time_t second);
int  mmisc_compare_int(const void *a, const void *b);
int  mmisc_compare_inta(const void *a, const void *b);

void mmisc_set_qrarray(char *qrcol, char qr_array[QR_NUM_MAX][LEN_ST], int *qr_cnt);

void mmisc_pagediv_get(HDF *hdf, char *inprefix, int *count, int *offset);
/* table, condition MUST be string literal, not variable */
#define MMISC_PAGEDIV_SET(hdf, offset, db, table, condition, sfmt, ...) \
	do {																\
		int zinta;														\
		mdb_exec(db, NULL, "SELECT COUNT(*) FROM " table " WHERE " condition ";", sfmt, ##__VA_ARGS__); \
		mdb_get(db, "i", &zinta);										\
		hdf_set_int_value(hdf, "ntt", zinta);							\
		hdf_set_int_value(hdf, "nst", offset);							\
	} while (0)
#define MMISC_PAGEDIV_SET_O(hdf, outprefix, offset, db, table, condition, sfmt, ...) \
	do {																\
		int zinta;														\
		mdb_exec(db, NULL, "SELECT COUNT(*) FROM " table " WHERE " condition ";", sfmt, ##__VA_ARGS__); \
		mdb_get(db, "i", &zinta);										\
		hdf_set_valuef(hdf, "%s.ntt=%d", outprefix, zinta);				\
		hdf_set_valuef(hdf, "%s.nst=%d", outprefix, offset);			\
	} while (0)

void mmisc_str_repchr(char *s, char from, char to);
char* mmisc_str_strip (char *s, char n);
unsigned int hash_string(const char *str);

/*
 * IE: make sure timezone & time set correct on web server
 */
void mmisc_cache_headers(time_t second);

/*
 * make sure charout has inlen*2+1 len
 */
void mmisc_hex2str(unsigned char *hexin, unsigned int inlen, unsigned char *charout);

__END_DECLS
#endif	/* __MMISC_H__ */
