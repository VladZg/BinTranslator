#ifndef ASSERT_H
#define ASSERT_H

#include <stdio.h>

//! @brief Макрос, печатающий сообщение об ошибке в заданном условии, если оно неверно
//! @param[in] condition Проверяемое условие

#define ASSERT(condition)                                                               \
        {                                                                               \
                if (!(condition))                                                       \
                {                                                                       \
                    char assert_str[200] = {};                                          \
                    sprintf(assert_str, "Error in '%s' in line %d in file %s in %s",    \
                            #condition, __LINE__, __FILE__, __PRETTY_FUNCTION__);       \
                    perror(assert_str);                                                 \
                }                                                                       \
        }

#endif
