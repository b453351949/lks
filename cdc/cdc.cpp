#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <iostream>
#include <cstdint>
#include <chrono>



#define WIN_SIZE 16
#define PRIME 3
#define MODULUS 256
#define TARGET 0

class stopwatch
{
          public:
                  double total_time, calls;
                          std::chrono::time_point<std::chrono::high_resolution_clock> start_time, end_time;
                              stopwatch() : total_time(0), calls(0) {};

                  inline void reset() {
                                                total_time = 0;
                                                      calls = 0;
                                                          }

                  inline void start() {
                                                    start_time = std::chrono::high_resolution_clock::now();
                                                          calls++;
                                                              };

                  inline void stop() {
                                                        end_time = std::chrono::high_resolution_clock::now();
                                                              auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time-start_time).count();
                                                                    total_time += static_cast<double>(elapsed);
                                                                        };

                //return latency in ns
                  inline double latency() {
                         return total_time;
                  };

                // return latency in ns
                  inline double avg_latency() {
                         return (total_time / calls);
                  };
};



uint64_t hash_func(unsigned char *input, unsigned int pos);
void cdc(unsigned char *buff, unsigned int buff_size);

uint64_t hash_func(unsigned char *input, unsigned int pos)
{
	uint64_t hash=0;
	for(int i=0;i<WIN_SIZE;i++)
		hash += (uint64_t)input[pos+WIN_SIZE-1-i]*(pow(PRIME, i+1));

	
	return hash;
}

void cdc(unsigned char *buff, unsigned int buff_size)
{
        stopwatch time_cdc1;
	time_cdc1.start();	
	for(unsigned int i=WIN_SIZE;i<buff_size-WIN_SIZE;i++){
  		 if(((hash_func(buff,i)%MODULUS))== TARGET)		
			printf("%d ",i);}

	time_cdc1.stop();
	std::cout << "Total latency of cdc1 is: " << time_cdc1.latency() << " ns." << std::endl;
	std::cout << "Average latency of cdc1 per loop iteration is: " << time_cdc1.avg_latency() << " ns." << std::endl;
}


void test_cdc( const char* file )
{
	FILE* fp = fopen(file,"r" );
	if(fp == NULL ){
		perror("fopen error");
		return;
	}

	fseek(fp, 0, SEEK_END); // seek to end of file
	int file_size = ftell(fp); // get current file pointer
	fseek(fp, 0, SEEK_SET); // seek back to beginning of file

	unsigned char* buff = (unsigned char *)malloc((sizeof(unsigned char) * file_size ));	
	if(buff == NULL)
	{
		perror("not enough space");
		fclose(fp);
		return;
	}

	int bytes_read = fread(&buff[0],sizeof(unsigned char),file_size,fp);

	cdc(buff, file_size);

    free(buff);
    return;
}

int main()
{
	test_cdc("prince.txt");
	return 0;
}
