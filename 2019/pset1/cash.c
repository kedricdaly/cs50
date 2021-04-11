// cs50 week2

#include <stdio.h>
#include <cs50.h>
#include <math.h>

// 0. get user input
// 1. convert float dollars.cents input to int cents via round
// 2. take away as many of the largest coins as possible, while keeping
//    track of how many total coins used.

// Declarations
int getCoins(int centsNeeded, int coinValue);


int main(void)
{
  string prompt = "Change owed: ";
  float changeNeeded;
  int nCoins = 0;

  // Get user input as a float
  do
  {
    changeNeeded = get_float("%s", prompt);
  }
  while (changeNeeded < 0);

  // Round change to nearest cent
  int centsNeeded = round(changeNeeded * 100);

  // Get number of coins needed per coin
  // Use modulo math and can do it in one loop
  int coinValue;
  while (centsNeeded > 0)
  {
    coinValue = 25;
    nCoins += getCoins(centsNeeded,coinValue);
    centsNeeded = centsNeeded % coinValue;

    coinValue = 10;
    nCoins += getCoins(centsNeeded,coinValue);
    centsNeeded = centsNeeded % coinValue;

    coinValue = 5;
    nCoins += getCoins(centsNeeded,coinValue);
    centsNeeded = centsNeeded % coinValue;

    coinValue = 1;
    nCoins += getCoins(centsNeeded,coinValue);
    centsNeeded = centsNeeded % coinValue;
  }

  printf("nCoins: %d\n", nCoins);

}

// Functions

// Returns number of coins given
//  A specific amount of centsNeeded and a specific coinValue
int getCoins(int centsNeeded, int coinValue)
{
  int nCoins = 0;

  while (centsNeeded >= coinValue)
  {
    nCoins = centsNeeded / coinValue;
    centsNeeded = centsNeeded % coinValue;
  }

  return nCoins;

}
