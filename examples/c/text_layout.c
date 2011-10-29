#define NILE_INCLUDE_PROCESS_API
#include "nile.h"
#include "text_layout.h"

#define IN_QUANTUM 2
#define OUT_QUANTUM 3

typedef struct {
    nile_Real_t v_W_w;
    nile_Real_t v_W_s;
    nile_Real_t v_W_n;
} text_layout_MakeWords_vars_t;

static nile_Buffer_t *
text_layout_MakeWords_prologue (nile_Process_t *p, nile_Buffer_t *out)
{
    text_layout_MakeWords_vars_t *vars = nile_Process_vars (p);
    text_layout_MakeWords_vars_t v = *vars;
    nile_Real_t t_7 = nile_Real (0);
    nile_Real_t t_8 = nile_Real (0);
    nile_Real_t t_9 = nile_Real (0);
    nile_Real_t t_6_1 = t_7;
    nile_Real_t t_6_2 = t_8;
    nile_Real_t t_6_3 = t_9;
    nile_Real_t t_10_w = t_6_1;
    nile_Real_t t_10_s = t_6_2;
    nile_Real_t t_10_n = t_6_3;
    v.v_W_w = t_10_w;
    v.v_W_s = t_10_s;
    v.v_W_n = t_10_n;
    *vars = v;
    return out;
}

static nile_Buffer_t *
text_layout_MakeWords_body (nile_Process_t *p,
                            nile_Buffer_t *in,
                            nile_Buffer_t *out)
{
    text_layout_MakeWords_vars_t *vars = nile_Process_vars (p);
    text_layout_MakeWords_vars_t v = *vars;
    
    while (!nile_Buffer_is_empty (in) && !nile_Buffer_quota_hit (out)) {
        text_layout_MakeWords_vars_t v_ = v;
        nile_Real_t v_G_w = nile_Buffer_pop_head(in);
        nile_Real_t v_G_s = nile_Buffer_pop_head(in);
        nile_Real_t t_11 = nile_Real_neq(v_G_s, v.v_W_s);
        if (nile_Real_nz (t_11)) {
            nile_Real_t t_13 = nile_Real (1);
            nile_Real_t t_12_1 = v_G_w;
            nile_Real_t t_12_2 = v_G_s;
            nile_Real_t t_12_3 = t_13;
            nile_Real_t t_14_w = t_12_1;
            nile_Real_t t_14_s = t_12_2;
            nile_Real_t t_14_n = t_12_3;
            v_.v_W_w = t_14_w;
            v_.v_W_s = t_14_s;
            v_.v_W_n = t_14_n;
            if (nile_Buffer_tailroom (out) < OUT_QUANTUM)
                out = nile_Process_append_output (p, out);
            nile_Buffer_push_tail(out, v.v_W_w);
            nile_Buffer_push_tail(out, v.v_W_s);
            nile_Buffer_push_tail(out, v.v_W_n);
        }
        else {
            nile_Real_t t_16 = nile_Real_add(v.v_W_w, v_G_w);
            nile_Real_t t_17 = nile_Real (1);
            nile_Real_t t_18 = nile_Real_add(v.v_W_n, t_17);
            nile_Real_t t_15_1 = t_16;
            nile_Real_t t_15_2 = v.v_W_s;
            nile_Real_t t_15_3 = t_18;
            nile_Real_t t_19_w = t_15_1;
            nile_Real_t t_19_s = t_15_2;
            nile_Real_t t_19_n = t_15_3;
            v_.v_W_w = t_19_w;
            v_.v_W_s = t_19_s;
            v_.v_W_n = t_19_n;
        }
        v = v_;
    }
    
    *vars = v;
    return out;
}

static nile_Buffer_t *
text_layout_MakeWords_epilogue (nile_Process_t *p, nile_Buffer_t *out)
{
    text_layout_MakeWords_vars_t *vars = nile_Process_vars (p);
    text_layout_MakeWords_vars_t v = *vars;
    if (nile_Buffer_tailroom (out) < OUT_QUANTUM)
        out = nile_Process_append_output (p, out);
    nile_Buffer_push_tail(out, v.v_W_w);
    nile_Buffer_push_tail(out, v.v_W_s);
    nile_Buffer_push_tail(out, v.v_W_n);
    return out;
}

nile_Process_t *
text_layout_MakeWords (nile_Process_t *p)
{
    text_layout_MakeWords_vars_t *vars;
    p = nile_Process (p, IN_QUANTUM, sizeof (*vars), text_layout_MakeWords_prologue, text_layout_MakeWords_body, text_layout_MakeWords_epilogue);
    return p;
}

