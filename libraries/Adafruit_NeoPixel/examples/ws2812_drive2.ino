//***********************************************************************
//***********************************************************************
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN 6
#define PixNum 6 //количество пикселей в ленте
Adafruit_NeoPixel strip = Adafruit_NeoPixel(6, PIN, NEO_RGB + NEO_KHZ800);
//***********************************************************************
//***********************************************************************
void setup() 
{
  Serial.begin(9600);
  strip.setBrightness(255);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  randomSeed(analogRead(0));
}
//***********************************************************************
//***********************************************************************
void loop() 
{
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //++++++++++++++++++ Пеебо ╜┤┤ектов в азн╗х сочетани┐х с азн╗ми гветами и веменем в╗полнени┐ +++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    FvdFillPixels(50, 0, 0, 0); OncomingPixels(90, 200, 80, 0, 200, 80, 0); FvdRunOnePixel(20, 200, 80, 0); RevRunOnePixel(20, 200, 80, 0); 
    FvdRunOnePixel(25, 0, 255, 0); RevRunOnePixel(30, 0, 255, 0); FvdRunOnePixel(25, 0, 0, 255); RevRunOnePixel(30, 0, 0, 255); 
    RevFillPixels(100, 225, 100, 0); BlinkAllPix(15, 15, 15, 100, 225, 0, 255, 0, 0); delay(200); FvdTurnOffAllPix(3,2); 
    FvdFillPixels(100, 225, 100, 0); RevTurnOffAllPix(3,2); RevRunMixPixel(50, 200, 20, 30); FvdRunMixPixel(50, 30, 20, 70);
    FvdFillPixels(50, 55, 55, 55); FvdRunOnePixel(35, 255, 0, 0);  RevRunOnePixel(40, 255, 0, 0);  RevFillPixels(50, 55, 10, 10);
    FvdRunOnePixel(35, 0, 255, 0); RevRunOnePixel(40, 0, 255, 0); RevFillPixels(50, 10, 55, 10); FvdRunOnePixel(35, 0, 0, 255);
    RevRunOnePixel(40, 0, 0, 255); RevFillPixels(50, 10, 10, 55); FvdRunOnePixel(35, 255, 0, 0); RevRunOnePixel(40, 255, 0, 0); 
    RevFillPixels(50, 0, 0, 0); RevRunOnePixel(25, 255, 165, 0); FvdRunOnePixel(30, 255, 165, 0); RevRunOnePixel(25, 255, 165, 0); 
    FvdRunOnePixel(30, 100, 255, 0); RevRunOnePixel(25, 100, 0, 255); FvdRunOnePixel(30, 100, 0, 255); RevFillPixels(100, 200, 30, 0);
    FvdTurnOffAllPix(3,2); FvdFillPixels(100, 200, 30, 0); RevTurnOffAllPix(3,2); RevRunMixPixel(50, 20, 80, 50); 
    FvdRunMixPixel(50, 40, 50, 100); RevFillPixels(50, 25, 75, 15); RevRunOnePixel(35, 255, 0, 0); FvdRunOnePixel(40, 255, 0, 0);
    FvdFillPixels(50, 15, 70, 90); RevRunOnePixel(35, 0, 255, 0); FvdRunOnePixel(40, 0, 255, 0); FvdFillPixels(50, 40, 155, 10);
    RevRunOnePixel(35, 0, 0, 255); FvdRunOnePixel(40, 0, 0, 255); FvdFillPixels(50, 200, 20, 0); BlinkAllPix(15, 15, 15, 200, 40, 0, 0, 0, 0); 
    RevRunOnePixel(35, 200, 20, 0); FvdRunOnePixel(40, 200, 20, 0); RevFillPixels(70, 225, 100, 0); BlinkAllPix(15, 15, 15, 225, 100, 0, 0, 0, 0); 
    FvdTurnOffAllPix(3,2); FvdFillPixels(70, 225, 100, 0); RevTurnOffAllPix(3,2); RevRunMixPixel(50, 200, 20, 60); FvdRunMixPixel(50, 80, 120, 60);
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    FvdRunMixPixel(20,255,255,255); delay(100); FvdTurnOffAllPix(40,2); RevRunMixPixel(20,255,255,255); delay(100); RevTurnOffAllPix(40,2);
    RevRiseAllPixel(30,2,255,255,255); BlinkAllPix(15, 15, 15, 255, 255, 255, 0, 0, 0); FvdShadowAllPixel(15, 1, 255, 255, 255);
    RevFillPixels(1, 0, 0, 255); OncomingPixels(100, 255, 0, 0, 0, 255, 0); FvdFillPixels(40,0,255,0); FvdTurnOffAllPix(40,2);
    RevFillPixels(40,0,100,255); OncomingPixels(100, 255, 0, 0, 0, 255, 0); RevFillPixels(40,255,0,0); FvdTurnOffAllPix(40,2);
    OncomingPixels(100, 0, 255, 0, 255, 0, 0); RevFillPixels(40,0,255,0); FvdTurnOffAllPix(40,2); FvdRunOnePixel(40,0,255,0);
    RevRunOnePixel(40,0,255,0); FvdFillPixels(40, 0, 255, 0); BlinkAllPix(15, 15, 15, 0, 255, 0, 0, 0, 0); 
    FvdShadowAllPixel(15, 1, 0, 255, 0); RevRiseAllPixel(30,2,0,0,255); BlinkAllPix(15, 15, 15, 0, 0, 255, 0, 0, 0); delay(200);
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //++++++++++++++++ Все ┤│нкгии и паамет╗ ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //FvdShadowAllPixel(0,0,0,0,0);            //Плавное га╕ение всех пикселей (задежка, ╕аг га╕ени┐, отделен╗е гвета R,G,B каждого из 3 пикселей )
  //RevRiseAllPixel(0,0,0,0,0);              //Плавное вкл╛чение всех пикселей (задежка, ╕аг вкл╛чени┐, отделен╗е гвета R,G,B каждого из 3 пикселей )
  //RevFillPixels(0,0,0,0);                  //Заполнение пикселей в╗банн╗м гветом в обатном напавлении (задежка, гвет R,G,B каждого пиксел┐)
  //FvdFillPixels(0,0,0,0);                  //Заполнение пикселей в╗банн╗м гветом в обатном напавлении (задежка, гвет R,G,B каждого пиксел┐)
  //FvdRunOnePixel(0,0,0,0);                 //Один бег│╣ий пикселе в п┐мом напавлении (задежка,гвет R,G,B пееме╣аемого пиксел┐)
  //RevRunOnePixel(0,0,0,0);                 //Один бег│╣ий пикселе в обатном напавлении (задежка, гвет R,G,B пееме╣аемого пиксел┐)
  //FvdTurnOffAllPix(0,0);                   //Плавное га╕ение пикселей по очееди в п┐мом напавлении (задежка, ╕аг га╕ени┐)
  //RevTurnOffAllPix(0,0);                   //Плавное га╕ение пикселей по очееди в обатном напавлении (задежка, ╕аг га╕ени┐)
  //FvdRunMixPixel(0,0,0,0);                 //Заполнение лент╗ гветн╗ми пиксел┐ми в╗банн╗х гветов в п┐мом напавлении (задежка, отделен╗е гвета R,G,B каждого из 3 пикселей)
  //RevRunMixPixel(0,0,0,0);                 //Заполнение лент╗ гветн╗ми пиксел┐ми в╗банн╗х гветов в п┐мом напавлении (задежка, отделен╗е гвета R,G,B каждого из 3 пикселей)
  //BlinkAllPix(0,0,0,0,0,0,0,0,0);          //Мегание всех пикселей(длителеносте мегани┐, вем┐ откл╛чени┐, вем┐ свечени┐, гвет свечени┐ R,G,B каждого пиксел┐, гвет мегани┐ R,G,B каждого пиксел┐)  
  //OncomingPixels(0,0,0,0,0,0,0);           //Два бег│╣их пиксел┐ во встечном напавлении (задежка,гвет R,G,B пееме╣аемого пиксел┐)
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}         
//***********************************************************************

