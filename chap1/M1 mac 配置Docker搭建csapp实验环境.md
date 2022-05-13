---
tags: [操作系统/01]
title: M1 mac 配置Docker搭建csapp实验环境
created: '2022-05-13T01:53:36.784Z'
modified: '2022-05-13T04:26:52.564Z'
---

# M1 mac 配置Docker搭建csapp实验环境

### 1.安装docker
[官网](https://www.docker.com/)

### 2.拉取linux系统
    docker pull ubuntu

### 3.建立建立目录挂载实现文件同步
因为Docker默认创建的是arm架构的ubuntu，如果没有指定为amd64，将无法执行 gcc -m32 等命令

    docker run -it --platform linux/amd64 -v /Users/shipengfei/Desktop/csapp:/csapp --name=csapp_env ubuntu /bin/bash
>/Users/xxxx/yourFilePath 请替换成你自己想要进行同步的目录
  :/csapp 也请替换成你自己想要命名的目录

### 4.安装必要的工具和库
1. 更新apt软件源

        apt-get update

2. 安装sudo

        apt-get install sudo

3. 安装c/c++编译环境
  build-essential软件包列表内包含libc6-dev、libc-dev、gcc、g++、make、dpkg等。

        sudo apt-get install build-essential

4. 补充gcc的完整环境(gcc-multilib)

        sudo apt-get install gcc-multilib

5. 安装gdb

        sudo apt-get install gdb

### 5.启动&退出环境

完成环境配置后，每次需要进行试验时执行如下命令即可，其它docker命令见官方文档https://docs.docker.com/engine/reference/commandline。

1. 重新运行容器实例

        docker container start csapp_env

2. 进入容器并打开bash

        docker container exec -it csapp_env /bin/bash

3. 退出环境

        CTRL+D或者使用exit命令即可从容器中退出

4. 停止容器
当不需要进行实验时使用kill命令终止实例的运行并释放系统资源

        docker container kill csapp_env

