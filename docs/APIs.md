# BrowserCommander - 开发与使用文档

## 概述

### 简介

`BrowserCommander` 是一个轻量级、可以使用脚本控制的的浏览器。可以方便地利用脚本实现多种自动化任务。

浏览器使用名为`BCS脚本`的脚本文件来实现自动化操作。`BCS脚本`底层使用JavaScript引擎，在其提供的强大编程能力的基础上加入了浏览器控制和大量的辅助API，使开发更加轻松友好。

下面是`BCS脚本`开发文档的相关内容。首先，是一些文档概述。

### 通用数据类型约定

>  虽然脚本语言是弱类型的，不需要关注具体的数据类型，但为了文档描述清晰起见，下面先描述了一些通用的数据类型。请首先务必熟悉这些数据类型称呼的意思。  

- `Null` - 空（类似于 `null` 和 `undefined`）
- `Number` - 整数 / 实数（小数，浮点数）
- `String` - 字符串
- `ByteBuffer` - 字节串
- `Boolean` - 布尔型
- `Function` - 函数
- `Array` - 数组
- `Object` - 对象

### API文档描述约定

1. 函数参数按照 **参数名 : 参数类型** 格式描述
   例如： cmd : `String` 表示一个**字符串**类型的变量cmd
   如果参数类型出现`Array<...>`表示一个以<>内的变量为内容的数组
2. 如果在参数描述处出现 `可选参数` 则代表你可以不传入这个参数。
   当你不传入这个参数的时候，引擎将使用描述中给出的默认值。



## 浏览器控制 API

> 这些API为BCS脚本提供了控制浏览器的能力

### 前往指定网址

`browser.go(addr)`

- 参数：
  1. addr: `String`
     目标网址
- 返回值：是否成功前往
- 返回值类型：`Boolean`

### 浏览器动作：后退

`browser.back()`

- 参数：

  无

- 返回值：是否成功执行

- 返回值类型：`Boolean`

### 浏览器动作：前进

`browser.forward()`

- 参数：

  无

- 返回值：是否成功执行

- 返回值类型：`Boolean`

### 浏览器动作：停止

`browser.stop()`

- 参数：

  无

- 返回值：是否成功执行

- 返回值类型：`Boolean`

### 浏览器动作：刷新

`browser.refresh()`

- 参数：

  无

- 返回值：是否成功执行

- 返回值类型：`Boolean`

### 浏览器动作：缩放

`browser.zoom(scale)`

- 参数：
  1. scale: `Number`
     缩放因子，浮点数，有效范围0.25 - 5.0，1.0为正常大小
- 返回值：是否成功执行
- 返回值类型：`Boolean`

### 获取浏览器历史记录

`browser.getHistory(num)`

- 参数：
  1. num: `Number`
     整数，返回最近的`num`条历史记录
- 返回值：最近的`num`条历史记录链接数组
- 返回值类型：`Array<String,String,...>`

### 打开一个新标签页（并切换过去）

`browser.newTab()`

- 参数：

  无

- 返回值：是否成功打开

- 返回值类型：`Boolean`

### 切换到下一个标签页

`browser.nextTab()`

- 参数：

  无

- 返回值：是否成功切换

- 返回值类型：`Boolean`

### 切换到上一个标签页

`browser.prevTab()`

- 参数：

  无

- 返回值：是否成功切换

- 返回值类型：`Boolean`

### 关闭当前标签页（并切换到上一个）

`browser.closeTab()`

- 参数：

  无

- 返回值：是否成功关闭

- 返回值类型：`Boolean`

### 等待当前页面加载完毕

`browser.wait()`

- 参数：

  无

- 返回值：是否等待成功

- 返回值类型：`Boolean`



## JavaScript注入 API

> 这些API为BCS脚本提供了向页面中注入Js脚本并执行的能力。注入脚本是BCS自动化测试体系中的核心功能，用于在实际代码中完成模拟操作等核心调试功能

### 在当前页面中执行 JavaScript 代码 

`browser.exec(code)`

- 参数：
  1. code: `String`
     待执行的JavaScript代码
- 返回值：代码（段）的返回值
- 返回值类型：任意可能的类型
  - 如返回值为 `Null` 则表示执行失败

### 在当前页面中执行 JavaScript 代码 （异步）

`browser.exec(code,callback)`

- 参数：

  1. code: `String`
     待执行的JavaScript代码

  2. callback : `Function`

     执行完成之后被调用的回调函数。函数原型：

     `CallbackFunc(result)`

     参数 `result` 为代码（段）的返回值，类型可为任意类型

- 返回值：是否成功发送异步执行请求

- 返回值类型：`Boolean`



## 浏览器设置 API

> 这些API为BCS脚本提供了修改部分浏览器设置的能力

### 设置UserAgent字符串

`settings.setUa(ua)`

- 参数：

  1. ua: `String`
     `UserAgent`字符串代码

- 返回值：是否成功设置
- 返回值类型：`Boolean`

### 清空Cookie数据

