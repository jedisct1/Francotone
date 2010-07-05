/* $Id: francotone-php.c 4192 2006-08-01 17:08:15Z j $ */
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "common.h"
#include "francotone.h"
#include "SAPI.h"

#ifdef PHP_WIN32
# define PHP_FRANCOTONE_API __declspec(dllexport)
#else
# define PHP_FRANCOTONE_API
#endif

#ifdef ZTS
# include "TSRM.h"
# define FRANCOTONE_G(v) TSRMG(francotone_globals_id, zend_francotone_globals *, v)
#else
# define FRANCOTONE_G(v) (francotone_globals.v)
#endif

ZEND_FUNCTION(francotone_xfrm);
PHP_MINFO_FUNCTION(francotone);
PHP_MINIT_FUNCTION(francotone);
PHP_MSHUTDOWN_FUNCTION(francotone);

zend_function_entry francotone_functions[] = {
    PHP_FE(francotone_xfrm, NULL)
    { NULL, NULL, NULL }
};

zend_module_entry francotone_module_entry = {
    STANDARD_MODULE_HEADER,
        "francotone",
        francotone_functions,
        PHP_MINIT(francotone),
        PHP_MSHUTDOWN(francotone),
        NULL,
        NULL,
        PHP_MINFO(francotone),
        NO_VERSION_YET,
        STANDARD_MODULE_PROPERTIES
};
#define phpext_francotone_ptr &francotone_module_entry

#ifdef COMPILE_DL_FRANCOTONE
ZEND_GET_MODULE(francotone)
#endif

PHP_MINFO_FUNCTION(francotone)
{
    php_info_print_table_start();
    php_info_print_table_row(2, "francotone functions", "enabled");
    php_info_print_table_row(2, "Version", "$Id: francotone-php.c 4192 2006-08-01 17:08:15Z j $");    
    php_info_print_table_end();
}

PHP_MINIT_FUNCTION(francotone)
{
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(francotone)
{
    return SUCCESS;
}

PHP_FUNCTION(francotone_xfrm)
{
    char *str;
    char *francotoned;
    int sizeof_str_;
    size_t sizeof_str;
        
    if (zend_parse_parameters
        (ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &sizeof_str_) == FAILURE ||
        sizeof_str_ < 0 ||
        (int) (sizeof_str = (size_t) sizeof_str_) != sizeof_str_) {
        WRONG_PARAM_COUNT;
    }
    francotoned = francotone(str);
    RETURN_STRING(francotoned, 1);
    free(francotoned);
}
