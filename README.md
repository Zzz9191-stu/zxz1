# ZXZ · 项目集合

一个正在积累的前端小项目仓库，每个项目都是一个独立的单文件 HTML，打开即用。

## 在线预览

**入口**：[https://zzz9191-stu.github.io/zxz1/](https://zzz9191-stu.github.io/zxz1/)

## 项目列表

### 工具类

| 项目 | 说明 | 在线地址 |
| --- | --- | --- |
| 媒体库管理系统 | 图书 / 视频 / 图画的增删改查、统计、本地存储，附 C++ 控制台版本 | [打开](https://zzz9191-stu.github.io/zxz1/media-library.html) |
| 画板涂鸦板 | 选色、粗细调节、橡皮擦、一键保存 PNG | [打开](https://zzz9191-stu.github.io/zxz1/sketch-board.html) |
| 实时时钟 | 大字体时钟，根据真实时间自动切换白天 / 黑夜模式 | [打开](https://zzz9191-stu.github.io/zxz1/clock.html) |

### 动画类

| 项目 | 说明 | 在线地址 |
| --- | --- | --- |
| 黑客帝国数字雨 | Canvas 字符流，鼠标划过扰动字符 | [打开](https://zzz9191-stu.github.io/zxz1/matrix-rain.html) |
| 鹈鹕扛车跑 | 纯 SVG 2D 循环动画，可暂停 | [打开](https://zzz9191-stu.github.io/zxz1/pelican.html) |

## 仓库结构

```
zxz1/
├── index.html          # 导航首页（自动渲染项目列表）
├── media-library.html  # 媒体库管理系统
├── sketch-board.html   # 画板涂鸦板
├── clock.html          # 实时时钟
├── matrix-rain.html    # 数字雨
├── pelican.html        # 鹈鹕动画
├── zxz.cpp             # 媒体库 C++ 原始版本
├── zxz_v2.cpp          # 媒体库 C++ 完善版
└── README.md
```

## 技术栈

- **前端**：原生 HTML / CSS / JavaScript，无框架，无构建工具
- **图形**：Canvas、SVG
- **后端**：无，纯静态页面
- **部署**：GitHub Pages

## 本地运行

每个 `.html` 文件双击即可在浏览器打开，无需本地服务器。

C++ 版本编译运行：

```bash
g++ -std=c++11 zxz_v2.cpp -o media_library
./media_library
```
