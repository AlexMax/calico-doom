/*
  CALICO

  Renderer phase 5 - Graphics caching
*/

#include "doomdef.h"
#include "r_local.h"

// Doom palette to CRY lookup (hardcoded for efficiency on the Jag ASIC?)
static pixel_t vgatojag[] =
{
       1, 51487, 55319, 30795, 30975, 30747, 30739, 30731, 30727, 43831, 44075, 48415, 53015, 47183, 47175, 51263, 
   38655, 38647, 42995, 42731, 42727, 42719, 46811, 46803, 46795, 46535, 46527, 46523, 46515, 50607, 50599, 50339, 
   50331, 54423, 54415, 54155, 54147, 54143, 53879, 57971, 57963, 57703, 57695, 57691, 57683, 61519, 61511, 61507, 
   34815, 34815, 39167, 38911, 38911, 43263, 43007, 43007, 47359, 47351, 47087, 47079, 47071, 51415, 51407, 51147, 
   51391, 51379, 51371, 51363, 51355, 51343, 51335, 51327, 51319, 51307, 51295, 51539, 51531, 51519, 51763, 51755, 
   30959, 30951, 30943, 30939, 30931, 30923, 30919, 30911, 30903, 30899, 30891, 30887, 30879, 30871, 30867, 30859, 
   30851, 30847, 30839, 30831, 30827, 30819, 30811, 30807, 30799, 30791, 30787, 30779, 30775, 30767, 30759, 30755, 
   36095, 36079, 36063, 36047, 36031, 36015, 35999, 35987, 35971, 35955, 35939, 35923, 35907, 40243, 35875, 40215, 
   39103, 39095, 39087, 39079, 39071, 43163, 43155, 43147, 43139, 43131, 43127, 43119, 43111, 43103, 43095, 47187, 
   43167, 43151, 43139, 47479, 47463, 47451, 47183, 51523, 39295, 39283, 39275, 35171, 43607, 39503, 39495, 39487, 
   48127, 52203, 56279, 56003, 60079, 59547, 63367, 63091, 30975, 34815, 38911, 42751, 46591, 50431, 54271, 58111, 
   61695, 61679, 61667, 61655, 61643, 61631, 61619, 61607, 61595, 61579, 61567, 61555, 61543, 61531, 61519, 61507, 
   30719, 26623, 22527, 18175, 17919, 13567,  9215,  4607,   255,   227,   203,   179,   155,   131,   107,    83, 
   30975, 34815, 39167, 43263, 47359, 51455, 55295, 59391, 59379, 63467, 59103, 63447, 63179, 63171, 63159, 63151, 
   30975, 35071, 39423, 48127, 52479, 56831, 61183, 65535, 63143, 62879, 62867, 62599, 47183, 51267, 51255, 55087,
      83,    71,    59,    47,    35,    23,    11,     1, 30975, 30975, 29951, 28927, 28879, 32927, 32879, 42663
};

