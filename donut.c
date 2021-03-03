__asm__(".code16\n");

#define ONLYROTATEX 1

#define SCREEN_HEIGHT 200
#define SCREEN_WIDTH 320

#define R1 1
#define R2 2
#define K2 10

#define COLOR 15

#if ONLYROTATEX

#define NTABLE 16
#define DELAY 128

float sin[] = {0.000000, 0.382499, 0.706825, 0.923651, 1.000000, 0.924260, 0.707951, 0.383971, 0.001593, -0.381027, -0.705698, -0.923039, -0.999997, -0.924867, -0.709075, -0.385441};
float cos[] = {1.000000, 0.923956, 0.707388, 0.383235, 0.000796, -0.381764, -0.706262, -0.923345, -0.999999, -0.924564, -0.708513, -0.384706, -0.002389, 0.380291, 0.705133, 0.922733};
#else

#define NTABLE 12
#define DELAY 256

float sin[] = {0.000000, 0.505533, 0.872355, 0.999816, 0.852940, 0.472031, -0.038398, -0.538291, -0.890484, -0.998341, -0.832267, -0.437832, };
float cos[] = {1.000000, 0.862807, 0.488872, -0.019202, -0.522008, -0.881582, -0.999263, -0.842759, -0.455015, 0.057579, 0.554374, 0.899057, };
#endif

void main() {
     const float K1 = SCREEN_WIDTH*K2*2/(8*(R1+R2));

     int cnt1 = 0;
     int cntA = 0;

     for(;;) 
     {
          float sinA;
          float cosA;

          if(cnt1%DELAY == 0) 
          {
               sinA = sin[cntA];
               cosA = cos[cntA];
               cntA++;
               cntA &= NTABLE-1;
               __asm__ __volatile__ ("int $0x10" : : "a"(0x0013));
          }

          for(int theta = 0; theta < NTABLE; theta += 1) 
          {
               float costheta = cos[theta];
               float sintheta = sin[theta];

               for(int phi = 0; phi < NTABLE; phi += 1)
               {
                    float cosphi = cos[phi];
                    float sinphi = sin[phi];
                                    
                    // rotation X
                    #if ONLYROTATEX
                    float x = (R2+R1*costheta)*cosphi;
                    float y = ((R1*sintheta)*cosA + (R2+R1*costheta)*sinphi*sinA);
                    float z = R1*sintheta*sinA - (R2+R1*costheta)*sinphi*cosA;
                    #else
                    float x1 = (R2+R1*costheta)*cosphi; 
                    float y1 = ((R1*sintheta)*cosA + (R2+R1*costheta)*sinphi*sinA);
                    float x = x1*cosA - y1*sinA;
                    float y = x1*sinA + y1*cosA;
                    float z = R1*sintheta*sinA - (R2+R1*costheta)*sinphi*cosA;
                    #endif
                    x = K1*x/(K2+z); y = K1*y/(K2+z);

                    x += SCREEN_WIDTH/2; y += SCREEN_HEIGHT/2;

                    __asm__ __volatile__ ("int $0x10" : : "a"(0x0c00 | COLOR), "c"((int)x), "d"((int)y));
               }

          }

          cnt1++;
     }
}
