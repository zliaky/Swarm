# Swarm

### 文件结构
- successfulVersion：能成功运行的代码
- storage：中间过程的代码存档
   - location：接电脑的arduino收发信息（目前完成上行）
   - PWMNEW：每个机器人发送信息
   - serial：电脑端接收arduino通过串口发送的信息
   - SUI：unity顶层图形界面

### 信息传输格式&流程图
![data-flow-format](https://i.loli.net/2019/03/31/5ca07ee9e56da.png)
