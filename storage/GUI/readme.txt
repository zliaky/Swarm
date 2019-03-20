> serial_wrapper folder
	- 新建serial_wrapper.h与serial_wrapper.cpp封装serial类便于unity调用。
	- 三个导出函数：
		bool initWrapper(); //初始化
		void loopWrapper(); //循环
		void getInfor(frame_w &f); //通过引用更新传入的结构体参数
	- VS2017项目，x64编译。
	- 编译完成的dll文件放入Unity项目文件夹的Assets>Plugins中
	
> Unity folder
	- sui.unitypakage: 安装好unity破解版后打开unity，直接双击此pakage文件导入项目即可，若有问题在unity新项目中assets>import pakage导入
	- SUI folder： 源文件。若sui pakage导入有问题，可直接到SUI>Assets>Scenes打开0.0
	- Unity 2017.3.0f3 (64-bit)
	
	- C#脚本都在Assets>Scripts中，只需要看Serial.sc即可。所有dll相关操作都在此脚本中，场景里挂在名叫Monitor的empty Gameobject上。
	- 还未将数据接到ui显示，若正常执行可以在控制台里面看到debug输出。