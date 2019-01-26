## 使用方法
* 安装好poky1.7之后，在build4.sh的目录下运行sh build4.sh 即可
* 若运行报错，则可手动在terminal里输入以下指令：
	* ```source /opt/poky/1.7/environment-setup-cortexa9hf-vfp-neon-poky-linux-gnueabi``` source后跟的路径一般不变
	* ```$CXX -I"../CW_CoreData_v1" -L ../CW_CoreData_v1/ -lcw_coredata -I"../CW_Shared_Lib_v1" -L ../CW_Shared_Lib_v1/ -lcw_shared_lib -lpthread -Wall -o testfil *.cpp```   注意，这里../样式的路径表示后面跟的lcw_文件所在目录，若CW文件夹在上一层，则格式不变，若CW文件夹在同一层，则../要改成./。同时，最后的-o testfil 应该是表示编译后的可执行文件名。
* 之后可以用scp命令将文件传到192.168.10.224,格式是```scp -r folder root@IP:directory```
* 目前上面已经有了（指CWAVEB170132的这一台），因此可以直接ssh上去运行，具体命令是 ```ssh 192.168.10.224 -l root``` 然后输入密码即可
* 登录之后在sdk文件夹下的demo_sdk即可看到testfil文件，运行```./testfil```即可，目前的信息好像全是0


* TODO：
	× 弄清楚为什么全是0
	× 编写c++，获取当前终端位置并且广播
	× 编写c++，获取位置并且通过tcp传给PC
