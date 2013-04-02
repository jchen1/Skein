#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "skein.h"
#include <assert.h>

void increment();
int difference(uint64_t *out);
unsigned long completed = 0;

bool done = false;

char in[10] = "60002E3dBo";
uint8_t answer[128] = {91, 77, 169, 95, 95, 160, 130, 128,
	252, 152, 121, 223, 68, 244, 24, 200, 249, 241, 43,
	164, 36, 183, 117, 125, 224, 43, 189, 251, 174, 13,
	76, 79, 223, 147, 23, 200, 12, 197, 254, 4, 198, 66,
	144, 115, 70, 108, 242, 151, 6, 184, 194, 89, 153,
	221, 210, 246, 84, 13, 68, 117, 204, 151, 123, 135,
	244, 117, 123, 224, 35, 241, 155, 143, 64, 53, 215,
	114, 40, 134, 183, 136, 105, 130, 109, 233, 22, 167,
	156, 249, 201, 76, 199, 156, 212, 52, 125, 36, 181,
	103, 170, 62, 35, 144, 165, 115, 163, 115, 164, 138,
	94, 103, 102, 64, 199, 156, 199, 1, 151, 225, 197,
	231, 249, 2, 251, 83, 202, 24, 88, 182};

int main()
{
	Skein1024_Ctxt_t skein;
	

	uint8_t out[128];

	/*
	uint8_t in[10] = {122, 122, 122, 122, 122, 122, 122, 122, 122, 122};
	Skein1024_Init(&skein, 1024);
	Skein1024_Update(&skein, in, 10);
	Skein1024_Final(&skein, out);
	int i = 0;
	for(i = 0; i < 128; i++)
	{
		printf("%x", out[i]);
	}
	printf("\n%d", difference(out));*/
	
	while (!done) {

		memset(&skein, 0, sizeof(Skein1024_Ctxt_t));
		memset(out, 0, 128);

		Skein1024_Init(&skein, 1024);
		Skein1024_Update(&skein, in, 10);
		Skein1024_Final(&skein, out);

		int diff = difference((uint64_t*)out);
		if (diff < 412)
		{
			printf("Found ");
			int i = 0;
			for (i = 0; i < 10; i++)
			{
				printf("%c", in[i]);
			}
			
			printf(" with hash ");
			for (i = 0; i < 128; i++)
			{
				printf("%x", out[i]);
			}
			printf(" with %d bits off\n", diff);
		}

		increment(9);
		completed++;
		//if (completed == 1000000) return; //Used for timing
		if (!(completed%10000000)) printf("Checked %lu hashes, %s last\n", completed, in);
	}
	
}

const uint64_t m1  = 0x5555555555555555; //binary: 0101...
const uint64_t m2  = 0x3333333333333333; //binary: 00110011..
const uint64_t m4  = 0x0f0f0f0f0f0f0f0f; //binary:  4 zeros,  4 ones ...
const uint64_t m8  = 0x00ff00ff00ff00ff; //binary:  8 zeros,  8 ones ...
const uint64_t m16 = 0x0000ffff0000ffff; //binary: 16 zeros, 16 ones ...
const uint64_t m32 = 0x00000000ffffffff; //binary: 32 zeros, 32 ones
const uint64_t hff = 0xffffffffffffffff; //binary: all ones
const uint64_t h01 = 0x0101010101010101; //the sum of 256 to the power of 0,1,2,3...

int difference(uint64_t *out)
{
	int count = 0, i = 0;
	
	for (i = 0; i < 16; i++)
	{
		uint64_t x = ((uint64_t*)answer)[i] ^ out[i];
		x -= (x >> 1) & m1;             //put count of each 2 bits into those 2 bits
	    x = (x & m2) + ((x >> 2) & m2); //put count of each 4 bits into those 4 bits 
	    x = (x + (x >> 4)) & m4;        //put count of each 8 bits into those 8 bits 
	    count += (x * h01)>>56;  //returns left 8 bits of x + (x<<8) + (x<<16) + (x<<24) + ... 
	}

	return count;
}

void increment(int digit)
{
	in[digit]++;

	if (in[digit] == 58) in[digit] = 65;
	if (in[digit] == 91) in[digit] = 97;
	if (in[digit] == 123)
	{
		in[digit] = 48;
		if (digit != 0)
		{
			increment(--digit);
		}
		else done = true;
	}

}