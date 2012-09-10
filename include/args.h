#pragma once

struct Args
{
    unsigned int count;
    char** argv;
    Args(unsigned int argc, char** argv): count(argc), argv(argv) {}
    const char* progName()
    {
        const char* result = argv[0];
        for(const char* p=result; *p; p++){
            if((*p == '/') || (*p == '\\')){
                result = p + 1;
            }
        }
        return result;
    }
    bool has(const char* arg)
    {
        for(unsigned int i=0; i<count; i++){
            if(0 == strcmp(argv[i], arg)){
                return true;
            }
        }
        return false;
    }
};
