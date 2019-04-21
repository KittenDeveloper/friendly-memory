#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
int main(int argc, char **argv)
{
        FILE *sda= fopen("/dev/sda", "r");
        FILE *sdb= fopen("/dev/sdb", "w");
        uint_8 *buffer= malloc(26000000000);
        size_t nr;
        while(true)
        {
                nr= fread(buffer, 26000000000, 1, sda);
                fwrite(buffer, nr, 1, sdb);
                if(nr!=26000000000) break;
        }
        fclose(sda);
        fclose(sdb);
        return 0;
}