static void R_Malloc(void)
{
   // CALICO_TODO
   /*
 subq #4,FP

 move FP,r0
 addq #4,r0 ; &size
 load (r0),r1
 addq #24,r1
 store r1,(r0)
 load (r0),r1
 addq #7,r1
 movei #-8,r2
 and r2,r1
 store r1,(r0)
 movei #_refzone,r0
 load (r0),r0
 addq #4,r0
 load (r0),r0
 move r0,r16 ;(base)
 move r0,r17 ;(start)

 movei #L83,r0
 jump T,(r0)
 nop

L82:

 move r16,r0 ;(base)
 addq #4,r0
 load (r0),r0
 moveq #0,r1
 cmp r0,r1
 movei #L85,scratch
 jump EQ,(scratch)
 nop

 move r16,r15 ;(base)(rover)

 movei #L86,r0
 jump T,(r0)
 nop

L85:

 move r16,r0 ;(base)
 addq #16,r0
 load (r0),r0
 move r0,r15 ;(rover)

L86:

 move r15,r0 ;(rover)
 moveq #0,r1
 cmp r0,r1
 movei #L87,scratch
 jump NE,(scratch)
 nop

 movei #L89,r0
 jump T,(r0)
 nop

L87:

 move r15,r0 ;(rover)
 addq #4,r0
 load (r0),r0
 moveq #0,r1
 cmp r0,r1
 movei #L90,scratch
 jump EQ,(scratch)
 nop
 move r15,r0 ;(rover)
 addq #12,r0
 load (r0),r0
 movei #_framecount,r1
 load (r1),r1
 cmp r0,r1
 movei #L90,scratch
 jump NE,(scratch)
 nop

 move r15,r0 ;(rover)
 addq #16,r0
 load (r0),r0
 move r0,r16 ;(base)
 move r16,r0 ;(base)
 moveq #0,r1
 cmp r0,r1
 movei #L92,scratch
 jump NE,(scratch)
 nop

L89:

 movei #_refzone,r0
 load (r0),r0
 addq #8,r0
 move r0,r16 ;(base)

L92:

 move r16,r0 ;(base)
 move r17,r1 ;(start)
 cmp r0,r1
 movei #L83,scratch
 jump NE,(scratch)
 nop

 movei #_framecount,r0
 load (r0),r1
 addq #1,r1
 store r1,(r0)

 movei #L83,r0
 jump T,(r0)
 nop

L90:

 move r15,r0 ;(rover)
 addq #4,r0
 load (r0),r0
 movei #1024,r1
 cmp r0,r1
 movei #L96,scratch
 jump PL,(scratch)
 nop

 move r15,r0 ;(rover)
 addq #4,r0
 load (r0),r0
 moveq #0,r1
 store r1,(r0)

L96:

 move r15,r0 ;(rover)
 addq #10,r0
 moveq #0,r1
 storew r1,(r0)

 move r15,r0 ;(rover)
 addq #4,r0
 moveq #0,r1
 store r1,(r0)

 move r16,r0 ;(base)
 move r15,r1 ;(rover)
 cmp r0,r1
 movei #L98,scratch
 jump EQ,(scratch)
 nop

 load (r16),r0 ;(base)
 load (r15),r1 ;(rover)
 add r1,r0
 store r0,(r16) ;(base)

 moveq #16,r0
 move r16,r1 ;(base)
 add r0,r1
 move r15,r2 ;(rover)
 add r0,r2
 load (r2),r0
 store r0,(r1)

 move r15,r0 ;(rover)
 addq #16,r0
 load (r0),r0
 moveq #0,r1
 cmp r0,r1
 movei #L100,scratch
 jump EQ,(scratch)
 nop

 move r15,r0 ;(rover)
 addq #16,r0
 load (r0),r0
 addq #20,r0
 store r16,(r0) ;(base)

L100:

L98:

L83:

 move r16,r0 ;(base)
 addq #4,r0
 load (r0),r0
 moveq #0,r1
 cmp r0,r1
 movei #L82,scratch
 jump NE,(scratch)
 nop
 load (r16),r0 ;(base)
 load (FP+1),r1 ; local size
 cmp r0,r1
 movei #L82,scratch
 jump S_LT,(scratch)
 nop

 move FP,r0 ; &extra
 load (r16),r1 ;(base)
 load (FP+1),r2 ; local size
 sub r2,r1
 store r1,(r0)
 load (r0),r0
 movei #64,r1
 cmp r0,r1
 movei #L102,scratch
 jump PL,(scratch)
 nop

 load (FP+1),r0 ; local size
 add r16,r0 ;(base)
 move r0,r18 ;(new)
 load (FP),r0 ; local extra
 store r0,(r18) ;(new)

 move r18,r0 ;(new)
 addq #4,r0
 moveq #0,r1
 store r1,(r0)

 move r18,r0 ;(new)
 addq #12,r0
 moveq #0,r1
 store r1,(r0)
 move r18,r0 ;(new)
 addq #8,r0
 storew r1,(r0)

 move r18,r0 ;(new)
 addq #20,r0
 store r16,(r0) ;(base)

 moveq #16,r0
 move r18,r1 ;(new)
 add r0,r1
 move r16,r2 ;(base)
 add r0,r2
 load (r2),r0
 store r0,(r1)

 move r18,r0 ;(new)
 addq #16,r0
 load (r0),r0
 moveq #0,r1
 cmp r0,r1
 movei #L104,scratch
 jump EQ,(scratch)
 nop

 move r18,r0 ;(new)
 addq #16,r0
 load (r0),r0
 addq #20,r0
 store r18,(r0) ;(new)

L104:

 move r16,r0 ;(base)
 addq #16,r0
 store r18,(r0) ;(new)

 load (FP+1),r0 ; local size
 store r0,(r16) ;(base)

L102:

 move r16,r0 ;(base)
 addq #4,r0
 load (FP+2),r1 ; local user
 store r1,(r0)

 move r16,r0 ;(base)
 addq #12,r0
 movei #_framecount,r1
 load (r1),r1
 store r1,(r0)

 move r16,r0 ;(base)
 addq #10,r0
 movei #7498,r1
 storew r1,(r0)

 move r16,r0 ;(base)
 addq #8,r0
 movei #101,r1
 storew r1,(r0)

 movei #_refzone,r0
 load (r0),r0
 addq #4,r0
 move r16,r1 ;(base)
 addq #16,r1
 load (r1),r1
 store r1,(r0)

 movei #_refzone,r0
 load (r0),r0
 addq #4,r0
 load (r0),r0
 moveq #0,r1
 cmp r0,r1
 movei #L106,scratch
 jump NE,(scratch)
 nop

 movei #_refzone,r0
 load (r0),r0
 move r0,r1
 addq #4,r1
 addq #8,r0
 store r0,(r1)

L106:

 load (FP+2),r0 ; local user
 move r16,r1 ;(base)
 addq #24,r1
 store r1,(r0)

 move r16,r0 ;(base)
 addq #24,r0
 move r0,RETURNVALUE

L81:
 jump T,(RETURNPOINT)
 addq #4,FP ; delay slot
   */
}

