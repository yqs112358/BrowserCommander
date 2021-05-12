# BrowerCommander - 脚本命令文档

## 浏览器命令

- `Go <地址>`

  前往指定网址

- `Back`

  浏览器动作：后退

- `Forward`

  浏览器动作：前进

- `Stop`

  浏览器动作：停止

- `Reload / Refresh`

  浏览器动作：刷新

- `Save <保存路径>`

  保存网页

- `Zoom <0.25~5.0>`

  浏览器动作：缩放

- `History [n]`

  输出（最近的n条）历史记录

- `GotoHistory <序号>`

  跳转到指定的历史记录

- `Title`

  输出当前页面的标题

- `ClearCookie`

  清空Cookie数据

- `ClearHistory`

  清空历史记录

- `ClearCache`

  清空浏览器缓存

- `UserAgent / ua [UA字符串]`

  输出 / 设置UserAgent字符串

- `PreviousTab / PrevTab`

  切换到上一个标签页

- `NextTab`

  切换到下一个标签页

- `Wait`

  等待直到页面加载完毕

- `NewTab`

  新建标签页

- `CloseTab`

  关闭标签页

## Js命令

- `Js <命令>`

  执行指定Js命令并输出返回结果

- `JsSend / Jss <命令>`

  执行指定Js命令（不等待结果）

- `JsBegin & JsEnd`

  批量执行Js命令

  用法：

  ```
  JsBegin
  ...(Js命令)
  ...(Js命令)
  ...
  JsEnd -> 开始执行
  ```

- `JsConsole`

  进入Js控制台模式，直接输入Js命令执行。输入Exit / Quit退出

- `AddJs <Begin / Ready / Deferred> <Js文件路径>`

  添加用户Js脚本，在接下来访问的所有页面中自动执行

  - Begin：页面开始加载时执行
  - Ready：DOM加载完成后执行
  - Deferred：空闲时执行

- `RemoveJs <Js文件名>`

  移除已经添加过的用户Js脚本

- `SetJsWorld <序号>`

  切换Js工作环境序号

  浏览器引擎为防止Js之间冲突而提供的运行环境隔离，默认所有网页的Js和用户自行注入的Js都在环境0中执行

## 控制命令

- `Sleep <毫秒>`

  控制线程休眠指定毫秒

- `SleepSec <秒>`

  控制线程休眠指定秒

- `Output / Log [文件路径]`

  重定向控制台输出至文件

  若留空则输出至控制台

- `Run <目标.bcs脚本路径>`

  执行.bcs脚本

- `Exit / Quit`

  退出程序（若在`JsConsole`模式中，则为退出`JsConsole`）

- `About`

  输出关于信息

- `Help / ? [命令]`

  输出帮助信息

- `ListWindow / lswnd`

  输出所有已存在的浏览器窗口

- `ChangeWindow / cwnd <序号>`

  切换当前控制的窗口

- `ClearScreen / cls`

  清空控制台缓存区

- `License`

  输出开源许可证

## 脚本命令

- `End`

  退出当前脚本（但不退出程序）

- `Print <信息>`

  输出信息到程序输出中

# 配置文件

配置文件位于程序目录的 `BrowserCommand.ini` ，相关配置选项的解释如下

``` ini
[Main]
; 默认输出写入的文件路径，留空则输出至控制台
DefaultOutput=
; 调试信息写入的文件路径，留空则不输出调试信息
DebugOutput=DebugOutputLatest.log
; 命令行回显选项，留空则为auto
; auto 仅在执行脚本时回显 / always 总是回显 / no 关闭回显
CommandEcho=auto
; 控制线程开始执行前的休眠时间，单位毫秒，留空则为0
StartWait=1000
; 放在此目录中的.bcs脚本将在程序开始时自动执行
AutoRunScriptsDir=./AutoRun/
; 放在此目录中的.js脚本将被自动添加到用户Js文件列表中
AutoAddJsDir=./AutoJs/
; 语言，留空则为auto，表示自动识别系统环境
; 否则须填入语言_地区的标准区域代号，如en_US, zh_CN等
Language=en_US

[Browser]
; 浏览器默认主页，留空则为空白页
FirstPage=www.baidu.com
; 是否默认进入隐私模式，1是 / 0否
DefaultPrivateMode=0
; 浏览器UserAgent字符串，留空则为默认值
UserAgent=
```

# 主程序命令行参数

- `[.bcs脚本文件路径]`

  在程序开始时自动执行的脚本

- `-h / --help / -?`

  输出帮助信息

- `-v / --version`

  输出版本信息