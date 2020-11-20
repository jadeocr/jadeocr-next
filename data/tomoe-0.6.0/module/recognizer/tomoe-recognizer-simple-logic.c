/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 *  Copyright (C) 2006 Kouhei Sutou <kou@cozmixng.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this program; if not, write to the
 *  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 *  Boston, MA  02111-1307  USA
 *
 *  $Id: tomoe-recognizer-simple-logic.c 917 2006-11-30 09:58:39Z ikezoe $
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "tomoe-char.h"
#include "tomoe-candidate.h"
#include "tomoe-recognizer-simple-logic.h"

#define LIMIT_LENGTH ((TOMOE_WRITING_WIDTH * 0.25) * (TOMOE_WRITING_WIDTH * 0.25))

typedef struct _tomoe_metric tomoe_metric;

struct _tomoe_metric
{
    gint    a;
    gint    b;
    gint    c;
    gdouble d;
    gint    e;
    gdouble angle;
};

typedef struct _cand_priv cand_priv;

struct _cand_priv
{
    TomoeCandidate  *cand;
    GArray          *adapted_strokes;
};

static cand_priv *cand_priv_new               (TomoeCandidate *candidate);
static void       cand_priv_free              (cand_priv      *cand_p);
static GPtrArray *get_candidates              (GList       *points,
                                               GPtrArray   *cands);
static gint       match_stroke_num            (TomoeChar   *chr,
                                               int          input_stroke_num,
                                               GArray      *adapted);
static TomoeWriting *create_sparse_writing    (TomoeWriting *writing);

static gboolean  _g_array_has_this_int_value  (GArray      *a,
                                               gint         i);
static GArray   *_g_array_copy_int_value      (GArray      *a);

static gint
_candidate_compare_func (gconstpointer a, gconstpointer b)
{
    return tomoe_candidate_compare (a, b);
}

GList *
_tomoe_recognizer_simple_get_candidates (TomoeRecognizer *recognizer, TomoeDict *dict, TomoeWriting *input)
{
    GList *matched = NULL;
    GPtrArray *matches = NULL;
    GPtrArray *cands = NULL;
    GPtrArray *first_cands = NULL;
    GList *target_chars, *node;
    const GList *input_strokes, *list;
    guint input_stroke_num, i, j;
    TomoeWriting *sparse_writing;
    TomoeQuery *query;

    g_return_val_if_fail (input, NULL);
    g_return_val_if_fail (dict, NULL);

    sparse_writing = create_sparse_writing (input);
    input_stroke_num = tomoe_writing_get_n_strokes (sparse_writing);
    g_return_val_if_fail (input_stroke_num > 0, NULL);

    query = tomoe_query_new ();
    tomoe_query_set_min_n_strokes (query, input_stroke_num);
    target_chars = tomoe_dict_search (dict, query);
    g_object_unref (query);
    if (!target_chars) return NULL;

    first_cands = g_ptr_array_new ();
    for (node = target_chars; node; node = g_list_next (node)) {
        TomoeCandidate *candidate = node->data;
        cand_priv *cand;
        cand = cand_priv_new (candidate);
        g_ptr_array_add (first_cands, cand);
    }
    g_list_free (target_chars);

    input_strokes = tomoe_writing_get_strokes (sparse_writing);

    cands = get_candidates (input_strokes->data, first_cands);
    for (list = g_list_next (input_strokes); list; list = g_list_next (list)) {
        GList *points = (GList *) list->data;
        GPtrArray *tmp;
        tmp = get_candidates (points, cands);
        g_ptr_array_free (cands, TRUE);
        cands = tmp;
    }

    matches = g_ptr_array_new ();
    for (i = 0; i < (guint)cands->len; i++) {
        cand_priv *cand_p;
        gint pj;
        TomoeChar *a;
        const gchar *ac;
        gboolean f = TRUE;

        cand_p = g_ptr_array_index (cands, i);
        a = tomoe_candidate_get_char (cand_p->cand);
        pj = match_stroke_num (a, input_stroke_num, cand_p->adapted_strokes);

        if (pj <= 0)
            continue;

        ac = tomoe_char_get_utf8 (a);

        for (j = 0; j < (guint)matches->len; j++) {
            const gchar *c = g_ptr_array_index (matches, j);
            if (!strcmp(c, ac)) {
                f = FALSE;
                break;
            }
        }

        if (f) {
            TomoeCandidate *c = cand_p->cand;
            TomoeCandidate *cand;
            cand = tomoe_candidate_new (a);
            tomoe_candidate_set_score (cand, tomoe_candidate_get_score (c) / pj);
            matched = g_list_prepend (matched, cand);
            g_ptr_array_add (matches, (gpointer) ac);
        }
    }

    g_ptr_array_free (matches, TRUE);

    matched = g_list_sort (matched, _candidate_compare_func);

    g_ptr_array_foreach (first_cands, (GFunc) cand_priv_free, NULL);
    g_ptr_array_free (first_cands, TRUE);

    g_object_unref (sparse_writing);

    return matched;
}

