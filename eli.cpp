#ifndef _INCLUDED_ELI_GENERATOR
#define _INCLUDED_ELI_GENERATOR
#include <stdlib.h>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <initializer_list>
#include <algorithm>
#include <vector>
#include <map>
#include <sstream>
#include <iostream>
#include <cmath>
#include <climits>
#include "testlib.h"
#ifdef WIN32
    #include <windows.h>
#endif
const std::string ELI_VERSION = "2.0";
void fail(std::string message) {
    std::cerr<<"[FAIL] "<<message<<'\n';
    exit(1);
}
template<typename value>
std::string toStr(value _Val) {
    std::stringstream stream;
    stream<<_Val;
    return stream.str();
}
void init(int argc,char **argv) {
    registerGen(argc,argv,1);
}
class __FileWriter {
    protected:
        std::ofstream fobj;
    public:
        std::string path;
        void load(std::string pth, bool is_append = false) {
            path = pth;
            if (is_append) {
                this->fobj.open(path, std::ios::out | std::ios::app);
            } else {
                this->fobj.open(path, std::ios::out);
            }
            if (!fobj.is_open()) {
                fail("Couldn't open file \"" + path + "\"");
            }
        }
        template<class T>
        void write(T object) {
            fobj<<object;
        }
        void close() {
            fobj.close();
        }
        ~__FileWriter() {
            this->close();
        }
};
class TestIO {
    protected:
        __FileWriter _in,_out;
    public:
        TestIO(std::string file_prefix, int id, std::string isuffix = "in", std::string osuffix = "out") {
            std::string _id_stmp = toStr(id);
            std::string inpath = file_prefix + _id_stmp + "." + isuffix;
            std::string outpath = file_prefix + _id_stmp + "." + osuffix;
            _in.load(inpath);
            _out.load(outpath);
        }
        TestIO(int id, std::string isuffix = "in", std::string osuffix = "out") {
            TestIO("", id, isuffix, osuffix);
        }
        template<class T>
        void inputWrite(T obj) {
            _in.write(obj);
        }
        template<class T>
        void outputWrite(T obj) {
            _out.write(obj);
        }
        template<class T>
        void inputWriteln(T obj) {
            inputWrite(obj);
            inputWrite('\n');
        }
        template<class T>
        void outputWriteln(T obj) {
            outputWrite(obj);
            outputWrite('\n');
        }
        template<class T>
        void inputWrites(std::initializer_list<T> values, std::string sep=" ") {
            int size = values.size();
            int tot = 0;
            for (auto value : values) {
                tot++;
                inputWrite(value);
                if (tot!=size) {
                    inputWrite(sep);
                }
            }
        }
        template<class T>
        void outputWrites(std::initializer_list<T> values, std::string sep=" ") {
            int size = values.size();
            int tot = 0;
            for (auto value : values) {
                tot++;
                outputWrite(value);
                if (tot!=size) {
                    outputWrite(sep);
                }
            }
        }
        template<class T>
        void inputWrites(T values, std::string sep=" ") {
            int size = values.size();
            int tot = 0;
            for (auto value : values) {
                tot++;
                inputWrite(value);
                if (tot!=size) {
                    inputWrite(sep);
                }
            }
        }
        template<class T>
        void outputWrites(T values, std::string sep=" ") {
            int size = values.size();
            int tot = 0;
            for (auto value : values) {
                tot++;
                outputWrite(value);
                if (tot!=size) {
                    outputWrite(sep);
                }
            }
        }
        template<class T>
        void inputWritesln(std::initializer_list<T> values, std::string sep=" ") {
            inputWrites(values, sep);
            inputWrite('\n');
        }
        template<class T>
        void outputWritesln(std::initializer_list<T> values, std::string sep=" ") {
            outputWrites(values, sep);
            outputWrite('\n');
        }
        template<class T>
        void inputWritesln(T values, std::string sep=" ") {
            inputWrites(values, sep);
            inputWrite('\n');
        }
        template<class T>
        void outputWritesln(T values, std::string sep=" ") {
            outputWrites(values, sep);
            outputWrite('\n');
        }
        void outputGen(std::string generator) {
            #ifdef WIN32
            _in.close();
            _out.close();
            char buff[622536];
            sprintf(buff,"\"%s\" < \"%s\" > \"%s\"",generator.c_str(),_in.path.c_str(),_out.path.c_str());
            __FileWriter writer;
            writer.load(".eli_output_gen.bat");
            writer.write("@echo off\n");
            writer.write(buff);
            writer.close();
            system(".eli_output_gen.bat");
            remove(".eli_output_gen.bat");
            _in.load(_in.path,true);
            _out.load(_out.path,true);
            #else
            fail("TestIO.outputGen couldn't run \"" + generator + "\",because it currently doesn't support running under non Windows operating systems.");
            #endif
        }
        void close() {
            _in.close();
            _out.close();
        }
        ~TestIO() {
            this->close();
        }
};
typedef TestIO IO;
template<class _Weight=int>
class GraphEdge {
    public:
        int from,to;
        _Weight weight;
        bool operator<(const GraphEdge rgt) const {
            if (from==rgt.from) {
                if (to==rgt.to) {
                    return weight<rgt.weight;
                } else return to<rgt.to;
            } else return from<rgt.from;
        }
        bool operator>(const GraphEdge rgt) const {
            return !((*this)<rgt);
        }
        bool operator==(const GraphEdge rgt) const {
            return (from==rgt.from)&&(to==rgt.to)&&(weight==rgt.weight);
        }
        bool operator!=(const GraphEdge rgt) const {
            return !((*this)==rgt);
        }
        bool operator<=(const GraphEdge rgt) const {
            return ((*this)<rgt)||((*this)==rgt);
        }
        bool operator>=(const GraphEdge rgt) const {
            return ((*this)>rgt)||((*this)==rgt);
        }
        GraphEdge(int u=0,int v=0,_Weight w=1) {
            from=u,to=v,weight=w;
        }
        std::string writeToString(std::string sep = ' ', bool weighted = true) {
            std::string ret;
            ret += toStr(from);
            ret += sep;
            ret += toStr(to);
            if (weighted) {
                ret += sep;
                ret += toStr(weight);
            }
			return ret;
        }
};
template<class _Weight=int>
class Graph {
    protected:
        struct LinkableStar {
            int nxt,to,w;
            LinkableStar(int Nxt=0,int To=0,int W=0) {
                nxt=Nxt,to=To,w=W;
            }
        };
        std::vector<LinkableStar> star;
        std::vector<int> head;
        typedef std::vector<GraphEdge<_Weight> > Edges;
        Edges edges;
    public:
        Graph() {
            star.push_back(LinkableStar());
        }
        Graph(Edges edg) {
            edges = edg;
			star.push_back(LinkableStar());
        }
        void insertEdge(GraphEdge<_Weight> edge) {
            edges.push_back(edge);
            if (((int)(head.size()))<edge.from) {
                for (; ((int)(head.size()))<=edge.from+1; head.push_back(0));
            }
            LinkableStar cstar(head[edge.from],edge.to,edge.weight);
            star.push_back(cstar);
            head[edge.from]=star.size()-1;
        }
        void insertEdge(int from,int to) {
            insertEdge(GraphEdge<_Weight>(from,to));
        }
        void insertEdge(int from,int to,_Weight weight) {
            insertEdge(GraphEdge<_Weight>(from,to,weight));
        }
        Edges &getAllEdges() {
            return edges;
        }
        Edges getAllEdgesFromX(int x) {
            Edges ret;
            for (; head.size()<=x+1; head.push_back(0)) {}
            for (int i=head[x]; i; i=star[i].nxt) {
                ret.push_back(GraphEdge<_Weight>(x,star[i].to,star[i].w));
            }
            return ret;
        }
        void shuffleEdges() {
            shuffle(edges.begin(),edges.end());
        }
        void sortEdges() {
            std::sort(edges.begin(),edges.end());
        }
        void sortEdgesGreater() {
            std::sort(edges.begin(),edges.end(),std::greater<GraphEdge<_Weight> >());
        }
        void sortEdgesLess() {
            std::sort(edges.begin(),edges.end(),std::less<GraphEdge<_Weight> >());
        }
        std::string writeToString(bool weighted=true,std::string sep=" ",std::string newline="\n") {
            std::string ret;
            for (GraphEdge<_Weight> e : getAllEdges()) {
                e.writeToString(sep=sep,weighted=weighted);
                ret += newline;
            }
            ret.erase(ret.begin()+ret.length()-1);
            return ret;
        }
};
template<class _Weight>
std::ostream &operator<<(std::ostream &_stream, Graph<_Weight> graph) {
    _stream<<graph.writeToString();
    return _stream;
}
template<class _Weight>
std::ostream &operator<<(std::ostream &_stream, GraphEdge<_Weight> edge) {
    _stream<<edge.writeToString();
    return _stream;
}
typedef Graph<> Tree;
typedef Tree Chain;
typedef Tree Flower;
class __Module_Randomer {
    protected:
        random_t device;
    public:
        __Module_Randomer() {}
        template<class T>
        T nextAny(T left,T right) {
            return device.next(left,right);
        }
        int nextInteger(int left,int right) {
            return device.next(left, right);
        }
        unsigned nextUnsigned(unsigned left,unsigned right) {
            return device.next(left, right);
        }
        long long nextLongLong(long long left,long long right) {
            return device.next(left,right);
        }
        unsigned long long nextUnsignedLongLong(unsigned long long left,unsigned long long right) {
            return device.next(left,right);
        }
        double nextDouble(double left,double right) {
            return device.next(left,right);
        }
        bool nextBoolean() {
            return (bool)(nextInteger(0,1));
        }
        std::vector<int> nextPermutation(int n) {
            std::vector<int> ret;
            for (int i=1; i<=n; i++) {
                ret.push_back(i);
            }
            shuffle(ret);
            return ret;
        }
        template<typename _RandomAccessIter>
        void shuffle(_RandomAccessIter __first, _RandomAccessIter __last) {
            if (__first == __last) return;
            for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i) {
                std::iter_swap(__i, __first + device.next(int(__i - __first) + 1));
            }
        }
        template<class Container>
        void shuffle(Container &ct) {
            shuffle(ct.begin(),ct.end());
        }
        template<class Ite>
        typename Ite::value_type choice(Ite begin,Ite end) {
            return *(nextInteger(0,end-begin-1)+begin);
        }
        template<typename STL>
        typename STL::value_type choice(STL stl) {
            return *(nextInteger(0,stl.size()-1)+stl.begin());
        }
        template<class T>
        T choice(std::initializer_list<T> initializer_list) {
            return *(nextInteger(0,initializer_list.size()-1)+initializer_list.begin());
        }
        template<class _Weight>
        GraphEdge<_Weight> choice(Graph<_Weight> g) {
            return choice(g.getAllEdges());
        }
        int tossCoin(int front = 1,int back = 0) {
            return nextInteger(0,1)?front:back;
        }
        int throwDice() {
            return nextInteger(1,6);
        }
        Tree tree(int n,int splay) {
            Tree ret;
            std::vector<int> father(n);
            for (int i=0; i<n; i++) {
                if (i>0) {
                    father[i]=device.wnext(i,splay);
                }
            }
            std::vector<int> permu(n);
            for (int i=0; i<n; i++) {
                permu[i]=i;
            }
            shuffle(permu.begin()+1,permu.end());
            for (int i=1; i<n; i++) {
                int left=permu[i]+1,right=permu[father[i]]+1;
                if (nextBoolean()) {
                    std::swap(left,right);
                }
                if (right==1) {
                    std::swap(left,right);
                }
                ret.insertEdge(left,right);
            }
            ret.shuffleEdges();
            return ret;
        }
        Tree tree(int n) {
            return tree(n,0);
        }
        Chain chain(int n) {
            Chain tree;
            for (int i=1; i<n; i++) {
                tree.insertEdge(i,i+1);
            }
            tree.shuffleEdges();
            return tree;
        }
        Flower flower(int n) {
            Flower flower;
            for (int i=2; i<=n; i++) {
                flower.insertEdge(1,i);
            }
            flower.shuffleEdges();
            return flower;
        }
        template<class _Weight>
        Graph<_Weight> graph(int n,int m,
                             _Weight weight_left,_Weight weight_right,
                             bool directed=true,
                             bool repeat_edge=true,
                             bool self_loop=true) {
            std::map<GraphEdge<_Weight>,bool> vis;
            Graph<_Weight> ret;
            for (int i=1; i<=m; i++) {
                GraphEdge<_Weight> eg;
                while (1) {
                    eg.from=nextInteger(1,n);
                    eg.to=nextInteger(1,n);
                    eg.weight=nextAny(weight_left,weight_right);
                    if (!directed) {
                        if (eg.from>eg.to) {
                            std::swap(eg.from,eg.to);
                        }
                    }
                    if (!repeat_edge && vis.count(eg)) {
                        continue;
                    }
                    if (!self_loop && eg.from == eg.to) {
                        continue;
                    }
                    vis[eg]=1;
                    break;
                }
                ret.insertEdge(eg);
            }
            ret.shuffleEdges();
            return ret;
        }
};
#ifdef random
    #undef random
#endif
__Module_Randomer randomer,random;
#define rand() (random.nextInteger(0,RAND_MAX))
#define random_shuffle(begin,end) (shuffle(begin,end))
#endif