`settings.clearCookie()`

- 参数：

  无

- 返回值：是否成功清空

- 返回值类型：`Boolean`

### 清空历史记录数据

`settings.clearHistory()`

- 参数：

  无

- 返回值：是否成功清空

- 返回值类型：`Boolean`

### 清空浏览器缓存数据

`settings.clearCache()`

- 参数：

  无

- 返回值：是否成功清空

- 返回值类型：`Boolean`



## 文件系统 API

> 这些API为BCS脚本提供了操作文件系统的能力

> 注：所有文本相关的操作均使用UTF-8编码，所有传入相对路径都以程序工作目录为基准

### 读入文件的所有内容

`file.read(path)`

- 参数：
  1. path : `String`
     目标文件的路径
- 返回值：文件的所有数据
- 返回值类型：`String`
  - 如返回值为 `Null` 则表示读取失败
  - 若文件不存在则会读取失败

### 向指定文件写入内容

`file.write(path,text)`

- 参数：
  1. path : `String`
     目标文件的路径
  2. text : `String` 
     要写入的内容
- 返回值：是否写入成功
- 返回值类型：`Boolean` 
  - 若文件不存在会自动创建，若存在则会先将其**清空**再写入

### 向指定文件追加一行

`file.writeLine(path,text)`

- 参数：
  1. path : `String` 
     目标文件的路径
  2. text : `String` 
     要写入的内容  
- 返回值：是否写入成功
- 返回值类型：`Boolean`
  - 若文件不存在会自动创建

### 创建文件夹  

`file.createDir(dir)`

- 参数：
  1. dir : `String` 
     目标文件夹的路径 
     可以直接创建多层，不需要逐层创建
- 返回值：是否成功创建
- 返回值类型：`Boolean` 

### 删除文件 / 文件夹  

`file.delete(path)`

- 参数：
  1. path : `String` 
     目标文件 / 文件夹的路径
- 返回值：是否成功删除
- 返回值类型：`Boolean` 

### 判断文件 / 文件夹是否存在  

`file.exists(path)`

- 参数：
  1. path : `String`
     目标文件 / 文件夹的路径
- 返回值：目标是否存在
- 返回值类型：`Boolean` 

### 复制文件 / 文件夹到指定位置  

`file.copy(from,to)`

- 参数：
  1. from : `String`
     源文件 / 文件夹的路径
  2. to : `String`
     目标文件 / 文件夹的位置
- 返回值：是否复制成功
- 返回值类型：`Boolean`

### 移动文件 / 文件夹到指定位置  

`file.move(from,to)`

- 参数：
  1. from : `String`
     源文件 / 文件夹的路径
  2. to : `String` 
     目标文件 / 文件夹的位置
- 返回值：是否复制成功
- 返回值类型：`Boolean` 



## 其他辅助 API

> 这些API是BCS脚本中其他的一些辅助功能

### 控制流休眠指定的时间  

`sleep(msec)`

- 参数：
  1. msec : `Number`
     休眠的时间，单位为**毫秒**
- 返回值：无

### 重定向 console 输出到指定文件

`setOutputPath(path)`

- 参数：
  1. path : `String`
     目标输出文件路径，如果为空字符串则表示切换回控制台输出
- 返回值：是否重定向成功
- 返回值类型：`Boolean` 

### 执行另一个BCS脚本

`runBcs(path)`

- 参数：
  1. path : `String`
     目标BCS脚本文件路径
- 返回值：是否重定向成功
- 返回值类型：`Boolean` 

### 输出帮助信息

`help()`

- 参数：

  无

- 返回值：无 

  - 输出帮助信息

### 退出程序

`exit()`

- 参数：

  无

- 返回值：无 

# 配置文件

配置文件位于程序目录的 `BrowserCommand.ini` ，相关配置选项的解释如下

``` ini
[Main]
; 放在此目录中的.bcs脚本将在程序开始时自动执行
AutoRunScriptsDir=./AutoRun/
; 放在此目录中的.js脚本将被自动添加到用户Js文件列表中
AutoAddJsDir=./AutoJs/
; 语言，留空则为auto，表示自动识别系统环境
; 否则须填入语言_地区的标准区域代号，如en_US, zh_CN等
Language=zh_CN

[Browser]
; 浏览器默认主页，留空则为空白页
FirstPage=www.baidu.com
; 是否默认进入隐私模式，1是 / 0否
DefaultPrivateMode=0
; 浏览器UserAgent字符串，留空则为默认值
UserAgent=

[Advanced]
; 控制线程开始执行前的休眠时间，单位毫秒，留空则为0
StartWait=1000
; 默认输出写入的文件路径，留空则输出至控制台
DefaultOutput=
; 调试信息写入的文件路径，留空则不输出调试信息
DebugOutput=DebugOutputLatest.log
```

# 主程序命令行参数

- `[.bcs脚本文件路径]`

  在程序开始时自动执行的脚本

- `-h / --help / -?`

  输出帮助信息

- `-v / --version`

  输出版本信息