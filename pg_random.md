# 随机生成测试数据
函数定义
```
/*
随机生成数字大小写字母组成的字符串
	$1:	表示字符串最小字符数量,默认为8
	$2:	表示字符串最大字符数量,默认为32
	$3:	标记位,默认为0

				1:	仅生成数字			
				2:	仅生成大写字母			
				3:	仅生成数字或大写字母			
				4:	仅生成小写字母			
				5:	仅生成数字或小写字母			
				6:	仅生成大写字母或小写字母			
				7:	仅生成数字或大写字母或小写字母			
				8:	仅生成汉字			
				9:	仅生成数字或汉字			
				10:	仅生成大写字母或汉字			
				11:	仅生成数字或大写字母或汉字			
				12:	仅生成小写字母或汉字			
				13:	仅生成数字或小写字母或汉字			
				14:	仅生成大写字母或小写字母或汉字			
				其它:	随机生成数字,大小写字母或汉字			
	备注:	函数每次调用的返回结果都不同,所以函数不能设置immutable
*/
create function generate_rand_string(integer default 8,integer default 32,integer default 0)
	returns text
as 'pg_random', 'generate_rand_string'
language C strict;
```

## 默认生成8-32个字符的随机字符串,字符中由数字\大写字母\小写字母\汉字组成<br/>
```
	select generate_rand_string()
```
output:"僚98a5L偭仁r12Gb8儉dx7仟z俴T1仩"<br/>

## 生成6个纯数字组成的随机字符串<br/>
```
	select generate_rand_string(6,6,1)
```

output:"742779"<br/>
详细用法请参考3.函数定义<br/>

## 性能对比测试
随机生成汉字,sql语言的函数<br/>
```
--drop function if exists gen_random_zh(int,int);
create or replace function gen_random_zh(int,int)
	returns text
as $$
	select string_agg(chr((random()*(20901-19968)+19968 )::integer) , '')  from generate_series(1,(random()*($2-$1)+$1)::integer);
$$ language sql;
```
在终端中执行,执行前打开\timing on计时, 分别执行三次<br/>
```
\timing on
```
注意不要在pgAdmin中执行,pgAdmin自身需要从PostgreSQL获取数据,然后后又要生成显示的数据,导致测量结果不正确)<br/>
```
select gen_random_zh(32,128) from generate_series(1,100000);
```
5703.715 ms<br/>
5662.481 ms<br/>
5695.228 ms<br/>
```
select generate_rand_string(32,128,8) from generate_series(1,100000);
```
567.180 ms<br/>
565.333 ms<br/>
570.695 ms<br/>
<br/>
两者效率相差10倍以上<br/>

## 用途

