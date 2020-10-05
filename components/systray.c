/* F2 - systray handling */
void
removesystrayicon(Client *i)
{
  Client **ii;

  if (!showsystray || !i)
    return;
  for (ii = &systray->icons; *ii && *ii != i; ii = &(*ii)->next);
  if (ii)
    *ii = i->next;
  free(i);
}

Monitor *
systraytomon(Monitor *m) {
  Monitor *t;
  int i, n;
  if(!systraypinning) {
    if(!m)
      return selmon;
    return m == selmon ? m : NULL;
  }
  for(n = 1, t = mons; t && t->next; n++, t = t->next) ;
  for(i = 1, t = mons; t && t->next && i < systraypinning; i++, t = t->next) ;
  if(systraypinningfailfirst && n < systraypinning)
    return mons;
  return t;
}

void
updatesystray(void)
{
  if (custombar) return;

  XSetWindowAttributes wa;
  XWindowChanges wc;
  Client *i;
  Monitor *m = systraytomon(NULL);
  unsigned int x = m->mx + m->mw;
  unsigned int w = 1;

  if (!showsystray)
    return;
  if (!systray) {
    /* init systray */
    if (!(systray = (Systray *)calloc(1, sizeof(Systray))))
      die("fatal: could not malloc() %u bytes\n", sizeof(Systray));
    systray->win = XCreateSimpleWindow(dpy, root, x, m->by, w, m->bh, 0, 0, scheme[SchemeSel][ColBg].pixel);
    wa.event_mask        = ButtonPressMask | ExposureMask;
    wa.override_redirect = True;
    wa.background_pixel  = scheme[SchemeNorm][ColBg].pixel;
    XSelectInput(dpy, systray->win, SubstructureNotifyMask);
    XChangeProperty(dpy, systray->win, netatom[NetSystemTrayOrientation], XA_CARDINAL, 32,
        PropModeReplace, (unsigned char *)&netatom[NetSystemTrayOrientationHorz], 1);
    XChangeWindowAttributes(dpy, systray->win, CWEventMask|CWOverrideRedirect|CWBackPixel, &wa);
    XMapRaised(dpy, systray->win);
    XSetSelectionOwner(dpy, netatom[NetSystemTray], systray->win, CurrentTime);
    if (XGetSelectionOwner(dpy, netatom[NetSystemTray]) == systray->win) {
      sendevent(root, xatom[Manager], StructureNotifyMask, CurrentTime, netatom[NetSystemTray], systray->win, 0, 0);
      XSync(dpy, False);
    }
    else {
      fprintf(stderr, "dwm: unable to obtain system tray.\n");
      free(systray);
      systray = NULL;
      return;
    }
  }
  for (w = 0, i = systray->icons; i; i = i->next) {
    /* make sure the background color stays the same */
    wa.background_pixel  = scheme[SchemeNorm][ColBg].pixel;
    XChangeWindowAttributes(dpy, i->win, CWBackPixel, &wa);
    XMapRaised(dpy, i->win);
    w += systrayspacing;
    i->x = w;
    XMoveResizeWindow(dpy, i->win, i->x, 0, i->w, i->h);
    w += i->w;
    if (i->mon != m)
      i->mon = m;
  }
  w = w ? w + systrayspacing : 1;
  x -= w;
  XMoveResizeWindow(dpy, systray->win, x, m->by, w, m->bh);
  wc.x = x; wc.y = m->by; wc.width = w; wc.height = m->bh;
  wc.stack_mode = Above; wc.sibling = m->barwin;
  XConfigureWindow(dpy, systray->win, CWX|CWY|CWWidth|CWHeight|CWSibling|CWStackMode, &wc);
  XMapWindow(dpy, systray->win);
  XMapSubwindows(dpy, systray->win);

  // add systray class and name: this allows for direct reference from picom and other X compositors and tools
  XClassHint ch = {"systray", "systray"};
  XSetClassHint(dpy, systray->win, &ch);
  XSync(dpy, False);
}

void
updatesystrayicongeom(Client *i, int w, int h)
{
  if (custombar) return;
  Monitor *m = i->mon;

  if (i) {
    i->h = m->bh;
    if (w == h)
      i->w = m->bh;
    else if (h == m->bh)
      i->w = w;
    else
      i->w = (int) ((float)m->bh * ((float)w / (float)h));
    applysizehints(i, &(i->x), &(i->y), &(i->w), &(i->h), False);
    /* force icons into the systray dimenons if they don't want to */
    if (i->h > m->bh) {
      if (i->w == i->h)
        i->w = m->bh;
      else
        i->w = (int) ((float)m->bh * ((float)i->w / (float)i->h));
      i->h = m->bh;
    }
  }
}

void
updatesystrayiconstate(Client *i, XPropertyEvent *ev)
{
  if (custombar) return; 

  long flags;
  int code = 0;

  if (!showsystray || !i || ev->atom != xatom[XembedInfo] ||
      !(flags = getatomprop(i, xatom[XembedInfo])))
    return;

  if (flags & XEMBED_MAPPED && !i->tags) {
    i->tags = 1;
    code = XEMBED_WINDOW_ACTIVATE;
    XMapRaised(dpy, i->win);
    setclientstate(i, NormalState);
  }
  else if (!(flags & XEMBED_MAPPED) && i->tags) {
    i->tags = 0;
    code = XEMBED_WINDOW_DEACTIVATE;
    XUnmapWindow(dpy, i->win);
    setclientstate(i, WithdrawnState);
  }
  else
    return;
  sendevent(i->win, xatom[Xembed], StructureNotifyMask, CurrentTime, code, 0,
      systray->win, XEMBED_EMBEDDED_VERSION);
}

Client *
wintosystrayicon(Window w) {
  Client *i = NULL;

  if (custombar) return NULL;

  if (!showsystray || !w)
    return i;
  for (i = systray->icons; i && i->win != w; i = i->next) ;
  return i;
}

unsigned int
getsystraywidth()
{
  if (custombar) return 0;

  unsigned int w = 0;
  Client *i;
  if(showsystray)
    for(i = systray->icons; i; w += i->w + systrayspacing, i = i->next) ;
  return w ? w + systrayspacing : 1;
}