#undef IN_QUANTUM
#undef OUT_QUANTUM

#define IN_QUANTUM 3
#define OUT_QUANTUM 2

typedef struct {
    nile_Real_t v_o_x;
    nile_Real_t v_o_y;
    nile_Real_t v_w;
    nile_Real_t v_h;
    nile_Real_t v_x;
    nile_Real_t v_y;
} text_layout_PlaceWords_vars_t;

static nile_Buffer_t *
text_layout_PlaceWords_prologue (nile_Process_t *p, nile_Buffer_t *out)
{
    text_layout_PlaceWords_vars_t *vars = nile_Process_vars (p);
    text_layout_PlaceWords_vars_t v = *vars;
    v.v_x = v.v_o_x;
    v.v_y = v.v_o_y;
    *vars = v;
    return out;
}

static nile_Buffer_t *
text_layout_PlaceWords_body (nile_Process_t *p,
                             nile_Buffer_t *in,
                             nile_Buffer_t *out)
{
    text_layout_PlaceWords_vars_t *vars = nile_Process_vars (p);
    text_layout_PlaceWords_vars_t v = *vars;
    
    while (!nile_Buffer_is_empty (in) && !nile_Buffer_quota_hit (out)) {
        text_layout_PlaceWords_vars_t v_ = v;
        nile_Real_t v_W_w = nile_Buffer_pop_head(in);
        nile_Real_t v_W_s = nile_Buffer_pop_head(in);
        nile_Real_t v_W_n = nile_Buffer_pop_head(in);
        nile_Real_t t_1 = nile_Real_add(v.v_x, v_W_w);
        nile_Real_t t_2 = nile_Real_add(v.v_o_x, v.v_w);
        nile_Real_t t_3 = nile_Real_lt(t_1, t_2);
        nile_Real_t t_4 = nile_Real (1);
        nile_Real_t t_5 = nile_Real_eq(v_W_s, t_4);
        nile_Real_t t_6 = nile_Real_or(t_3, t_5);
        if (nile_Real_nz (t_6)) {
            nile_Real_t t_7 = nile_Real_add(v.v_x, v_W_w);
            v_.v_x = t_7;
            nile_Real_t t_8_1 = v.v_x;
            nile_Real_t t_8_2 = v.v_y;
            nile_Real_t t_9_x = t_8_1;
            nile_Real_t t_9_y = t_8_2;
            if (nile_Buffer_tailroom (out) < OUT_QUANTUM)
                out = nile_Process_append_output (p, out);
            nile_Buffer_push_tail(out, t_9_x);
            nile_Buffer_push_tail(out, t_9_y);
        }
        else {
            nile_Real_t t_10 = nile_Real_add(v.v_o_x, v_W_w);
            v_.v_x = t_10;
            nile_Real_t t_11 = nile_Real_add(v.v_y, v.v_h);
            v_.v_y = t_11;
            nile_Real_t t_12_1 = v.v_o_x;
            nile_Real_t t_12_2 = v_.v_y;
            nile_Real_t t_13_x = t_12_1;
            nile_Real_t t_13_y = t_12_2;
            if (nile_Buffer_tailroom (out) < OUT_QUANTUM)
                out = nile_Process_append_output (p, out);
            nile_Buffer_push_tail(out, t_13_x);
            nile_Buffer_push_tail(out, t_13_y);
        }
        v = v_;
    }
    
    *vars = v;
    return out;
}

static nile_Buffer_t *
text_layout_PlaceWords_epilogue (nile_Process_t *p, nile_Buffer_t *out)
{
    text_layout_PlaceWords_vars_t *vars = nile_Process_vars (p);
    text_layout_PlaceWords_vars_t v = *vars;
    return out;
}

nile_Process_t *
text_layout_PlaceWords (nile_Process_t *p, 
                        float v_o_x, 
                        float v_o_y, 
                        float v_w, 
                        float v_h)
{
    text_layout_PlaceWords_vars_t *vars;
    text_layout_PlaceWords_vars_t v;
    p = nile_Process (p, IN_QUANTUM, sizeof (*vars), text_layout_PlaceWords_prologue, text_layout_PlaceWords_body, text_layout_PlaceWords_epilogue);
    if (p) {
        vars = nile_Process_vars (p);
        v.v_o_x = nile_Real (v_o_x);
        v.v_o_y = nile_Real (v_o_y);
        v.v_w = nile_Real (v_w);
        v.v_h = nile_Real (v_h);
        *vars = v;
    }
    return p;
}

#undef IN_QUANTUM
#undef OUT_QUANTUM

