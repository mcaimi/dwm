/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx  = 1;        /* border pixel of windows */
static unsigned int gappx     = 1;        /* gap between windows in pixels */
static unsigned int snap      = 32;       /* snap pixel */

static unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static unsigned int systrayspacing = 2;   /* systray spacing */
static int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static int showsystray        = 1;     /* 0 means no systray */

// dwm bar height
static unsigned int bar_height = 0; /* 0 means default height */
static int showbar      = 1;        /* 0 means no bar */
static int topbar       = 1;        /* 0 means bottom bar */

static int corner_radius = 0;   /* window corners radius */
static int round_windows = 0;   /* 0 == round only floating windows, 1 == round only fullscreen windows, 2 == round all windows */

static int lockfullscreen = 1;
static unsigned int bar_win_spacing = 0; // spacing between the status bar and window frames

static char *interfacefont          = NULL;
static char *dmenufont              = NULL;
static char defaultfont[]           = "Iosevka Term Medium 12";
static char dmenufont_default[]     = "Iosevka Term Medium:size=11";
static char *font                   = defaultfont;
static char normbg[]           = "#222222";
static char normborder[]       = "#444444";
static char normfg[]           = "#bbbbbb";
static char selbg[]            = "#005577";
static char selborder[]        = "#005577";
static char selfg[]            = "#eeeeee";
static char *custom_normbg    = NULL;
static char *custom_normfg    = NULL;
static char *custom_normborder    = NULL;
static char *custom_selbg    = NULL;
static char *custom_selfg    = NULL;
static char *custom_selborder    = NULL;

static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;

// center the current window name in the top bar
static unsigned int centerwindowname = 0;

static char *colors[][3]      = {
  /*               fg      bg      border   */
  [SchemeNorm] = { normfg, normbg, normborder },
  [SchemeSel]  = { selfg, selbg,  selborder  },
};
static unsigned int alphas[][3]      = {
  /*               fg      bg        border     */
  [SchemeNorm] = { OPAQUE, baralpha, borderalpha },
  [SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
  /* xprop(1):
   *  WM_CLASS(STRING) = instance, class
   *  WM_NAME(STRING) = title
   */
  /* class      instance    title       centered      tags mask     isfloating   monitor */
  { "Gimp",     NULL,       NULL,       0,            0,            1,           -1 },
  { "Firefox",  NULL,       NULL,       0,            1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
  /* symbol     arrange function */
  { "[]=",      tile },    /* first entry is default */
  { "><>",      NULL },    /* no layout function means floating behavior */
  { "[M]",      monocle },
  { "TTT",      bstack },
  { "===",      bstackhoriz },
  { "|M|",      centeredmaster},
  { ">M>",      centeredfloatingmaster},
  { "(@)",      spiral},
  { "><",         dwindle},
  { NULL, NULL},
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
  { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
  { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
  { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
  { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/bash", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont_default, "-nb", normbg, "-nf", normfg, "-sb", selbg, "-sf", selfg, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };

static Key keys[] = {
  /* modifier                     key        function        argument */
  { MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
  { MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
  { MODKEY,                       XK_grave,  togglescratch,  {.v = scratchpadcmd } },
  { MODKEY|ControlMask,           XK_b,      togglebar,      {0} },
  { MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
  { MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
  { MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
  { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
  { MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
  { MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
  { MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
  { MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
  { MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
  { MODKEY,                       XK_Return, zoom,           {0} },
  { MODKEY,                       XK_Tab,    view,           {0} },
  { MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
  { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
  { MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
  { MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
  { MODKEY,                       XK_b,      setlayout,      {.v = &layouts[3]} },
  { MODKEY,                       XK_n,      setlayout,      {.v = &layouts[4]} },
  { MODKEY,                       XK_c,      setlayout,      {.v = &layouts[5]} },
  { MODKEY,                       XK_v,      setlayout,      {.v = &layouts[6]} },
  { MODKEY|ControlMask,           XK_plus,   cyclelayout,    {0} },
  { MODKEY,                       XK_space,  setlayout,      {0} },
  { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
  { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
  { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
  { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
  { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
  { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
  { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
  { MODKEY|ShiftMask,             XK_minus,  updategaps,     {.i = -1 } },
  { MODKEY|ShiftMask,             XK_plus,   updategaps,     {.i = +1 } },
  { MODKEY|ControlMask,           XK_minus,  updategaps,     {.i = 0 } },
  { MODKEY|ControlMask,           XK_comma,  flip_topbar,    {0} },
  { MODKEY,                       XK_r,     xrdbreload,     {.v = NULL} },
  TAGKEYS(                        XK_1,                      0)
  TAGKEYS(                        XK_2,                      1)
  TAGKEYS(                        XK_3,                      2)
  TAGKEYS(                        XK_4,                      3)
  TAGKEYS(                        XK_5,                      4)
  TAGKEYS(                        XK_6,                      5)
  TAGKEYS(                        XK_7,                      6)
  TAGKEYS(                        XK_8,                      7)
  TAGKEYS(                        XK_9,                      8)
  { MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
  /* click                event mask      button          function        argument */
  { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
  { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
  { ClkWinTitle,          0,              Button2,        zoom,           {0} },
  { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
  { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
  { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
  { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
  { ClkTagBar,            0,              Button1,        view,           {0} },
  { ClkTagBar,            0,              Button3,        toggleview,     {0} },
  { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
  { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

/* configurable Xresources */
ResourceAtom configurable_resources[] = {
  { "interfacefont", STRING, &interfacefont },
  { "dmenufont", STRING, &dmenufont },
  { "normbackground", STRING, &custom_normbg },
  { "normforeground", STRING, &custom_normfg },
  { "normborder", STRING, &custom_normborder },
  { "selbackground", STRING, &custom_selbg },
  { "selforeground", STRING, &custom_selfg },
  { "selborder", STRING, &custom_selborder },
  { "gappx", INTEGER, &gappx },
  { "topbar", INTEGER, &topbar },
  { "showbar", INTEGER, &showbar },
  { "borderpx", INTEGER, &borderpx },
  { "systraypinning", INTEGER, &systraypinning },
  { "systrayspacing", INTEGER, &systrayspacing },
  { "systraypinningfailfirst", INTEGER, &systraypinningfailfirst },
  { "showsystray", INTEGER, &showsystray },
  { "centerwindowname", INTEGER, &centerwindowname },
  { "corner_radius", INTEGER, &corner_radius },
  { "round_windows", INTEGER, &round_windows },
  { "bar_height", INTEGER, &bar_height },
  { "lockfullscreen", INTEGER, &lockfullscreen },
  { "bar_win_spacing", INTEGER, &bar_win_spacing },
};

unsigned int resource_inventory_size = sizeof(configurable_resources)/sizeof(configurable_resources[0]);
unsigned int number_of_layouts = sizeof(layouts)/sizeof(layouts[0]);