#define LENSHIFT 4 // this must be log2(LOOKAHEAD_SIZE)

//
// Decompress an lzss-compressed lump
//
static void R_decode(byte *input, pixel_t *output)
{
   int getidbyte = 0;
   int len;
   int pos;
   int i;
   pixel_t *source;
   int idbyte = 0;

   while(1)
   {
      // get a new idbyte if necessary
      if(!getidbyte)
         idbyte = *input++;
      getidbyte = (getidbyte + 1) & 7;

      if(idbyte&1)
      {
         // decompress
         pos = *input++ << LENSHIFT;
         pos = pos | (*input >> LENSHIFT);
         source = output - pos - 1;
         len = (*input++ & 0xf)+1;
         if(len == 1)
            break;
         for(i = 0; i < len; i++)
            *output++ = *source++;
      } 
      else 
         *output++ = vgatojag[*input++];

      idbyte = idbyte >> 1;
   }
}

static void R_LoadPixels(void)
{
   // CALICO_TODO
   /*
movei #40,scratch
 sub scratch,FP

 load (FP+10),r0 ; local lumpnum
 shlq #2,r0
 movei #_lumpcache,r1
 add r1,r0
 load (r0),r0
 move r0,r15 ;(rdest)
 move r15,r0 ;(rdest)
 moveq #0,r1
 cmp r0,r1
 movei #L109,scratch
 jump EQ,(scratch)
 nop

 move r15,r0 ;(rdest)
 move r0,RETURNVALUE

 movei #L108,r0
 jump T,(r0)
 nop

L109:

 load (FP+10),r0 ; local lumpnum
 move r0,r1
 shlq #4,r1
 movei #_lumpinfo,r2
 load (r2),r2
 add r2,r1
 move r1,r16 ;(info)
 move FP,r1
 addq #8,r1 ; &count
 move r16,r2 ;(info)
 addq #4,r2
 load (r2),r2
 store r2,(r1)
 load (r1),r1
 shlq #1,r1
 store r1,(FP) ; arg[]
 shlq #2,r0
 movei #_lumpcache,r1
 add r1,r0
 or r0,scratch ; scoreboard bug
 store r0,(FP+1) ; arg[]
 movei #_R_Malloc,r0
 store r28,(FP+4) ; push ;(RETURNPOINT)
 store r16,(FP+5) ; push ;(info)
 movei #L111,RETURNPOINT
 jump T,(r0)
 store r15,(FP+6) ; delay slot push ;(rdest)
L111:
 load (FP+5),r16 ; pop ;(info)
 load (FP+6),r15 ; pop ;(rdest)
 load (FP+4), RETURNPOINT ; pop
 move r29,r0 ;(RETURNVALUE)
 move r0,r15 ;(rdest)
 move FP,r0
 addq #12,r0 ; &rsrc
 load (r16),r1 ;(info)
 movei #_wadfileptr,r2
 load (r2),r2
 add r2,r1
 store r1,(r0)
 load (r0),r0
 store r0,(FP) ; arg[]
 or r15,scratch ; scoreboard bug ;(rdest)
 store r15,(FP+1) ; arg[] ;(rdest)
 movei #_R_decode,r0
 store r28,(FP+4) ; push ;(RETURNPOINT)
 store r16,(FP+5) ; push ;(info)
 movei #L112,RETURNPOINT
 jump T,(r0)
 store r15,(FP+6) ; delay slot push ;(rdest)
L112:
 load (FP+5),r16 ; pop ;(info)
 load (FP+6),r15 ; pop ;(rdest)
 load (FP+4), RETURNPOINT ; pop

 load (FP+10),r0 ; local lumpnum
 shlq #2,r0
 movei #_lumpcache,r1
 add r1,r0
 load (r0),r0
 move r0,RETURNVALUE

L108:
 movei #40,scratch
 jump T,(RETURNPOINT)
 add scratch,FP ; delay slot
   */
}

