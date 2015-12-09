#Othello
##简介
* 2015年计算概论大作业
* 实现了基本下子，对战，存读档，设置等功能
* 实现了3D显示~~其实2D版本也是有的，不过被移除了~~
* 控制台输入或者鼠标输入
* ~~随机算法AI~~

##编译事项
* `master` 分支应于Linux/Unix环境下编译；
* 编译前需安装库文件`freeglut3-dev`与`libglu1-mesa-dev`：
```
sudo apt-get install freeglut3-dev
sudo apt-get install libglu1-mesa-dev
```
* `Makefile`文件已经给出，在git仓库目录下键入命令`make`即可编译。

##操作指南
**注意事项：所有控制台操作输入后，必须键入回车以确定**
###有关开始界面
* 运行`bin/Othello`即可开始游戏（或者编译时使用命令`make test`)；
* 开始游戏后，请将重叠的图形窗口与控制台窗口拖动分离（如果它们重叠的话），并在控制台窗口内选择您喜爱的游戏模式（数字1-3）；
* 如果此前有过存档记录，可以键入数字`0`继续上次保存的游戏；
* 键入数字`4`可以进入设置菜单；
* 键入数字`-1`可以退出游戏。

###主游戏循环
* 黑白棋规则请移步[黑白棋_百度百科](http://baike.baidu.com/link?url=xHe8eg0s1iZDeTQ2gS3pu-6BVHFQJbACmsoP_H9-BfH8zmVIKSBHUTlO8kDgOFbhapcn2FCe291C6YP3YfCGjACygxVrNY0Zl6m7QWxjP47)；
* 在*控制台输入模式*下，请先键入列所对应的字母，再键入行所对应的数字以键入您想要落子的位置；
* 在*图形界面输入模式*下，请将鼠标移至图形窗口中，仔细的观察窗口中的桌子上的光标位置，移动鼠标以改变光标位置；当您移动到合适的位置时，按下鼠标左键以落子；
* 当您无子可下的时候，您可能需要在控制开窗口键入一个回车，以表示您确实知道自己无子可下；
* 当游戏进行到无法继续进行时，游戏将自动结算胜负情况，并打印在控制台窗口中；键入回车已确认。

###悔棋
* 当您处于*控制台输入模式*时，键入`uu`来回到上一步操作；
* 当您处于*图形界面输入模式*时，右键单击窗口内部来回到上一步操作；
* 这个时候，对应游戏模式的不同，可能出现以下两种情况：
```
1. 当您与A对弈的时候，将回到您最近一次可以落子的局面；
2. 当您与好友对弈的时候，将回到您二人最近一次某一人可以落子的局面；
```
