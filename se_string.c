#include "se_string.h"

text **split_string_v0(text *in_str, uint32 split_size, uint32 *split_array_size) {
	text *tunpacked;
	uint32 in_str_mem_size, character_count, array_size,item_count;

	char *ptr_in_tmp;
	size_t  array_head_size, all_txt_mem_size, utf8_mem_size;
	size_t first, other_size;
	text **ptr_values;
	char *ptr_val_tmp;

	/*��ȡ����������Ƿ�Ϊ''��Ϊ''ʱֱ�ӷ���NULL*/
	tunpacked = pg_detoast_datum_packed((struct varlena *) in_str);
	in_str_mem_size = VARSIZE_ANY_EXHDR(tunpacked);
	ptr_in_tmp = VARDATA_ANY(in_str);
	if ('\0' == ptr_in_tmp[0])
		return NULL;
	/*	��ָ���������з��ַ�,���ص������СΪ"�ַ�����-(split_size-1)"
	Ϊ��������ڴ�,һ���Է�����������ڴ�(����ָ��ͷ + ʵ�ʵ�����)
	*/
	character_count = utf8_character_count(ptr_in_tmp, in_str_mem_size);
	/*����ַ�����С�ڵ����зֵ�����,ֱ�ӽ����ַ�ת��Ϊֻ��һ��Ԫ�ص�����*/
	array_size = (character_count <= split_size ? 1 : (character_count - (split_size - 1)));
	/*���������ַ���С���зֵ�����*/
	item_count = (character_count < split_size ? character_count : split_size);
	array_head_size = (array_size * sizeof(char *));
	all_txt_mem_size = (
		array_size * VARHDRSZ /*�ַ���С�����ռ�*/
		+ (array_size * split_size) * UTF8_MAX_SIZE /*�ַ����ݿռ�*/
		);
	ptr_values = (text **)palloc(array_head_size + all_txt_mem_size);
	ptr_val_tmp = (((char *)ptr_values) + array_head_size);
	for (unsigned int i = 0; i < array_size; ++i) {
		first = UTF8LEN((const unsigned char)ptr_in_tmp[0]);
		utf8_mem_size = first;
		/*�����з�Ϊ�����,һ��Ԫ��ռ�õ��ڴ��С*/
		for (uint32 j = 1; j < item_count; ++j) {
			other_size = UTF8LEN((const unsigned char)ptr_in_tmp[utf8_mem_size]);
			utf8_mem_size += other_size;
		}
		ptr_values[i] = (text *)ptr_val_tmp;
		SET_VARSIZE(ptr_val_tmp, utf8_mem_size + VARHDRSZ);
		memcpy(VARDATA(ptr_val_tmp), ptr_in_tmp, utf8_mem_size);
		//elog(NOTICE, "%s", VARDATA(ptr_values[i]));
		ptr_val_tmp += (VARHDRSZ + utf8_mem_size);
		ptr_in_tmp += first;
	}
	(*split_array_size) = array_size;
	return ptr_values;
}


int32 compare_string(char *a, int lena, char *b, int lenb, bool prefix) {
	int         cmp;
	if (lena == 0) {
		if (prefix)
			cmp = 0;            /* empty string is prefix of anything */
		else
			cmp = (lenb > 0) ? -1 : 0;
	} else if (lenb == 0) {
		cmp = (lena > 0) ? 1 : 0;
	} else {
		cmp = memcmp(a, b, Min(lena, lenb));

		if (prefix) {
			if (cmp == 0 && lena > lenb)
				cmp = 1;        /* a is longer, so not a prefix of b */
		} else if (cmp == 0 && lena != lenb) {
			cmp = (lena < lenb) ? -1 : 1;
		}
	}
	return cmp;
}

int compare_text_lexemes(const void *va, const void *vb) {
	Datum       a = *((const Datum *)va);
	Datum       b = *((const Datum *)vb);
	char       *alex = VARDATA_ANY(a);
	int         alex_len = VARSIZE_ANY_EXHDR(a);
	char       *blex = VARDATA_ANY(b);
	int         blex_len = VARSIZE_ANY_EXHDR(b);
	return compare_string(alex, alex_len, blex, blex_len, false);
}
