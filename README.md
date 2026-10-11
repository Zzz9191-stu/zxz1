# 媒体库管理系统

C++ 课程设计：基于多态的媒体库管理系统，同时提供网页版。

## 功能

- 三类媒体统一管理：**图书** / **视频** / **图画**
- 增、删、改、查
- 按编号 / 标题 / 类型 / 评级 查询
- 数量统计与类型分布
- 数据存盘 / 读出（C++ 版用 txt 文件，网页版用浏览器 localStorage + JSON 导入导出）

## 文件说明

| 文件 | 说明 |
| --- | --- |
| `zxz.cpp` | 原始版本 |
| `zxz_v2.cpp` | 完善版（修复 EOF 读取 bug、增加输入校验、编辑全部字段、删除二次确认、按评级查询） |
| `media-library.html` | 网页版（单文件，双击即可打开） |
| `index.html` | 仓库原有内容，保留 |

## 运行方式

### C++ 版本

```bash
g++ -std=c++11 zxz_v2.cpp -o media_library
./media_library
```

运行后会在当前目录生成 `books.txt` / `videos.txt` / `pics.txt` 作为数据文件。

### 网页版

直接用浏览器打开 `media-library.html` 即可。数据自动保存在浏览器 localStorage，也可通过"存盘/读出"页导出 JSON 备份。

## 技术栈

- C++11，面向对象，纯虚函数实现多态
- HTML / CSS / 原生 JavaScript，无框架
