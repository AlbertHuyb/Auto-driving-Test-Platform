CW_CommonTest 使用方法

1.运行build5.sh，会看到生成一个cw_commtest文件
2.将这个文件移动到设备中的opt/NL/CWAVE文件夹下
3.将所有的库都移动到上述文件夹下
4.修改cw_commtest权限，代码为：chmod 0774 [文件名]
5.配置环境，代码为export LD_LIBRARY_PATH=opt/NL/CWAVE
6.此时如果运行会报错，报错显示一条路径找不到或不存在，因此要增加一条路径以存放后续生成的records
7.将commtest.conf文件中的第一行commtype改成net
8.运行cw_commtest，代码为./文件名
----
TO DO:
将位置信息准确地在两台设备之间传递
将位置信息摘出来放在RVIZ中