插入测试数据.示例如下:<br/>
```
/*企业基本信息*/
create table t_enterprises(
	objectid bigint not null,	/*唯一编号*/
	name text not null,	/*企业名称*/
	bank text, /*开户银行*/
	registered text,	/*纳税人登记号*/
	number text,	/*账号*/
	address text, /*企业地址*/
	post text,		/*企业邮编*/
	legal text,		/*法人代表*/
	contacts text,	/*联系人*/
	tel text,	/*联系电话*/
	fax text,		/*传真*/
	mail text,	/*邮箱*/
	describe text,	/*企业备注*/
	constraint pk_enterprisess_objectid primary key (objectid) with (fillfactor=60)
) with (fillfactor=60);
```
导入1000万测试数据,为提高效率,开10个进程导入,每个写入100万,顺便测试服务器的随机IO写入性能.<br/>
10个进程平均耗时约1分钟左右,也就是 说1千万的数据1分钟插入完成,平均每秒17万条/秒.<br/>
注意,脚本主要是测试随机写入的,不是IOPS
```
/*进程1*/
do $$
	declare
		v_start bigint;
		v_end bigint;
	begin
		for i in 1..1000 loop		
			v_start := (i-1)*1000 + 1;
			v_end := v_start + 999;
			insert into t_enterprises
				select id,generate_rand_string(8,32,8) as name,generate_rand_string(8,32,8) as bank,
					generate_rand_string(12,12,1) as registered,generate_rand_string(12,16,1) as number,
					generate_rand_string(16,64,8) as address,generate_rand_string(6,6,1) as post,
					generate_rand_string(2,6,8) as legal,generate_rand_string(2,6,8) as contacts,
					generate_rand_string(7,8,1) as tel,generate_rand_string(7,8,1) as fax,
					generate_rand_string(8,32,7) as mail,generate_rand_string(16,64,8) as describe
				from generate_series(v_start,v_end) as id;
			raise notice  '%,%', v_start,v_end;
		end loop;
	end;
$$;
/*进程2*/
do $$
	declare
		v_start bigint;
		v_end bigint;
	begin
		for i in 1001..2000 loop		
			v_start := (i-1)*1000 + 1;
			v_end := v_start + 999;
			insert into t_enterprises
				select id,generate_rand_string(8,32,8) as name,generate_rand_string(8,32,8) as bank,
					generate_rand_string(12,12,1) as registered,generate_rand_string(12,16,1) as number,
					generate_rand_string(16,64,8) as address,generate_rand_string(6,6,1) as post,
					generate_rand_string(2,6,8) as legal,generate_rand_string(2,6,8) as contacts,
					generate_rand_string(7,8,1) as tel,generate_rand_string(7,8,1) as fax,
					generate_rand_string(8,32,7) as mail,generate_rand_string(16,64,8) as describe
				from generate_series(v_start,v_end) as id;
			raise notice  '%,%', v_start,v_end;
		end loop;
	end;
$$;
/*进程3*/
do $$
	declare
		v_start bigint;
		v_end bigint;
	begin
		for i in 2001..3000 loop		
			v_start := (i-1)*1000 + 1;
			v_end := v_start + 999;
			insert into t_enterprises
				select id,generate_rand_string(8,32,8) as name,generate_rand_string(8,32,8) as bank,
					generate_rand_string(12,12,1) as registered,generate_rand_string(12,16,1) as number,
					generate_rand_string(16,64,8) as address,generate_rand_string(6,6,1) as post,
					generate_rand_string(2,6,8) as legal,generate_rand_string(2,6,8) as contacts,
					generate_rand_string(7,8,1) as tel,generate_rand_string(7,8,1) as fax,
					generate_rand_string(8,32,7) as mail,generate_rand_string(16,64,8) as describe
				from generate_series(v_start,v_end) as id;
			raise notice  '%,%', v_start,v_end;
		end loop;
	end;
$$;
/*进程4*/
do $$
	declare
		v_start bigint;
		v_end bigint;
	begin
		for i in 3001..4000 loop		
			v_start := (i-1)*1000 + 1;
			v_end := v_start + 999;
			insert into t_enterprises
				select id,generate_rand_string(8,32,8) as name,generate_rand_string(8,32,8) as bank,
					generate_rand_string(12,12,1) as registered,generate_rand_string(12,16,1) as number,
					generate_rand_string(16,64,8) as address,generate_rand_string(6,6,1) as post,
					generate_rand_string(2,6,8) as legal,generate_rand_string(2,6,8) as contacts,
					generate_rand_string(7,8,1) as tel,generate_rand_string(7,8,1) as fax,
					generate_rand_string(8,32,7) as mail,generate_rand_string(16,64,8) as describe
				from generate_series(v_start,v_end) as id;
			raise notice  '%,%', v_start,v_end;
		end loop;
	end;
$$;
/*进程5*/
do $$
	declare
		v_start bigint;
		v_end bigint;
	begin
		for i in 4001..5000 loop		
			v_start := (i-1)*1000 + 1;
			v_end := v_start + 999;
			insert into t_enterprises
				select id,generate_rand_string(8,32,8) as name,generate_rand_string(8,32,8) as bank,
					generate_rand_string(12,12,1) as registered,generate_rand_string(12,16,1) as number,
					generate_rand_string(16,64,8) as address,generate_rand_string(6,6,1) as post,
					generate_rand_string(2,6,8) as legal,generate_rand_string(2,6,8) as contacts,
					generate_rand_string(7,8,1) as tel,generate_rand_string(7,8,1) as fax,
					generate_rand_string(8,32,7) as mail,generate_rand_string(16,64,8) as describe
				from generate_series(v_start,v_end) as id;
			raise notice  '%,%', v_start,v_end;
		end loop;
	end;
$$;
/*进程6*/
do $$
	declare
		v_start bigint;
		v_end bigint;
	begin
		for i in 5001..6000 loop		
			v_start := (i-1)*1000 + 1;
			v_end := v_start + 999;
			insert into t_enterprises
				select id,generate_rand_string(8,32,8) as name,generate_rand_string(8,32,8) as bank,
					generate_rand_string(12,12,1) as registered,generate_rand_string(12,16,1) as number,
					generate_rand_string(16,64,8) as address,generate_rand_string(6,6,1) as post,
					generate_rand_string(2,6,8) as legal,generate_rand_string(2,6,8) as contacts,
					generate_rand_string(7,8,1) as tel,generate_rand_string(7,8,1) as fax,
					generate_rand_string(8,32,7) as mail,generate_rand_string(16,64,8) as describe
				from generate_series(v_start,v_end) as id;
			raise notice  '%,%', v_start,v_end;
		end loop;
	end;
$$;
/*进程7*/
do $$
	declare
		v_start bigint;
		v_end bigint;
	begin
		for i in 6001..7000 loop		
			v_start := (i-1)*1000 + 1;
			v_end := v_start + 999;
			insert into t_enterprises
				select id,generate_rand_string(8,32,8) as name,generate_rand_string(8,32,8) as bank,
					generate_rand_string(12,12,1) as registered,generate_rand_string(12,16,1) as number,
					generate_rand_string(16,64,8) as address,generate_rand_string(6,6,1) as post,
					generate_rand_string(2,6,8) as legal,generate_rand_string(2,6,8) as contacts,
					generate_rand_string(7,8,1) as tel,generate_rand_string(7,8,1) as fax,
					generate_rand_string(8,32,7) as mail,generate_rand_string(16,64,8) as describe
				from generate_series(v_start,v_end) as id;
			raise notice  '%,%', v_start,v_end;
		end loop;
	end;
$$;
/*进程8*/
do $$
	declare
		v_start bigint;
		v_end bigint;
	begin
		for i in 7001..8000 loop		
			v_start := (i-1)*1000 + 1;
			v_end := v_start + 999;
			insert into t_enterprises
				select id,generate_rand_string(8,32,8) as name,generate_rand_string(8,32,8) as bank,
					generate_rand_string(12,12,1) as registered,generate_rand_string(12,16,1) as number,
					generate_rand_string(16,64,8) as address,generate_rand_string(6,6,1) as post,
					generate_rand_string(2,6,8) as legal,generate_rand_string(2,6,8) as contacts,
					generate_rand_string(7,8,1) as tel,generate_rand_string(7,8,1) as fax,
					generate_rand_string(8,32,7) as mail,generate_rand_string(16,64,8) as describe
				from generate_series(v_start,v_end) as id;
			raise notice  '%,%', v_start,v_end;
		end loop;
	end;
$$;
/*进程9*/
do $$
	declare
		v_start bigint;
		v_end bigint;
	begin
		for i in 8001..9000 loop		
			v_start := (i-1)*1000 + 1;
			v_end := v_start + 999;
			insert into t_enterprises
				select id,generate_rand_string(8,32,8) as name,generate_rand_string(8,32,8) as bank,
					generate_rand_string(12,12,1) as registered,generate_rand_string(12,16,1) as number,
					generate_rand_string(16,64,8) as address,generate_rand_string(6,6,1) as post,
					generate_rand_string(2,6,8) as legal,generate_rand_string(2,6,8) as contacts,
					generate_rand_string(7,8,1) as tel,generate_rand_string(7,8,1) as fax,
					generate_rand_string(8,32,7) as mail,generate_rand_string(16,64,8) as describe
				from generate_series(v_start,v_end) as id;
			raise notice  '%,%', v_start,v_end;
		end loop;
	end;
$$;
/*进程10*/
do $$
	declare
		v_start bigint;
		v_end bigint;
	begin
		for i in 9001..10000 loop		
			v_start := (i-1)*1000 + 1;
			v_end := v_start + 999;
			insert into t_enterprises
				select id,generate_rand_string(8,32,8) as name,generate_rand_string(8,32,8) as bank,
					generate_rand_string(12,12,1) as registered,generate_rand_string(12,16,1) as number,
					generate_rand_string(16,64,8) as address,generate_rand_string(6,6,1) as post,
					generate_rand_string(2,6,8) as legal,generate_rand_string(2,6,8) as contacts,
					generate_rand_string(7,8,1) as tel,generate_rand_string(7,8,1) as fax,
					generate_rand_string(8,32,7) as mail,generate_rand_string(16,64,8) as describe
				from generate_series(v_start,v_end) as id;
			raise notice  '%,%', v_start,v_end;
		end loop;
	end;
$$;
```
