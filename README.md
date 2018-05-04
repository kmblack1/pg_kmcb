# **PostgreSQL 插件pg_kmcb**

## **1.编译安装**

### **1.1 Windows**

直接打开pg_kmcb.vcxproj编译(VS2015或更高版本)<br/>
- **配置文件仅支持Release X64版本,如要编译其它版本请自行设置**<br/>
- **在"Release X64的属性页->生成事件->后期生成事件""请根据自己的环境修改自动部署命令**<br/>
- **[vs编译PostgreSQL插件基础](https://blog.csdn.net/kmblack1/article/details/80096021)**<br/>

编译成功后根据**""后期生成事件""**自动复制.<br />

手动安装步骤:<br />
- **复制pg_kmcb.dll到"pg安装目录\lib"**<br/>
- **复制pg_kmcb.control和pg_kmcb--1.0.sql到"pg安装目录\share\extension"**<br/>

### **1.2 Linux**
测试环境的PostgreSQL安装位置为usr/local/pgsql,请根据自己的安装目录修改.
```
make USE_PGXS=1 PG_CONFIG=/usr/local/pgsql/bin/pg_config
sudo make install USE_PGXS=1 PG_CONFIG=/usr/local/pgsql/bin/pg_config
```

## **2.使用方法**

必须以postgres用户登录到数据库<br/>
```
psql -h 127.0.0.1 -U postgres -d 数据库
```
创建扩展<br/>
```sql
create extension pg_kmcb
```
删除扩展<br/>
```sql
drop extension if exists pg_kmcb;
```
删除扩展使用的文件(自行修改文件路径)
- windows
```
del /q C:\pgsql\lib\pg_kmcb.so
del /q C:\pgsql\share\extension\pg_kmcb.control
del /q C:\pgsql\share\extension\pg_kmcb--1.0.sql
```
- Linux
```
sudo rm -f /usr/local/pgsql/lib/pg_kmcb.so &
sudo rm -f /usr/local/pgsql/share/extension/pg_kmcb.control &
sudo rm -f /usr/local/pgsql/share/extension/pg_kmcb--1.0.sql
```

## **3.函数定义**

[函数完整定义](https://github.com/kmblack1/pg_kmcb/blob/master/pg_kmcb--1.0.sql)

[小数修约](https://github.com/kmblack1/pg_kmcb/blob/master/pg_round.md)
- 四舍五入
- 四舍六入五成双
- 格式化为字符串

[字符函数](https://github.com/kmblack1/pg_kmcb/blob/master/pg_string.md)
- 按指定的字符个数切分字符串为数组
- 计算字符串CRC32
- 计算两个字符串的相似度
- 获取两个字符串相同的部份

[生成随机数据函数](https://github.com/kmblack1/pg_kmcb/blob/master/pg_random.md)
- 随机生成数字大小写字母组成的字符串

[AES加密解密](https://github.com/kmblack1/pg_kmcb/blob/master/pg_scrypto.md)
- 生成AES密钥
- 生成AES向量
- AES加密
- AES解密

**日期和流水号**
- 从YYDOY编号中获取YYDOY部分
- 从YYDOY编号中获取ID部分
- 从YYDOY编号中获取YYDOY和ID部分(一次性取出)
- 从YYDOY编号中获取日期

例如有下列流水号:
1800100000001
1811900000001
1811900000002
1811900000003
1811900000004
1811900000005

```sql
with cte as(
	select * from unnest(array[1800100000001,1811900000001,1811900000002,1811900000003,1811900000004,1811900000005]) as f
)select get_yydoy_from_sn(f,8),get_id_from_sn(f,8),get_yydoy_id_from_sn(f,8),get_date_from_sn(f,8) from cte
--output
18001	1	(18001,1)	2018-01-01
18119	1	(18119,1)	2018-04-29
18119	2	(18119,2)	2018-04-29
18119	3	(18119,3)	2018-04-29
18119	4	(18119,4)	2018-04-29
18119	5	(18119,5)	2018-04-29
```