//***********************************************************************
void RevRunOnePixel(uint8_t pause, byte r, byte g, byte b)
       {
         uint8_t curr_r, curr_g, curr_b;
         uint16_t j;
         long i;
         for(j=0; j<PixNum+1; j++)
           {
              strip.setPixelColor(j-1, curr_r, curr_g, curr_b);
              //--------------------------------------- 
               i = strip.getPixelColor(j);
               curr_r = (uint8_t)((i >> 16) & 0xff),
               curr_g = (uint8_t)((i >> 8) & 0xff),
               curr_b = (uint8_t)(i & 0xff);
               strip.setPixelColor(j, r, g, b);
             //--------------------------------------- 
               strip.show();
               if(j==PixNum-1)
               strip.setPixelColor(j, curr_r, curr_g, curr_b);
               delay(pause);
               strip.show();
               delay(pause);
           }
      }
//***********************************************************************

//***********************************************************************
void FvdRunOnePixel(uint8_t pause, byte r, byte g, byte b)
       {
         uint8_t curr_r=0, curr_g=0, curr_b=0;
         uint16_t j;
         long i;
         for(j=PixNum; j>0; j--)
           {
              strip.setPixelColor(j, curr_r, curr_g, curr_b);
              //--------------------------------------- 
               i = strip.getPixelColor(j-1);
               curr_r = (uint8_t)((i >> 16) & 0xff),
               curr_g = (uint8_t)((i >> 8) & 0xff),
               curr_b = (uint8_t)(i & 0xff);
               strip.setPixelColor(j-1, r, g, b);
             //--------------------------------------- 
               strip.show();
               if(j==1)
               strip.setPixelColor(j-1, curr_r, curr_g, curr_b);
               delay(pause);
               strip.show();
               delay(pause);
           }
      }
