#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ios>
#include <limits>
using namespace std;
#ifdef _WIN32
    #define CLEAR_SCREEN "cls"
    #define PAUSE "pause"
#else
    #define CLEAR_SCREEN "clear"
    #define PAUSE "read -n 1 -s -p \"按任意键继续...\""
#endif
// 基类：统一媒体接口
class Media {
public:
    string id, title, author, grade;
    virtual ~Media() = default;
    virtual void add() = 0;
    virtual void show() = 0;
    virtual void save(ofstream& ofs) = 0;
    virtual void load(ifstream& ifs) = 0;
};
// 图书类
class Book : public Media {
public:
    string publisher, isbn;
    int pages;
    void add() override {
        cout << "标题："; cin >> title;
        cout << "作者："; cin >> author;
        cout << "评级(未评级/一般/成人/儿童)："; cin >> grade;
        cout << "出版社："; cin >> publisher;
        cout << "ISBN："; cin >> isbn;
        cout << "页数："; cin >> pages;
    }
    void show() override {
        cout << "[图书] " << id << " | " << title << " | " << author << " | " << publisher << " | " << isbn << endl;
    }
    void save(ofstream& ofs) override {
        ofs << id << " " << title << " " << author << " " << grade << " " << publisher << " " << isbn << " " << pages << endl;
    }
    void load(ifstream& ifs) override {
        ifs >> id >> title >> author >> grade >> publisher >> isbn >> pages;
    }
};
// 视频类
class Video : public Media {
public:
    string producer, duration;
    int year;
    void add() override {
        cout << "标题："; cin >> title;
        cout << "作者："; cin >> author;
        cout << "评级："; cin >> grade;
        cout << "出品方："; cin >> producer;
        cout << "年份："; cin >> year;
        cout << "时长："; cin >> duration;
    }
    void show() override {
        cout << "[视频] " << id << " | " << title << " | " << author << " | " << producer << " | " << year << " | " << duration << endl;
    }
    void save(ofstream& ofs) override {
        ofs << id << " " << title << " " << author << " " << grade << " " << producer << " " << year << " " << duration << endl;
    }
    void load(ifstream& ifs) override {
        ifs >> id >> title >> author >> grade >> producer >> year >> duration;
    }
};
// 图画类
class Picture : public Media {
public:
    string country;
    int len, wid;
    void add() override {
        cout << "标题："; cin >> title;
        cout << "作者："; cin >> author;
        cout << "评级："; cin >> grade;
        cout << "国籍："; cin >> country;
        cout << "长度(cm)："; cin >> len;
        cout << "宽度(cm)："; cin >> wid;
    }
    void show() override {
        cout << "[图画] " << id << " | " << title << " | " << author << " | " << country << " | " << len << "x" << wid << "cm" << endl;
    }
    void save(ofstream& ofs) override {
        ofs << id << " " << title << " " << author << " " << grade << " " << country << " " << len << " " << wid << endl;
    }
    void load(ifstream& ifs) override {
        ifs >> id >> title >> author >> grade >> country >> len >> wid;
    }
};
// 核心管理类
class MediaManager {
private:
    static const int MAX = 70;
    Book books[MAX];
    Video videos[MAX];
    Picture pics[MAX];
    int bCnt = 0, vCnt = 0, pCnt = 0;
    // 检查ID是否重复
    bool isIdExist(string id) {
        for (int i = 0; i < bCnt; i++) if (books[i].id == id) return true;
        for (int i = 0; i < vCnt; i++) if (videos[i].id == id) return true;
        for (int i = 0; i < pCnt; i++) if (pics[i].id == id) return true;
        return false;
    }
public:
    // 添加媒体
    void addMedia() {
        string type;
        cout << "物品种类(图书/视频/图画)："; cin >> type;
        if ((type == "图书" && bCnt >= MAX) || (type == "视频" && vCnt >= MAX) || (type == "图画" && pCnt >= MAX)) {
            cout << type << "库已满！" << endl;
            return;
        }
        string id;
        while (true) {
            cout << "唯一编号："; cin >> id;
            if (!isIdExist(id)) break;
            cout << "编号重复！重新输入" << endl;
        }
        if (type == "图书") {
            books[bCnt].id = id;
            books[bCnt++].add();
        } else if (type == "视频") {
            videos[vCnt].id = id;
            videos[vCnt++].add();
        } else if (type == "图画") {
            pics[pCnt].id = id;
            pics[pCnt++].add();
        }
        cout << "添加成功！" << endl;
    }
    // 查询媒体
    void findMedia() {
        int opt;
        cout << "1.按ID  2.按标题  3.按类型："; cin >> opt;
        string key;
        bool found = false;
        if (opt == 1) {
            cout << "输入ID："; cin >> key;
            for (int i = 0; i < bCnt; i++) if (books[i].id == key) { books[i].show(); found = true; }
            for (int i = 0; i < vCnt; i++) if (videos[i].id == key) { videos[i].show(); found = true; }
            for (int i = 0; i < pCnt; i++) if (pics[i].id == key) { pics[i].show(); found = true; }
        } else if (opt == 2) {
            cout << "输入标题："; cin >> key;
            for (int i = 0; i < bCnt; i++) if (books[i].title == key) { books[i].show(); found = true; }
            for (int i = 0; i < vCnt; i++) if (videos[i].title == key) { videos[i].show(); found = true; }
            for (int i = 0; i < pCnt; i++) if (pics[i].title == key) { pics[i].show(); found = true; }
        } else if (opt == 3) {
            cout << "输入类型(图书/视频/图画)："; cin >> key;
            if (key == "图书") for (int i = 0; i < bCnt; i++) { books[i].show(); found = true; }
            else if (key == "视频") for (int i = 0; i < vCnt; i++) { videos[i].show(); found = true; }
            else if (key == "图画") for (int i = 0; i < pCnt; i++) { pics[i].show(); found = true; }
        }

        if (!found) cout << "无匹配结果！" << endl;
    }
    // 编辑媒体
    void editMedia() {
        string id;
        cout << "编辑ID："; cin >> id;
        for (int i = 0; i < bCnt; i++) if (books[i].id == id) {
            cout << "新标题："; cin >> books[i].title;
            cout << "新作者："; cin >> books[i].author;
            cout << "编辑完成！" << endl;
            return;
        }
        for (int i = 0; i < vCnt; i++) if (videos[i].id == id) {
            cout << "新标题："; cin >> videos[i].title;
            cout << "新作者："; cin >> videos[i].author;
            cout << "编辑完成！" << endl;
            return;
        }
        for (int i = 0; i < pCnt; i++) if (pics[i].id == id) {
            cout << "新标题："; cin >> pics[i].title;
            cout << "新作者："; cin >> pics[i].author;
            cout << "编辑完成！" << endl;
            return;
        }
        cout << "ID不存在！" << endl;
    }
    // 删除媒体
    void delMedia() {
        string id;
        cout << "删除ID："; cin >> id;
        // 删除图书
        for (int i = 0; i < bCnt; i++) if (books[i].id == id) {
            for (int j = i; j < bCnt-1; j++) books[j] = books[j+1];
            bCnt--; cout << "删除成功！" << endl; return;
        }
        // 删除视频
        for (int i = 0; i < vCnt; i++) if (videos[i].id == id) {
            for (int j = i; j < vCnt-1; j++) videos[j] = videos[j+1];
            vCnt--; cout << "删除成功！" << endl; return;
        }
        // 删除图画
        for (int i = 0; i < pCnt; i++) if (pics[i].id == id) {
            for (int j = i; j < pCnt-1; j++) pics[j] = pics[j+1];
            pCnt--; cout << "删除成功！" << endl; return;
        }
        cout << "ID不存在！" << endl;
    }
    // 显示全部
    void showAll() {
        cout << "\n===== 全部媒体 =====\n";
        for (int i = 0; i < bCnt; i++) books[i].show();
        for (int i = 0; i < vCnt; i++) videos[i].show();
        for (int i = 0; i < pCnt; i++) pics[i].show();
        cout << "总数量：" << bCnt + vCnt + pCnt << endl;
    }
    // 统计信息
    void showStat() {
        cout << "\n===== 媒体库统计信息 =====\n";
        cout << "图书数量：" << bCnt << " 条" << endl;
        cout << "视频数量：" << vCnt << " 条" << endl;
        cout << "图画数量：" << pCnt << " 条" << endl;
        cout << "总数量：" << bCnt + vCnt + pCnt << " 条" << endl;
    }
    // 保存到文件
    void saveAll() {
        ofstream bfs("books.txt"), vfs("videos.txt"), pfs("pics.txt");
        for (int i = 0; i < bCnt; i++) books[i].save(bfs);
        for (int i = 0; i < vCnt; i++) videos[i].save(vfs);
        for (int i = 0; i < pCnt; i++) pics[i].save(pfs);
        bfs.close(); vfs.close(); pfs.close();
        cout << "保存成功！" << endl;
    }
    // 从文件加载
    void loadAll() {
        ifstream bfs("books.txt"), vfs("videos.txt"), pfs("pics.txt");
        bCnt = 0; while (bfs.peek() != EOF) { books[bCnt].load(bfs); bCnt++; }
        vCnt = 0; while (vfs.peek() != EOF) { videos[vCnt].load(vfs); vCnt++; }
        pCnt = 0; while (pfs.peek() != EOF) { pics[pCnt].load(pfs); pCnt++; }
        bfs.close(); vfs.close(); pfs.close();
        cout << "加载成功！" << endl;
    }
};
// 主菜单（按要求修改）
void showMenu() {
    cout << "**************************************" << endl;
    cout << "******* 欢迎使用媒体库管理系统 *******" << endl;
    cout << "************  1、添加物品  ***********" << endl;
    cout << "************  2、查询物品  ***********" << endl;
    cout << "************ 3、显示物品库  ***********" << endl;
    cout << "************  4、编辑物品  ***********" << endl;
    cout << "************  5、删除物品  ***********" << endl;
    cout << "************  6、统计信息  ***********" << endl;
    cout << "************  7、物品存盘  ***********" << endl;
    cout << "************  8、读出物品  ***********" << endl;
    cout << "************  0、退出系统  ***********" << endl;
    cout << "**************************************" << endl;
}
// 主函数
int main() {
    MediaManager mgr;
    int opt;
    while (true) {
        showMenu();
        cout << "选择操作："; cin >> opt;
        switch (opt) {
            case 1: mgr.addMedia(); break;
            case 2: mgr.findMedia(); break;
            case 3: mgr.showAll(); break;
            case 4: mgr.editMedia(); break;
            case 5: mgr.delMedia(); break;
            case 6: mgr.showStat(); break;  // 新增统计信息功能
            case 7: mgr.saveAll(); break;
            case 8: mgr.loadAll(); break;
            case 0: cout << "退出成功！"; return 0;
            default: cout << "输入错误！" << endl;
        }
        system(PAUSE);
        system(CLEAR_SCREEN);
    }
    return 0;
}