/*
 Copyright (C) Intel Corp.  2006.  All Rights Reserved.
 Intel funded Tungsten Graphics (http://www.tungstengraphics.com) to
 develop this 3D driver.
 
 Permission is hereby granted, free of charge, to any person obtaining
 a copy of this software and associated documentation files (the
 "Software"), to deal in the Software without restriction, including
 without limitation the rights to use, copy, modify, merge, publish,
 distribute, sublicense, and/or sell copies of the Software, and to
 permit persons to whom the Software is furnished to do so, subject to
 the following conditions:
 
 The above copyright notice and this permission notice (including the
 next paragraph) shall be included in all copies or substantial
 portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE COPYRIGHT OWNER(S) AND/OR ITS SUPPLIERS BE
 LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 **********************************************************************/
 /*
  * Authors:
  *   Keith Whitwell <keith@tungstengraphics.com>
  */
              

#ifndef BRW_WM_H
#define BRW_WM_H

#include "brw_context.h"
#include "brw_eu.h"

#define SATURATE (1<<5)

/* A big lookup table is used to figure out which and how many
 * additional regs will inserted before the main payload in the WM
 * program execution.  These mainly relate to depth and stencil
 * processing and the early-depth-test optimization.
 */
#define IZ_PS_KILL_ALPHATEST_BIT    0x1
#define IZ_PS_COMPUTES_DEPTH_BIT    0x2
#define IZ_DEPTH_WRITE_ENABLE_BIT   0x4
#define IZ_DEPTH_TEST_ENABLE_BIT    0x8
#define IZ_STENCIL_WRITE_ENABLE_BIT 0x10
#define IZ_STENCIL_TEST_ENABLE_BIT  0x20
#define IZ_BIT_MAX                  0x40

#define AA_NEVER     0
#define AA_SOMETIMES 1
#define AA_ALWAYS    2

struct brw_wm_prog_key {
   GLuint source_depth_reg:3;
   GLuint aa_dest_stencil_reg:3;
   GLuint dest_depth_reg:3;
   GLuint nr_depth_regs:3;
   GLuint computes_depth:1;
   GLuint source_depth_to_render_target:1;
   GLuint flat_shade:1;
   GLuint runtime_check_aads_emit:1;

   GLuint shadowtex_mask:16;
   GLuint yuvtex_mask:16;
   GLuint yuvtex_swap_mask:16;	/* UV swaped */

   GLuint vp_nr_outputs:6;
   GLuint nr_inputs:6;
   GLuint nr_cbufs:3;
   GLuint has_flow_control:1;

   GLuint program_string_id;
};


/* A bit of a glossary:
 *
 * brw_wm_value: A computed value or program input.  Values are
 * constant, they are created once and are never modified.  When a
 * fragment program register is written or overwritten, new values are
 * created fresh, preserving the rule that values are constant.
 *
 * brw_wm_ref: A reference to a value.  Wherever a value used is by an
 * instruction or as a program output, that is tracked with an
 * instance of this struct.  All references to a value occur after it
 * is created.  After the last reference, a value is dead and can be
 * discarded.
 *
 * brw_wm_grf: Represents a physical hardware register.  May be either
 * empty or hold a value.  Register allocation is the process of
 * assigning values to grf registers.  This occurs in pass2 and the
 * brw_wm_grf struct is not used before that.
 *
 * Fragment program registers: These are time-varying constructs that
 * are hard to reason about and which we translate away in pass0.  A
 * single fragment program register element (eg. temp[0].x) will be
 * translated to one or more brw_wm_value structs, one for each time
 * that temp[0].x is written to during the program. 
 */



/* Used in pass2 to track register allocation.
 */
struct brw_wm_grf {
   struct brw_wm_value *value;
   GLuint nextuse;
};

struct brw_wm_value {
   struct brw_reg hw_reg;	/* emitted to this reg, may not always be there */
   struct brw_wm_ref *lastuse;
   struct brw_wm_grf *resident; 
   GLuint contributes_to_output:1;
   GLuint spill_slot:16;	/* if non-zero, spill immediately after calculation */
};

