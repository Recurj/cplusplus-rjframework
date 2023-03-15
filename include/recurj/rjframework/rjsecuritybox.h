#pragma once
#ifdef _uToolset_GCC
#define SECURITY_BOX_LIB(p)  p[0]  = 'u'; p[1]  = 'w'; p[2]  = 'l'; p[3]  = 'd';p[4]  = 'a'; \
        p[5]  = 't'; p[6]  = 'a'; p[7]  = 't'; p[8]  = 'o';p[9]  = 'o'; \
        p[10] = 'l'; p[11] = 's'; p[12] = '.'; p[13] = 'd';p[14] = 'l'; \
        p[15] = 'l'; p[16] = 0x00
#else 
#define SECURITY_BOX_LIB(p)  p[0]  = L'u'; p[1]  = L'w'; p[2]  = L'l'; p[3]  = L'd';p[4]  = L'a'; \
        p[5]  = L't'; p[6]  = L'a'; p[7]  = L't'; p[8]  = L'o';p[9]  = L'o'; \
        p[10] = L'l'; p[11] = L's'; p[12] = L'.'; p[13] = L'd';p[14] = L'l'; \
        p[15] = L'l'; p[16] = 0x00
#endif 
#define SIZE_SECURITY_BOX_LIB 16
#define SECURITY_BOX_ENCRYPT(p) p[0] = 'B';p[1] = 'u';p[2] = 'i';p[3] = 'l';p[4] = 'd'; \
         p[5] = 'U';p[6] = 'r';p[7] = 'l';p[8]=0x00
#define SIZE_SECURITY_BOX_ENCRYPT 8
#define SECURITY_BOX_DECRYPT(p) p[0] = 'F';p[1] = 'o';p[2] = 'r';p[3] = 'm';p[4] = 'a';p[5] = 't'; \
         p[6] = 'U';p[7] = 'r';p[8] = 'l';p[9] = 0x00
#define SIZE_SECURITY_BOX_DECRYPT 9
#define SECURITY_BOX_MARKER(p) p[0] = 'U'; p[1] = 'N'; p[2] = 'I'; p[3] = 'C';\
        p[4] = 'O'; p[5] = 'D'; p[6] = 'E'; p[7] = 'S'; p[8] = 0x00
#define SIZE_SECURITY_BOX_MARKER 8

