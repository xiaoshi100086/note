# python包的搜索路径

搜索路径见sys.path

1. 首先是当前目录，如果是交互式场景，为空
2. 然后查找环境变量PYTHONPATH指定的路径
3. 接着是python默认的搜索路径，这个跟python安装有关，一般是`/usr/local/lib/python3.7/site-packages`
4. 最后是在python的site-packages文件夹下的`.pth`指明的路径