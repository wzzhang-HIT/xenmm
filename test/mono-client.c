#define IMPLEMENT_CLIENT
// with this macro, it means we provide our self implement for client.  in
// here, we provide a client, which doesn't read real memory info from
// /proc/meminfo. but a generated fake memory number from low to high and from
// high to low. just like mm_test_mono. it can make mmserver adjust memory all
// the time.
//
// we can test the program cpu performance decrease with this client.

#include "../src/mmclient.c"

#define DIRECT_DECREASE 1
#define DIRECT_INCREASE 0
#define max(a,b) a<b?b:a

static void show_help(char* prog)
{
   printf("useage: %s :Base :Delta\n"
         "Delta is changed mem in every 2 seconds\n",
         prog);
}

static int unit_expand(char u)
{
    switch(u){
        case 'k':
        case 'K':
            return 1;
        case 'm':
        case 'M':
            return 1024;
        case 'G':
            return 1024*1024;
        default:
            return 1;
    }
}

int main(int argc, char** argv)
{
   MemInfo mem, real_mem;
   mem_t used_mem, real_used_mem;
   char * unit = NULL;

   if(argc!=3){
      show_help(argv[0]);
      return 0;
   }

   double base_mem = strtod(argv[1], &unit);
   base_mem *= unit?unit_expand(*unit):1024;

   double speed_mem = strtod(argv[2], &unit);
   speed_mem *= unit?unit_expand(*unit):1024;
   speed_mem /= 2; // changed in every two seconds

   if(s_g_init()) return 0;

   read_mem(&real_mem);
   mem = real_mem;
   used_mem = mem.tot_mem - mem.free_mem;
   used_mem = max(base_mem, used_mem);
   int direction = DIRECT_DECREASE;
   while(1){
      read_mem(&real_mem);
      real_used_mem = real_mem.tot_mem - real_mem.free_mem;
      real_used_mem = max(real_used_mem, base_mem);
      mem.tot_mem = real_mem.tot_mem;

      if(direction == DIRECT_DECREASE) 
         used_mem - speed_mem > real_used_mem ? used_mem -= speed_mem : (direction = DIRECT_INCREASE);
      else
         if( used_mem + speed_mem < real_mem.tot_mem ) 
            used_mem += speed_mem ;
         else {
            int i=3;
            do{
               sleep(1);
            }while(--i);
            direction = DIRECT_DECREASE;
         }

      mem.free_mem = mem.tot_mem - used_mem;
      s_g_write_mem(mem);
      sleep(1);
   }

   s_g_close();
}
