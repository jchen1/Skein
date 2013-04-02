#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "skein.h"

void increment();
int difference(uint8_t *out);
unsigned long completed = 0;

bool done = false;

char in[10] = "20001foVTU";
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

		int diff = difference(out);
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
		if (!(completed%10000000)) printf("Checked %lu hashes, %s last\n", completed, in);
	}
	
}

int difference(uint8_t *out)
{
	int count = 0, i = 0, j = 0;
		
	for (i = 0; i < 32; i++)
	{
		uint32_t v = ((uint32_t*)answer)[i] ^ ((uint32_t*)out)[i];
		//MAGIC that gets number of 1s in a 32-bit int
		//Only works on x64 processors with fast mod division according to
		//http://graphics.stanford.edu/~seander/bithacks.html
		uint32_t c =  ((v & 0xfff) * 0x1001001001001ULL & 0x84210842108421ULL) % 0x1f;
		c += (((v & 0xfff000) >> 12) * 0x1001001001001ULL & 0x84210842108421ULL) % 
		     0x1f;
		c += ((v >> 24) * 0x1001001001001ULL & 0x84210842108421ULL) % 0x1f;
		count += c;
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