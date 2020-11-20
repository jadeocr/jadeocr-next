/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */

#ifndef __RBTOMOE_H__
#define __RBTOMOE_H__

#include <tomoe.h>

#include <ruby.h>
#include <rbgobject.h>

G_BEGIN_DECLS

#define RVAL2TCTX(obj) (TOMOE_CONTEXT(RVAL2GOBJ(obj)))
#define RVAL2TDIC(obj) (TOMOE_DICT(RVAL2GOBJ(obj)))
#define RVAL2TWTG(obj) (TOMOE_WRITING(RVAL2GOBJ(obj)))
#define RVAL2TQRY(obj) (TOMOE_QUERY(RVAL2GOBJ(obj)))
#define RVAL2TRDG(obj) (TOMOE_READING(RVAL2GOBJ(obj)))
#define RVAL2TCHR(obj) (TOMOE_CHAR(RVAL2GOBJ(obj)))
#define RVAL2TCND(obj) (TOMOE_CANDIDATE(RVAL2GOBJ(obj)))
#define RVAL2TREC(obj) (TOMOE_RECOGNIZER(RVAL2GOBJ(obj)))
#define RVAL2TCFG(obj) (TOMOE_CONFIG(RVAL2GOBJ(obj)))

#define RVAL2TRT(obj) (RVAL2GENUM(obj, TOMOE_TYPE_READING_TYPE))

G_END_DECLS

#endif /* __RBTOMOE_H__ */

/*
vi:expandtab
*/
