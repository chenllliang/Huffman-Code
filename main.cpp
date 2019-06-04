//
//  main.cpp
//  huffman
//
//  Created by 陈亮 on 2019/6/3.
//  Copyright © 2019 陈亮. All rights reserved.
//

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "Tree.h"
#include <fstream>

#define MAX 100000

using namespace std;

map<char,string> codes;
void getcodes(Node*p,string co=""){     //构建哈弗曼树的函数
    if(p->left==NULL&&p->right==NULL)
    {
        codes[p->a]=co;
        return;}
    
    if(p->left)
        getcodes(p->left,co+"0");
    
    if(p->right)
        getcodes(p->right,co+"1");
}

string turnachar(unsigned char c)       //将字符转换成8位二进制串的函数
{
    string k="";
    int j=128; //后八位为 1000_0000
    for(int i=0; i <8; i++)
    {
        //判断原char该位数是0或1
        k+=(unsigned char)(bool)(c&j)+'0';
        j>>=1; //将1右移
    }
    //cout<<k;
    return k;
    
}


int main(int argc, const char * argv[]) {
    
    //第一步，读取频率表
    map<char,int> frequency;
    FILE * p =fopen("source.in", "r");
    char temp='a';
    while (1) {
        temp=fgetc(p);
        if (temp!=EOF)
            frequency[temp]=frequency[temp]+1;
        else
            break;
    }
    
    //第二步，哈夫曼编码
    
    vector<Node*> Huffman;  //用vector实现哈夫曼树的构建
    
    map<char,int>::iterator it;
    it = frequency.begin();
    
    
    while(it != frequency.end())        //首先将所有字符作为树，构建森林
    {
        Node * tempnode=new Node;
        tempnode->quote=it->second;
        tempnode->a=it->first;
        tempnode->left=NULL;
        tempnode->right=NULL;
        Huffman.push_back(tempnode);
       // cout<<it->first<<":"<<it->second<<endl;
        it++;
    }
    
    
    
    sort(Huffman.begin(), Huffman.end(), compare);// vector中森林按照compare方法（tree.h中定义）排序，保持出现次数递增
    
    while (Huffman.size()>1) {
        
        Node * tempnode=new Node;//将森林中出现次数最高的两个结点（由于排过序，选vector中前两个即可）作为一个新节点的左右儿子，新节点的出现次数为儿子的次数的和，反复直到只剩一棵树。
        tempnode->left=Huffman[0];
        tempnode->right=Huffman[1];
        tempnode->quote=Huffman[0]->quote+Huffman[1]->quote;
        Huffman.push_back(tempnode);
        Huffman.erase(Huffman.begin());//弹出前两个结点
        Huffman.erase(Huffman.begin());
        
    }
    //树造好了，需要遍历编码
    getcodes(Huffman[0]);
    //codes配置完成
    
    //第三步 压缩
    
    ofstream file("zipped.out",ios_base::trunc|ios_base::binary);   //zipped.out 为压缩文件
    file<<codes.size()<<endl;//输入共有多少种字符
    map<char,string>::iterator it1;
    for (it1=codes.begin(); it1!=codes.end(); it1++) {
        file<<it1->first<<it1->second<<" ";
    }//将编码方式写入
    
    file<<endl;//空一行，也可以不空，我这里有点强迫症
    
    //开始写正文
    rewind(p);
    char tempc;
    string finals="";
    
    while(1){                   //首先将翻译好的源文件读入字符串finals，这里没有注意长度，你可以加对长度的限制，string最对容纳4G字符，利用map的查找
        tempc=fgetc(p);
        if(tempc!=EOF)
        {
            finals=finals+codes[tempc];
        }
        else
            break;
    }
    
    //关键
    //按编码方式按位写入文件
    
    char ch;
    string s;
    string cs="";
    long length=finals.length();
    string str="";
    unsigned char byte;
    unsigned long tempp;
                                    
    while(length>=8)
    {
        //取前8b
        str=finals.substr(0,8);
        bitset<8> bits(str);
        tempp=bits.to_ulong();//转换为long类型
        byte=tempp;//转换为char类型
        file<<byte;//写入文件
        //取出剩下的二进制串
        finals=finals.substr(8,length-8);
        length=finals.length();
    }

//假如剩余不足8位时，补足0并写入文件

    if(length!=0)
{
    str=finals.substr(0,length);
    int n=0;
    while(n<8-length)   //补0
    {
        str+='0';
        n++;
    }
    bitset<8> bits(str);
    tempp=bits.to_ulong();//转换为long类型
    byte=tempp;//转换为char类型
    file<<byte; //写入文件
    }
    //写入补0的个数，如果没有则写入0
    int pp=(8-length)%8;
    file<<pp;
    
    file.close();
    
   //编码结束,end with nums of zeros
    
    
   //开始解码
    
    
    //获取Huffman译码表并进行译码
    
        //1、获取Huffman译码表
        //以二进制的形式打开编码后的文件
        ifstream in("zipped.out",ios_base::binary);
        //假如文件打开失败

        //要以文本文件形式打开并写入输出文件，不然回车换行不能正常显示
        ofstream out("unzipped.out",ios_base::trunc);           //unzipped.out 为解压文件

    
        map<string,char>map4;
    
        int size;
        char key;
        char h;
        string value;
        //如果只是>>这种的话，会读取不到\n，然后会出错
        in>>size;
        in.get(h); //读取掉换行
        while(size>0)
        {
            in.get(key); //读取key
            in>>value;  //读取value
            in.get(h);  //读取掉空格
            map4[value]=key; //将key与value写入map4
            size--;
            cout<<value<<">>"<<key<<endl;
        }
        in.get(h);//读去掉换行
    
    
        //2、开始译码
        char c;
        unsigned char c1;
        string sc="";
        string result="";
    
    
    
    while(!in.eof()){
        
        sc.clear();
        result.clear();

    while(sc.length()<=MAX){
       
        if(!in.eof())
        {
            in.get(c);
            c1 = (unsigned char)c;
            sc+=turnachar(c1);
        }
        
        else{
            int nums = c-48;
            //此时c是字符
            cout<<nums;
            
            sc=sc.substr(0,sc.length()-16-nums);   //关键
            cout<<sc;
            break;
            
        }
    }
        
        
        string ss="";
        int i=1;
        bool check=false;
        
        while(sc.length()>0)
        {
            //开始解码
            ss=sc.substr(0,i);
            while(map4.find(ss)==map4.end()) //假如在Huffman表中找不到，继续读取下一位
            {
                i++;
                length=sc.length();
                //判断是否超过原字符串大小，避免报错
                if(i>length){
                    check=true;
                    break;
                }
                ss=sc.substr(0,i);
            }
            if(check==true){
                break;
            }
            //解码
            result+=map4[ss];
            //去除已解码的部分，继续解码
            sc=sc.substr(i,sc.length()-i);
            i=1;
        }
        
        //将解码后的结果写入文件
        out<<result;
        
    }
    return 0;
}

 
