/*
  CALICO

  Renderer phase 8 - Sprites
*/

#include "r_local.h"

static int spropening[SCREENWIDTH + 1];

static void R_DrawVisSprite(vissprite_t *vis)
{
   // CALICO_TODO
   /*
dv_blitter		.equr	r15
dv_bottom		.equr	r16
dv_column		.equr	r17
dv_frac			.equr	r18
dv_topclip		.equr	r19
dv_spryscale	.equr	r20
dv_bottomclip	.equr	r21
dv_x			.equr	r22

dv_col_offset	.equr	r23
dv_oldcolread	.equr	r24
dv_top			.equr	r25

dv_patch		.equr	r13
dv_iscale		.equr	r12
dv_xfrac		.equr	r9
dv_sprtop		.equr	r8
dv_stopx		.equr	r7
dv_fracstep		.equr	r6

dv_vis		.equr	r15

	load	(FP),dv_vis

	btst	#0,dv_vis				; scoreboard bug	
	load	(dv_vis+8),dv_patch		; vis->patch
	load	(dv_vis+6),dv_iscale	; vis->yiscale	 
 	load	(dv_vis+2),dv_xfrac		; vis->startfrac
	load	(dv_vis+5),dv_spryscale		; spryscale
	
	shrq	#8,dv_spryscale			; make fit in 16 bits
	
	load	(dv_vis+7),MATH_A			; vis->texturemid
 
;======= FixedMul 16 * +-32 ===
 
	move    MATH_A,MATH_SIGN
	abs     MATH_A
	move    MATH_A,RETURNVALUE
	shrq	#16,RETURNVALUE
	mult    dv_spryscale,RETURNVALUE
	mult    dv_spryscale,MATH_A
	shlq	#8,RETURNVALUE
	shrq	#8,MATH_A

	btst    #31,MATH_SIGN
	jr		EQ,fixednotneg
	add		MATH_A,RETURNVALUE		; delay slot
	
	neg     RETURNVALUE
fixednotneg:
;====================
	movei #5898240,dv_sprtop
	sub RETURNVALUE,dv_sprtop

	movei	#$f02238,r0
blitwait1:
	load	(r0),r1
	btst	#0,r1
	jr		EQ,blitwait1
	nop
	
	movei	#15737344,r0
	load	(dv_vis+14),r1		; vis->pixels
	store	r1,(r0)

	movei	#15737360,r0
	move	dv_iscale,r1
	sharq	#16,r1
	store	r1,(r0)

	movei	#15737364,r0
	move	dv_iscale,r1 ; local iscale
	shlq	#16,r1
	shrq	#16,r1		; mask off high bits
	store 	r1,(r0)

	load	(dv_vis+9),r0	; vis->colormap
	movei	#255,r1
	sub		r0,r1
	shlq	#14,r1
	neg		r1
	shlq	#8,r1
	shrq	#8,r1			; mask off high 8 bits
	movei	#$f02270,r0
	store	r1,(r0)			; blitter iinc
	
;===========================
;
; for x = vis->x1 ; x<stopx ; x++, xfrac+= fracstep
;
;===========================
	load	(dv_vis),dv_x
	load	(dv_vis+1),dv_stopx
	load	(dv_vis+4),dv_fracstep
	
	movei	#$f02200,dv_blitter

	movei	#L414,r0
	jump	T,(r0)
	addq	#1,dv_stopx				; delay slot

L411:

	move	dv_xfrac,dv_column
	shrq	#16,dv_column
	shlq	#1,dv_column
	addq	#8,dv_column
	add		dv_patch,dv_column
	loadw	(dv_column),dv_column
	add		dv_patch,dv_column
	 
	move	dv_x,r0 ;(x)
	shlq	#2,r0
	movei	#_spropening,r1
	add		r1,r0
	load	(r0),dv_bottomclip
	load	(dv_column),dv_oldcolread	; start first read now
	move	dv_bottomclip,dv_topclip
	shrq	#8,dv_topclip

	shlq	#24,dv_bottomclip
	shrq	#24,dv_bottomclip		; mask off upper 24 bits


	movei	#nextpost,r0
	jump	T,(r0)
	subq	#1,dv_bottomclip		; delay slot

;
; column loop
;
L415:
	mult	dv_spryscale,dv_top
	shlq	#8,dv_top
	add		dv_sprtop,dv_top

	mult	dv_spryscale,dv_bottom
	shlq	#8,dv_bottom
	add		dv_top,dv_bottom
	 
	movei	#65535,r0
	add		r0,dv_top
	sharq	#16,dv_top

	subq	#1,dv_bottom
	sharq	#16,dv_bottom
		
;
; clip to bottom
;
	cmp		dv_bottom,dv_bottomclip ;(bottom)(bottomclip)
	jr		PL,bottomclipped
	nop
	move	dv_bottomclip,dv_bottom ;(bottomclip)(bottom)
bottomclipped:

;
; clip to top
;
	cmp		dv_topclip,dv_top
	jr		PL,topclipped
	move	dv_col_offset,dv_frac			; delay slot

	move	dv_topclip,r0
	sub		dv_top,r0
	
	move	dv_iscale,dv_frac
	shrq	#16,dv_frac
	mult	r0,dv_frac
	shlq	#16,dv_frac
	move	dv_iscale,scratch
	mult	r0,scratch
	add		scratch,dv_frac	; delay slot

	move 	dv_topclip,dv_top
	add		dv_col_offset,dv_frac
	
topclipped:
;
; calc count
;
	sub		dv_top,dv_bottom
	movei	#nextpost,scratch
	jump	MI,(scratch)
	addq	#1,dv_bottom			; delay slot

;
; program blitter
;
blitwait2:
	load	(dv_blitter+14),r1
	btst	#0,r1
	jr		EQ,blitwait2
	move	dv_frac,r1 ;(frac)			; delay slot
	
	shrq	#16,r1
	shlq	#16,dv_frac
	store	r1,(dv_blitter+3)			; a1 pixel
	shrq	#16,dv_frac
	store 	dv_frac,(dv_blitter+6)		; a1 frac

	move	dv_top,r1
	shlq	#16,r1
	add		dv_x,r1
	store	r1,(dv_blitter+12)

	shlq	#16,dv_bottom
	addq	#1,dv_bottom
	store	dv_bottom,(dv_blitter+15)		; count

	movei	#1098919681,r1
	store	r1,(dv_blitter+14)

;
; next post
;	
nextpost:

; a post record has four bytes: topdelta length pixelofs*2
	addq	#4,dv_column				; advance to next post
	btst	#15,dv_oldcolread			; last column marker
	move	dv_oldcolread,dv_col_offset
	jr		NE,nextx
	move	dv_col_offset,dv_top		; harmless delay slot
	
	load	(dv_column),dv_oldcolread	; start next read now
	
	shrq	#24,dv_top
	move	dv_col_offset,dv_bottom
	shlq	#8,dv_bottom
	shrq	#24,dv_bottom
		
	movei	#L415,scratch
	jump	T,(scratch)
	shlq	#16,dv_col_offset		; delay slot, leave offset <<16 

;
; next x
;
nextx:
	addq	#1,dv_x
	add		dv_fracstep,dv_xfrac

L414:
	cmp		dv_x,dv_stopx
	movei	#L411,scratch
	jump	U_LT,(scratch)
	nop

	jump T,(RETURNPOINT)
	nop
   */
}

