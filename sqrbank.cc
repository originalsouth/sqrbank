#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <ctime>
#include <cstring>
#include <vector>
#include <string>
#include <map>

typedef float flt;

const char DELIM=',';
const uint32_t YEAR32BIT_MAX=137;
const uint32_t MONTH32BIT_MAX=1644;
const flt ZERO=(flt)0;
const size_t LINE_SIZE=1024;

flt bank[MONTH32BIT_MAX]={};

struct account
{
    flt data[YEAR32BIT_MAX];
    account()
    {
        memset(data,0,YEAR32BIT_MAX*sizeof(flt));
    }
};
std::map<uint32_t,account> holders;

std::vector<std::string> tokenize_csv_line(std::string source,char delim)
{
    std::vector<std::string> retval;
    uint32_t I=source.size();
    for(uint32_t i=0,j=0;i<I;i++) if(source[i]==delim or source[i]=='\n')
    {
        retval.push_back(source.substr(j,i-j));
        i++;
        j=i;
    }
    return retval;
}

bool proccess(char *fname)
{
    FILE *file=strcmp(fname,"-")?fopen(fname,"r"):stdin;
    if(!file) return false;
    char line[LINE_SIZE];
    uint32_t k=0;
    while(fgets(line,LINE_SIZE,file))
    {
        if(line[0]=='#') continue;
        else
        {
            const std::vector<std::string> csv_line=tokenize_csv_line(line,DELIM);
            if(csv_line.size()==3)
            {
                const std::vector<std::string> data_date=tokenize_csv_line(csv_line[2],'-');
                if(data_date.size()==2)
                {
                    k++;
                    const uint32_t uid=std::stoul(csv_line[0].c_str(),nullptr,10);
                    uint32_t year=std::stoul(data_date[0].c_str(),nullptr,10);
                    uint32_t month=std::stoul(data_date[1].c_str(),nullptr,10);
                    const flt value=atof(csv_line[1].c_str());
                    holders[uid].data[year-1970U]+=value;
                    bank[12*(year-1970U)+month]+=value;
                }
            }
        }
    }
    if(k==0) return false;
    else return true;
}

void print_holders()
{
    printf("### HOLDERS\n");
    for(auto x:holders)
    {
        flt current=ZERO;
        printf("## HOLDER %u\n",x.first);
        for(uint32_t i=0;i<YEAR32BIT_MAX;++i) if(x.second.data[i]!=ZERO) printf("%u,%u,%g\n",1970U+i,x.first,current+=x.second.data[i]);
    }
}

void print_bank()
{
    flt current=ZERO;
    printf("### BANK\n");
    for(uint32_t i=0;i<MONTH32BIT_MAX;++i) if(bank[i]!=ZERO) printf("%u,%u,%g\n",1970U+i/12U,i%12U,current+=bank[i]);
}

int main(int argc,char *argv[])
{

    const char* HELP_MSG="%s: file1 [file2] ... (stdin='-')\n";
    if(argc<2)
    {
        printf(HELP_MSG,argv[0]);
        return EXIT_FAILURE;
    }
    else
    {
        if(!strcmp(argv[1],"--help") or !strcmp(argv[1],"-h")) printf(HELP_MSG,argv[0]);
        else
        {
            int corrupt_files=0;
            for(int arg=1;arg<argc;++arg) if(!proccess(argv[arg]))
            {
                printf("%s: %s is invalid\n",argv[0],argv[arg]);
                corrupt_files++;
            }
            if(corrupt_files==argc-1)
            {
                printf("%s: zero files were read\n",argv[0]);
                return EXIT_FAILURE;
            }
            print_holders();
            printf("\n");
            print_bank();
        }
        return EXIT_SUCCESS;
    }
}
