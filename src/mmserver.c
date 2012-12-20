#include "type.h"
#include "mmstore.h"


int main()
{
    if(s_h_init()==MM_FAILED){
        return -1;
    }
    s_h_list_domains(NULL);
    s_h_close();
}
