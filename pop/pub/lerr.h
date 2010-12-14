#ifndef __LERR_H__
#define __LERR_H__
#include "mheads.h"

__BEGIN_DECLS

extern NERR_TYPE LERR_MEVENT;
extern NERR_TYPE LERR_NOTLOGIN;
extern NERR_TYPE LERR_LOGINPSW;
extern NERR_TYPE LERR_USERINPUT;
extern NERR_TYPE LERR_LIMIT;
extern NERR_TYPE LERR_NEEDVIP;
extern NERR_TYPE LERR_NEEDVVIP;
extern NERR_TYPE LERR_NEXIST;
extern NERR_TYPE LERR_EXIST;
extern NERR_TYPE LERR_IMGPROE;
extern NERR_TYPE LERR_ATTACK;

NEOERR* lerr_init();
void lerr_opfinish_json(NEOERR *err, HDF *hdf);

__END_DECLS
#endif	/* __LERR_H__ */