/*
 * *******************
 *  stroke functions.
 * *******************
 */
static gint
stroke_calculate_metrics (GList *points, tomoe_metric **met)
{
    guint i = 0;
    gint x1, y1, x2, y2;
    tomoe_metric *m;
    guint n_points;

    g_return_val_if_fail (points, 0);
    n_points = g_list_length (points);
    if (!n_points) return 0;

    m = g_new (tomoe_metric, n_points - 1);
 
    for (i = 0; i < n_points - 1; i++) {
        TomoePoint *p1 = (TomoePoint *) g_list_nth_data (points, i);
        TomoePoint *p2 = (TomoePoint *) g_list_nth_data (points, i + 1);
        x1 = p1->x;
        y1 = p1->y;
        x2 = p2->x;
        y2 = p2->y;
        m[i].a     = x2 - x1;
        m[i].b     = y2 - y1;
        m[i].c     = x2 * y1 - y2 * x1;
        m[i].d     = m[i].a * m[i].a + m[i].b * m[i].b;
        m[i].e     = m[i].a * x1 + m[i].b * y1;
        m[i].angle = atan2 (y2 - y1, x2 - x1);
    }
 
    *met = m;
    return n_points - 1;
}

/*
 * **********************
 *  candidate private_functions.
 * **********************
 */

static cand_priv *
cand_priv_new (TomoeCandidate *cand)
{
    cand_priv *cand_p;

    cand_p                  = g_new (cand_priv, 1);
    cand_p->cand            = cand;
    cand_p->adapted_strokes = g_array_new (FALSE, FALSE, sizeof (gint));

    return cand_p;
}

static void
cand_priv_free (cand_priv *cand_p)
{
    if (!cand_p) return;

    g_array_free (cand_p->adapted_strokes, TRUE);
    cand_p->adapted_strokes = NULL;

    g_object_unref (G_OBJECT (cand_p->cand));
    cand_p->cand = NULL;

    g_free (cand_p);
}


/*
 * *******************
 *  utility functions
 * *******************
 */

#define SQUARE_LENGTH(x, y) ((x) * (x) + (y) * (y))

static gint
dist_tomoe_points (TomoePoint *a, TomoePoint *b)
{
    return SQUARE_LENGTH (a->x - b->x, a->y - b->y);
}

/*
 * ************************************
 *  handwriting recognition functions.
 * ************************************
 */

