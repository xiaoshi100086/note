# 安装方法

## 使用`pip`进行安装
  ```bash
  显示已经安装了那些包
  pip list
  
  查看某一个包的细节
  pip show somepackage
  
  搜索某一个包
  pip search somepackage
  
  安装某一个包
  pip install somepackage
  
  卸载某一个包
  pip uninstall somepackage
  ```
  
  ## 下载源码进行安装
  
  1. 首先解码源码，找到源码中的`setup.py`文件
  
  1. 然后运行`python setup.py build`
    这一步会在源码的根目录中创建文件夹`build`、`build/lib`、`build/lib/package-name`，然后把一些文件拷贝进去。
    
  1. 最后运行`python setup.py install`
    这一步则会把`build/lib/package-name`文件夹拷贝到`/usr/local/lib`或者`~/.local/lib`
    
  1. 安装完毕后，可以使用`pip list`进行查看
  
  ## 下载whel文件进行安装
