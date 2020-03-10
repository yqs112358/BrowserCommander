# BrowserCommander BC浏览器![GitHub license](https://img.shields.io/badge/license-BSD--2--Clause-blue) ![Qt5.12 LTS](https://img.shields.io/badge/Qt-5.12%20LTS-orange) ![Releases version](https://img.shields.io/github/v/release/yqs112358/BrowserCommander?color=green&include_prereleases)
## 项目介绍
一个可用命令行或脚本控制的小巧编写的浏览器。可以方便地利用脚本实现多种自动化任务，自动签到，定时打卡，乃至脚本刷分，发挥创造力来解放你的双手！

源于对按键精灵老旧浏览器插件的遗憾而自行开发，项目基于Qt开源框架（Qt5.12 LTS），由Qt Example提供的简单浏览器为蓝本打造。浏览器采用Chromium内核，支持各种最新前端特性，同时远离种种卡顿与不兼容的困扰。

###### 目前项目尚处于初级开发阶段，学生党更新代码不易。。。   ------> 欢迎大家积极贡献ideas和代码！:smiley:
------------
发布版下载地址：[Github Releases](https://github.com/yqs112358/BrowserCommander/releases "Github Release")

说明：由于当初的架构设计缺陷（消息循环与控制台读写不能共存）故采用单独线程创建控制台与浏览器交互，这导致一定程度的代码复杂以及用户体验问题。后续版本计划单独拆分出控制台进程，使用C/S模型通信以解决上述缺陷同时避免控制台操作以简化跨平台移植难度。

## 运行截图
![Example.JPG](https://i.loli.net/2020/03/10/x1z5dawZnlvDKE6.jpg)

## 命令列表


## 实用脚本实例
######  作者开发中。。。
