### docker ps
查看当前运行的docker容器

### docker imagers
查看本机所有的docker镜像

### docker stop CONTAINER_NAME/CONTAINER_ID
根据容器名称/容器ID来停止容器

### docker rm CONTAINER_NAME/CONTAINER_ID
容器的删除

运行中的Docker容器是无法删除的

## docker暂存修改的方法
1. 使用run -d命令让docker在后台运行

    `docker run -d --name NAME -p 10022:22 XXX`
    
    * -d 表示 Run container in background and print container ID
    * --name 表示 Assign a name to the container 方便使用记忆
    * -p 表示是容器内部端口绑定到指定的主机端口，先是宿主机端口，再是docker端口
    
1. 使用exec -it命令再开一个新进程进入容器，这样退出不会最终退出容器。

    `docker exec -it NAME bash`
    
    * -i 表示 Keep STDIN open even if not attached
    * -t 表示 Allocate a pseudo-TTY
   
### docker search ubuntu
搜索一个镜像

### docker pull ubuntu:13.10
获取一个新的镜像

### docker commit -m="has update" -a="runoob" e218edb10161 runoob/ubuntu:v2
提交一个镜像

* -m 表示 提交的描述信息
* -a 表示 指定镜像作者
* e218edb10161 表示 容器ID
* runoob/ubuntu:v2 表示 指定要创建的目标镜像名