struct brw_wm_ref {
   struct brw_reg hw_reg;	/* nr filled in in pass2, everything else, pass0 */
   struct brw_wm_value *value;
   struct brw_wm_ref *prevuse;
   GLuint unspill_reg:7;	/* unspill to reg */
   GLuint emitted:1;
   GLuint insn:24;
};

struct brw_wm_instruction {
   struct brw_wm_value *dst[4];
   struct brw_wm_ref *src[3][4];
   GLuint opcode:8;
   GLuint saturate:1;
   GLuint writemask:4;
   GLuint sampler:4;
   GLuint tex_unit:4;   /* texture/sampler unit for texture instructions */
   GLuint target:4;     /* TGSI_TEXTURE_x for texture instructions,
                         * target binding table index for FB_WRITE
                         */
   GLuint eot:1;    	/* End of thread indicator for FB_WRITE*/
};


#define BRW_WM_MAX_INSN  2048
#define BRW_WM_MAX_GRF   128		/* hardware limit */
#define BRW_WM_MAX_VREG  (BRW_WM_MAX_INSN * 4)
#define BRW_WM_MAX_REF   (BRW_WM_MAX_INSN * 12)
#define BRW_WM_MAX_PARAM 256
#define BRW_WM_MAX_CONST 256
#define BRW_WM_MAX_KILLS MAX_NV_FRAGMENT_PROGRAM_INSTRUCTIONS
#define BRW_WM_MAX_SUBROUTINE 16


/* New opcodes to track internal operations required for WM unit.
 * These are added early so that the registers used can be tracked,
 * freed and reused like those of other instructions.
 */
#define MAX_OPCODE        TGSI_OPCODE_LAST
#define WM_PIXELXY        (MAX_OPCODE)
#define WM_DELTAXY        (MAX_OPCODE + 1)
#define WM_PIXELW         (MAX_OPCODE + 2)
#define WM_LINTERP        (MAX_OPCODE + 3)
#define WM_PINTERP        (MAX_OPCODE + 4)
#define WM_CINTERP        (MAX_OPCODE + 5)
#define WM_WPOSXY         (MAX_OPCODE + 6)
#define WM_FB_WRITE       (MAX_OPCODE + 7)
#define WM_FRONTFACING    (MAX_OPCODE + 8)
#define MAX_WM_OPCODE     (MAX_OPCODE + 9)

#define BRW_FILE_PAYLOAD   (TGSI_FILE_COUNT)
#define PAYLOAD_DEPTH      (PIPE_MAX_SHADER_INPUTS) /* ?? */

#define X    0
#define Y    1
#define Z    2
#define W    3


struct brw_fp_src {
   unsigned file:4;
   unsigned index:16;
   unsigned swizzle:8;
   unsigned indirect:1;
   unsigned negate:1;
   unsigned abs:1;
};

struct brw_fp_dst {
   unsigned file:4;
   unsigned index:16;
   unsigned writemask:4;
   unsigned indirect:1;
   unsigned saturate:1;
};

struct brw_fp_instruction {
   struct brw_fp_dst dst;
   struct brw_fp_src src[3];
   unsigned opcode:8;
   unsigned target:8; /* XXX: special usage for FB_WRITE */
   unsigned tex_unit:4;
   unsigned sampler:4;
   unsigned pad:8;
};


struct brw_wm_compile {
   struct brw_compile func;
   struct brw_wm_prog_key key;
   struct brw_wm_prog_data prog_data;

   struct brw_fragment_shader *fp;

   GLfloat (*env_param)[4];

   enum {
      START,
      PASS2_DONE
   } state;

   /* Initial pass - translate fp instructions to fp instructions,
    * simplifying and adding instructions for interpolation and
    * framebuffer writes.
    */
   struct {
      GLfloat v[4];
      unsigned nr;
   } immediate[BRW_WM_MAX_CONST+3];
   GLuint nr_immediates;
   