//
// Compare the vissprite to a viswall. Similar to R_PointOnSegSide, but less accurate.
//
static boolean R_SegBehindPoint(viswall_t *viswall, int dx, int dy)
{
   fixed_t x1, y1, sdx, sdy;

   x1  = viswall->seg->v1->x;
   y1  = viswall->seg->v1->y;
   sdx = viswall->seg->v2->x;
   sdy = viswall->seg->v2->y;

   sdx -= x1;
   sdy -= y1;
   dx  -= x1;
   dy  -= y1;

   sdx /= FRACUNIT;
   sdy /= FRACUNIT;
   dx  /= FRACUNIT;
   dy  /= FRACUNIT;

   dx  *= sdy;
   sdx *=  dy;

   return (sdx < dx);
}

//
// Clip a sprite to the openings created by walls
//
static void R_ClipVisSprite(vissprite_t *vis)
{
   // CALICO_TODO
   /*
 movei #104,scratch
 sub scratch,FP

 move FP,r0
 addq #20,r0 ; &x1
 load (FP+26),r1 ; local vis
 load (r1),r2
 store r2,(r0)
 move r1,r0
 addq #4,r0
 load (r0),r0
 move r0,r22 ;(x2)
 move FP,r0
 addq #32,r0 ; &gz
 movei #48,r2
 move r1,r3
 add r2,r3
 load (r3),r2
 movei #_viewz,r3
 load (r3),r3
 sub r3,r2
 moveq #10,r4
 sha r4,r2
 store r2,(r0)
 movei #36,r0
 add FP,r0 ; &gzt
 movei #52,r2
 move r1,r5
 add r2,r5
 load (r5),r2
 sub r3,r2
 sha r4,r2
 store r2,(r0)
 move FP,r0
 addq #12,r0 ; &scalefrac
 move r1,r2
 addq #20,r2
 load (r2),r2
 store r2,(r0)
 load (r1),r0
 move r0,r15 ;(x)

 movei #L80,r0
 jump T,(r0)
 nop

L77:
 move r15,r0 ;(x)
 shlq #2,r0
 movei #_spropening,r1
 add r1,r0
 movei #180,r1
 store r1,(r0)

L78:
 move r15,r0 ;(x)
 addq #1,r0
 move r0,r15 ;(x)

L80:
 cmp r15,r22 ;(x)(x2)
 movei #L77,scratch
 jump PL,(scratch)
 nop

 movei #_lastwallcmd,r0
 load (r0),r0
 movei #-112,r1
 add r1,r0
 move r0,r17 ;(ds)

 movei #L84,r0
 jump T,(r0)
 nop

L81:
 move r17,r0 ;(ds)
 addq #4,r0
 load (r0),r0
 cmp r0,r22 ;(x2)
 movei #L89,scratch
 jump MI,(scratch)
 nop
 move r17,r0 ;(ds)
 addq #8,r0
 load (r0),r0
 load (FP+5),r1 ; local x1
 cmp r0,r1
 movei #L89,scratch
 jump S_LT,(scratch)
 nop
 movei #84,r0
 move r17,r1 ;(ds)
 add r0,r1
 load (r1),r0
 load (FP+3),r1 ; local scalefrac
 cmp r0,r1
 movei #L90,scratch
 jump EQ,(scratch)
 nop
 jump CS,(scratch)
 nop
 movei #88,r0
 move r17,r2 ;(ds)
 add r0,r2
 load (r2),r0
 cmp r0,r1
 movei #L89,scratch
 jump U_LT,(scratch)
 nop
L90:
 move r17,r0 ;(ds)
 addq #24,r0
 load (r0),r0
 movei #1792,r1
 and r1,r0
 moveq #0,r1
 cmp r0,r1
 movei #L85,scratch
 jump NE,(scratch)
 nop
L89:
 movei #L82,r0
 jump T,(r0)
 nop

L85:
 movei #84,r0
 move r17,r1 ;(ds)
 add r0,r1
 load (r1),r0
 load (FP+3),r1 ; local scalefrac
 cmp r0,r1
 movei #L91,scratch
 jump CC,(scratch)
 nop
 movei #88,r0
 move r17,r2 ;(ds)
 add r0,r2
 load (r2),r0
 cmp r0,r1
 movei #L91,scratch
 jump CC,(scratch)
 nop

 movei #L93,r0
 jump T,(r0)
 nop

L91:
 store r17,(FP) ; arg[] ;(ds)
 load (FP+26),r0 ; local vis
 movei #40,r1
 move r0,r2
 add r1,r2
 load (r2),r1
 or r1,scratch ; scoreboard bug
 store r1,(FP+1) ; arg[]
 movei #44,r1
 add r1,r0
 load (r0),r0
 or r0,scratch ; scoreboard bug
 store r0,(FP+2) ; arg[]
 movei #_R_SegBehindPoint,r0
 store r28,(FP+11) ; push ;(RETURNPOINT)
 store r22,(FP+12) ; push ;(x2)
 store r21,(FP+13) ; push ;(bottomsil)
 store r20,(FP+14) ; push ;(bottom)
 store r19,(FP+15) ; push ;(top)
 store r18,(FP+16) ; push ;(r2)
 store r17,(FP+17) ; push ;(ds)
 store r16,(FP+18) ; push ;(opening)
 movei #L133,RETURNPOINT
 jump T,(r0)
 store r15,(FP+19) ; delay slot push ;(x)
L133:
 load (FP+12),r22 ; pop ;(x2)
 load (FP+13),r21 ; pop ;(bottomsil)
 load (FP+14),r20 ; pop ;(bottom)
 load (FP+15),r19 ; pop ;(top)
 load (FP+16),r18 ; pop ;(r2)
 load (FP+17),r17 ; pop ;(ds)
 load (FP+18),r16 ; pop ;(opening)
 load (FP+19),r15 ; pop ;(x)
 load (FP+11), RETURNPOINT ; pop
 moveq #0,r0
 cmp r29,r0 ;(RETURNVALUE)
 movei #L94,scratch
 jump EQ,(scratch)
 nop

 movei #L82,r0
 jump T,(r0)
 nop

L94:
L93:
 move r17,r0 ;(ds)
 addq #4,r0
 load (r0),r0
 load (FP+5),r1 ; local x1
 cmp r0,r1
 movei #L97,scratch
 jump EQ,(scratch)
 nop
 jump MI,(scratch)
 nop
 movei #40,r0
 add FP,r0 ; &96
 load (FP+5),r1 ; local x1
 store r1,(r0)
 movei #L98,r0
 jump T,(r0)
 nop
L97:
 movei #40,r0
 add FP,r0 ; &96
 move r17,r1 ;(ds)
 addq #4,r1
 load (r1),r1
 store r1,(r0)
L98:
 move FP,r0
 addq #28,r0 ; &r1
 load (FP+10),r1 ; local 96
 store r1,(r0)
 move r17,r0 ;(ds)
 addq #8,r0
 load (r0),r0
 cmp r0,r22 ;(x2)
 movei #L99,scratch
 jump PL,(scratch)
 nop
 movei #40,r0
 add FP,r0 ; &96
 store r22,(r0) ;(x2)
 movei #L100,r0
 jump T,(r0)
 nop
L99:
 movei #40,r0
 add FP,r0 ; &96
 move r17,r1 ;(ds)
 addq #8,r1
 load (r1),r1
 store r1,(r0)
L100:
 load (FP+10),r0 ; local 96
 move r0,r18 ;(r2)
 move FP,r0
 addq #16,r0 ; &silhouette
 move r17,r1 ;(ds)
 addq #24,r1
 load (r1),r1
 movei #1792,r2
 and r2,r1
 store r1,(r0)
 load (r0),r0
 movei #1024,r1
 cmp r0,r1
 movei #L101,scratch
 jump NE,(scratch)
 nop

 load (FP+7),r0 ; local r1
 move r0,r15 ;(x)

 movei #L106,r0
 jump T,(r0)
 nop

L103:
 move r15,r0 ;(x)
 shlq #2,r0
 movei #_spropening,r1
 add r1,r0
 movei #46080,r1
 store r1,(r0)

L104:
 move r15,r0 ;(x)
 addq #1,r0
 move r0,r15 ;(x)

L106:
 cmp r15,r18 ;(x)(r2)
 movei #L103,scratch
 jump PL,(scratch)
 nop

 movei #L82,r0
 jump T,(r0)
 nop

L101:
 move FP,r0
 addq #24,r0 ; &topsil
 movei #76,r1
 move r17,r2 ;(ds)
 add r1,r2
 load (r2),r1
 store r1,(r0)
 movei #80,r0
 move r17,r1 ;(ds)
 add r0,r1
 load (r1),r0
 move r0,r21 ;(bottomsil)
 load (FP+4),r0 ; local silhouette
 movei #512,r1
 cmp r0,r1
 movei #L107,scratch
 jump NE,(scratch)
 nop

 load (FP+7),r0 ; local r1
 move r0,r15 ;(x)

 movei #L112,r0
 jump T,(r0)
 nop

L109:
 move r15,r0 ;(x)
 shlq #2,r0
 movei #_spropening,r1
 add r1,r0
 load (r0),r0
 move r0,r16 ;(opening)
 movei #255,r0
 move r16,r1 ;(opening)
 and r0,r1
 movei #180,r0
 cmp r1,r0
 movei #L113,scratch
 jump NE,(scratch)
 nop

 move r15,r0 ;(x)
 shlq #2,r0
 movei #_spropening,r1
 add r1,r0
 movei #65280,r1
 move r16,r2 ;(opening)
 and r1,r2
 move r15,r1 ;(x)
 add r21,r1 ;(bottomsil)
 loadb (r1),r1
 add r1,r2
 store r2,(r0)

L113:
L110:
 move r15,r0 ;(x)
 addq #1,r0
 move r0,r15 ;(x)

L112:
 cmp r15,r18 ;(x)(r2)
 movei #L109,scratch
 jump PL,(scratch)
 nop

 movei #L108,r0
 jump T,(r0)
 nop

L107:
 load (FP+4),r0 ; local silhouette
 movei #256,r1
 cmp r0,r1
 movei #L115,scratch
 jump NE,(scratch)
 nop

 load (FP+7),r0 ; local r1
 move r0,r15 ;(x)

 movei #L120,r0
 jump T,(r0)
 nop

L117:
 move r15,r0 ;(x)
 shlq #2,r0
 movei #_spropening,r1
 add r1,r0
 load (r0),r0
 move r0,r16 ;(opening)
 movei #65280,r0
 move r16,r1 ;(opening)
 and r0,r1
 moveq #0,r0
 cmp r1,r0
 movei #L121,scratch
 jump NE,(scratch)
 nop

 move r15,r0 ;(x)
 shlq #2,r0
 movei #_spropening,r1
 add r1,r0
 load (FP+6),r1 ; local topsil
 move r15,r2 ;(x)
 add r1,r2
 loadb (r2),r1
 shlq #8,r1
 movei #255,r2
 move r16,r3 ;(opening)
 and r2,r3
 add r3,r1
 store r1,(r0)

L121:
L118:
 move r15,r0 ;(x)
 addq #1,r0
 move r0,r15 ;(x)

L120:
 cmp r15,r18 ;(x)(r2)
 movei #L117,scratch
 jump PL,(scratch)
 nop

 movei #L116,r0
 jump T,(r0)
 nop

L115:
 load (FP+4),r0 ; local silhouette
 movei #768,r1
 cmp r0,r1
 movei #L123,scratch
 jump NE,(scratch)
 nop

 load (FP+7),r0 ; local r1
 move r0,r15 ;(x)

 movei #L128,r0
 jump T,(r0)
 nop

L125:
 move r15,r0 ;(x)
 shlq #2,r0
 movei #_spropening,r1
 add r1,r0
 load (r0),r0
 move r0,r19 ;(top)
 move r19,r0 ;(top)
 movei #255,r1
 and r1,r0
 move r0,r20 ;(bottom)
 move r19,r0 ;(top)
 sharq #8,r0
 move r0,r19 ;(top)
 movei #180,r0
 cmp r20,r0 ;(bottom)
 movei #L129,scratch
 jump NE,(scratch)
 nop

 move r15,r0 ;(x)
 add r21,r0 ;(bottomsil)
 loadb (r0),r0
 move r0,r20 ;(bottom)

L129:
 moveq #0,r0
 cmp r19,r0 ;(top)
 movei #L131,scratch
 jump NE,(scratch)
 nop

 load (FP+6),r0 ; local topsil
 move r15,r1 ;(x)
 add r0,r1
 loadb (r1),r0
 move r0,r19 ;(top)

L131:
 move r15,r0 ;(x)
 shlq #2,r0
 movei #_spropening,r1
 add r1,r0
 move r19,r1 ;(top)
 shlq #8,r1
 add r20,r1 ;(bottom)
 store r1,(r0)

L126:
 move r15,r0 ;(x)
 addq #1,r0
 move r0,r15 ;(x)

L128:
 cmp r15,r18 ;(x)(r2)
 movei #L125,scratch
 jump PL,(scratch)
 nop

L123:
L116:
L108:
L82:
 movei #-112,r0
 move r17,r1 ;(ds)
 add r0,r1
 move r1,r17 ;(ds)

L84:
 move r17,r0 ;(ds)
 movei #_viswalls,r1
 cmp r0,r1
 movei #L81,scratch
 jump EQ,(scratch)
 nop
 jump CS,(scratch)
 nop

L76:
 movei #104,scratch
 jump T,(RETURNPOINT)
 add scratch,FP ; delay slot
   */
}

