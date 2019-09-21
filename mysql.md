# 创建用户
```
create user 'USER'@'HOST' identified by 'PASSWORD';
```
ps: mysql用户有两个字段构成，USER和HOST，HOST可以是`localhost`、`%`或者普通ip。其中%表示所有ip。


# 修改用户密码
```
update user set authentication_string=password('PASSWORD') where user='USER' and host='HOST';
```
ps：注意在5.7版本之前，密码是`password`字段而不是`authentication_string`字段。这里的`HOST`同创建用户的HOST

# 删除用户
```
DROP USER  'USER'@'HOST';
```


# 设置用户权限
```
grant all privileges on DATABASE.TABLE to 'USER'@'HOST' identified by 'PASSWORD';
flush privileges
```
* DATABASE : 数据库名称，`*`表示所有
* TABLE : 表名称，`*`表示所有
* USER : 用户名
* HOST : 访问msyql所在host的ip
* PASSWORD : 密码

设置后，可以通过如下命令查看
```
#查看一个用户的权限
show grants for USER

#或者查看mysql.user表
select Host,User from mysql.user;

# 结果如下
+----------------+------------------+
| Host           | User             |
+----------------+------------------+
| %              | cmcc             |
| %              | debian-sys-maint |
| %              | logsys           |
| %              | mysql.session    |
| %              | mysql.sys        |
| %              | root             |
| 127.0.0.1      | logsys           |
| 192.168.92.134 | logsys           |
| 192.168.92.134 | root             |
| localhost      | logsys           |
+----------------+------------------+
```

**注意**

就算在mysql.user表设置好某一HOST登录，还是被mysql拒绝，可能是配置文件绑定了127.0.0.1，默认只能通过本地登录。

解决方法
在`/etc/mysql/mysql.conf.d/mysqld.cnf` 注释掉`bind-address = 127.0.0.1`这一行
