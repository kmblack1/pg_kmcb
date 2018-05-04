#ifndef H_EE198021_31C1_4D72_AE3F_65F564B13D46
#define H_EE198021_31C1_4D72_AE3F_65F564B13D46

#include "postgres.h"
#include "fmgr.h"
#include "se_std.h"

/*���ַ�����ָ��������ת��Ϊ����*/
extern text **split_string_v0(text *in_str, uint32 split_size, uint32 *character_count1);
/*�Ƚ��ַ���*/
extern int32 compare_string(char *a, int lena, char *b, int lenb, bool prefix);
/*�Ƚ��ַ���*/
extern int compare_text_lexemes(const void *va, const void *vb);

#endif /*H_EE198021_31C1_4D72_AE3F_65F564B13D46*/