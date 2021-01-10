# Othello
## <a name="index"/>目录
* [简介](#intro)
* [编译事项](#compile)
* [操作指南](#op)
  * [开始界面](#start)
  * [主游戏循环](#main)
  * [悔棋](#regret)
  * [游戏菜单](#menu)
    * [控制台输入模式](#console)
    * [图形界面输入模式](#graphic)
  * [设置选项](#settings)
  * [特色功能](#feature)

## <a name="intro"/>简介
* 2015 年计算概论大作业（的 ~~史诗级~~ 加强版）
* 实现了基本下子，对战，存读档，设置等功能
* 实现了3D显示 ~~其实2D版本也是有的，不过被移除了~~
* 控制台输入或者鼠标输入
* 简单 alpha-beta 剪枝 AI

## <a name="compile"/>编译事项
* `master` 分支应于 Linux/Unix 环境下编译；
* 编译前需安装 GLFW 库文件：
```Bash
sudo pacman -S glfw-x11
```
* Native 版本请用 cmake 编译，wasm 版本用 `make bin-em/Othello.html` 编译。

## <a name="op"/>操作指南

**注意事项：所有控制台操作输入后，必须键入回车以确定**

### <a name="start"/>开始界面
* 运行 `bin/Othello` 即可开始游戏（或者编译时使用命令 `make test` )；
* 开始游戏后，请将重叠的图形窗口与控制台窗口拖动分离（如果它们重叠的话），并在控制台窗口内选择您喜爱的游戏模式：

| 游戏模式                                  | 详细说明       |
| ----------------------------------------- | -------------- |
| 1. Play with Friends                      | 与好友对弈     |
| 2. Play with Watch_Doge                   | 与内置 AI 对弈；您必须之后键入 `b`（或 `B`）以表示您选择黑方，或者键入 `w`（或 `W`）以表示您选择白方 |
| 3. Watch\_Doge plays with Watch\_Doge     | 观看内置 AI 对弈 |

* 如果此前有过存档记录，可以键入数字 `0` 继续上次保存的游戏；
* 键入数字 `4` 可以进入设置菜单；
* 键入数字 `-1` 可以退出游戏。

### <a name="main"/>主游戏循环

* 黑白棋规则请移步 [黑白棋 百度百科](http://baike.baidu.com/link?url=xHe8eg0s1iZDeTQ2gS3pu-6BVHFQJbACmsoP_H9-BfH8zmVIKSBHUTlO8kDgOFbhapcn2FCe291C6YP3YfCGjACygxVrNY0Zl6m7QWxjP47) ；
* 在*控制台输入模式*下，请先键入列所对应的字母，再键入行所对应的数字以键入您想要落子的位置；
* 在*图形界面输入模式*下，请将鼠标移至图形窗口中，仔细的观察窗口中的桌子上的光标位置，移动鼠标以改变光标位置；当您移动到合适的位置时，按下鼠标左键以落子；
* 当您无子可下的时候，您可能需要在控制台窗口键入一个回车，以表示您确实知道自己无子可下；
* 当游戏进行到无法继续进行时，游戏将自动结算胜负情况，并打印在控制台窗口中；键入回车以确认。

### <a name="regret"/>悔棋
* 当您处于*控制台输入模式*时，键入 `uu` 来回到上一步操作；
* 当您处于*图形界面输入模式*时，右键单击窗口内部来回到上一步操作；
* 这个时候，对应游戏模式的不同，可能出现以下两种情况：
```
1. 当您与AI对弈的时候，将回到您最近一次可以落子的局面；
2. 当您与好友对弈的时候，将回到您二人最近一次某一人可以落子的局面；
```

### <a name="menu"/>游戏菜单
#### <a name="console"/>控制台输入模式

**此时设置界面下 `3.Input by GUI` 选项开关应显示 `Off`。**
* 当轮到您下棋的时候，你可以键入 `menu` 进入菜单界面：
* 键入 `1` 以保存游戏；您可能需要键入回车已确认存档以保存；
* 键入 `2` 以加载存档，恢复上一次保存的游戏状态；您可能需要键入回车已确认存档以加载；
* 键入 `3` 以保存游戏并且回到主界面；
* 键入 `4` 以退出游戏并且回到主界面，但是不写入存档；**注意：从上次存档起任何操作影响都将丢失！**
* 键入 `5` 以进入设定界面；
* 键入 `-1` 以回到游戏。

#### <a name="graphic"/>图形界面输入模式
**此时设置界面下 `3.Input by GUI` 选项开关应显示 `On`。**
* 在游戏进行的任何时刻，您都可以按下 `Esc` 键进入图形界面菜单；
* 点击红色框以保存游戏；保存后将直接返回游戏；
* 点击黄色框以加载存档，恢复上一次保存的游戏状态；
* 点击绿色框以保存游戏并且回到主界面；
* 点击蓝色框以退出游戏并且回到主界面，但是不写入存档；**注意：从上次存档起任何操作影响都将丢失！**
* 再次按下 `Esc` 键后将继续游戏。

### <a name="settings"/>设置选项
* 键入选项对应的数字以切换选项开关：

| 选项                    | `On` 代表的含义          | `Off` 代表的含义        |
| ----------------------- | ----------------------- | ---------------------- |
| 1. Game Assistance      | 落子辅助开启            | 落子辅助关闭           |
| 2. Show Big Balls(Cool) | 棋子外围显示线框球      | 棋子外围不显示线框球   |
| 3. Input by GUI         | 图形界面鼠标输入        | 控制台界面键盘输入     |
| 4. Ctrl bullet time     | 按住 Ctrl 键进入子弹时间  | 使 Ctrl 键的操作无效     |
| 5. Shift bullet time    | 按住 Shift 键进入子弹时间 | 使 Shift 键的操作无效    |
| 6. Fancy updateLights         | 图形界面中开启炫酷灯光  | 图形界面中关闭炫酷灯光 |
| 7. Show Axises          | 图形界面中显示坐标轴    | 图形界面中不显示坐标轴 |

* 键入`-1`以回到先前的位置。

### <a name="feature"/>特色功能
* 若您与好友（或者内置 AI）对弈，并且在**正在游戏的过程**中，感到大势已去，可以按下键盘上的 `l` 键（此时图形窗口必须被激活），进入掀棋子模式；
* 在**掀棋子模式**下，当您打开 `4.Ctrl bullet time` 开关或是 `5.Shift bullet time` 开关时，您可以**按住**开关对应的按键激活子弹模式；
* 在**掀棋子模式**下，您可以按住鼠标左键并拖动切换视角，已查看那些被您掀到地面上的棋子；
* 在**正在游戏**或者**掀棋子的过程**中，您可以使用鼠标滚轮进行视角缩放，不论此时 `3.Input by GUI` 是否被开启。
