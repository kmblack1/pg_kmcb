

/*小数修约*/
/*四舍五入*/
create function round_v1(float8,integer)
	returns float8
as 'pg_kmcb', 'round_v1'
language C immutable strict;

create function round2str_v1(float8,integer)
	returns text
as 'pg_kmcb', 'round2str_v1'
language C immutable strict;

/*四舍六入五考虑*/
create function round_v2(float8,integer)
	returns float8
as 'pg_kmcb', 'round_v2'
language C immutable strict;

create function round2str_v2(float8,integer)
	returns text
as 'pg_kmcb', 'round2str_v2'
language C immutable strict;

/*将指定的字节大小转换为带单位的人类可读的格式 */
create function byte_units(bigint)
	returns text
as 'pg_kmcb', 'byte_units'
language C immutable strict;

/*获取主机名*/
create function hostname()
	returns text
as 'pg_kmcb', 'hostname'
language C immutable strict;


/*
*	按每$2个字拆分字符串为数组（只支持UTF-8格式的字符串）
*	默认按每二个字做为一个词
*	数组大小为"字符数量-($2-1)"
*	当输入为''返回NULL，注意检查
*/
create function split_string(text,integer default 2)
	returns text[]
as 'pg_kmcb', 'split_string'
language C immutable strict;

/*计算字符串的CRC32值*/
create function generate_crc32_v1(text)
	returns bigint
as 'pg_kmcb', 'generate_crc32_v1'
language C immutable strict;

/*计算两个字符串的相似度,返回值在0-1之间*/
create function string_simil(text,text)
	returns float4
as 'pg_kmcb', 'string_simil'
language C immutable strict;

/*获取两个字符串相同的部份*/
create function string_same(text,text)
	returns text
as 'pg_kmcb', 'string_same'
language C immutable strict;

/*
*	随机生成数字大小写字母组成的字符串
*		$1:	表示字符串最小字符数量,默认为8
*		$2:	表示字符串最大字符数量,默认为32
*		$3:	标记位,默认为0
*					1:	仅生成数字
*					2:	仅生成大写字母
*					3:	仅生成数字或大写字母
*					4:	仅生成小写字母
*					5:	仅生成数字或小写字母
*					6:	仅生成大写字母或小写字母
*					7:	仅生成数字或大写字母或小写字母
*					8:	仅生成汉字
*					9:	仅生成数字或汉字
*					10:	仅生成大写字母或汉字
*					11:	仅生成数字或大写字母或汉字
*					12:	仅生成小写字母或汉字
*					13:	仅生成数字或小写字母或汉字
*					14:	仅生成大写字母或小写字母或汉字
*					其它:	随机生成数字,大小写字母或汉字
*	备注:函数每次调用的返回结果都不同,所以函数不能设置immutable
*/
create function generate_rand_string(integer default 8,integer default 32,integer default 0)
	returns text
as 'pg_kmcb', 'generate_rand_string'
language C strict;

/*
*	生成AES密钥
*/
create function generate_aes_key()
	returns text
as 'pg_kmcb', 'generate_aes_key'
language C strict;

/*
*	生成AES向量
*/
create function generate_aes_iv()
	returns text
as 'pg_kmcb', 'generate_aes_iv'
language C strict;


/*
*	AES加密
*		$1:AES密钥（KEY）,由generate_aes_key生成
*		$2:AES向量（IV）,由generate_aes_iv生成
*		$3:要加密的字符串
*/
create function aes_encrypt(text,text,text)
	returns text
as 'pg_kmcb', 'aes_encrypt'
language C strict;

/*
*	AES解密
*		$1:AES密钥（KEY）,由generate_aes_key生成
*		$2:AES向量（IV）,由generate_aes_iv生成
*		$2:要解密的字符串
*/
create function aes_decrypt(text,text,text)
	returns text
as 'pg_kmcb', 'aes_decrypt'
language C strict;




/*从YYDOY编号中获取YYDOY部分*/
create function get_yydoy_from_sn(bigint,integer)
	returns int
as 'pg_kmcb', 'get_yydoy_from_sn'
language C immutable strict;

/*从YYDOY编号中获取ID部分*/
create function get_id_from_sn(bigint,integer)
	returns bigint
as 'pg_kmcb', 'get_id_from_sn'
language C immutable strict;
/*
*	从YYDOY编号中获取YYDOY和ID部分(一次性取出)
*	备注:函数的返回结果为复合类型(记录集),所以函数不能设置immutable
*/
create function get_yydoy_id_from_sn(bigint,integer,int default 1)
	returns table(yydoy int, id bigint)
as 'pg_kmcb', 'get_yydoy_id_from_sn'
language C strict;

/*从YYDOY编号中获取日期*/
create function get_date_from_sn(bigint,integer)
	returns date
as 'pg_kmcb', 'get_date_from_sn'
language C immutable strict;