//***********************************************************************

//***********************************************************************
void OncomingPixels(uint16_t pause, uint8_t R_fvd, uint8_t G_fvd, uint8_t B_fvd, uint8_t R_rev, uint8_t G_rev, uint8_t B_rev)
  {
         uint8_t curr_R_fvd=0, curr_G_fvd=0, curr_B_fvd=0;
         uint8_t curr_R_rev=0, curr_G_rev=0, curr_B_rev=0;
         uint16_t j,k=0;
         k=PixNum;
         long i;
         for(j=0; j<PixNum; j++, k--)
           {
              strip.setPixelColor(j-1, curr_R_fvd, curr_G_fvd, curr_B_fvd);
              strip.setPixelColor(k, curr_R_rev, curr_R_rev, curr_B_rev);
              //--------------------------------------- 
               i = strip.getPixelColor(j);
               curr_R_fvd = (uint8_t)((i >> 16) & 0xff),
               curr_G_fvd = (uint8_t)((i >> 8) & 0xff),
               curr_B_fvd = (uint8_t)(i & 0xff);
             //--------------------------------------- 
               i = strip.getPixelColor(k-1);
               curr_R_rev = (uint8_t)((i >> 16) & 0xff),
               curr_G_rev = (uint8_t)((i >> 8) & 0xff),
               curr_B_rev = (uint8_t)(i & 0xff);
               strip.setPixelColor(k-1, R_rev, G_rev, B_rev);
               strip.setPixelColor(j, R_fvd, G_fvd, B_fvd);
             //---------------------------------------             
              if(j==PixNum-1)strip.setPixelColor(j-1, curr_R_fvd, curr_G_fvd, curr_B_fvd);
              if(k==1)       strip.setPixelColor(k, curr_R_rev, curr_G_rev, curr_B_rev);
              strip.show();
              delay(pause);
           }
  }
//***********************************************************************

