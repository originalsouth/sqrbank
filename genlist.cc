#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <ctime>
#include <cstring>

uint32_t yeartoepoch(uint32_t year)
{
    static const uint32_t SECONDS_PER_YEAR=(uint32_t)(60*60*24*365.24);
    return SECONDS_PER_YEAR*(year-1970U);
}

void gen_list(uint32_t I,uint32_t CLIENTS,uint32_t start=0U,uint32_t stop=UINT32_MAX)
{
    for(uint32_t i=0U;i<I;++i)
    {
        char random_time_string[32];
        const time_t random_time=(time_t)(uint32_t)(start+drand48()*(stop-start));
        strftime(random_time_string,32,"%Y-%m-%d %H:%M:%S",localtime(&random_time));
        printf("%u,%g,%s\n",(uint32_t)(drand48()*CLIENTS),(drand48()-0.4)*1e3,random_time_string);
    }
}

int main(int argc,char *argv[])
{
    srand48(time(nullptr));
    const char* HELP_MSG="%s: [size=100] [clients=10] [startyear=1970] [endyear=2106]\n";
    switch(argc)
    {
        case 1:
            gen_list(100U,10U);
            return EXIT_SUCCESS;
        case 2:
            if(!strcmp(argv[1],"--help") or !strcmp(argv[1],"-h")) printf(HELP_MSG,argv[0]);
            else gen_list(atoi(argv[1]),10U);
            return EXIT_SUCCESS;
        case 3:
            gen_list(atoi(argv[1]),atoi(argv[2]));
            return EXIT_SUCCESS;
        case 4:
            gen_list(atoi(argv[1]),atoi(argv[2]),yeartoepoch(atoi(argv[3])));
            return EXIT_SUCCESS;
        case 5:
            gen_list(atoi(argv[1]),atoi(argv[2]),yeartoepoch(atoi(argv[3])),yeartoepoch(atoi(argv[4])));
            return EXIT_SUCCESS;
        default:
            printf(HELP_MSG,argv[0]);
            return EXIT_FAILURE;
    }
}
