#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<string>
#include<stdlib.h>
#include<ctime>
#include <unordered_map>
#include<stdio.h>
#include<unordered_set>
#include<numeric>
#include<set>
#include<array>


using namespace std;

void CoutTime(clock_t starttime) { // 计时函数。
    clock_t endtime = clock();
    double endtime1 = (double(endtime) - double(starttime)) / CLOCKS_PER_SEC;
    cout << endtime1 << "s" << endl;
}

void read(string filename, vector<vector<int>>& a) { //读取.csv文件，提取出矩阵存储到a, 
    string line;
    vector<int> b; //读取一行的单元格数字字符提取出来
    ifstream Fil(filename);
    if (Fil.fail()) { cout << "File not found" << endl; return; }
    while (getline(Fil, line))
    {
        b.clear();
        int linepos = 0;
        char c;
        int linemax = int(line.length());
        while (linepos < linemax)
        {
            c = line[linepos];
            if (isdigit(c)) {
                b.push_back(c - '0');
            }
            ++linepos;
        }
        a.push_back(b);
    }
    Fil.close();
    return;
}

template <size_t SIZE>
inline void insertSort1(array<int, SIZE>& arr1, int length) {
    for (int i = 1; i < length; i++) {
        for (int j = i - 1; j >= 0 && arr1[j + 1] < arr1[j]; j--) {
            swap(arr1[j], arr1[j + 1]);
        }
    }
}


template<typename iterator>
struct ArrayHasher {
    auto operator()(const iterator& arr)const {
        rsize_t sum = arr[0];
        for (auto it = arr.begin(); it != arr.end(); it++) {
            sum = sum ^ hash<size_t>{}(*it);
        }
        return hash<size_t>{}(sum);
    }
};

template<typename iterator>
using ArrayHashSet = std::unordered_set<iterator, ArrayHasher<iterator>>;

