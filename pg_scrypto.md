# AES 128位CBC加密函数

## 生成KEY和IV
```
	select generate_aes_key(),generate_aes_iv();
```
KEY和IV为BASE64字符串。<br />
生成的KEY和IV自行保存，可改造函数，将KEY或IV再次加密后保存到U盘或其它外部设备，提高安全性。<br />
示例中<br />
	KEY：7NOUvRJkWAWNBYWamMVhyA==<br />
	IV:o3B6xQ1FQwLXttBrGVy0Ug==<br />

## 加密字符串
```
select aes_encrypt('7NOUvRJkWAWNBYWamMVhyA==','o3B6xQ1FQwLXttBrGVy0Ug==','工作是高端大气上档次，工资是低调奢华接地气！');
select aes_encrypt('7NOUvRJkWAWNBYWamMVhyA==','o3B6xQ1FQwLXttBrGVy0Ug==','我采用PKCS#7的填充方式');
```
## 解密字符串
```
select aes_decrypt('7NOUvRJkWAWNBYWamMVhyA==','o3B6xQ1FQwLXttBrGVy0Ug==','YPdpeaIxrRB14wWv2fIVpRuOXwpAmCeVRsAesx40GjQ=');
```
## 性能测试
```
select aes_decrypt('7NOUvRJkWAWNBYWamMVhyA==','o3B6xQ1FQwLXttBrGVy0Ug==',
	aes_encrypt('7NOUvRJkWAWNBYWamMVhyA==','o3B6xQ1FQwLXttBrGVy0Ug==','3、问：怎样才算一个合格的上班族呢？答：不以雾喜，不以挤悲。')
) from generate_series(1,10000);
```