static gint
match_input_to_dict (GList *input_points, GList *writing_points)
{
    gint i_nop = 0;              /* input stroke number of points */
    tomoe_metric *i_met = NULL; /* input stroke metrics */
    gint d_nop = 0;              /* dict stroke number of points */
    tomoe_metric *d_met = NULL; /* dict stroke metrics */
    gint i_k_end = 0;
    gint i_k = 0;
    gint d_k = 0;
    gint m = 0;
    tomoe_metric i_me;
    tomoe_metric d_me;
    gint r = 0;
    gint d = 0;
    gint ret = 0;

    i_nop = g_list_length (input_points);
    stroke_calculate_metrics (input_points, &i_met);
  
    d_nop = g_list_length (writing_points);
    stroke_calculate_metrics (writing_points, &d_met);

    /* 
     * if the length between last point and second last point is lesser than
     * LIMIT_LENGTH, the last itinerary assumes "hane".
     */
    if (i_met[i_nop - 2].d < LIMIT_LENGTH)
        i_k_end = i_nop - 2;
    else
        i_k_end = i_nop - 1;
  
    m = 0;
    for (i_k = 1; i_k < i_k_end; i_k++) {
        TomoePoint *pi;

        pi = (TomoePoint *) g_list_nth_data (input_points, i_k);

        i_me = i_met[i_k];
        for (d_k = m; d_k < d_nop; d_k++) {
            TomoePoint *pw;

            pw = (TomoePoint *) g_list_nth_data (writing_points, d_k);
            d = dist_tomoe_points (pi, pw);
            if (d_k < d_nop - 1) {
                d_me = d_met[d_k];
                if (d < LIMIT_LENGTH &&
                    abs (i_me.angle - d_me.angle) < M_PI_2) {
                    m = d_k;
                    ret += d;
                    break;
                } else {
                    /* Distance between each characteristic points and line */
                    r = d_me.a * pi->x + d_me.b * pi->y - d_me.e;
                    d = abs (d_me.a * pi->y - d_me.b * pi->x - d_me.c);
                    if (0 <= r && r <= d_me.d &&
                        d < LIMIT_LENGTH * sqrt (d_me.d) &&
                        abs (i_me.angle - d_me.angle) < M_PI_2) {
                        m = d_k;
                        ret += d;
                        break;
                    }
                }
            } else {
                if (d < LIMIT_LENGTH) {
                    m = d_k;
                    ret += d;
                    break;
                }
            }
        }
        if (d_k >= d_nop) {
            free (i_met);
            free (d_met);
            return -1;
        }
    }
    free (i_met);
    free (d_met);
    return ret;
}

static int
match_dict_to_input (GList *writing_points, GList *input_points)
{
    gint           d_nop = 0;    /* dict stroke number of points */
    tomoe_metric *d_met = NULL; /* dict stroke metrics */
    gint           i_nop = 0;    /* input stroke number of points */
    tomoe_metric *i_met = NULL; /* input stroke metrics */
    gint d_k_end = 0;
    gint d_k = 0;
    gint i_k = 0;
    gint m = 0;
    tomoe_metric d_me;
    tomoe_metric i_me;
    gint r = 0;
    gint d = 0;
    gint ret = 0;

    d_nop = g_list_length (writing_points);
    stroke_calculate_metrics (writing_points, &d_met);
    i_nop = g_list_length (input_points);
    stroke_calculate_metrics (input_points, &i_met);

    /* 
     * if the length between last point and second last point is lesser than
     * LIMIT_LENGTH, the last itineraryassumes "hane".
     */
    if (d_met[d_nop - 2].d < LIMIT_LENGTH)
        d_k_end = d_nop - 2;
    else
        d_k_end = d_nop - 1;

    m = 0;
    for (d_k = 1; d_k < d_k_end - 1; d_k++) /* note difference: -1 */ {
        TomoePoint *pw;

        pw = (TomoePoint *) g_list_nth_data (writing_points, d_k); 
        d_me = d_met[d_k];
        for (i_k = m; i_k < i_nop; i_k++) {
            TomoePoint *pi;

            pi = (TomoePoint *) g_list_nth_data (input_points, i_k); 
            d = dist_tomoe_points (pw, pi);
            if (i_k < i_nop - 1) {
                i_me = i_met[i_k];
                if (d < LIMIT_LENGTH &&
                    abs (d_me.angle - i_me.angle) < M_PI_2) {
                    m = i_k;
                    ret += d;
                    break;
                } else {
                    /* Distance between each characteristic points and line */
                    r = i_me.a * pw->x + i_me.b * pw->y - i_me.e;
                    d = abs (i_me.a * pw->y - i_me.b * pw->x - i_me.c);
                    if (0 <= r && r <= i_me.d &&
                        d < LIMIT_LENGTH * sqrt (i_me.d) &&
                        abs (d_me.angle - i_me.angle) < M_PI_2) {
                        m = i_k;
                        ret += d;
                        break;
                    }
                }
            } else {
                if (d < LIMIT_LENGTH) {
                    m = i_k;
                    ret += d;
                    break;
                }
            }
        }
        if (i_k >= i_nop) {
            free (i_met);
            free (d_met);
            return -1;
        }
    }
    free (i_met);
    free (d_met);
    return d;
}