class Solution {
public:
    void iniadjs() {    //申请内存空间
        adjS1 = createdArray2D(numTA, numTB);      //  cout << "iniadjs1";
        adjS3 = createdArray2D(numTA, numTB);       // cout << "iniadjs3";
        adjS5 = createdArray2D(numTA, numTB);       // cout << "iniadjs5";
        adjS2 = createdArray2D(numTA, numTA);       // cout << "iniadjs2";
        adjS4 = createdArray2D(numTA, numTA);       // cout << "iniadjs4";
    }
    void setadjs12345() { //计算 走i步的可连通的点，作为剪枝判断
        iniadjs();
        for (int i = 0; i < numTA; i++) {
            for (int j = 0; j < adjV[i]; j++) {
                adjS1[i][adjarray[i][j] - numTA] = true;
            }
        }
        for (int i = 0; i < numTA; i++) {
            for (int j = 0; j < adjV[i]; j++) {
                int aij = adjarray[i][j];
                for (int k = 0; k < adjV[aij]; k++) {
                    adjS2[i][adjarray[aij][k]] = true;
                }
            }
        }
        for (int i = 0; i < numTA; i++) {
            for (int j = 0; j < numTA; j++) {
                if (adjS2[i][j]) {
                    for (int k = 0; k < numTB; k++) {
                        if (adjS1[j][k]) {
                            adjS3[i][k] = true;
                        }
                    }
                }
            }
        }
        // 4
        for (int i = 0; i < numTA; i++) {
            for (int j = i; j < numTA; j++) {
                if (adjS2[i][j]) {
                    for (int k = 0; k < numTA; k++) {
                        if (adjS2[j][k]) {
                            adjS4[i][k] = true;
                            adjS4[k][i] = true;
                        }
                    }
                }
            }
        }
        //5
        for (int i = 0; i < numTA; i++) {
            for (int j = 0; j < numTA; j++) {
                if (adjS4[i][j]) {
                    for (int k = 0; k < numTB; k++) {
                        if (adjS1[j][k]) {
                            adjS5[i][k] = true;
                        }
                    }
                }
            }
        }
    }
    void setcount(int num, int numTA1, int numTB1) { //设置最大搜索名字数。部落A，B的数量
        step = num;
        numTA = numTA1;
        numTB = numTB1;
        numTotalAB = numTA + numTB;
        visited = new int[numTotalAB]; //定义访问数组
        cout << "Set count is ok. Maximum number of names:  " << step << endl;
    }
    void setstar(int star) {  //设置搜索起点
        memset(visited, false, numTotalAB * sizeof(int));
        start = star;
    }
    void setadjV(const vector<vector<int>>& adj, int numTotalAB) {  // 邻接数组
        adjV = new int[numTotalAB];    //邻接数组中每个节点的相邻节点数
        adjarray = new int* [numTotalAB];//邻接数组
        for (int i = 0; i < numTotalAB; i++) {
            int i_Size = adj[i].size();
            adjV[i] = i_Size;
            adjarray[i] = new int[i_Size];
            for (int j = 0; j < i_Size; j++) {
                adjarray[i][j] = adj[i][j];
            }
        }
    }
    void coutsumname(clock_t starttime) { //输出结果
        clock_t endtime = clock();
        double endtime1 = (double(endtime) - double(starttime)) / CLOCKS_PER_SEC;
        ofstream ofs;
        ofs.open("result.txt", ios::out);
        if (!ofs) {
            cout << "creat result.txt";
            system("result.txt");
        }
        ofs << " time " << endtime1 << "s" << endl;
        for (int i = 4; i < 15; i++, i++) {

            ofs << i << " names: " << Hash_name_num[i] << ";\n";
        }
        ofs.close();
        for (int i = 4; i < 15; i++, i++) {
            std::cout << i << " names: " << Hash_name_num[i] << ";\n";
        }
    }
    void mysetclear() {
        myArrSet6.clear();
        myArrSet8.clear();
        myArrSet10.clear();
        myArrSet12.clear();
        myArrSet14.clear();
    }
    void mysetreserve() {   //预分配内存
        myArrSet6.reserve(256);
        myArrSet8.reserve(512);
        myArrSet10.reserve(4096);
        myArrSet12.reserve(31921 * 2);
        myArrSet14.reserve(690598 * 6);
    }
    void hash_num(void) {
        Hash_name_num[14] += myArrSet14.size();
        Hash_name_num[12] += myArrSet12.size();
        Hash_name_num[10] += myArrSet10.size();
        Hash_name_num[8] += myArrSet8.size();
        Hash_name_num[6] += myArrSet6.size();
    }
    void DFS(int stapos) { //DFS      
        visited[stapos] = true;  // 标记 节点 stapos 为 已访问过的节点        
        temp[length] = stapos;
        length++;
        int i = adjV[stapos];  //节点stapos邻接节点数即好友数。
        if (length == 1) {  // 第一个节点的下一个节点不搜索最大的个数
            i--;
        }
        while (i--) {
            int k = adjarray[stapos][i]; // 第i个好友为k;
            if (k == start && length > 3) {  // 回到初始节点，计数。
                if (temp[1] < temp[length - 1]) {
                    if (length == 12) {
                        memcpy(&temparr11[0], temp + 1, 11 * sizeof(int));
                        swap(temparr11[0], temparr11[5]);
                        swap(temparr11[2], temparr11[7]);
                        swap(temparr11[4], temparr11[9]);
                        insertSort1(temparr11, 11);
                        temparrul3[0] = temparr11[0];
                        for (int i = 1; i < 5; i++) {
                            temparrul3[0] = (temparrul3[0] << 12) + temparr11[i];
                        }
                        temparrul3[1] = temparr11[5];
                        for (int i = 6; i < 10; i++) {
                            temparrul3[1] = (temparrul3[1] << 12) + temparr11[i];
                        }
                        temparrul3[2] = temparr11[10];
                        myArrSet12.insert(temparrul3);
                    }
                    else if (length == 10) {
                        memcpy(&temparr9[0], temp + 1, 9 * sizeof(int));
                        swap(temparr9[0], temparr9[5]);
                        swap(temparr9[2], temparr9[7]);
                        insertSort1(temparr9, 9);
                        temparrul2[0] = temparr9[0];
                        for (int i = 1; i < 5; i++) {
                            temparrul2[0] = (temparrul2[0] << 12) + temparr9[i];
                        }
                        temparrul2[1] = temparr9[5];
                        for (int i = 6; i < 9; i++) {
                            temparrul2[1] = (temparrul2[1] << 12) + temparr9[i];
                        }
                        myArrSet10.insert(temparrul2);
                    }
                    else if (length == 8) {
                        memcpy(&temparr7[0], temp + 1, 7 * sizeof(int));
                        swap(temparr7[0], temparr7[3]);
                        swap(temparr7[2], temparr7[5]);
                        insertSort1(temparr7, 7);
                        temparrui4[0] = (temparr7[0] << 12) + temparr7[1];
                        temparrui4[1] = (temparr7[2] << 12) + temparr7[3];
                        temparrui4[2] = (temparr7[4] << 12) + temparr7[5];
                        temparrui4[3] = temparr7[6];
                        myArrSet8.insert(temparrui4);
                    }
                    else if (length == 6) {
                        memcpy(&temparr5[0], temp + 1, 5 * sizeof(int));
                        swap(temparr5[0], temparr5[3]);
                        insertSort1(temparr5, 5);
                        temparrui3[0] = (temparr5[0] << 12) + temparr5[1];
                        temparrui3[1] = (temparr5[2] << 12) + temparr5[3];
                        temparrui3[2] = temparr5[4];
                        myArrSet6.emplace(temparrui3);
                    }
                    else if (length == 4) {
                        Hash_name_num[4] ++;
                    }
                }
            }
            else if (!visited[k] && k > start) {    //// 去重 第i个好友为已经搜索过了 { // 去掉已经访问过的节点;
                if (length < 9) {
                    DFS(k);
                }
                else if (length == 12) {
                    if (adjS2[start][k]) {
                        DFS(k);
                    }
                }
                else if (length == 13) { //已经记录了13个名字 仅可再搜索一步
                    if (adjS1[start][k - numTA] && (temp[1] < k)) {
                        memcpy(&temparr13[0], temp + 1, 12 * sizeof(int));
                        swap(temparr13[0], temparr13[7]);
                        swap(temparr13[2], temparr13[9]);
                        swap(temparr13[4], temparr13[11]);
                        temparr13[12] = k;
                        insertSort1(temparr13, 13);
                        temparrul3[0] = temparr13[0];
                        for (int i = 1; i < 5; i++) {
                            temparrul3[0] = (temparrul3[0] << 12) + temparr13[i];
                        }
                        temparrul3[1] = temparr13[5];
                        for (int i = 6; i < 10; i++) {
                            temparrul3[1] = (temparrul3[1] << 12) + temparr13[i];
                        }
                        temparrul3[2] = (long long(temparr13[10]) << 24) + (temparr13[11] << 12) + temparr13[12];
                        myArrSet14.insert(temparrul3);
                    }
                }
                else if (length == 9) {  //已经搜索了记录了9个名字
                    if (adjS5[start][k - numTA]) { //剪枝 判断5步能否回到开始节点;
                        DFS(k);
                    }
                }
                else if (length == 10) {
                    if (adjS4[start][k]) {
                        DFS(k);
                    }
                }
                else if (length == 11) {
                    if (adjS3[start][k - numTA]) {
                        DFS(k);
                    }
                }
            }
        }
        visited[stapos] = false;
        length--;
        temp[length] = 0;
    }
    void freeall() {  //释放堆空间
        freeArray2D(adjS1, numTA);
        freeArray2D(adjS2, numTA);
        freeArray2D(adjS3, numTA);
        freeArray2D(adjS4, numTA);
        freeArray2D(adjS5, numTA);
        freeArray2D(adjarray, numTA);
        delete[] visited;
        delete[] adjV;
    }
private:
    int temp[14] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0 };  //保存路径
    array<int, 13> temparr13 = { 0,0,0,0,0,0,0,0,0,0,0,0,0 };  //保存的环节点，然后排序
    array<int, 11> temparr11 = { 0,0,0,0,0,0,0,0,0,0,0 };
    array<int, 9> temparr9 = { 0,0,0,0,0,0,0,0,0 };
    array<int, 7> temparr7 = { 0,0,0,0,0,0,0 };
    array<int, 5> temparr5 = { 0,0,0,0,0 };

    array<int, 4> temparrui4 = { 0,0,0,0 }; // unordered_set 存的数据。
    array<int, 3> temparrui3 = { 0,0,0 };
    array<long long, 2> temparrul2 = { 0,0 };
    array<long long, 3> temparrul3 = { 0,0,0 };

    ArrayHashSet<array<long long, 3>> myArrSet14;   // unordered_set  集合
    ArrayHashSet<array<long long, 3>> myArrSet12;
    ArrayHashSet<array<long long, 2>> myArrSet10;
    ArrayHashSet<array<int, 4>> myArrSet8;
    ArrayHashSet<array<int, 3>> myArrSet6;
    int* adjV = nullptr;
    int** adjarray = nullptr;
    int numTA = 0, numTB = 0, start = 0;
    int numTotalAB = 0;   //   numTA + numTB;
    int length = 0;  //已经记录的名字数目
    int step = 0;  // 搜索
    int Hash_name_num[15] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }; // 存储托盘上名字数为4,6,8,10,12,14的的祭品数量。
    int* visited = nullptr;
    int** adjS1 = nullptr;
    int** adjS2 = nullptr;
    int** adjS3 = nullptr;
    int** adjS4 = nullptr;
    int** adjS5 = nullptr;
    int** createdArray2D(int rows, int cols) { //申请空间
        int** array2D = new int* [rows];
        for (int i = 0; i < rows; ++i)
        {
            array2D[i] = new int[cols]();
            memset(array2D[i], 0, sizeof(int) * cols);
        }
        return array2D;
    }
    void freeArray2D(int** array2D, int rows) { //释放堆空间
        for (int i = 0; i < rows; ++i)
        {
            delete[] array2D[i];
        }
        delete[] array2D;
    }
};


