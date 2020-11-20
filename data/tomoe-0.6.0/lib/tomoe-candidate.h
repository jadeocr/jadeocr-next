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
 *  $Id: tomoe-candidate.h 850 2006-11-28 06:40:31Z ikezoe $
 */

#ifndef __TOMOE_CANDIDATE_H__
#define __TOMOE_CANDIDATE_H__

#include <glib-object.h>
#include "tomoe-char.h"

G_BEGIN_DECLS

#define TOMOE_TYPE_CANDIDATE            (tomoe_candidate_get_type ())
#define TOMOE_CANDIDATE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TOMOE_TYPE_CANDIDATE, TomoeCandidate))
#define TOMOE_CANDIDATE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TOMOE_TYPE_CANDIDATE, TomoeCandidateClass))
#define TOMOE_IS_CANDIDATE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TOMOE_TYPE_CANDIDATE))
#define TOMOE_IS_CANDIDATE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TOMOE_TYPE_CANDIDATE))
#define TOMOE_CANDIDATE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), TOMOE_TYPE_CANDIDATE, TomoeCandidateClass))

typedef struct _TomoeCandidate TomoeCandidate;
typedef struct _TomoeCandidateClass TomoeCandidateClass;

struct _TomoeCandidate
{
    GObject       object;
};

struct _TomoeCandidateClass
{
    GObjectClass parent_class;
};

GType           tomoe_candidate_get_type        (void) G_GNUC_CONST;

TomoeCandidate *tomoe_candidate_new             (TomoeChar      *chr);

TomoeChar      *tomoe_candidate_get_char        (TomoeCandidate *cand);

void            tomoe_candidate_set_score       (TomoeCandidate *cand,
                                                 gint            score);

gint            tomoe_candidate_get_score       (TomoeCandidate *cand);

gint            tomoe_candidate_compare         (const TomoeCandidate *a,
                                                 const TomoeCandidate *b);

G_END_DECLS

#endif /* __TOMOE_CANDIDATE_H__ */

/*
vi:ts=4:nowrap:ai:expandtab
*/
