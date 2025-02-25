#ifndef UTILS_H
#define UTILS_H

#include "lib/inc/eud.h"
#include "lib/inc/eud_api.h"
#include "lib/inc/eud_api.h"
#include "lib/inc/jtag_api.h"
#include "lib/inc/ctl_api.h"
#include "lib/inc/com_api.h"
#include "lib/inc/swd_api.h"
#include "lib/inc/usb.h"
#include "lib/inc/eud_error_defines.h"

void check_error(const char *msg, EUD_ERR_t errcode);


#endif //UTILS_H