//***********************************************************************
void BlinkAllPix(uint16_t cycle, uint8_t StripOn, uint8_t StripOff, uint8_t R_on, uint8_t G_on, uint8_t B_on, uint8_t R_off, uint8_t G_off, uint8_t B_off)
  {
    uint16_t j, k;                                       //счетчики
    for(cycle; cycle>0; cycle--)                         //количество повтоений
     {
       for(j=PixNum; j>0; j--)strip.setPixelColor(j-1, R_on, G_on, B_on);    //светим
          strip.show();                                 
          delay(StripOn); //задежка свечени┐
       for(j=PixNum; j>0; j--)strip.setPixelColor(j-1, R_off, G_off, B_off); //гасим
          strip.show();
          delay(StripOff); //задежка до вкл╛чени┐
     }
          for(j=PixNum; j>0; j--)strip.setPixelColor(j-1, R_on, G_on, B_on); //в╗ходим с вкл╛ченн╗ми пиксел┐ми
          strip.show();
  }
//***********************************************************************

//***********************************************************************
void FvdShadowAllPixel(uint8_t pause, uint8_t StepOff, uint8_t r, uint8_t g, uint8_t b)
  {
         float coeff_R, coeff_G, coeff_B, m;
         uint16_t j, k;                          // счетчики
         float cr, cg, cb;                       // │станавливаем╗е гвета на ленте в гикле 
         m = max(r, g);                          // Сам╗й ┐кий гвет (бОле╕ее значение из r, g, b)
         m = max(m, b);                    
         coeff_R = StepOff/m*r;                  // Значени┐ ╕агов │мене╕ени┐ каждого гвета в пикселе
         coeff_G = StepOff/m*g;
         coeff_B = StepOff/m*b;
         for(j=PixNum; j>0; j--)
          {
            strip.setPixelColor(j-1, r, g, b);
          }   
          strip.show();
          cr=r; cg=g; cb=b;
         for(k=(m/StepOff/PixNum)+2; k>0; k--)   //Вне╕ний гикл
          {
              for(j=PixNum; j>0; j--)            //Цикл дл┐ пикселей в ленте
               {  //Замена каждого гвета на ок│гленн│╛ с│мм│ тек│╣его гвета и числа пибавлени┐
                  if(cr-coeff_R>0)cr=(cr-coeff_R); 
                  if(cg-coeff_G>0)cg=(cg-coeff_G);
                  if(cb-coeff_B>0)cb=(cb-coeff_B);
                  strip.setPixelColor(j-1, cr, cg, cb);
                  strip.show();
               }
                 delay(pause);                   //Плавносте
                 if(k==2)
                 {
                  cr=0; cg=0; cb=0;
                 }
          }        
  }

//***********************************************************************

//***********************************************************************
void RevRiseAllPixel(uint8_t pause, uint8_t StepOn, uint8_t r, uint8_t g, uint8_t b)
  {
         float coeff_R, coeff_G, coeff_B, m;    // Значени┐ ╕агов │величени┐ каждого гвета в пикселе 
         uint16_t j, k;                         // счетчики
         
         float cr, cg, cb;                      // │станавливаем╗е гвета на ленте в гикле 
         m = max(r, g);                         // Сам╗й ┐кий гвет (бОле╕ее значение из r, g, b)
         m = max(m, b);                    
         coeff_R = StepOn/m*r;
         coeff_G = StepOn/m*g;
         coeff_B = StepOn/m*b;
         for(k=(m/StepOn/PixNum)+1; k>0; k--)   //Вне╕ний гикл
          {
              for(j=PixNum; j>0; j--)           //Цикл дл┐ пикселей в ленте
               {  //Замена каждого гвета на ок│гленн│╛ с│мм│ тек│╣его гвета и числа пибавлени┐
                  if(r!=0 and ceil(coeff_R+cr)<r)cr=ceil(cr+coeff_R); 
                  if(g!=0 and ceil(coeff_G+cg)<g)cg=ceil(cg+coeff_G);
                  if(b!=0 and ceil(coeff_B+cb)<b)cb=ceil(cb+coeff_B);
                  strip.setPixelColor(j-1, cr, cg, cb);
                  strip.show();
              }
              delay(pause);                     //Плавносте
          }        
  }