void R_Cache(void)
{
   // CALICO_TODO: R_Cache
   /*
subq #24,FP

 movei #_viswalls,r0  r0 = viswalls;
 move r0,r15 ;(wall)  wall = r0

 movei #L55,r0        goto L55
 jump T,(r0)
 nop

L52: // loop start

 move r15,r0 ;(wall)  r0 = wall;
 addq #24,r0          r0 += &viswall_t::actionbits;
 load (r0),r0         r0 = *r0;
 moveq #4,r1          r1 = AC_TOPTEXTURE;
 and r1,r0            r0 &= r1;
 moveq #0,r1          r1 = 0;
 cmp r0,r1            if(r0 == r1)
 movei #L56,scratch     goto L56;
 jump EQ,(scratch)
 nop
 movei #40,r0         r0 = &viswall_t::t_texture;
 move r15,r1 ;(wall)  r1 = wall;
 add r0,r1            r1 += r0;
 load (r1),r0         r0 = *r1;
 addq #16,r0          r0 += &texture_t::data;
 load (r0),r0         r0 = *r0;
 movei #4096,r1       r1 = 4096;
 cmp r0,r1            if(r0 >= r1)
 movei #L56,scratch     goto L56;
 jump EQ,(scratch)
 nop
 jump MI,(scratch)
 nop

 movei #40,r0         r0 = &viswall_t::t_texture;
 move r15,r1 ;(wall)  r1 = wall;
 add r0,r1            r1 += r0;
 load (r1),r0         r0 = *r1;
 addq #16,r0          r0 += &texture_t::data;
 load (r0),r1                             r1 = *r0;
 store r1,(FP) ; arg[]                    *FP = r1;
 movei #_R_LoadPixels,r1                  r1 = R_LoadPixels;
 store r28,(FP+1) ; push ;(RETURNPOINT)   *(FP+1) = RETURNPOINT;
 store r16,(FP+2) ; push ;(spr)           *(FP+2) = spr;
 store r15,(FP+3) ; push ;(wall)          *(FP+3) = wall;
 movei #L75,RETURNPOINT
 jump T,(r1)                              call R_LoadPixels
 store r0,(FP+4) ; delay slot push          *(FP+4) = r0;
L75:
 load (FP+2),r16 ; pop ;(spr)             r16 = spr;
 load (FP+3),r15 ; pop ;(wall)            r15 = wall;
 load (FP+4),r0 ; pop                     r0  = *(FP+4);
 load (FP+1), RETURNPOINT ; pop           RETURNPOINT = *(FP+1);
 move r29,r1 ;(RETURNVALUE)               r1 = RETURNVALUE;
 store r1,(r0)                            *r0 = r1; // wall->t_texture->data = R_LoadPixels(...);

L56: // !(actionbits & AC_TOPTEXTURE)

 move r15,r0 ;(wall)  r0 = wall;
 addq #24,r0          r0 += &viswall_t::actionbits;
 load (r0),r0         r0 = *r0;
 moveq #8,r1          r1 = AC_BOTTOMTEXTURE;
 and r1,r0            r0 &= r1;
 moveq #0,r1          r1 = 0;
 cmp r0,r1            if(r0 == r1)
 movei #L58,scratch     goto L58;
 jump EQ,(scratch)
 nop
 movei #56,r0         r0 = &viswall_t::b_texture;
 move r15,r1 ;(wall)  r1 = wall;
 add r0,r1            r1 += r0;
 load (r1),r0         r0 = *r1;
 addq #16,r0          r0 += &texture_t::data;
 load (r0),r0         *r0 = r0;
 movei #4096,r1       r1 = 4096;
 cmp r0,r1            if(r0 >= r1)
 movei #L58,scratch     goto L58;
 jump EQ,(scratch)
 nop
 jump MI,(scratch)
 nop

 movei #56,r0                             r0 = &viswall_t::b_texture;
 move r15,r1 ;(wall)                      r1 = wall;
 add r0,r1                                r1 += r0;
 load (r1),r0                             r0 = *r1;
 addq #16,r0                              r0 += &texture_t::data;
 load (r0),r1                             r1 = *r0;
 store r1,(FP) ; arg[]                    *(FP) = r1;
 movei #_R_LoadPixels,r1                  r1 = R_LoadPixels;
 store r28,(FP+1) ; push ;(RETURNPOINT)   *(FP+1) = RETURNPOINT;
 store r16,(FP+2) ; push ;(spr)           *(FP+2) = spr;
 store r15,(FP+3) ; push ;(wall)          *(FP+3) = wall;
 movei #L76,RETURNPOINT
 jump T,(r1)                              call R_LoadPixels;
 store r0,(FP+4) ; delay slot push          *(FP+4) = r0;
L76:
 load (FP+2),r16 ; pop ;(spr)             spr = *(FP+2);
 load (FP+3),r15 ; pop ;(wall)            wall = *(FP+3);
 load (FP+4),r0 ; pop                     r0 = *(FP+4);
 load (FP+1), RETURNPOINT ; pop           RETURNPOINT = *(FP+1);
 move r29,r1 ;(RETURNVALUE)               r1 = RETURNVALUE;
 store r1,(r0)                            *r0 = r1; // wall->b_texture->data = R_LoadPixels(...)

L58: // !(actionbits & AC_BOTTOMTEXTURE)

 move r15,r0 ;(wall)  r0 = wall;
 addq #16,r0          r0 += &viswall_t::floorpic;
 load (r0),r0         r0 = *r0;
 movei #4096,r1       r1 = 4096;
 cmp r0,r1            if(r0 >= r1)
 movei #L60,scratch     goto L60;
 jump EQ,(scratch)
 nop
 jump MI,(scratch)
 nop

 move r15,r0 ;(wall)                      r0 = wall;
 addq #16,r0                              r0 += &viswall_t::floorpic;
 load (r0),r1                             r1 = *r0;
 store r1,(FP) ; arg[]                    *(FP) = r1;
 movei #_R_LoadPixels,r1                  r1 = R_LoadPixels;
 store r28,(FP+1) ; push ;(RETURNPOINT)   *(FP+1) = RETURNPOINT;
 store r16,(FP+2) ; push ;(spr)           *(FP+2) = spr;
 store r15,(FP+3) ; push ;(wall)          *(FP+3) = wall;
 movei #L77,RETURNPOINT
 jump T,(r1)                              call R_LoadPixels
 store r0,(FP+4) ; delay slot push          *(FP+4) = r0;
L77:
 load (FP+2),r16 ; pop ;(spr)             spr = *(FP+2);
 load (FP+3),r15 ; pop ;(wall)            wall = *(FP+3);
 load (FP+4),r0 ; pop                     r0 = *(FP+4);
 load (FP+1), RETURNPOINT ; pop           RETURNPOINT + *(FP+1);
 move r29,r1 ;(RETURNVALUE)               r1 = RETURNVALUE;
 store r1,(r0)                            *r0 = r1; // wall->floorpic = R_LoadPixels(...);

L60: // wall->floorpic >= 4096 (ie, != NULL)

 move r15,r0 ;(wall)  r0 = wall;
 addq #20,r0          r0 += &viswall_t::ceilingpic;
 load (r0),r0         r0 = *r0;
 movei #-1,r1         r1 = -1;
 cmp r0,r1            if(r0 != r1)
 movei #L62,scratch     goto L62;
 jump NE,(scratch)
 nop

 movei #_skytexturep,r0  r0 = skytexturep;
 load (r0),r0            r0 = *r0;
 addq #16,r0             r0 = &texture_t::data;
 load (r0),r0            r0 = *r0;
 movei #4096,r1          r1 = 4096;
 cmp r0,r1               if(r0 >= r1)
 movei #L63,scratch        goto L63;
 jump EQ,(scratch)
 nop
 jump MI,(scratch)
 nop

 movei #_skytexturep,r0                   r0 = skytexturep;
 load (r0),r0                             r0 = *r0;
 addq #16,r0                              r0 += &texture_t::data;
 load (r0),r1                             r1 = *r0;
 store r1,(FP) ; arg[]                    *(FP) = r1;
 movei #_R_LoadPixels,r1                  r1 = R_LoadPixels;
 store r28,(FP+1) ; push ;(RETURNPOINT)   *(FP+1) = RETURNPOINT;
 store r16,(FP+2) ; push ;(spr)           *(FP+2) = spr;
 store r15,(FP+3) ; push ;(wall)          *(FP+3) = wall;
 movei #L78,RETURNPOINT
 jump T,(r1)                              call R_LoadPixels;
 store r0,(FP+4) ; delay slot push          *(FP+4) = r0;
L78:
 load (FP+2),r16 ; pop ;(spr)             spr = *(FP+2);
 load (FP+3),r15 ; pop ;(wall)            wall = *(FP+3);
 load (FP+4),r0 ; pop                     r0 = *(FP+4);
 load (FP+1), RETURNPOINT ; pop           RETURNPOINT = *(FP+1);
 move r29,r1 ;(RETURNVALUE)               r1 = RETURNVALUE;
 store r1,(r0)                            *r0 = r1; // skytexturep->data = R_LoadPixels(...);

 movei #L63,r0
 jump T,(r0)
 nop

L62: // wall->floorpic != -1, ie., NOT sky ceiling

 move r15,r0 ;(wall)  r0 = wall;
 addq #20,r0          r0 += &viswall_t::ceilingpic;
 load (r0),r0         r0 = *r0;
 movei #4096,r1       r1 = 4096;
 cmp r0,r1            if(r0 >= r1)
 movei #L66,scratch     goto L66;
 jump EQ,(scratch)
 nop
 jump MI,(scratch)
 nop

 move r15,r0 ;(wall)                      r0 = wall;
 addq #20,r0                              r0 += &viswall_t::ceilingpic;
 load (r0),r1                             r1 = *r0;
 store r1,(FP) ; arg[]                    *(FP) = r1;
 movei #_R_LoadPixels,r1                  r1 = R_LoadPixels;
 store r28,(FP+1) ; push ;(RETURNPOINT)   *(FP+1) = RETURNPOINT;
 store r16,(FP+2) ; push ;(spr)           *(FP+2) = spr;
 store r15,(FP+3) ; push ;(wall)          *(FP+3) = wall;
 movei #L79,RETURNPOINT
 jump T,(r1)                              call R_LoadPixels;
 store r0,(FP+4) ; delay slot push          *(FP+4) = r0;
L79:
 load (FP+2),r16 ; pop ;(spr)             spr = *(FP+2);
 load (FP+3),r15 ; pop ;(wall)            wall = *(FP+3);
 load (FP+4),r0 ; pop                     r0 = *(FP+4);
 load (FP+1), RETURNPOINT ; pop           RETURNPOINT = *(FP+1);
 move r29,r1 ;(RETURNVALUE)               r1 = RETURNVALUE;
 store r1,(r0)                            *r0 = r1; // viswall->ceilingpic = R_LoadPixels(...);

L66: // wall->ceilingpic >= 4096 (ie, != NULL)

L63: // skytexturep->data >= 4096 (ie, != NULL)

L53:

 // increment pointer
 movei #112,r0           r0 = sizeof(viswall_t)
 move r15,r1 ;(wall)     r1 = wall;
 add r0,r1               r1 += r0;
 move r1,r15 ;(wall)     wall = r1;

L55: // loop end

 move r15,r0 ;(wall)     r0 = wall;
 movei #_lastwallcmd,r1  r1 = lastwallcmd;
 load (r1),r1            r1 = *r1;
 cmp r0,r1               if(r0 < r1)
 movei #L52,scratch         goto L52; // loop
 jump U_LT,(scratch)
 nop

 movei #_vissprites,r0   r0 = vissprites;
 move r0,r16 ;(spr)      r16 = r0;

 movei #L71,r0           goto L71
 jump T,(r0)
 nop

L68: // loop start

 movei #56,r0            r0 = &vissprite_t::pixels;
 move r16,r1 ;(spr)      r1 = spr;
 add r0,r1               r1 += r0;
 load (r1),r0            r0 = *r1;
 movei #4096,r1          r1 = 4096;
 cmp r0,r1               if(r0 >= r1)
 movei #L72,scratch        goto L72;
 jump EQ,(scratch)
 nop
 jump MI,(scratch)
 nop

 movei #56,r0                             r0 = &vissprite_t::pixels;
 move r16,r1 ;(spr)                       r1 = spr;
 add r0,r1                                r1 += r0;
 load (r1),r0                             r0 = *r1;
 store r0,(FP) ; arg[]                    *(FP) = r0;
 movei #_R_LoadPixels,r0                  r0 = R_LoadPixels;
 store r28,(FP+1) ; push ;(RETURNPOINT)   *(FP+1) = RETURNPOINT;
 store r16,(FP+2) ; push ;(spr)           *(FP+2) = spr;
 store r15,(FP+3) ; push ;(wall)          *(FP+3) = wall;
 movei #L80,RETURNPOINT                  
 jump T,(r0)                              call R_LoadPixels;
 store r1,(FP+4) ; delay slot push          *(FP+4) = r1;
L80:
 load (FP+2),r16 ; pop ;(spr)             spr = *(FP+2);
 load (FP+3),r15 ; pop ;(wall)            wall = *(FP+3);
 load (FP+4),r1 ; pop                     r1 = *(FP+4);
 load (FP+1), RETURNPOINT ; pop           RETURNPOINT = *(FP+1);
 move r29,r0 ;(RETURNVALUE)               r0 = RETURNVALUE;
 store r0,(r1)                            *r1 = r0; // spr->pixels = R_LoadPixels(...);

L72: // spr->pixels >= 4096 (ie, != NULL)

L69: // pointer increment

 movei #60,r0        r0 = sizeof(vissprite_t);
 move r16,r1 ;(spr)  r1 = spr;
 add r0,r1           r1 += r0;
 move r1,r16 ;(spr)  spr = r1;

L71: // loop end

 move r16,r0 ;(spr)      r0 = spr;
 movei #_vissprite_p,r1  r1 = vissprite_p;
 load (r1),r1            r1 = *r1;
 cmp r0,r1               if(r0 < r1)
 movei #L68,scratch        goto L68; // loop
 jump U_LT,(scratch)
 nop

 movei #_phasetime+20,r0
 movei #_samplecount,r1
 load (r1),r1
 store r1,(r0)

 movei #_gpucodestart,r0
 movei #_ref6_start,r1
 store r1,(r0)
 
L51:
 jump T,(RETURNPOINT)
 addq #24,FP ; delay slot
   */
}

// EOF

