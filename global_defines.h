/* 
  DECLARATION SECTION
  Following all program defines and declarations

    - MACROS
    - STRUCTS
    - ENUMS
    - FUNCTION PROTOTYPES
    - VARIABLES
*/

/* macros */
#define BUTTONMASK              (ButtonPressMask|ButtonReleaseMask)
#define CLEANMASK(mask)         (mask & ~(numlockmask|LockMask) & (ShiftMask|ControlMask|Mod1Mask|Mod2Mask|Mod3Mask|Mod4Mask|Mod5Mask))
#define INTERSECT(x,y,w,h,m)    (MAX(0, MIN((x)+(w),(m)->wx+(m)->ww) - MAX((x),(m)->wx)) * MAX(0, MIN((y)+(h),(m)->wy+(m)->wh) - MAX((y),(m)->wy)))
#define LENGTH(X)               (sizeof X / sizeof X[0])
#define MOUSEMASK               (BUTTONMASK|PointerMotionMask)
#define WIDTH(X)                ((X)->w + 2 * (X)->bw + gappx)
#define HEIGHT(X)               ((X)->h + 2 * (X)->bw + gappx)
#define TEXTW(X)                (drw_fontset_getwidth(drw, (X)) + lrpad)

#define TAGMASK                       ((1 << LENGTH(tags)) - 1)
#define TAGSLENGTH                    (LENGTH(tags))
#define TAGACTIVE(CLIENT_TAGS, TAG)   (CLIENT_TAGS & (1 << TAG))
#define HASTAG(MONITOR,TAG)           (MONITOR->tagset[MONITOR->seltags] & (1 << TAG))
#define ISVISIBLE(CLIENT)             ((CLIENT->tags & CLIENT->mon->tagset[CLIENT->mon->seltags]))

#define OPAQUE  0xffU

#define SYSTEM_TRAY_REQUEST_DOCK    0

/* XEMBED messages */
#define XEMBED_EMBEDDED_NOTIFY      0
#define XEMBED_WINDOW_ACTIVATE      1
#define XEMBED_FOCUS_IN             4
#define XEMBED_MODALITY_ON         10

#define XEMBED_MAPPED              (1 << 0)
#define XEMBED_WINDOW_ACTIVATE      1
#define XEMBED_WINDOW_DEACTIVATE    2

#define VERSION_MAJOR               0
#define VERSION_MINOR               0
#define XEMBED_EMBEDDED_VERSION (VERSION_MAJOR << 16) | VERSION_MINOR