/* The implementation reference of FlexenTech.c S16|rnd4 */
#include<stdio.h>
#define S              16       // data size in bits
#define rnd            4        // number of rounds
unsigned int N     =   101;     // the key value
unsigned int nonce =   3;       // the used nonce value

#define ROTR(x,y) (((x)>>(y&(S-1))) | ((x)<<(S-(y&(S-1)))))
//Setup Configuration
void TableSetup();

short unsigned int plainData= 37402, rotation, tempData; // 37402 is 1001001000011010 
static const int randValue = 0;
static const int decrypt = 1;
static const int encrypt = 2;
static int table[3][S]; // the encryption table
int main()
{
    TableSetup();
    printf("plain data  = %.4X\n",plainData);
    for (int rd = 1 ;rd <= rnd; rd++)
    {
        //Rotation
        rotation = (N * (rd+nonce))%S-1;
        tempData = ROTR(plainData, rotation);
        //reverse
        tempData = tempData^((short unsigned int)65535>>rotation);
        //permutation
        plainData = 0;
        for (short int i = (S-1); i >= 0; i--)
            if ((tempData >> i) & 1)
                plainData = plainData^(1<<((S-1)-table[encrypt][(S-1)-i])); 
        printf("encrypted %d = %.4X\n",rd,plainData);
    }
    return 0;
}
void TableSetup(){
    //Compute and Fill the table with V values
    for (int i = 0; i < S; ++i) 
    {
        table[randValue][i] =(S * (N - i - 1)) % N;
        table[decrypt][i] = i;
    }
    //sort the V values and keep the decryption feedback
    int temp;
    for (int i = 0; i < S; ++i) 
        {
            for (int j = i + 1; j < S; ++j)
            {
                if (table[randValue][i] > table[randValue][j]) 
                {
                    temp =  table[randValue][i];
                    table[randValue][i] = table[randValue][j];
                    table[randValue][j] = temp;
                    temp =  table[decrypt][i];
                    table[decrypt][i] = table[decrypt][j];
                    table[decrypt][j] = temp;
 
                }
            }
        }
    //keep the encryption feedback
    for (short int i = 0; i < S; ++i) 
        {
            for (short int j = 0; j < S; ++j)
            {
 
                if (table[decrypt][j] == i){
                    table[encrypt][i] = (short int)j;
                } 
                    
            }
        }
}
----------------------------------------------------------------------------
----------------------------------------------------------------------------
Results:
plain data  = 921A
encrypted 1 = F0B9
encrypted 2 = 2B5D
encrypted 3 = 3CEB
encrypted 4 = 0FAC
