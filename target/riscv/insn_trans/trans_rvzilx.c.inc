/*
 * RISC-V translation routines for the Zilx Standard Extension.
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

#define REQUIRE_ZILX(ctx) do {          \
    if (!ctx->cfg_ptr->ext_zilx) {      \
        return false;                     \
    }                                     \
} while (0)

static bool gen_indexed_load(DisasContext *ctx, arg_r *a, MemOp memop, int scale, bool zext)
{
    REQUIRE_ZILX(ctx);

    TCGv rd = dest_gpr(ctx, a->rd);
    TCGv src2 = get_gpr(ctx, a->rs2, EXT_NONE);
    TCGv addr;

    if (zext) {
        addr = tcg_temp_new();
        tcg_gen_ext_tl(addr, src2, MO_UL);
        if (scale > 0) {
            tcg_gen_shli_tl(addr, addr, scale);
        }
    } else if (scale > 0) {
        addr = tcg_temp_new();
        tcg_gen_shli_tl(addr, src2, scale);
    } else {
        addr = src2;
    }

    addr = get_address_indexed(ctx, a->rs1, addr);

    tcg_gen_qemu_ld_tl(rd, addr, ctx->mem_idx, memop);
    gen_set_gpr(ctx, a->rd, rd);
    return true;
}

static bool trans_lxb(DisasContext *ctx, arg_lxb *a)
{
    return gen_indexed_load(ctx, a, MO_SB, 0, false);
}

static bool trans_lxb_uw(DisasContext *ctx, arg_lxb_uw *a)
{
    return gen_indexed_load(ctx, a, MO_SB, 0, true);
}

static bool trans_lxbu(DisasContext *ctx, arg_lxbu *a)
{
    return gen_indexed_load(ctx, a, MO_UB, 0, false);
}

static bool trans_lxbu_uw(DisasContext *ctx, arg_lxbu_uw *a)
{
    return gen_indexed_load(ctx, a, MO_UB, 0, true);
}

static bool trans_lxh(DisasContext *ctx, arg_lxh *a)
{
    return gen_indexed_load(ctx, a, MO_TESW, 0, false);
}

static bool trans_lxh_uw(DisasContext *ctx, arg_lxh_uw *a)
{
    return gen_indexed_load(ctx, a, MO_TESW, 0, true);
}

static bool trans_lxhu(DisasContext *ctx, arg_lxhu *a)
{
    return gen_indexed_load(ctx, a, MO_TEUW, 0, false);
}

static bool trans_lxhu_uw(DisasContext *ctx, arg_lxhu_uw *a)
{
    return gen_indexed_load(ctx, a, MO_TEUW, 0, true);
}

static bool trans_lxw(DisasContext *ctx, arg_lxw *a)
{
    return gen_indexed_load(ctx, a, MO_TESL, 0, false);
}

static bool trans_lxw_uw(DisasContext *ctx, arg_lxw_uw *a)
{
    return gen_indexed_load(ctx, a, MO_TESL, 0, true);
}

static bool trans_lxwu(DisasContext *ctx, arg_lxwu *a)
{
    return gen_indexed_load(ctx, a, MO_TEUL, 0, false);
}

static bool trans_lxwu_uw(DisasContext *ctx, arg_lxwu_uw *a)
{
    return gen_indexed_load(ctx, a, MO_TEUL, 0, true);
}

static bool trans_lxd(DisasContext *ctx, arg_lxd *a)
{
    REQUIRE_64_OR_128BIT(ctx);
    return gen_indexed_load(ctx, a, MO_TESQ, 0, false);
}

static bool trans_lxd_uw(DisasContext *ctx, arg_lxd_uw *a)
{
    REQUIRE_64_OR_128BIT(ctx);
    return gen_indexed_load(ctx, a, MO_TESQ, 0, true);
}

static bool trans_lxh_s(DisasContext *ctx, arg_lxh_s *a)
{
    return gen_indexed_load(ctx, a, MO_TESW, 1, false);
}

static bool trans_lxh_s_uw(DisasContext *ctx, arg_lxh_s_uw *a)
{
    return gen_indexed_load(ctx, a, MO_TESW, 1, true);
}

static bool trans_lxhu_s(DisasContext *ctx, arg_lxhu_s *a)
{
    return gen_indexed_load(ctx, a, MO_TEUW, 1, false);
}

static bool trans_lxhu_s_uw(DisasContext *ctx, arg_lxhu_s_uw *a)
{
    return gen_indexed_load(ctx, a, MO_TEUW, 1, true);
}

static bool trans_lxw_s(DisasContext *ctx, arg_lxw_s *a)
{
    return gen_indexed_load(ctx, a, MO_TESL, 2, false);
}

static bool trans_lxw_s_uw(DisasContext *ctx, arg_lxw_s_uw *a)
{
    return gen_indexed_load(ctx, a, MO_TESL, 2, true);
}

static bool trans_lxwu_s(DisasContext *ctx, arg_lxwu_s *a)
{
    return gen_indexed_load(ctx, a, MO_TEUL, 2, false);
}

static bool trans_lxwu_s_uw(DisasContext *ctx, arg_lxwu_s_uw *a)
{
    return gen_indexed_load(ctx, a, MO_TEUL, 2, true);
}

static bool trans_lxd_s(DisasContext *ctx, arg_lxd_s *a)
{
    REQUIRE_64_OR_128BIT(ctx);
    return gen_indexed_load(ctx, a, MO_TESQ, 3, false);
}

static bool trans_lxd_s_uw(DisasContext *ctx, arg_lxd_s_uw *a)
{
    REQUIRE_64_OR_128BIT(ctx);
    return gen_indexed_load(ctx, a, MO_TESQ, 3, true);
}