int main() {
    clock_t starttime = clock();  // 开始计时
    string filename = "./Example.csv"; //6912,3840,25344,577920,11781824,230278656 第二阶段的一个用例
    //string filename = "./Example421h.csv"; //64, 384,5920,56704,667392,7854016  第一阶段的一个用例
    //string filename = "./Example427.csv";// 128,448,4832,50624,608704,7199808  
    vector<vector<int>> a(0);
    read(filename, a);
    int numTA = int(a.size());
    int numTB = int(a[0].size());
    int numTotalAB = numTA + numTB;
    cout << "total numbers of tribal A: " << numTA << endl;
    cout << "total numbers of tribal B: " << numTB << endl;
    cout << " read time :  ";
    CoutTime(starttime);
    vector<vector<int>> adjarray(numTotalAB);  // 转换数组形式邻接表
    for (int i = 0; i < numTA; i++) {
        for (int j = 0; j < numTB; j++) {
            if (a[i][j]) {
                adjarray.at(i).push_back(numTA + j);
                adjarray.at(numTA + j).push_back(i);
            }
        }
    }
    a.clear();
    Solution solt1;
    solt1.setcount(14, numTA, numTB);
    solt1.setadjV(adjarray, numTotalAB);
    solt1.setadjs12345();
    if (numTotalAB > 2000) {// set 预分配内存
        solt1.mysetreserve();
    }
    /*cout << "InI time:    ";
    CoutTime(starttime);*/
    //numTA = 2; 
    for (int i = 0; i < numTA; i++) {
        cout << "\r i = " << i << "   " << i * 100 / numTA << "% ";
        solt1.setstar(i);
        solt1.mysetclear();
        solt1.DFS(i);
        solt1.hash_num();
    }
    cout << endl;
    solt1.coutsumname(starttime);
    solt1.freeall();
    cout << "time:    "; CoutTime(starttime);
    system("pause");
    return 1;
}
