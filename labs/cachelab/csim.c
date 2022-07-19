#include "cachelab.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

//line
typedef struct{
    int valid;
    int tag;
    int LRUcount;//越大，替换优先级越高
}Line;

//set
typedef struct{
    Line* lines;
}Set;

typedef struct{
    Set* sets;
    int setnum;//S
    int linenum;//E
}Cache;

int hitcount = 0;
int misscount = 0;
int evictioncount = 0;

void printHelp(){//和帮助文档中csim-ref程序相同
    printf("Usage: ./csim [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n");
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n\n");
    printf("Examples:\n");
    printf("  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n\n");
}

//初始化cache，分配空间，初始化valid，LRUcount.
void init_cache(int s, int E, int b, Cache* sim_cache){
    if(s <= 0 || E <= 0)
        exit(0);
    
    sim_cache->setnum = 1 << s;//组数
    sim_cache->linenum = E;//每组行数
    sim_cache->sets = (Set*)malloc(sizeof(Set) * sim_cache->setnum);
    if(!sim_cache->sets){
        exit(0);
    }
    for(int i = 0; i < sim_cache->setnum; i++){
        sim_cache->sets[i].lines = (Line*)malloc(sizeof(Line) * E);
        for(int j = 0; j < E; j++){
            sim_cache->sets[i].lines[j].valid = 0;
            sim_cache->sets[i].lines[j].LRUcount = 0;
        }
    }
}

//将命中行的LRUcount置为0，其余行+1.
void updateLRU(Cache *sim_cache, int setid, int lineid){
    int ln = sim_cache->linenum;
    for(int i = 0; i < ln; i++){
        sim_cache->sets[setid].lines[i].LRUcount++;
    }
    sim_cache->sets[setid].lines[lineid].LRUcount = 0;
}

//判断是否命中,命中return 1， 未命中return 0
int judgeHit(Cache *sim_cache, int setid, int cur_tag){  
    int ln = sim_cache->linenum;
    for(int i = 0; i < ln; i++){
        if(sim_cache->sets[setid].lines[i].valid == 1 && sim_cache->sets[setid].lines[i].tag == cur_tag){
            //命中，更新LRUcount
            updateLRU(sim_cache, setid, i);
            return 1;
        }
    }
    return 0;
}

void visitCache(Cache *sim_cache, int setid, int cur_tag, int verbose){
    
    if(judgeHit(sim_cache, setid, cur_tag)){
        hitcount++;
        if(verbose == 1)
            printf("hit ");
    }else{
        //未命中,分两种情况，1.目标组行没满，直接写入cache，2.目标组行已满，需要替换
        misscount++;
        if(verbose == 1)
            printf("miss ");
        
        //行没满,直接写入
        int ln = sim_cache->linenum;
        for(int i = 0; i < ln; i++){
            if(sim_cache->sets[setid].lines[i].valid == 0){
                sim_cache->sets[setid].lines[i].valid = 1;
                sim_cache->sets[setid].lines[i].tag = cur_tag;
                updateLRU(sim_cache, setid, i);
                return; 
            }
        }

        //行已满，寻找替换行
        int max_LRu = -1;
        int replace_line = 0;
        for(int i = 0; i < ln; i++){
            if(sim_cache->sets[setid].lines[i].LRUcount > max_LRu){
                max_LRu = sim_cache->sets[setid].lines[i].LRUcount;
                replace_line = i; 
            }
        }
        sim_cache->sets[setid].lines[replace_line].valid = 1;
        sim_cache->sets[setid].lines[replace_line].tag = cur_tag;
        updateLRU(sim_cache, setid, replace_line); 
        evictioncount++;
        if(verbose == 1)
            printf("eviction ");
        }   
}


int main(int argc, char** argv)
{
    int s = -1, E = -1, b = -1, verbose = 0;//当输入命中有-v时，vervose = 1,显示具体信息
    FILE* tracefile = NULL;
    Cache sim_cache;
    char ch;

    while((ch = getopt(argc, argv, "hvs:E:b:t:")) != -1){// man 3 getopt
        switch(ch){
            case 'h' : 
                printHelp();
                break;
            case 'v':
                verbose = 1;
                break;
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                tracefile = fopen(optarg, "r");
                break;
            default:
                printHelp();
                break; 
        }
    }

    if (s == -1 || E == -1 || b == -1 || tracefile == NULL)
	{
		printf("%s: Missing required command line argument\n", argv[0]);
		printHelp();
		//exit(1);
	}

    init_cache(s, E, b, &sim_cache);//初始化cache

    {
        //trcae中数据信息格式 option addr size
        char option;
        unsigned long long addr;
        int size;
        while(fscanf(tracefile, "%c %llx,%d", &option, &addr, &size) > 0){//按行读入trace文件
            if(option == 'I')//忽略指令load
                continue;
            int cur_tag = addr >> (s + b);//根据地址解析tag
            int setid = (addr >> b) & ((1 << s) - 1);//根据地址解析行号
            if(option == 'L' || option == 'S'){
                if(verbose == 1){
                    printf("%c %llx,%d ", option, addr, size);
                }
                visitCache(&sim_cache, setid, cur_tag, verbose);
            }
            if(option == 'M'){//相当于同时执行load+store
                if(verbose == 1){
                    printf("%c %llx,%d ", option, addr, size);
                }
                visitCache(&sim_cache, setid, cur_tag, verbose);
                visitCache(&sim_cache, setid, cur_tag, verbose);
            }
            if(verbose == 1)
                printf("\n");
        }
    }

    printSummary(hitcount, misscount, evictioncount);
    return 0;
}