   struct brw_fp_instruction fp_instructions[BRW_WM_MAX_INSN];
   GLuint nr_fp_insns;
   GLuint fp_temp;
   GLuint fp_interp_emitted;
   GLuint fp_fragcolor_emitted;
   GLuint fp_first_internal_temp;

   struct brw_fp_src fp_pixel_xy;
   struct brw_fp_src fp_delta_xy;
   struct brw_fp_src fp_pixel_w;


   /* Subsequent passes using SSA representation:
    */
   struct brw_wm_value vreg[BRW_WM_MAX_VREG];
   GLuint nr_vreg;

   struct brw_wm_value creg[BRW_WM_MAX_PARAM];
   GLuint nr_creg;

   struct {
      struct brw_wm_value depth[4]; /* includes r0/r1 */
      struct brw_wm_value input_interp[PIPE_MAX_SHADER_INPUTS];
   } payload;


   const struct brw_wm_ref *pass0_fp_reg[BRW_FILE_PAYLOAD+1][256][4];

   struct brw_wm_ref undef_ref;
   struct brw_wm_value undef_value;

   struct brw_wm_ref refs[BRW_WM_MAX_REF];
   GLuint nr_refs;

   struct brw_wm_instruction instruction[BRW_WM_MAX_INSN];
   GLuint nr_insns;

   struct brw_wm_grf pass2_grf[BRW_WM_MAX_GRF/2];

   GLuint grf_limit;
   GLuint max_wm_grf;
   GLuint last_scratch;

   GLuint cur_inst;  /**< index of current instruction */

   GLboolean out_of_regs;  /**< ran out of GRF registers? */

   /** Mapping from Mesa registers to hardware registers */
   struct {
      GLboolean inited;
      struct brw_reg reg;
   } wm_regs[BRW_FILE_PAYLOAD+1][256][4];

   GLboolean used_grf[BRW_WM_MAX_GRF];
   GLuint first_free_grf;
   struct brw_reg stack;
   struct brw_reg emit_mask_reg;
   GLuint tmp_regs[BRW_WM_MAX_GRF];
   GLuint tmp_index;
   GLuint tmp_max;
   GLuint subroutines[BRW_WM_MAX_SUBROUTINE];
   GLuint dispatch_width;

   /** we may need up to 3 constants per instruction (if use_const_buffer) */
   struct {
      GLint index;
      struct brw_reg reg;
   } current_const[3];

   GLuint error;
};


GLuint brw_wm_nr_args( GLuint opcode );
GLuint brw_wm_is_scalar_result( GLuint opcode );

int brw_wm_pass_fp( struct brw_wm_compile *c );
void brw_wm_pass0( struct brw_wm_compile *c );
void brw_wm_pass1( struct brw_wm_compile *c );
void brw_wm_pass2( struct brw_wm_compile *c );
void brw_wm_emit( struct brw_wm_compile *c );

void brw_wm_print_value( struct brw_wm_compile *c,
			 struct brw_wm_value *value );

void brw_wm_print_ref( struct brw_wm_compile *c,
		       struct brw_wm_ref *ref );

void brw_wm_print_insn( struct brw_wm_compile *c,
			struct brw_wm_instruction *inst );

void brw_wm_print_program( struct brw_wm_compile *c,
			   const char *stage );

void brw_wm_print_fp_program( struct brw_wm_compile *c,
                              const char *stage );

void brw_wm_lookup_iz( GLuint line_aa,
		       GLuint lookup,
		       GLboolean ps_uses_depth,
		       struct brw_wm_prog_key *key );

void brw_wm_branching_shader_emit(struct brw_context *brw, struct brw_wm_compile *c);

void emit_ddxy(struct brw_compile *p,
	       const struct brw_reg *dst,
	       GLuint mask,
	       GLboolean is_ddx,
	       const struct brw_reg *arg0);

#endif
