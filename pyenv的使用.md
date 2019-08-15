# pyenv的安装

请阅读[https://github.com/pyenv/pyenv-installer](https://github.com/pyenv/pyenv-installer)

但安装后，还是会报找不到pyenv的问题，运行如下命令

```bash
echo 'export PATH=~/.pyenv/bin:$PATH' >> ~/.bashrc
echo 'export PYENV_ROOT=~/.pyenv' >> ~/.bashrc
echo 'eval "$(pyenv init -)"' >> ~/.bashrc
source ~/.bashrc
```

## pyenv的使用

### 在pyenv中安装python环境

```bash
pyenv install 3.4.7
```

### 布置pyenv的虚拟环境

这一步可以不做，因为pyenv安装某一个版本，就默认初始化一个虚拟环境，然后把对应版本的python安装到里面，虚拟环境以python版本命名。
但是考虑到同一个版本可能已有程序使用，这时默认的虚拟环境不是纯净的。所以需要手动布置pyenv的虚拟环境。

```bash
pyenv virtualenv 3.4.7 name
```

可以使用`pyenv versions`查看虚拟空间，其中`3.4.7/envs/cmlb-3.4.7`和`cmlb-3.4.7`是一个虚拟空间
```bash
$ pyenv versions
  system
* 3.4.7 (set by /data/home/frankshi/cmlb/.python-version)
  3.4.7/envs/cmlb-3.4.7
  cmlb-3.4.7
```
### cd到工作目录，切换python版本

```bash
pyenv local cmlb-3.4.7
```

**现在就是纯净的python环境了**
