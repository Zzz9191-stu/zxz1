/*
 * 媒体库管理系统（完善版）
 * 基于原 zxz.cpp 修改，修复与增强：
 *   1. 修复 loadAll() 用 peek()!=EOF 在空文件时多读一条导致数组越界的 bug
 *   2. 增加输入校验：编号非空、页数/年份/尺寸为正整数
 *   3. 查询支持按评级筛选
 *   4. 编辑功能扩展到全部字段（原版本只能改标题、作者）
 *   5. 删除前二次确认
 *   6. 主菜单循环内非法输入不会死循环（cin 失败时清空缓冲区）
 *   7. 文件保存失败时给出提示
 * 编译：g++ -std=c++11 zxz_v2.cpp -o media_library
 */
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

// ---------- 工具函数 ----------
// 安全读取一行（用 getline，避免 cin>> 遇到空格就截断）
static string readLine(const string& prompt) {
    cout << prompt;
    string s;
    // 先吃掉可能残留的换行
    if (cin.peek() == '\n') cin.ignore();
    getline(cin, s);
    return s;
}
// 读取正整数，输入非法会重新提示
static int readPositiveInt(const string& prompt) {
    int v;
    while (true) {
        cout << prompt;
        if (cin >> v && v > 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return v;
        }
        cout << "请输入大于 0 的整数！" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}
// 暂停 + 清屏
static void pauseClear() {
    system(PAUSE);
    system(CLEAR_SCREEN);
}

// ---------- 基类：统一媒体接口 ----------
class Media {
public:
    string id, title, author, grade;
    virtual ~Media() = default;
    virtual void add() = 0;
    virtual void show() = 0;
    virtual void edit() = 0;                       // 新增：编辑全部字段
    virtual void save(ofstream& ofs) = 0;
    virtual void load(ifstream& ifs) = 0;
    virtual string typeName() = 0;
};

// ---------- 图书类 ----------
class Book : public Media {
public:
    string publisher, isbn;
    int pages = 0;
    string typeName() override { return "图书"; }
    void add() override {
        title  = readLine("标题：");
        author = readLine("作者：");
        grade  = readLine("评级(未评级/一般/成人/儿童)：");
        publisher = readLine("出版社：");
        isbn   = readLine("ISBN：");
        pages  = readPositiveInt("页数：");
    }
    void edit() override {
        cout << "（直接回车保留原值）" << endl;
        string s;
        s = readLine("新标题 [" + title + "]：");  if (!s.empty()) title = s;
        s = readLine("新作者 [" + author + "]："); if (!s.empty()) author = s;
        s = readLine("新评级 [" + grade + "]：");  if (!s.empty()) grade = s;
        s = readLine("新出版社 [" + publisher + "]："); if (!s.empty()) publisher = s;
        s = readLine("新 ISBN [" + isbn + "]：");  if (!s.empty()) isbn = s;
        string sp = readLine("新页数 [" + to_string(pages) + "]：");
        if (!sp.empty()) pages = stoi(sp);
    }
    void show() override {
        cout << "[图书] " << id << " | " << title << " | " << author
             << " | " << publisher << " | ISBN " << isbn
             << " | " << pages << "页 | " << grade << endl;
    }
    void save(ofstream& ofs) override {
        ofs << id << " " << title << " " << author << " " << grade << " "
            << publisher << " " << isbn << " " << pages << endl;
    }
    void load(ifstream& ifs) override {
        ifs >> id >> title >> author >> grade >> publisher >> isbn >> pages;
    }
};

// ---------- 视频类 ----------
class Video : public Media {
public:
    string producer, duration;
    int year = 0;
    string typeName() override { return "视频"; }
    void add() override {
        title  = readLine("标题：");
        author = readLine("作者：");
        grade  = readLine("评级：");
        producer = readLine("出品方：");
        year   = readPositiveInt("年份：");
        duration = readLine("时长(如 90min)：");
    }
    void edit() override {
        cout << "（直接回车保留原值）" << endl;
        string s;
        s = readLine("新标题 [" + title + "]：");   if (!s.empty()) title = s;
        s = readLine("新作者 [" + author + "]：");  if (!s.empty()) author = s;
        s = readLine("新评级 [" + grade + "]：");   if (!s.empty()) grade = s;
        s = readLine("新出品方 [" + producer + "]："); if (!s.empty()) producer = s;
        string sy = readLine("新年份 [" + to_string(year) + "]：");
        if (!sy.empty()) year = stoi(sy);
        s = readLine("新时长 [" + duration + "]："); if (!s.empty()) duration = s;
    }
    void show() override {
        cout << "[视频] " << id << " | " << title << " | " << author
             << " | " << producer << " | " << year << " | " << duration
             << " | " << grade << endl;
    }
    void save(ofstream& ofs) override {
        ofs << id << " " << title << " " << author << " " << grade << " "
            << producer << " " << year << " " << duration << endl;
    }
    void load(ifstream& ifs) override {
        ifs >> id >> title >> author >> grade >> producer >> year >> duration;
    }
};

// ---------- 图画类 ----------
class Picture : public Media {
public:
    string country;
    int len = 0, wid = 0;
    string typeName() override { return "图画"; }
    void add() override {
        title  = readLine("标题：");
        author = readLine("作者：");
        grade  = readLine("评级：");
        country = readLine("国籍：");
        len = readPositiveInt("长度(cm)：");
        wid = readPositiveInt("宽度(cm)：");
    }
    void edit() override {
        cout << "（直接回车保留原值）" << endl;
        string s;
        s = readLine("新标题 [" + title + "]：");   if (!s.empty()) title = s;
        s = readLine("新作者 [" + author + "]：");  if (!s.empty()) author = s;
        s = readLine("新评级 [" + grade + "]：");   if (!s.empty()) grade = s;
        s = readLine("新国籍 [" + country + "]："); if (!s.empty()) country = s;
        string sl = readLine("新长度 [" + to_string(len) + "]cm：");
        if (!sl.empty()) len = stoi(sl);
        string sw = readLine("新宽度 [" + to_string(wid) + "]cm：");
        if (!sw.empty()) wid = stoi(sw);
    }
    void show() override {
        cout << "[图画] " << id << " | " << title << " | " << author
             << " | " << country << " | " << len << "x" << wid << "cm | " << grade << endl;
    }
    void save(ofstream& ofs) override {
        ofs << id << " " << title << " " << author << " " << grade << " "
            << country << " " << len << " " << wid << endl;
    }
    void load(ifstream& ifs) override {
        ifs >> id >> title >> author >> grade >> country >> len >> wid;
    }
};

// ---------- 核心管理类 ----------
class MediaManager {
private:
    static const int MAX = 70;
    Book    books[MAX];
    Video   videos[MAX];
    Picture pics[MAX];
    int bCnt = 0, vCnt = 0, pCnt = 0;

    bool isIdExist(const string& id) const {
        for (int i = 0; i < bCnt; i++) if (books[i].id == id) return true;
        for (int i = 0; i < vCnt; i++) if (videos[i].id == id) return true;
        for (int i = 0; i < pCnt; i++) if (pics[i].id == id) return true;
        return false;
    }

public:
    // 添加媒体
    void addMedia() {
        string type = readLine("物品种类(图书/视频/图画)：");
        if (type != "图书" && type != "视频" && type != "图画") {
            cout << "类型无效！" << endl; return;
        }
        if ((type == "图书" && bCnt >= MAX) ||
            (type == "视频" && vCnt >= MAX) ||
            (type == "图画" && pCnt >= MAX)) {
            cout << type << "库已满！" << endl; return;
        }
        string id;
        while (true) {
            id = readLine("唯一编号：");
            if (id.empty()) { cout << "编号不能为空！" << endl; continue; }
            if (!isIdExist(id)) break;
            cout << "编号重复，重新输入：" << endl;
        }
        if (type == "图书") {
            books[bCnt].id = id;
            books[bCnt].add();
            bCnt++;
        } else if (type == "视频") {
            videos[vCnt].id = id;
            videos[vCnt].add();
            vCnt++;
        } else {
            pics[pCnt].id = id;
            pics[pCnt].add();
            pCnt++;
        }
        cout << "添加成功！" << endl;
    }

    // 查询媒体（支持按 ID / 标题 / 类型 / 评级）
    void findMedia() {
        int opt = 0;
        cout << "1.按ID  2.按标题  3.按类型  4.按评级：";
        cin >> opt;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        string key;
        bool found = false;
        if (opt == 1) {
            key = readLine("输入ID：");
            for (int i = 0; i < bCnt; i++) if (books[i].id == key)    { books[i].show(); found = true; }
            for (int i = 0; i < vCnt; i++) if (videos[i].id == key)   { videos[i].show(); found = true; }
            for (int i = 0; i < pCnt; i++) if (pics[i].id == key)    { pics[i].show(); found = true; }
        } else if (opt == 2) {
            key = readLine("输入标题：");
            for (int i = 0; i < bCnt; i++) if (books[i].title == key) { books[i].show(); found = true; }
            for (int i = 0; i < vCnt; i++) if (videos[i].title == key){ videos[i].show(); found = true; }
            for (int i = 0; i < pCnt; i++) if (pics[i].title == key)  { pics[i].show(); found = true; }
        } else if (opt == 3) {
            key = readLine("输入类型(图书/视频/图画)：");
            if (key == "图书") for (int i = 0; i < bCnt; i++) { books[i].show(); found = true; }
            else if (key == "视频") for (int i = 0; i < vCnt; i++) { videos[i].show(); found = true; }
            else if (key == "图画") for (int i = 0; i < pCnt; i++) { pics[i].show(); found = true; }
        } else if (opt == 4) {
            key = readLine("输入评级(未评级/一般/成人/儿童)：");
            for (int i = 0; i < bCnt; i++) if (books[i].grade == key)  { books[i].show(); found = true; }
            for (int i = 0; i < vCnt; i++) if (videos[i].grade == key) { videos[i].show(); found = true; }
            for (int i = 0; i < pCnt; i++) if (pics[i].grade == key)   { pics[i].show(); found = true; }
        } else {
            cout << "选项无效！" << endl; return;
        }
        if (!found) cout << "无匹配结果！" << endl;
    }

    // 编辑媒体
    void editMedia() {
        string id = readLine("编辑ID：");
        for (int i = 0; i < bCnt; i++) if (books[i].id == id)  { books[i].edit();  cout << "编辑完成！" << endl; return; }
        for (int i = 0; i < vCnt; i++) if (videos[i].id == id) { videos[i].edit(); cout << "编辑完成！" << endl; return; }
        for (int i = 0; i < pCnt; i++) if (pics[i].id == id)  { pics[i].edit();  cout << "编辑完成！" << endl; return; }
        cout << "ID不存在！" << endl;
    }

    // 删除媒体（二次确认）
    void delMedia() {
        string id = readLine("删除ID：");
        int idx = -1;
        Media* target = nullptr;
        // 图书
        for (int i = 0; i < bCnt; i++) if (books[i].id == id)  { idx = i; target = &books[i]; break; }
        if (target) {
            cout << "将要删除：" << endl; target->show();
            string confirm = readLine("确认删除？(y/n)：");
            if (confirm != "y" && confirm != "Y") { cout << "已取消。" << endl; return; }
            for (int j = idx; j < bCnt-1; j++) books[j] = books[j+1];
            bCnt--; cout << "删除成功！" << endl; return;
        }
        // 视频
        target = nullptr;
        for (int i = 0; i < vCnt; i++) if (videos[i].id == id) { idx = i; target = &videos[i]; break; }
        if (target) {
            cout << "将要删除：" << endl; target->show();
            string confirm = readLine("确认删除？(y/n)：");
            if (confirm != "y" && confirm != "Y") { cout << "已取消。" << endl; return; }
            for (int j = idx; j < vCnt-1; j++) videos[j] = videos[j+1];
            vCnt--; cout << "删除成功！" << endl; return;
        }
        // 图画
        target = nullptr;
        for (int i = 0; i < pCnt; i++) if (pics[i].id == id)  { idx = i; target = &pics[i]; break; }
        if (target) {
            cout << "将要删除：" << endl; target->show();
            string confirm = readLine("确认删除？(y/n)：");
            if (confirm != "y" && confirm != "Y") { cout << "已取消。" << endl; return; }
            for (int j = idx; j < pCnt-1; j++) pics[j] = pics[j+1];
            pCnt--; cout << "删除成功！" << endl; return;
        }
        cout << "ID不存在！" << endl;
    }

    // 显示全部
    void showAll() {
        cout << "\n===== 全部媒体 =====" << endl;
        for (int i = 0; i < bCnt; i++) books[i].show();
        for (int i = 0; i < vCnt; i++) videos[i].show();
        for (int i = 0; i < pCnt; i++) pics[i].show();
        cout << "总数量：" << bCnt + vCnt + pCnt << endl;
    }

    // 统计
    void showStat() {
        cout << "\n===== 媒体库统计信息 =====" << endl;
        cout << "图书数量：" << bCnt << " 条" << endl;
        cout << "视频数量：" << vCnt << " 条" << endl;
        cout << "图画数量：" << pCnt << " 条" << endl;
        cout << "总数量：" << bCnt + vCnt + pCnt << " 条" << endl;
    }

    // 保存
    void saveAll() {
        ofstream bfs("books.txt");
        ofstream vfs("videos.txt");
        ofstream pfs("pics.txt");
        if (!bfs || !vfs || !pfs) {
            cout << "文件打开失败，保存未完成！" << endl; return;
        }
        for (int i = 0; i < bCnt; i++) books[i].save(bfs);
        for (int i = 0; i < vCnt; i++) videos[i].save(vfs);
        for (int i = 0; i < pCnt; i++) pics[i].save(pfs);
        bfs.close(); vfs.close(); pfs.close();
        cout << "保存成功！" << endl;
    }

    // 加载（修复原版本 EOF 判断 bug）
    void loadAll() {
        ifstream bfs("books.txt");
        ifstream vfs("videos.txt");
        ifstream pfs("pics.txt");
        bCnt = vCnt = pCnt = 0;
        // 正确做法：直接读，读到 EOF 自然停止；失败时回退计数
        while (bCnt < MAX && bfs >> books[bCnt].id) {
            books[bCnt].load(bfs);
            if (bfs.fail()) break;
            bCnt++;
        }
        while (vCnt < MAX && vfs >> videos[vCnt].id) {
            videos[vCnt].load(vfs);
            if (vfs.fail()) break;
            vCnt++;
        }
        while (pCnt < MAX && pfs >> pics[pCnt].id) {
            pics[pCnt].load(pfs);
            if (pfs.fail()) break;
            pCnt++;
        }
        bfs.close(); vfs.close(); pfs.close();
        cout << "加载成功！共 " << bCnt + vCnt + pCnt << " 条" << endl;
    }
};

// ---------- 主菜单 ----------
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

int main() {
    MediaManager mgr;
    int opt;
    while (true) {
        showMenu();
        cout << "选择操作：";
        if (!(cin >> opt)) {
            // 非法输入（比如字母）清空缓冲区，避免死循环
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "输入无效，请输入数字！" << endl;
            pauseClear();
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        switch (opt) {
            case 1: mgr.addMedia(); break;
            case 2: mgr.findMedia(); break;
            case 3: mgr.showAll(); break;
            case 4: mgr.editMedia(); break;
            case 5: mgr.delMedia(); break;
            case 6: mgr.showStat(); break;
            case 7: mgr.saveAll(); break;
            case 8: mgr.loadAll(); break;
            case 0: cout << "退出成功！" << endl; return 0;
            default: cout << "输入错误！" << endl;
        }
        pauseClear();
    }
    return 0;
}
