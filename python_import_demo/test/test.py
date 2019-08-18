#!/usr/bin/python
# -*- coding: UTF-8 -*-

# 1. 系统文件，直接导入
import sys
# 2. 同文件夹下的py，直接导入
import hello
# 3. 同文件夹下的包或者子包，直接导入
import some_package1.hello
# 4. 可以通过修改sys.path参数，增加包路径，本次运行py有效
sys.path.append("/root/vscode/python-test")
import some_package2.hello
# 5. 可以通过设置环境变量PYTHONPATH，增加包路径
# 设置环境变量，才可以正常import，export PYTHONPATH=/root/vscode/python-test/some_package2，但是只有本次生效
# 如果想永久生效，最好写入"~/.bashrc"，然后"source ~/.bashrc"
import some_package3.hello
# 6. 可以通过设置site-packages目录下的.pth文件，增加包路径，永久生效
# 6.1 先找到site-packages安装路径
# python -c "import site;print(site.getsitepackages())"
# 6.2 进入python安装路径下的site-packages
# cd /root/.pyenv/versions/3.7.0/lib/python3.7/site-packages
# 6.3 编辑.pth，加入路径
# /root/vscode/python-test/some_package2/some_package3
import some_package4.hello
# 7. 第三方库
import aiohttp


if __name__ == "__main__":
    hello.say_hello()
    some_package1.hello.say_hello()
    some_package2.hello.say_hello()
    some_package3.hello.say_hello()
    some_package4.hello.say_hello()