static GPtrArray *
get_candidates (GList *points, GPtrArray *cands)
{
    GPtrArray     *rtn_cands;
    guint          cand_index = 0;
    guint          strk_index = 0;
    gint           i_nop = 0;    /* input stroke number of points */
    tomoe_metric  *i_met = NULL; /* input stroke metrics */
    gint           d_nop = 0;    /* dict stroke number of points */
    TomoePoint *pi0, *pil;

    rtn_cands = g_ptr_array_new ();

    i_nop = g_list_length (points);
    stroke_calculate_metrics (points, &i_met);
    pi0 = (TomoePoint *) g_list_nth_data (points, 0);
    pil = (TomoePoint *) g_list_nth_data (points, i_nop - 1);

    for (cand_index = 0; cand_index < cands->len; cand_index++) {
        gboolean match_flag = FALSE;
        cand_priv *cand_p;
        GArray *tmp = NULL;
        TomoeChar *lttr;
        TomoeWriting *writing;
        TomoeCandidate *cand;
        GList *writing_strokes, *list;
        guint stroke_num;

        cand_p = g_ptr_array_index (cands, cand_index);
        tmp = _g_array_copy_int_value (cand_p->adapted_strokes);
        cand = TOMOE_CANDIDATE (cand_p->cand);
        lttr = tomoe_candidate_get_char (cand);
        writing = tomoe_char_get_writing (lttr);
        writing_strokes = (GList *) tomoe_writing_get_strokes (writing);
        stroke_num = g_list_length (writing_strokes);

        for (list = writing_strokes, strk_index = 0;
	     list;
	     list = g_list_next (list), strk_index++) {
            GList *writing_points;
            TomoePoint *pw0, *pw1, *pwl;
            gint d1 = 0, d2 = 0;
            gint d3 = 0, d4 = 0;
            gint score1 = 0, score2 = 0;
            gint score3 = 0;
            gdouble d_angle = 0;

            /* if the stroke index is already appended to, the value is ignored */
            if (_g_array_has_this_int_value (tmp, strk_index))
                continue;

            writing_points = (GList *) list->data;
            d_nop = g_list_length (writing_points);

            /*
             * Distance between the point and begining point.
             * Distance between the point and ending point.
             * Number of characteristic points.
             */
            pw0 = (TomoePoint *) g_list_nth_data (writing_points, 0);
            d1 = dist_tomoe_points (pi0, pw0);

            pwl = (TomoePoint *) g_list_nth_data (writing_points, d_nop - 1);
            d2 = dist_tomoe_points (pil, pwl);

            score3 = (d1 + d2);
            tomoe_candidate_set_score (
                cand,
                tomoe_candidate_get_score (cand) + score3);
            if (d1 > LIMIT_LENGTH ||
                d2 > LIMIT_LENGTH ||
                abs (d_nop - i_nop) > 3) {
                continue;
            }

            d3 = i_met[0].d;

            pw1 = (TomoePoint *) g_list_nth_data (writing_points, 1);
            d_angle = atan2 (pw1->y - pw0->y, pw1->x - pw0->x);
            d4 = dist_tomoe_points (pw0, pw1);;

            /* threshold is (angle of bigining line) % 45[degree] (PI/4)*/
            if (d3 > LIMIT_LENGTH &&
                d4 > LIMIT_LENGTH &&
                abs (d_angle - i_met[0].angle) > M_PI_4) {
                continue;
            }

            /*
             * Distance and angle of each characteristic points:
             * (Compare handwriting data with dictionary data)
             */
            score1 = match_input_to_dict (points, writing_points);
            if (score1 < 0) {
                tomoe_candidate_set_score (
                    cand,
                    tomoe_candidate_get_score (cand) * 2);
                continue;
            }
            tomoe_candidate_set_score (
                cand,
                tomoe_candidate_get_score (cand) + score1);

            /*
             * Distance and angle of each characteristic points:
             * (Compare dictionary data with handwriting data)
             */
            score2 = match_dict_to_input (writing_points, points);
            /* score2 = match_input_to_dict (writing_points, points); */
            if (score2 < 0) {
                tomoe_candidate_set_score (
                    cand,
                    tomoe_candidate_get_score (cand) * 2);
                continue;
            }
            tomoe_candidate_set_score (
                cand_p->cand,
                tomoe_candidate_get_score (cand) + score2);

            g_array_append_val (cand_p->adapted_strokes, strk_index);
            match_flag = TRUE;

            list = NULL;
        }

        if (match_flag) {
            g_ptr_array_add (rtn_cands, cand_p);
        }
        g_array_free (tmp, TRUE);
    }

    free (i_met);

    return rtn_cands;
}

