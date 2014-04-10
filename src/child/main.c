#include "child.h"

int main(int argc, char **argv)
{
  main1(argc, argv); /* test framework "check" needs main function 
		      * separated from other functions */
  return 0;
}
