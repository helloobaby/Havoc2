#include <stdio.h>
#include <ctype.h>
#include <stdint.h>

long Hash( char* String )
{
  unsigned long Hash = 5381;
	int c;

	while (c = *String++)
		Hash = ((Hash << 5) + Hash) + c;

	return Hash;
}

__int64_t HashString( void* String, void* Length )
{
    int	Hash = 5381;
    unsigned char*	Ptr  = String;

    do
    {
        unsigned char character = *Ptr;

        if ( ! Length )
        {
            if ( !*Ptr ) break;
        }
        else
        {
            if ( (unsigned long) ( Ptr - (unsigned char*)String ) >= Length ) break;
            if ( !*Ptr ) ++Ptr;
        }

        if ( character >= 'a' )
            character -= 0x20;

        Hash = ( ( Hash << 5 ) + Hash ) + character;
        ++Ptr;
    } while ( 1 );

    return Hash;
}

void ToUpperString(char * temp) {
  // Convert to upper case
  char *s = temp;
  while (*s) {
    *s = toupper((unsigned char) *s);
    s++;
  }
}

int main(int argc, char** argv) 
{
  if (argc < 2)
    return 0;
  ToUpperString(argv[1]);

  printf("\n[+] Hashed %s ==> 0x%x\n\n", argv[1], Hash( argv[1] ));
  
  // https://gchq.github.io/CyberChef/#recipe=Encode_text('UTF-16LE%20(1200)')To_Hex('0x%20with%20comma',0)&input=bnRkbGwuZGxs
  // char ntdlltest[]={0x6e,0x00,0x74,0x00,0x64,0x00,0x6c,0x00,0x6c,0x00,0x2e,0x00,0x64,0x00,0x6c,0x00,0x6c,0x00};
  // char kernel32test[]={0x6b,0x00,0x65,0x00,0x72,0x00,0x6e,0x00,0x65,0x00,0x6c,0x00,0x33,0x00,0x32,0x00,0x2e,0x00,0x64,0x00,0x6c,0x00,0x6c,0x00};
  // char ldrloaddll[]={0x6c,0x64,0x72,0x6c,0x6f,0x61,0x64,0x64,0x6c,0x6c};
  // printf("\n[+] Hashed ntdll full path.dll ==> 0x%x\n\n",HashString(kernel32test,24)); 
  return 0;
}
