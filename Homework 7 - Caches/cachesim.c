#include <stdio.h>
#include <stdlib.h>
#include <cstring>

struct block
{
    int tag;
    int count;
    char *data;
};


typedef struct {
    int Hit;
    int value;
} isHitResult;



int cache_size;
int associativity;
int block_size;
int num_of_frames;
int num_of_sets;

char main_memory[16777216];

int log2(int num)
{
    int r = -1;
    while (num > 0) {
        num >>= 1;
        r++;
    }
    return r;
}


void copy_mem(char *a, const char *b, int size)
{
    std::memcpy(a, b, size);
}

int main(int argc, char *argv[])
{
    
    int address;
    int access_size;

    int Hit;
    int WayHit;

    char instruction[10];
    cache_size = atoi(argv[2]) * 1024;
    // printf("The cache size is: %d\n", cache_size);
    // printf("\n");
    associativity = atoi(argv[3]);
    // printf("The associativit is: %d\n", associativity);
    // printf("\n");
    block_size = atoi(argv[4]);
    // printf("The block_size is: %d\n", block_size);
    // printf("\n");

    num_of_frames = cache_size / block_size;

    // printf("The num_of_frames is: %d\n", num_of_frames);
    // printf("\n");

    num_of_sets = num_of_frames / associativity;
    // printf("The num_of_sets is: %d\n", num_of_sets);
    // printf("\n");
    FILE *file = fopen(argv[1], "r");


    struct block cache[num_of_sets][associativity];
    for (int i = 0; i < num_of_sets; i++)
    {
        for (int j = 0; j < associativity; j++)
        {
            cache[i][j].tag = 0;
            cache[i][j].count = -1;
            cache[i][j].data = (char *)malloc(block_size);
        }
    }

    while (fscanf(file, "%s", instruction) != EOF)
    {
        int Hit = 0;
        
        int address_offset_bit = log2(block_size);

        int set_bit = log2(num_of_sets);

        int tag_bit = 24 - address_offset_bit - set_bit;

        fscanf(file, "%x", &address);
        fscanf(file, "%d", &access_size);
        // printf("The access_size is: %d\n", access_size);
        // printf("\n");
        

        int address_offset =  (((1 << address_offset_bit) - 1)) & address;

        int address_tag = address >> (address_offset_bit + set_bit);

        // printf("The address_tag is: %d\n", address_tag);
        // printf("\n");

        int address_index = ((1 << set_bit) - 1) & (address >> address_offset_bit);
        
        // keep track of which one is the least recently used
        for (int i = 0; i < associativity; i++)
        {
            if (cache[address_index][i].count != -1) // if it is not empty, count++
            {
                cache[address_index][i].count = cache[address_index][i].count + 1;
            }
           
        }
        
        // check if it is a hit
        for (int i = 0; i < associativity; i++)
        {
            if (cache[address_index][i].count != -1)
            {
                if (cache[address_index][i].tag == address_tag)
                    {
                        cache[address_index][i].count=0;// reset the count
                        Hit = 1;// hit
                        WayHit = i;// which way is hit
                        break;
                    }
            }
           
        }

        // load
        if (strcmp(instruction, "load") == 0)
        {
            if (Hit == 1)
            {
                printf("%s 0x%x hit ", instruction, address);

                    for (int i = 0; i < access_size; i++)
                    {
                        printf("%02hhx", cache[address_index][WayHit].data[address_offset + i]);
                    }
                    printf("\n");
                
            }
            if (Hit != 1)
            {  
                int MaxCount=-2;// the max count
                int ReplaceWay;// the way to be replaced
                for (int i=0;i < associativity;i++)
                {
                    if (cache[address_index][i].count == -1)// First replace the empty block
                    {
                        ReplaceWay = i;
                        break;
                    }

                    if (cache[address_index][i].count > MaxCount)
                    {
                        MaxCount = cache[address_index][i].count;
                        ReplaceWay = i;
                    }
                }
                cache[address_index][ReplaceWay].count = 0;// reset the count
                cache[address_index][ReplaceWay].tag = address_tag;// update the tag
                copy_mem(cache[address_index][ReplaceWay].data, &main_memory[address - address_offset], block_size);// copy the data from the main memory to the cache
                // printf("The data is store in set %d ,the way %d\n",address_index,ReplaceWay);
                printf("%s 0x%x miss ", instruction, address);
                for (int i = 0; i < access_size; i++)
                {
                    printf("%02hhx", cache[address_index][ReplaceWay].data[address_offset + i]);
                }
                printf("\n");
            }
        }

        // store
        if (strcmp(instruction, "store") == 0)// if it is a store
        {
            char input;// input from the file
            for (int i = 0; i < access_size; i++)// read the data from the file
            {
                fscanf(file, "%2hhx", &input);// read the data from the file
                main_memory[address + i] = input;// store the data in the main memory
                
            }
            // printf("westore %02hhx in the main memory @ %d - %d \n", input, address, address + access_size);

            // copy the data from main memory to cache
            if (Hit)
            {
                    if (cache[address_index][WayHit].count == 0)
                    {
                        copy_mem(cache[address_index][WayHit].data, &main_memory[address - address_offset], block_size);
                    }
                printf("%s 0x%x hit\n", instruction, address);
            }
            else
            {
                printf("%s 0x%x miss\n", instruction, address);
            }
        }

    }
    return 0;
}