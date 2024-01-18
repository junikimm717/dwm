/* See LICENSE file for copyright and license details. */
#include "fibonacci.c"
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 4;        /* border pixel of windows */
static const unsigned int gappx     = 15;        /* gaps between windows */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 20;       /* vert inner gap between windows */
static const unsigned int gappoh    = 20;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 20;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Jetbrains Mono:size=14", "nerd-fonts:size=14" };
static const char dmenufont[]       = "Jetbrains Mono:size=12";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_juni[]        = "#073e19";
static const char col_beige[]        = "#cec0af";
static const char col_darkblue[]        = "#242e35";
static const char col_pink[]        = "#f0cad5";
//static const char col_juni[]        = "#005577";
//static const char col_juni[]        = "#091935";
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;

static const char *colors[][3]      = {
  /*               fg         bg         border   */
  [SchemeNorm] = { col_gray3, col_darkblue, col_gray2 },
  [SchemeSel]  = { col_gray1, col_beige,  col_beige  },
};
static const unsigned int alphas[][3]      = {
  /*               fg      bg        border     */
  [SchemeNorm] = { OPAQUE, baralpha, borderalpha },
  [SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
//static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
//static const char *tags[] = { "1", "2", "3", "4", "5" , "6", ""};
static const char *tags[] = { "", "爵", "", "", "", "", "", "󰙯", "9"};

static const Rule rules[] = {
  /* xprop(1):
   *	WM_CLASS(STRING) = instance, class
   *	WM_NAME(STRING) = title
   */
  /* class      instance    title       tags mask     isfloating   monitor */
  //{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
  { "Alacritty",      NULL,       NULL,         1<<0,            0,           -1 },
  { "Brave-browser",  NULL,       NULL,         1<<1,            0,           -1 },
  { "Tor Browser",    NULL,       NULL,         1<<1,            0,           -1 },
  { "torbrowser",     NULL,       NULL,         1<<1,            0,           -1 },
  { "firefox",        NULL,       NULL,         1<<1,            0,           -1 },

  // social media
  { "discord",        NULL,       NULL,         1<<7,            0,           -1 },
  { "Skype",          NULL,       NULL,         1<<3,            0,           -1 },
  { "zoom",           NULL,       NULL,         1<<3,            0,           -1 },
  { "zoom",           NULL,       "Chat",       1<<3,            1,           -1 },
  { "zoom",           NULL,       "Meetings",   1<<3,            1,           -1 },
  { "Signal",         "signal",   NULL,         1<<3,            0,           -1 },
  { NULL,             NULL,       "WhatsApp",   1<<3,            0,           -1 },
  { "GStreamer",      NULL,       NULL,         1<<3,            0,           -1 },
  
  // Programming
  { "Code",           NULL,       NULL,         1<<2,            0,           -1 },
  { "jetbrains",      NULL,       NULL,         1<<2,            0,           -1 },
  { "sublime",        NULL,       NULL,         1<<2,            0,           -1 },
  { "Pulsar",         NULL,       NULL,         1<<2,            0,           -1 },
  { "geogebra",         NULL,       NULL,         1<<2,            0,           -1 },

  // Virtualization
  { "Virt-manager",   NULL,       NULL,         1<<4,            0,           -1 },

  // Music
  { "cantata",        NULL,       NULL,         1<<5,            0,           -1 },
  { "youtube-music",  NULL,       NULL,         1<<5,            0,           -1 },

  { "Blueberry.py",   NULL,       NULL,         1<<6,            0,           -1 },
  { "control-center", NULL,       NULL,         1<<6,            0,           -1 },

};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
  /* symbol     arrange function */
  { "[M]",      monocle },
  { "[]=",      tile },    /* first entry is default */
  { "><>",      NULL },    /* no layout function means floating behavior */
  { "(@)",                spiral },
  { "[\\]",               dwindle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALT Mod1Mask
#define TAGKEYS(KEY,TAG) \
{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
//static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_darkblue, "-sf", col_gray4, NULL };
static const char *dmenucmd[] = { "rofi", "-show", "drun" };
//static const char *dmenucmd[] = { "rofi", "-show", "drun", NULL };
static const char *termcmd[]  = { "/usr/bin/alacritty", NULL };
static const char *tmuxcmd[]  = { "/usr/bin/alacritty", "-e", "/usr/bin/tmux", NULL };
static const char *browser[]  = { "/usr/bin/firefox", NULL };
static const char *upvol[]   = { "/usr/bin/pactl", "set-sink-volume", "0", "+5%",     NULL };
static const char *downvol[] = { "/usr/bin/pactl", "set-sink-volume", "0", "-5%",     NULL };
static const char *mutevol[] = { "/usr/bin/pactl", "set-sink-mute",   "0", "toggle",  NULL };
static const char *upbright[] = { "/usr/bin/brightnessctl", "set", "10%+" };
static const char *downbright[] = { "/usr/bin/brightnessctl", "set", "10%-" };
static const char *killdwm[] = {"/home/junikim/.local/bin/quit.sh", NULL};
static const char *join[] = {"/usr/local/bin/mt", NULL};
static const char *toggle[] = {"/usr/bin/mpc", "toggle", NULL};

static Key keys[] = {
  /* modifier                     key        function        argument */
  { MODKEY,                       XK_p,       spawn,          {.v = dmenucmd } },
  { MODKEY|ShiftMask,             XK_Return,  spawn,          {.v = tmuxcmd } },
  { MODKEY|ShiftMask,             XK_a,       spawn,          {.v = termcmd} },
  { MODKEY|ShiftMask,             XK_w,       spawn,          {.v = browser } },
  { MODKEY,                       XK_e,       spawn,          {.v = join } },
  { MODKEY,                       XK_b,       togglebar,      {0} },
  { MODKEY,                       XK_j,       focusstack,     {.i = +1 } },
  { MODKEY,                       XK_k,       focusstack,     {.i = -1 } },
  { MODKEY,                       XK_i,       incnmaster,     {.i = +1 } },
  { MODKEY,                       XK_d,       incnmaster,     {.i = -1 } },
  { MODKEY,                       XK_h,       setmfact,       {.f = -0.05} },
  { MODKEY,                       XK_l,       setmfact,       {.f = +0.05} },
  { MODKEY,                       XK_Return,  zoom,           {0} },
  { MODKEY,                       XK_Tab,     view,           {0} },
  { MODKEY|ShiftMask,             XK_q,       killclient,     {0} },
  { MODKEY,                       XK_m,       setlayout,      {.v = &layouts[0]} },
  { MODKEY,                       XK_t,       setlayout,      {.v = &layouts[1]} },
  { MODKEY,                       XK_f,       setlayout,      {.v = &layouts[2]} },
  //{ MODKEY,                       XK_space,  setlayout,      {0} },
  { MODKEY,                       XK_space,   spawn,          {.v = dmenucmd } },
  { MODKEY|ShiftMask,             XK_space,   togglefloating, {0} },
  { MODKEY,                       XK_0,       view,           {.ui = ~0 } },
  { MODKEY|ShiftMask,             XK_0,       tag,            {.ui = ~0 } },
  { MODKEY,                       XK_comma,   focusmon,       {.i = -1 } },
  { MODKEY,                       XK_period,  focusmon,       {.i = +1 } },
  { MODKEY|ShiftMask,             XK_comma,   tagmon,         {.i = -1 } },
  { MODKEY|ShiftMask,             XK_period,  tagmon,         {.i = +1 } },

	{ 0,                            XF86XK_AudioMute,           spawn,  {.v = mutevol } },
	{ 0,                            XF86XK_AudioRaiseVolume,    spawn,  {.v = upvol   } },
	{ 0,                            XF86XK_AudioLowerVolume,    spawn,  {.v = downvol } },
  { 0,                            XF86XK_MonBrightnessUp,     spawn,  {.v = upbright} },
	{ 0,                            XF86XK_MonBrightnessDown,   spawn,  {.v = downbright} },
	{ 0,                            XF86XK_AudioPlay,   spawn,  {.v = toggle} },

  { MODKEY,                       XK_r,      setlayout,      {.v = &layouts[3]} },
  { MODKEY|ShiftMask,             XK_r,      setlayout,      {.v = &layouts[4]} },
  TAGKEYS(                        XK_1,                      0)
    TAGKEYS(                        XK_2,                      1)
    TAGKEYS(                        XK_3,                      2)
    TAGKEYS(                        XK_4,                      3)
    TAGKEYS(                        XK_5,                      4)
    TAGKEYS(                        XK_6,                      5)
    TAGKEYS(                        XK_7,                      6)
    TAGKEYS(                        XK_8,                      7)
    TAGKEYS(                        XK_9,                      8)
    { MODKEY|ShiftMask,             XK_e,      spawn,           {.v = killdwm} },
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

