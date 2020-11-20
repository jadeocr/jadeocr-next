#include "tomoe-rb.h"

#define _SELF(obj) (TOMOE_CANDIDATE(RVAL2GOBJ(obj)))

void
_tomoe_rb_init_tomoe_candidate(VALUE mTomoe)
{
    VALUE cTomoeCandidate;

    cTomoeCandidate = G_DEF_CLASS(TOMOE_TYPE_CANDIDATE, "Candidate", mTomoe);
}

