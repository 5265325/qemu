/*
 * RISC-V translation routines for the Zisxib Standard Extension.
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

#define REQUIRE_ZISXIB(ctx) do {          \
    if (!ctx->cfg_ptr->ext_zisxib) {      \
        return false;                     \
    }                                     \
} while (0)

static bool gen_pre_inc_store(DisasContext *ctx, arg_b *a, MemOp memop, int shift)
{
    REQUIRE_ZISXIB(ctx);

    TCGv src1 = get_gpr(ctx, a->rs1, EXT_NONE);
    TCGv src2 = get_gpr(ctx, a->rs2, EXT_NONE);

    tcg_gen_addi_tl(src1, src1, a->imm << shift);
    tcg_gen_qemu_st_tl(src2, src1, ctx->mem_idx, memop);

    gen_set_gpr(ctx, a->rs1, src1);
    return true;
}

static bool trans_sbib(DisasContext *ctx, arg_sbib *a)
{
    return gen_pre_inc_store(ctx, a, MO_SB, 0);
}

static bool trans_shib(DisasContext *ctx, arg_shib *a)
{
    return gen_pre_inc_store(ctx, a, MO_TESW, 1);
}

static bool trans_swib(DisasContext *ctx, arg_swib *a)
{
    return gen_pre_inc_store(ctx, a, MO_TESL, 2);
}

static bool trans_sdib(DisasContext *ctx, arg_sdib *a)
{
    return gen_pre_inc_store(ctx, a, MO_TESQ, 3);
}