//
// Render all sprites
//
void R_Sprites(void)
{
   // Jag-specific blitter setup
   /*
   movei #15737348,r0                         r0 = 0xf02204
   movei #145440,r1                           r1 = 0x23820
   store r1,(r0)                              *r0 = r1
   
   movei #15737384,r0                         r0 = 0xf02228
   movei #145952,r1                           r1 = 0x23A20
   store r1,(r0)                              *r0 = r1
   */

   ptrdiff_t i = 0, count = lastsprite_p - vissprites;
   vissprite_t *best = NULL;

   // draw mobj sprites
   while(i < count)
   {
      fixed_t bestscale = MAXINT;
      vissprite_t *ds = vissprites;

      while(ds != lastsprite_p)
      {
         if(ds->xscale < bestscale)
         {
            bestscale = ds->xscale;
            best = ds;
         }
         ++ds;
      }

      if(best->patch != NULL)
      {
         R_ClipVisSprite(best);
         R_DrawVisSprite(best);
      }

      best->xscale = MAXINT;

      ++i;
   }

   // draw psprites
   while(lastsprite_p < vissprite_p)
   {
      ptrdiff_t stopx = lastsprite_p->x2 + 1;
      i = lastsprite_p->x1;
      
      // clear out the clipping array across the range of the psprite
      while(i < stopx)
      {
         spropening[i] = SCREENHEIGHT;
         ++i;
      }

      R_DrawVisSprite(lastsprite_p);
      
      ++lastsprite_p;
   }
}

// EOF

