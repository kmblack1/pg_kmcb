#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "se_rand.h"



size_t random_utf8_zh(int code, char *buffer) {
	if (code <= 0x7F) {
		buffer[0] = code;
		return 1;
	} else if (code <= 0x7FF) {
		buffer[0] = (code >> 6) + 192;
		buffer[1] = (code & 63) + 128;
		return 2;
	} else if (0xd800 <= code && code <= 0xdfff) {
		return 0;
	} //invalid block of utf8
	else if (code <= 0xFFFF) {
		buffer[0] = (code >> 12) + 224;
		buffer[1] = ((code >> 6) & 63) + 128;
		buffer[2] = (code & 63) + 128;
		return 3;
	} else if (code <= 0x10FFFF) {
		buffer[0] = (code >> 18) + 240;
		buffer[1] = ((code >> 12) & 63) + 128;
		buffer[2] = ((code >> 6) & 63) + 128;
		buffer[3] = (code & 63) + 128;
		return 4;
	}
	return 0;
}

size_t rand_num_abc_zh(int flag, char *buffer) {
	int flag_upper;
	size_t is_valid;
	switch (flag) {
	case 1:
		buffer[0] = RANGE_RANDOM(48, 57);	/*生成数字*/
		return 1;
	case 2:
		buffer[0] = RANGE_RANDOM(65, 90);	/*生成大写字母*/
		return 1;
	case 3:
		buffer[0] = RANGE_RANDOM(97, 122);	/*生成小写字母*/
		return 1;
	case 4:/*生成汉字*/
		do {
			is_valid = random_utf8_zh(RANGE_RANDOM(19968, 20901), buffer);
		} while (0 == is_valid);
		return is_valid;
	default:	/*随机生成数字大小写汉字混合字符串*/
		flag_upper = RANGE_RANDOM(0, 3);
		switch (flag_upper) {
		case 1:
			buffer[0] = RANGE_RANDOM(65, 90);  /*生成大写字母*/
			return 1;
		case 2:
			buffer[0] = RANGE_RANDOM(97, 122);	/*生成小写字母*/
			return 1;
		case 3:/*生成汉字*/
			do {
				is_valid = random_utf8_zh(RANGE_RANDOM(19968, 20901), buffer);
			} while (0 == is_valid);
			return is_valid;
		default:
			buffer[0] = RANGE_RANDOM(48, 57);	/*生成数字*/
			return 1;
		}
		break;
	}
}

int ckeck_flag(int flag) {
	switch (flag) {
	case 1:
		return 1;
	case 2:
		return 2;
	case 3:
		return RANGE_RANDOM(1, 2);
	case 4:
		return 3;
	case 5:
		return RANGE_RANDOM(0, 1) ? 1 : 3;
	case 6:
		return RANGE_RANDOM(2, 3);
	case 7:
		return RANGE_RANDOM(1, 3);
	case 8:
		return 4;
	case 9:
		return RANGE_RANDOM(0, 1) ? 1 : 4;
	case 10:
		return RANGE_RANDOM(0, 1) ? 2 : 4;
	case 11:
		return RANGE_RANDOM(0, 1) ? RANGE_RANDOM(1, 2) : 4;
	case 12:
		return RANGE_RANDOM(3, 4);
	case 13:
		return RANGE_RANDOM(0, 1) ? 1 : RANGE_RANDOM(3, 4);
	case 14:
		return RANGE_RANDOM(2, 4);
	default:
		return 0;
	}
}


int num_abc_zh_rand(int flag, int character_count, char *buffer, size_t *use_mem_size) {
	size_t chr_mem_size = 0;
	char *ptmp;
	int i;
	if (NULL == buffer || NULL == use_mem_size)
		return SE_INVALID_PARAMETER_VALUE;
	if (character_count > 0x8000)
		return SE_PARAMETER_VALUE_OUT_OF_RANGE;
	ptmp = buffer;
	(*use_mem_size) = 0;
	for (i = 0; i < character_count; ++i) {
		chr_mem_size = rand_num_abc_zh(ckeck_flag(flag), ptmp);
		ptmp += chr_mem_size;
		(*use_mem_size) += chr_mem_size;
	}
	return SE_OK;
}





void random_anis_zh(char *buffer) {
	int high_zn, low_zn;
	/*双字节汉字区*/
	int high_part = RANGE_RANDOM(0, 2);
	switch (high_part) {
	case 1:/*双字节3区(0x81-0xA0,0x40-0x7E|0x80-0xFE)*/
		high_zn = RANGE_RANDOM(0x81, 0xA0);
		low_zn = RANGE_RANDOM(0, 1) ? RANGE_RANDOM(0x40, 0x7E) : RANGE_RANDOM(0x80, 0xFE);
		break;
	case 2:/*双字节4区(0xAA-0xFE,0x40-0x7E|0x80-0xA0)*/
		high_zn = RANGE_RANDOM(0xAA, 0xFE);
		low_zn = RANGE_RANDOM(0, 1) ? RANGE_RANDOM(0x40, 0x7E) : RANGE_RANDOM(0x80, 0xA0);
		break;
	default:/*双字节2区(0x81-0xA0,0xA1-0xFE)*/
		high_zn = RANGE_RANDOM(0xB0, 0xF7);
		low_zn = RANGE_RANDOM(0xA1, 0xFE);
		break;
	}
	buffer[0] = high_zn;
	buffer[1] = low_zn;
}

int zh_random_anis(char *buffer, int character_count, size_t *use_mem_size) {
	char  *ptmp;
	int i;
	if (NULL == buffer || NULL == use_mem_size)
		return SE_INVALID_PARAMETER_VALUE;
	if (character_count > 0x8000)
		return SE_PARAMETER_VALUE_OUT_OF_RANGE;
	ptmp = buffer;
	(*use_mem_size) = 0;
	for (i = 0; i < character_count; ++i) {
		random_anis_zh(buffer);
		memcpy(ptmp, buffer, 2);
		ptmp += 2;
		(*use_mem_size) += 2;
	}
	return SE_OK;
}



