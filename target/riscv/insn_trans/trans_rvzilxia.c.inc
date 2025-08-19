/*
 * RISC-V translation routines for the Zilxia Standard Extension.
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

#define REQUIRE_ZILXIA(ctx) do {          \
    if (!ctx->cfg_ptr->ext_zilxia) {      \
        return false;                     \
    }                                     \
} while (0)

static bool gen_post_inc_load(DisasContext *ctx, arg_i *a, MemOp memop, int shift)
{
    REQUIRE_ZILXIA(ctx);

    TCGv rd = dest_gpr(ctx, a->rd);
    TCGv src1 = get_gpr(ctx, a->rs1, EXT_NONE);

    tcg_gen_qemu_ld_tl(rd, src1, ctx->mem_idx, memop);
    tcg_gen_addi_tl(src1, src1, a->imm << shift);

    gen_set_gpr(ctx, a->rd, rd);
    gen_set_gpr(ctx, a->rs1, src1);
    return true;
}

static bool trans_lbia(DisasContext *ctx, arg_lbia *a)
{
    return gen_post_inc_load(ctx, a, MO_SB, 0);
}

static bool trans_lbuia(DisasContext *ctx, arg_lbuia *a)
{
    return gen_post_inc_load(ctx, a, MO_UB, 0);
}

static bool trans_lhia(DisasContext *ctx, arg_lhia *a)
{
    return gen_post_inc_load(ctx, a, MO_TESW, 1);
}

static bool trans_lhuia(DisasContext *ctx, arg_lhuia *a)
{
    return gen_post_inc_load(ctx, a, MO_TEUW, 1);
}

static bool trans_lwia(DisasContext *ctx, arg_lwia *a)
{
    return gen_post_inc_load(ctx, a, MO_TESL, 2);
}

static bool trans_lwuia(DisasContext *ctx, arg_lwuia *a)
{
    return gen_post_inc_load(ctx, a, MO_TEUL, 2);
}

static bool trans_ldia(DisasContext *ctx, arg_ldia *a)
{
    return gen_post_inc_load(ctx, a, MO_TESQ, 3);
}
