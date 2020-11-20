
/* Generated data (by glib-mkenums) */

#include "tomoe-enum-types.h"
#include "tomoe.h"
#include "tomoe-candidate.h"
#include "tomoe-char.h"
#include "tomoe-context.h"
#include "tomoe-config.h"
#include "tomoe-dict.h"
#include "tomoe-module.h"
#include "tomoe-module-impl.h"
#include "tomoe-query.h"
#include "tomoe-reading.h"
#include "tomoe-recognizer.h"
#include "tomoe-shelf.h"
#include "tomoe-writing.h"

/* enumerations from "tomoe-reading.h" */
GType
tomoe_reading_type_get_type (void)
{
  static GType etype = 0;
  if (etype == 0) {
    static const GEnumValue values[] = {
      { TOMOE_READING_INVALID, "TOMOE_READING_INVALID", "invalid" },
      { TOMOE_READING_UNKNOWN, "TOMOE_READING_UNKNOWN", "unknown" },
      { TOMOE_READING_JA_ON, "TOMOE_READING_JA_ON", "ja-on" },
      { TOMOE_READING_JA_KUN, "TOMOE_READING_JA_KUN", "ja-kun" },
      { 0, NULL, NULL }
    };
    etype = g_enum_register_static ("TomoeReadingType", values);
  }
  return etype;
}

/* Generated data ends here */

