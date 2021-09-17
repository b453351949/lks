#include "App.h"
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <cstdint>
#include <iostream>

#define DATA_SIZE (4000 * 3000)
#define STAGES (4)

unsigned char * Data[STAGES + 1];

void Exit_with_error(void)
{
  perror(NULL);
  exit(EXIT_FAILURE);
}

void Load_data(void)
{
  FILE * File = fopen("Input.bin", "rb");
  if (File == NULL)
    Exit_with_error();

  if (fread(Data[0], 1, DATA_SIZE, File) != DATA_SIZE)
    Exit_with_error();

  if (fclose(File) != 0)
    Exit_with_error();
}

void Store_data(const char * Filename, int Stage, unsigned int Size)
{
  FILE * File = fopen(Filename, "wb");
  if (File == NULL)
    Exit_with_error();

  if (fwrite(Data[Stage], 1, Size, File) != Size)
    Exit_with_error();

  if (fclose(File) != 0)
    Exit_with_error();
}


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
					     

int main()
{

 stopwatch time_scale;
 stopwatch time_differentiate;
 stopwatch time_compress;
 stopwatch total_time;
 int Size;
 total_time.start();

  for (int i = 0; i <= STAGES; i++)
  {
    // We could strictly allocate less memory for some of these buffers, but
    // that is irrelevant here.
    Data[i] = (unsigned char*)malloc(DATA_SIZE);
    if (Data[i] == NULL)
      Exit_with_error();
  }
  Load_data();

  time_scale.start();
  for(int i=0;i<100;i++){
  Scale(Data[0], Data[1]);}
  time_scale.stop();
  
 
  Filter(Data[1], Data[2]);


  time_differentiate.start();
  for(int i=0;i<100;i++){

  Differentiate(Data[2], Data[3]);
  }
  time_differentiate.stop();

  time_compress.start();
 
  for(int i=0;i<100;i++){

  Size = Compress(Data[3], Data[4]);
  }
  time_compress.stop();


  Store_data("Output.bin", 4, Size);

  for (int i = 0; i <= STAGES; i++)
    free(Data[i]);

  puts("Application completed successfully.");
  total_time.stop();

  std::cout << "Total latency of scale is: " << time_scale.latency() << " ns." << std::endl;
  
 // std::cout << "Total latency of filter is: " << time_filter.latency() << " ns." << std::endl;
  std::cout << "Total latency of differentiate is: " << time_differentiate.latency() << " ns." << std::endl;
  std::cout << "Total latency of compress is: " << time_compress.latency() << " ns." << std::endl;
  std::cout << "Total time taken by the loop is: " << total_time.latency() << " ns." << std::endl;
  std::cout << "---------------------------------------------------------------" << std::endl;
  std::cout << "Average latency of scale per loop iteration is: " << time_scale.avg_latency() << " ns." << std::endl;
   
 // std::cout << "Average latency of filter per loop iteration is: " << time_filter.avg_latency() << " ns." << std::endl;
  std::cout << "Average latency of differentiate per loop iteration is: " << time_differentiate.avg_latency() << " ns." << std::endl;
  std::cout << "Average latency of compress per loop iteration is: " << time_compress.avg_latency() << " ns." << std::endl;
  std::cout << "Average latency of each loop iteration is: " << total_time.avg_latency() << " ns." << std::endl;
  return EXIT_SUCCESS;
}


