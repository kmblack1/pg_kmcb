#ifndef H_EE198021_31C1_4D72_AE3F_65F564B13D46
#define H_EE198021_31C1_4D72_AE3F_65F564B13D46

#include "postgres.h"
#include "fmgr.h"
#include "se_std.h"

/*将字符串按指定的字数转换为数组*/
extern text **split_string_v0(text *in_str, uint32 split_size, uint32 *character_count1);
/*比较字符串*/
extern int32 compare_string(char *a, int lena, char *b, int lenb, bool prefix);
/*比较字符串*/
extern int compare_text_lexemes(const void *va, const void *vb);

#endif /*H_EE198021_31C1_4D72_AE3F_65F564B13D46*/