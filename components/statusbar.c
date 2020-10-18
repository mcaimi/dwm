/* F3 - bar handling */
void
resizebarwin(Monitor *m) {
  unsigned int w = m->ww;
  if (showsystray && m == systraytomon(m))
    w -= getsystraywidth();
  XMoveResizeWindow(dpy, m->barwin, m->wx, m->by, w, bh);
}

void
drawbar(Monitor *m)
{
  int x, w, tw = 0, stw = 0;
  int boxs = drw->fonts->h / 9;
  int boxw = drw->fonts->h / 6 + 2;
  unsigned int i, occ = 0, urg = 0, middle;
  Client *c;

  if(showsystray && m == systraytomon(m))
    stw = getsystraywidth();

  /* draw status first so it can be overdrawn by tags later */
  if (m == selmon) { /* status is only drawn on selected monitor */
    drw_setscheme(drw, scheme[SchemeNorm]);
    tw = TEXTW(stext) - lrpad / 2 + 2; /* 2px right padding */
    drw_text(drw, m->ww - tw - stw, 0, tw, bh, lrpad / 2 - 2, stext, 0);
  }

  resizebarwin(m);

  for (c = m->clients; c; c = c->next) {
    occ |= c->tags;
    if (c->isurgent)
      urg |= c->tags;
  }

  x = 0;
  for (i = 0; i < LENGTH(tags); i++) {
    if (!(TAGACTIVE(occ, i) || HASTAG(m, i))) continue;

    w = TEXTW(tags[i]);
    drw_setscheme(drw, scheme[m->tagset[m->seltags] & 1 << i ? SchemeSel : SchemeNorm]);
    drw_text(drw, x, 0, w, bh, lrpad / 2, tags[i], urg & 1 << i);

    if (TAGACTIVE(occ, i) && HASTAG(m, i))
      drw_rect(drw, x + boxw, 0, w - (2*boxw) + 1, boxw/2, 
        m == selmon && selmon->sel && selmon->sel->tags & (1 << i),
        urg & (1 << i));

    x += w;
  }

  w = blw = TEXTW(m->ltsymbol);
  drw_setscheme(drw, scheme[SchemeNorm]);
  x = drw_text(drw, x, 0, w, bh, lrpad / 2, m->ltsymbol, 0);

  if ((w = m->ww - tw - stw - x) > bh) {
    if (m->sel) {
      drw_setscheme(drw, scheme[m == selmon ? SchemeSel : SchemeNorm]);

      if (centerwindowname) {
        middle = TEXTCLAMP(m, x, m->sel->name);
        drw_text(drw, x, 0, w, bh, middle, m->sel->name, 0);
      } else drw_text(drw, x, 0, w, bh, lrpad/2, m->sel->name, 0);

      if (m->sel->isfloating)
        drw_rect(drw, x + boxs, boxs, boxw, boxw, m->sel->isfixed, 0);
    } else {
      drw_setscheme(drw, scheme[SchemeNorm]);
      drw_rect(drw, x, 0, w, bh, 1, 1);
    }
  }
  drw_map(drw, m->barwin, 0, 0, m->ww - stw, bh);
}

void
drawbars(void)
{
  Monitor *m;

  for (m = mons; m; m = m->next)
    drawbar(m);
}

void
togglebar(const Arg *arg)
{
  selmon->showbar = !selmon->showbar;
  updatebarpos(selmon);
  resizebarwin(selmon);
  if (showsystray) {
    XWindowChanges wc;
    if (!selmon->showbar)
      wc.y = -bh;
    else if (selmon->showbar) {
      wc.y = 0;
      if (!selmon->topbar)
        wc.y = selmon->mh - bh;
    }
    XConfigureWindow(dpy, systray->win, CWY, &wc);
  }
  arrange(selmon);
}

void
flip_topbar(const Arg *arg)
{
  // update selected monitor properties
  selmon->topbar = !selmon->topbar;
  updatebarpos(selmon);
  XMoveResizeWindow(dpy, selmon->barwin, selmon->wx, selmon->by, selmon->ww, bh);

  // arrange monitor
  arrange(selmon);

  return;
}

void
updatebarpos(Monitor *m)
{
  m->wy = m->my;
  m->wh = m->mh;
  if (m->showbar) {
    m->wh -= bh;
    m->by = m->topbar ? m->wy : m->wy + m->wh;
    m->wy = m->topbar ? m->wy + bh : m->wy;
  } else
    m->by = -bh;
}

void
updatebars(void)
{
  unsigned int w;
  Monitor *m;
  XSetWindowAttributes wa = {
    .override_redirect = True,
    .background_pixel = 0,
    .border_pixel = 0,
    .colormap = cmap,
    .event_mask = ButtonPressMask|ExposureMask
  };
  XClassHint ch = {"dwm", "dwm"};
  for (m = mons; m; m = m->next) {
    if (m->barwin)
      continue;
    w = m->ww;
    if (showsystray && m == systraytomon(m))
      w -= getsystraywidth();
    m->barwin = XCreateWindow(dpy, root, m->wx, m->by, w, bh, 0, depth,
                              InputOutput, visual,
                              CWOverrideRedirect|CWBackPixel|CWBorderPixel|CWColormap|CWEventMask, &wa);
    XDefineCursor(dpy, m->barwin, cursor[CurNormal]->cursor);
    if (showsystray && m == systraytomon(m))
      XMapRaised(dpy, systray->win);
    XMapRaised(dpy, m->barwin);
    XSetClassHint(dpy, m->barwin, &ch);
  }
}
