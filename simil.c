#include	<stdlib.h>
#include "simil.h"
/* Quick and dirty swap of the address of 2 arrays of unsigned int
*/
static void swap(unsigned **first, unsigned **second) {
	unsigned *temp;
	temp = *first;
	*first = *second;
	*second = temp;
}

/* A function which returns how similar 2 strings are
Assumes that both point to 2 valid null terminated array of chars.
Returns the similarity between them.
*/

float simil(const char *str1, unsigned int len1, const char *str2, unsigned int len2) {
	const char *ptr_str1, *ptr_str2, *ptr_temp;
	unsigned int temp;
	float lenLCS;
	unsigned j, k, *previous, *next;

	if (len1 == 0 || len2 == 0)
		return 0;
	/*********************************/
	//2018 - 05 - 02
	if (len1 < len2) {
		temp = len1;
		len1 = len2;
		len2 = temp;

		ptr_temp = str1;
		ptr_str1 = str2;
		ptr_str2 = ptr_temp;
	} else {
		ptr_str1 = str1;
		ptr_str2 = str2;
	}
	/*********************************/
	previous = (unsigned *)palloc0(len1 + 1, sizeof(unsigned));
	next = (unsigned *)palloc0(len1 + 1, sizeof(unsigned));
	for (j = 0; j < len2; ++j) {
		for (k = 1; k <= len1; ++k) {
			if (ptr_str1[k - 1] == ptr_str2[j])
				next[k] = previous[k - 1] + 1;
			else
				next[k] = previous[k] >= next[k - 1] ? previous[k] : next[k - 1];
		}
		swap(&previous, &next);
	}
	lenLCS = (float)previous[len1];
	pfree(previous);
	pfree(next);
	return lenLCS /= len1;
}
/*	Returns a pointer to the Longest Common Sequence in str1 and str2
Assumes str1 and str2 point to 2 null terminated array of char
*/
text *same(const char *str1, unsigned int slen1, const char *str2, unsigned int slen2) {
	text *out_result;
	char *ptr_result;
	int i, r, c, len1 = (int)slen1, len2 = (int)slen2;
	unsigned **align;

	if (len1 == 0 || len2 == 0)
		return 0;
	//elog(NOTICE, "%s(%u)\t%s(%u)\n", str1, slen1, str2, slen2);
	out_result =(text *) palloc((len1 > len2 ? len1 : len2) + VARHDRSZ + sizeof(char) );
	ptr_result = VARDATA(out_result);

	align = (unsigned **)palloc0(len2 + 1, sizeof(unsigned *));
	for (r = 0; r <= len2; ++r)
		align[r] = (unsigned *)palloc0(len1 + 1, sizeof(unsigned));
	for (r = 1; r <= len2; ++r) {
		for (c = 1; c <= len1; ++c) {
			if (str1[c - 1] == str2[r - 1])
				align[r][c] = align[r - 1][c - 1] + 1;
			else
				align[r][c] = align[r - 1][c] >= align[r][c - 1] ? align[r - 1][c] : align[r][c - 1];
		}
	}
	for (r = len2, c = len1, i = align[r][c], ptr_result[i] = '\0'; i > 0 && r > 0 && c > 0; i = align[r][c]) {
		if (align[r - 1][c] == i)
			--r;
		else if (align[r][c - 1] == i)
			--c;
		else if (align[r - 1][c - 1] == i - 1) {
			ptr_result[i - 1] = str2[--r];
			--c;
		}
	}
	for (r = len2; r >= 0; --r)
		pfree(align[r]);
	pfree(align);
	//elog(NOTICE, "%s(%u)\n", ptr_result, strlen(ptr_result));
	SET_VARSIZE(out_result, VARHDRSZ + strlen(ptr_result) );
	//elog(NOTICE, "%p%s(%u)\n", ptr_result, ptr_result, strlen(ptr_result));
	//elog(NOTICE, "%p,%p\n", out_result, ptr_result);
	return out_result;
}