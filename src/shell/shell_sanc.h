/*
 * @file : shell_sanc.h
 * 
 * @brief : 
 * 
 * @author : louiey, louiey@thountech.com
 * 
 * @date : 2026-07-14
 * @copyright : Copyright (c) 2026
 */

#ifndef SHELL_SANC_H
#define SHELL_SANC_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************
 * MACROs
 **************************************************/
#define LOG_FLOAT_FMT "%d.%02d"
#define LOG_FLOAT_VAL(f) \
    ((int)(f)), \
    ((int)(((f) - (int)(f)) * 100 < 0 ? -(((f) - (int)(f)) * 100) : (((f) - (int)(f)) * 100)))


/**
 * @brief Check return value and prints shell error message if failed
 */
#define SHELL_CHECK_RET(expr, sh, fmt, ...)                             \
        do {                                                            \
            int __ret = (expr);                                         \
            if (__ret < 0) {                                            \
                shell_error(sh, fmt, ##__VA_ARGS__);                    \
                return -EINVAL;                                         \
            }                                                           \
        } while (0)

#define SHELL_CHECK_ARGC(sh, argc, required, fmt, ...)                  \
        do {                                                            \
            if ((argc) < (required)) {                                  \
                shell_error(sh, fmt, ##__VA_ARGS__);                    \
                return -EINVAL;                                         \
            }                                                           \
        } while (0)


#ifdef __cplusplus
}
#endif

#endif // SHELL_SANC_H