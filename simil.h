#ifndef H_8F98B694_4296_4F0B_8FEF_3F937A0ED0C1
#define H_8F98B694_4296_4F0B_8FEF_3F937A0ED0C1
/*
* https://www.codeproject.com/Articles/8265/More-than-strcmp-similarity-in-strings
*	计算两个字符的相似度或相同的部份
*/

#include	<string.h>
#include "postgres.h"
#include "fmgr.h"

extern float simil(const char *str1, unsigned int len1, const char *str2, unsigned int len2);
extern text *same(const char *str1, unsigned int slen1, const char *str2, unsigned int slen2);

#endif /*H_8F98B694_4296_4F0B_8FEF_3F937A0ED0C1*/