static int
match_stroke_num (TomoeChar *chr, int input_stroke_num, GArray *adapted)
{
    gint pj = 100;
    gint adapted_num;
    gint d_stroke_num = tomoe_writing_get_n_strokes (tomoe_char_get_writing (chr));

    if (!adapted)
        return -1;

    adapted_num = adapted->len;

    if (d_stroke_num - input_stroke_num >= 3) {
        gint i, j;

        for (i = 0; i < adapted_num; i++) {
            j = g_array_index (adapted, gint, i);
            if (j - pj >= 3)
                return -1;
            pj = j;
        }
    }
    return pj;
}

static gint
get_distance (GList *first_node, GList *last_node, GList **most_node)
{
    /* Getting distance 
     * MAX( |aw - bv + c| )
     * a = x-p : b = y-q : c = py - qx
     * first = (p, q) : last = (x, y) : other = (v, w)
     */

    GList *dot;
    gint a, b, c;
    gint dist = 0;
    gint max  = 0;
    gint denom;
    TomoePoint *first = (TomoePoint*) first_node->data;
    TomoePoint *last  = (TomoePoint*) last_node->data;
    TomoePoint *p;

    *most_node = NULL;
    if (first_node == last_node)
        return 0;

    a = last->x - first->x;
    b = last->y - first->y;
    c = last->y * first->x - last->x * first->y;

    for (dot = first_node; dot != last_node; dot = dot->next) {
        p = (TomoePoint*) dot->data;
        dist = abs((a * p->y) - (b * p->x) + c);
        if (dist > max) {
            max = dist;
            *most_node = dot;
        }
    }

    denom = a * a + b * b;

    if (denom == 0)
        return 0;
    else
        return max * max / denom;
}

static GList*
get_vertex (GList *first_node, GList *last_node)
{
    GList *rv = NULL;
    GList *most_node;
    gint dist;
    gint error = TOMOE_WRITING_WIDTH * TOMOE_WRITING_WIDTH / 4444; /* 5% */

    dist = get_distance(first_node, last_node, &most_node);

    if (dist > error) {
        rv = g_list_concat(get_vertex(first_node, most_node),
                           get_vertex(most_node, last_node));
    } else {
        rv = g_list_append(rv, last_node->data);
    }
    return rv;
}

static TomoeWriting *
create_sparse_writing (TomoeWriting *writing)
{
    TomoeWriting *new;
    const GList *strokes, *list;

    g_return_val_if_fail (TOMOE_IS_WRITING (writing), NULL);

    new = tomoe_writing_new ();

    strokes = tomoe_writing_get_strokes (writing);
    for (list = strokes; list; list = g_list_next (list)) {
        GList *points = (GList *) list->data;
        GList *point, *new_points;
        TomoePoint *start_point = (TomoePoint *) points->data;

        tomoe_writing_move_to (new, start_point->x, start_point->y);
    
        new_points = g_list_prepend (get_vertex(points, g_list_last(points)),
                                     points->data);
    
        for (point = new_points; point; point = g_list_next (point)) {
            TomoePoint *p = (TomoePoint *) point->data;
            tomoe_writing_line_to (new, p->x, p->y);
        }
    }

    return new;
}

static gboolean
_g_array_has_this_int_value (GArray *a, gint i)
{
    guint l;

    if (!a || a->len == 0) return FALSE;

    for (l = 0; l < a->len; l++) {
        if (g_array_index (a, gint, l) == i)
	    return TRUE;
    }
    return FALSE;
}

static GArray *
_g_array_copy_int_value (GArray *a)
{
    guint i;
    GArray *b = g_array_new (FALSE, FALSE, sizeof (gint));

    for (i = 0; i < a->len; i++)
        g_array_append_val (b, g_array_index (a, gint, i));

    return b;
}

/*
vi:ts=4:nowrap:ai:expandtab
*/
