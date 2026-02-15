#include <stdio.h>
#include <png.h>

#define A 123

#ifdef A
    #if A == 123 
        #define A_IS_123 true
    #else
        #define A_IS_123 false
    #endif
#endif

#if A_IS_123
    #include <sys-header.h>
    #include "a-is-123.h"
#else
    #include "nah.h"
#endif

const byte[] elash_logo = #embed "../assets/logo.png";
const usize elash_logo_size = sizeof(elash_logo);

int main() { 
    PngImg png = PngImg::LoadFromMem(elash_logo, elash_logo_size);
    #ifdef DEBUG_BUILD
        log_debugf("Image size: ", png.getSize());
    #endif
    png.paintRect({ { 100, 100 }, { 300, 150 } }, Color::Black);
    png.save("output.png");
 
    return 0;
}
