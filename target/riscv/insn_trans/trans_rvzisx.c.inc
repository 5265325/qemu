/*
 * RISC-V translation routines for the Zisx Standard Extension.
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

#define REQUIRE_ZISX(ctx) do {            \
    if (!ctx->cfg_ptr->ext_zilx) {        \
        return false;                     \
    }                                     \
} while (0)

static bool gen_indexed_store(DisasContext *ctx, arg_r3 *a, MemOp memop, int scale, bool zext)
{
    REQUIRE_ZISX(ctx);

    TCGv src3 = get_gpr(ctx, a->rs3, EXT_NONE);
    TCGv data = get_gpr(ctx, a->rs2, EXT_NONE);
    TCGv addr;

    if (zext) {
        addr = tcg_temp_new();
        tcg_gen_ext_tl(addr, src3, MO_UL);
        if (scale > 0) {
            tcg_gen_shli_tl(addr, addr, scale);
        }
    } else if (scale > 0) {
        addr = tcg_temp_new();
        tcg_gen_shli_tl(addr, src3, scale);
    } else {
        addr = src3;
    }

    addr = get_address_indexed(ctx, a->rs1, addr);

    tcg_gen_qemu_st_tl(data, addr, ctx->mem_idx, memop);
    return true;
}

static bool trans_sxb(DisasContext *ctx, arg_sxb *a)
{
    return gen_indexed_store(ctx, a, MO_SB, 0, false);
}

static bool trans_sxb_uw(DisasContext *ctx, arg_sxb_uw *a)
{
    return gen_indexed_store(ctx, a, MO_SB, 0, true);
}

static bool trans_sxh(DisasContext *ctx, arg_sxh *a)
{
    return gen_indexed_store(ctx, a, MO_TESW, 0, false);
}

static bool trans_sxh_uw(DisasContext *ctx, arg_sxh_uw *a)
{
    return gen_indexed_store(ctx, a, MO_TESW, 0, true);
}

static bool trans_sxw(DisasContext *ctx, arg_sxw *a)
{
    return gen_indexed_store(ctx, a, MO_TESL, 0, false);
}

static bool trans_sxw_uw(DisasContext *ctx, arg_sxw_uw *a)
{
    return gen_indexed_store(ctx, a, MO_TESL, 0, true);
}

static bool trans_sxd(DisasContext *ctx, arg_sxd *a)
{
    REQUIRE_64_OR_128BIT(ctx);
    return gen_indexed_store(ctx, a, MO_TEUQ, 0, false);
}

static bool trans_sxd_uw(DisasContext *ctx, arg_sxd_uw *a)
{
    REQUIRE_64_OR_128BIT(ctx);
    return gen_indexed_store(ctx, a, MO_TEUQ, 0, true);
}

static bool trans_sxh_s(DisasContext *ctx, arg_sxh_s *a)
{
    return gen_indexed_store(ctx, a, MO_TESW, 1, false);
}

static bool trans_sxh_s_uw(DisasContext *ctx, arg_sxh_s_uw *a)
{
    return gen_indexed_store(ctx, a, MO_TESW, 1, true);
}

static bool trans_sxw_s(DisasContext *ctx, arg_sxw_s *a)
{
    return gen_indexed_store(ctx, a, MO_TESL, 2, false);
}

static bool trans_sxw_s_uw(DisasContext *ctx, arg_sxw_s_uw *a)
{
    return gen_indexed_store(ctx, a, MO_TESL, 2, true);
}

static bool trans_sxd_s(DisasContext *ctx, arg_sxd_s *a)
{
    REQUIRE_64_OR_128BIT(ctx);
    return gen_indexed_store(ctx, a, MO_TEUQ, 3, false);
}

static bool trans_sxd_s_uw(DisasContext *ctx, arg_sxd_s_uw *a)
{
    REQUIRE_64_OR_128BIT(ctx);
    return gen_indexed_store(ctx, a, MO_TEUQ, 3, true);
}
