#ifndef UICREATE_RET_CHK
#define UICREATE_RET_CHK(_x_)                                                                       \
            do  {                                                                                   \
                if ( ! ( _x_ ) )                                                                    \
                {                                                                                   \
                    printf("UI CREATE FAIL: %s %s %d\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);  \
                    return 0;                                                                       \
                }                                                                                   \
            } while (0)
#endif