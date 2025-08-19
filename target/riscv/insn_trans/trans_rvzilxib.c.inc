/*
 * RISC-V translation routines for the Zilxib Standard Extension.
 *
 * Copyright (c) 2025 Bytedance Ltd.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2 or later, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define REQUIRE_ZILXIB(ctx) do {          \
    if (!ctx->cfg_ptr->ext_zilxib) {      \
        return false;                     \
    }                                     \
} while (0)

static bool gen_pre_inc_load(DisasContext *ctx, arg_i *a, MemOp memop, int shift)
{
    REQUIRE_ZILXIB(ctx);

    TCGv rd = dest_gpr(ctx, a->rd);
    TCGv src1 = get_gpr(ctx, a->rs1, EXT_NONE);

    tcg_gen_addi_tl(src1, src1, a->imm << shift);
    tcg_gen_qemu_ld_tl(rd, src1, ctx->mem_idx, memop);

    gen_set_gpr(ctx, a->rs1, src1);
    gen_set_gpr(ctx, a->rd, rd);
    return true;
}

static bool trans_lbib(DisasContext *ctx, arg_lbib *a)
{
    return gen_pre_inc_load(ctx, a, MO_SB, 0);
}

static bool trans_lbuib(DisasContext *ctx, arg_lbuib *a)
{
    return gen_pre_inc_load(ctx, a, MO_UB, 0);
}

static bool trans_lhib(DisasContext *ctx, arg_lhib *a)
{
    return gen_pre_inc_load(ctx, a, MO_TESW, 1);
}

static bool trans_lhuib(DisasContext *ctx, arg_lhuib *a)
{
    return gen_pre_inc_load(ctx, a, MO_TEUW, 1);
}

static bool trans_lwib(DisasContext *ctx, arg_lwib *a)
{
    return gen_pre_inc_load(ctx, a, MO_TESL, 2);
}

static bool trans_lwuib(DisasContext *ctx, arg_lwuib *a)
{
    return gen_pre_inc_load(ctx, a, MO_TEUL, 2);
}

static bool trans_ldib(DisasContext *ctx, arg_ldib *a)
{
    return gen_pre_inc_load(ctx, a, MO_TESQ, 3);
}
