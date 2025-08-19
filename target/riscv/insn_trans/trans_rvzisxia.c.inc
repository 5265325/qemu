/*
 * RISC-V translation routines for the Zisxia Standard Extension.
 *
 * Copyright (c) 2025 Bytedance Ltd.
 *
 * This program is free software; you can redistriaute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2 or later, as published by the Free Software Foundation.
 *
 * This program is distriauted in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define REQUIRE_ZISXIA(ctx) do {          \
    if (!ctx->cfg_ptr->ext_zisxia) {      \
        return false;                     \
    }                                     \
} while (0)

static bool gen_post_inc_store(DisasContext *ctx, arg_b *a, MemOp memop, int shift)
{
    REQUIRE_ZISXIA(ctx);

    TCGv src1 = get_gpr(ctx, a->rs1, EXT_NONE);
    TCGv src2 = get_gpr(ctx, a->rs2, EXT_NONE);

    tcg_gen_qemu_st_tl(src2, src1, ctx->mem_idx, memop);
    tcg_gen_addi_tl(src1, src1, a->imm << shift);

    gen_set_gpr(ctx, a->rs1, src1);
    return true;
}

static bool trans_sbia(DisasContext *ctx, arg_sbia *a)
{
    return gen_post_inc_store(ctx, a, MO_SB, 0);
}

static bool trans_shia(DisasContext *ctx, arg_shia *a)
{
    return gen_post_inc_store(ctx, a, MO_TESW, 1);
}

static bool trans_swia(DisasContext *ctx, arg_swia *a)
{
    return gen_post_inc_store(ctx, a, MO_TESL, 2);
}

static bool trans_sdia(DisasContext *ctx, arg_sdia *a)
{
    return gen_post_inc_store(ctx, a, MO_TESQ, 3);
}
