/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 *  Copyright (C) 2005 Takuro Ashie <ashie@homa.ne.jp>
 *  Copyright (C) 2006 Juernjakob Harder <juernjakob.harder@gmail.com>
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
 *  $Id: tomoe-candidate.c 1307 2007-06-07 03:31:25Z ikezoe $
 */

#include "tomoe-candidate.h"

enum {
    PROP_0,
    PROP_CHAR,
    PROP_SCORE
};

#define TOMOE_CANDIDATE_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), TOMOE_TYPE_CANDIDATE, TomoeCandidatePrivate))

typedef struct _TomoeCandidatePrivate   TomoeCandidatePrivate;
struct _TomoeCandidatePrivate
{
    TomoeChar    *character;
    gint          score;
};

G_DEFINE_TYPE (TomoeCandidate, tomoe_candidate, G_TYPE_OBJECT)

static void dispose      (GObject      *object);
static void set_property (GObject      *object,
                          guint         prop_id,
                          const GValue *value,
                          GParamSpec   *pspec);
static void get_property (GObject      *object,
                          guint         prop_id,
                          GValue       *value,
                          GParamSpec   *pspec);

static void
tomoe_candidate_class_init (TomoeCandidateClass *klass)
{
    GObjectClass *gobject_class;

    gobject_class = G_OBJECT_CLASS (klass);

    gobject_class->dispose      = dispose;
    gobject_class->set_property = set_property;
    gobject_class->get_property = get_property;

    g_object_class_install_property (
        gobject_class,
        PROP_CHAR,
        g_param_spec_object (
            "char",
            "Character",
            "A TomoeChar object",
            TOMOE_TYPE_CHAR,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
    g_object_class_install_property(
        gobject_class,
        PROP_SCORE,
        g_param_spec_uint(
            "score",
            "Score",
            "Score of this candidate. Lower value has higher priority.",
            0,
            G_MAXINT,
            0,
            G_PARAM_READWRITE));


    g_type_class_add_private (gobject_class, sizeof (TomoeCandidatePrivate));
}

static void
tomoe_candidate_init (TomoeCandidate *cand)
{
    TomoeCandidatePrivate *priv = TOMOE_CANDIDATE_GET_PRIVATE (cand);

    priv->character = NULL;
    priv->score     = 0;
}

static void
dispose (GObject *object)
{
    TomoeCandidatePrivate *priv = TOMOE_CANDIDATE_GET_PRIVATE (object);

    if (priv->character)
        g_object_unref (G_OBJECT (priv->character));
    priv->character = NULL;

    G_OBJECT_CLASS (tomoe_candidate_parent_class)->dispose (object);
}

static void
set_property (GObject *object,
              guint prop_id,
              const GValue *value,
              GParamSpec *pspec)
{
    TomoeCandidate *cand = TOMOE_CANDIDATE (object);
    TomoeCandidatePrivate *priv = TOMOE_CANDIDATE_GET_PRIVATE (cand);

    switch (prop_id) {
    case PROP_CHAR:
    {
        GObject *obj = g_value_get_object (value);
        priv->character = TOMOE_CHAR (g_object_ref (obj));
        break;
    }
    case PROP_SCORE:
        priv->score = g_value_get_int (value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}


static void
get_property (GObject *object,
              guint prop_id,
              GValue *value,
              GParamSpec *pspec)
{
    TomoeCandidate *cand = TOMOE_CANDIDATE (object);
    TomoeCandidatePrivate *priv = TOMOE_CANDIDATE_GET_PRIVATE (cand);

    switch (prop_id) {
    case PROP_CHAR:
        g_value_set_object (value, G_OBJECT (priv->character));
        break;
    case PROP_SCORE:
        g_value_set_int (value, priv->score);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

TomoeCandidate*
tomoe_candidate_new (TomoeChar *chr)
{
    TomoeCandidate *cand;

    cand = g_object_new(TOMOE_TYPE_CANDIDATE,
                        "char", chr,
                        NULL);

    return cand;
}

TomoeChar *
tomoe_candidate_get_char (TomoeCandidate *cand)
{
    TomoeCandidatePrivate *priv;

    g_return_val_if_fail (TOMOE_IS_CANDIDATE (cand), NULL);

    priv = TOMOE_CANDIDATE_GET_PRIVATE (cand);
    g_return_val_if_fail (priv, NULL);

    return priv->character;
}

void
tomoe_candidate_set_score (TomoeCandidate *cand, gint score)
{
    TomoeCandidatePrivate *priv;

    g_return_if_fail (TOMOE_IS_CANDIDATE (cand));

    priv = TOMOE_CANDIDATE_GET_PRIVATE (cand);
    g_return_if_fail (priv);

    priv->score = score;

    g_object_notify (G_OBJECT (cand), "score");
}

gint
tomoe_candidate_get_score (TomoeCandidate *cand)
{
    TomoeCandidatePrivate *priv;

    g_return_val_if_fail (TOMOE_IS_CANDIDATE (cand), 0);

    priv = TOMOE_CANDIDATE_GET_PRIVATE (cand);
    g_return_val_if_fail (priv, 0);

    return priv->score;
}

/**
 * tomoe_candidate_compare:
 * @a: 1st TomoeCandidate object to compare.
 * @b: 2nd TomoeCandidate object to compare.
 *
 * Compare two TomoeCandidate.
 *
 * Return value: -1 a < b, 0 a= b, 1 a > b
 */
gint
tomoe_candidate_compare (const TomoeCandidate *a, const TomoeCandidate *b)
{
    TomoeCandidatePrivate *priv_a, *priv_b;

    if (!TOMOE_IS_CANDIDATE (a) || !TOMOE_IS_CANDIDATE (b)) return 0;

    priv_a = TOMOE_CANDIDATE_GET_PRIVATE (a);
    priv_b = TOMOE_CANDIDATE_GET_PRIVATE (b);
    if (!priv_a || !priv_b) return 0;

    return priv_a->score > priv_b->score ? 1
         : priv_a->score < priv_b->score ? - 1
         : 0;
}

/*
vi:ts=4:nowrap:ai:expandtab
*/
