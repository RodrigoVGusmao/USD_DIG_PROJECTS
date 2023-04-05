#pragma once

#include<malloc.h>

#define TRY const char* EHD_ERR_TRY_CATCH_MSG = "success";
#define THROW(err, msg) {EHD_ERR_TRY_CATCH_MSG = msg;goto err;}
#define CATCH(err) err:
#define GET_MSG() EHD_ERR_TRY_CATCH_MSG