//***********************************************************************

//***********************************************************************
void RevFillPixels(uint16_t pause, byte r, byte g, byte b)
{
  uint16_t j;
  for(j=PixNum; j>0; j--)
     {
       strip.setPixelColor(j-1, r, g, b);
       strip.show();
       delay(pause);
     }
       delay(pause);  
}
//***********************************************************************

//***********************************************************************
void FvdFillPixels(uint16_t pause, byte r, byte g, byte b)
{
  uint16_t j;
  for(j=0; j<PixNum; j++)
     {
       strip.setPixelColor(j, r, g, b);
       strip.show();
       delay(pause);
     }
       delay(pause);  
}
//***********************************************************************

//***********************************************************************
void FvdRunMixPixel(uint8_t pause, byte fst, byte scd, byte th) 
         {
           uint16_t i, j, k, n;
           i=fst; k=0; n=0;          
           for(j=PixNum; j>0; j--)
            {
              strip.setPixelColor(j-1, i, k, n);
              if(i==fst)
                {
                  i=0; k=scd; n=0;
                } 
              else if(k==scd)
                {
                  i=0; k=0; n=th;
                }
              else if(n==th)
                {
                 i=fst; k=0; n=0;
                }
            strip.show();
            delay(pause);
            }
            delay(pause);
         }
//***********************************************************************

//***********************************************************************
void RevRunMixPixel(uint8_t pause, byte fst, byte scd, byte th) 
         {
           uint16_t i, j, k, n;
           i=fst; k=0; n=0;          
           for(j=0; j<PixNum; j++)
            {
              strip.setPixelColor(j, i, k, n);
              if(i==fst)
                {
                  i=0; k=scd; n=0;
                } 
              else if(k==scd)
                {
                  i=0; k=0; n=th;
                }
              else if(n==th)
                {
                 i=fst; k=0; n=0;
                }
            strip.show();
            delay(pause);
            }
            delay(pause);
         }
//***********************************************************************

//***********************************************************************
void FvdTurnOffAllPix(uint8_t pause, uint32_t StepOff)
       {
         uint8_t r, g, b;
         uint16_t j;
         uint8_t gradat;
         long i;
         for(j=PixNum; j>0; j--)
           {
               long i = strip.getPixelColor(j-1); 
               r = (uint8_t)((i >> 16) & 0xff),
               g = (uint8_t)((i >> 8) & 0xff),
               b = (uint8_t)(i & 0xff);
               gradat = 255/StepOff+1;
            for(gradat; gradat>0; gradat--)
              {
                  if(r>=StepOff)
                   r=r-StepOff;
                  else r-=r;
                  if(g>=StepOff)
                   g=g-StepOff;
                  else g-=g;
                  if(b>=StepOff)
                   b=b-StepOff;
                  else b-=b;                  
               strip.setPixelColor(j-1, r, g, b);
               strip.show();
              }
            delay(pause);
           }
       }            
//***********************************************************************

//***********************************************************************
void RevTurnOffAllPix(uint8_t pause, uint32_t StepOff)
       {
         uint8_t r, g, b;
         uint16_t j;
         uint8_t gradat;
         long i;
         for(j=0; j<PixNum; j++)
           {
               long i = strip.getPixelColor(j); 
               r = (uint8_t)((i >> 16) & 0xff),
               g = (uint8_t)((i >> 8) & 0xff),
               b = (uint8_t)(i & 0xff);
               gradat = 255/StepOff+1;
            for(gradat; gradat>0; gradat--)
              {
                  if(r>=StepOff)
                   r=r-StepOff;
                  else r-=r;
                  if(g>=StepOff)
                   g=g-StepOff;
                  else g-=g;
                  if(b>=StepOff)
                   b=b-StepOff;
                  else b-=b;                  
               strip.setPixelColor(j, r, g, b);
               strip.show();
              }
               delay(pause);
           }
       }   
//***********************************************************************

//***********************************************************************



