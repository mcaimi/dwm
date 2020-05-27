/* F5 - layouts */
void
tile(Monitor *m)
{
  unsigned int i, n, h, mw, my, ty;
  float mfacts = 0, sfacts = 0;
  Client *c;

  for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) {
    if (n < m->nmaster)
      mfacts += c->cfact;
    else
      sfacts += c->cfact;
  }
  if (n == 0)
    return;

  if (n > m->nmaster)
    mw = m->nmaster ? (m->ww) * m->mfact : 0;
  else
    mw = m->ww;

  for (i = my = ty = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
    if (i < m->nmaster) {
      h = (m->wh - my) * (c->cfact / mfacts);
      resize(c, m->wx, m->wy + my, mw - (2*c->bw), h - (2*c->bw), 0);
      if (my + HEIGHT(c) < m->wh)
        my += HEIGHT(c);
      mfacts -= c->cfact;
    } else {
      h = (m->wh - ty) * (c->cfact / sfacts);
      resize(c, m->wx + mw, m->wy + ty, m->ww - mw - (2*c->bw), h - (2*c->bw), False);
      if (ty + HEIGHT(c) < m->wh)
        ty += HEIGHT(c);
      sfacts -= c->cfact;
    }
}

void
monocle(Monitor *m)
{
  unsigned int n = 0;
  Client *c;

  for (c = m->clients; c; c = c->next)
    if (ISVISIBLE(c))
      n++;
  if (n > 0) /* override layout symbol */
    snprintf(m->ltsymbol, sizeof m->ltsymbol, "[%d]", n);
  for (c = nexttiled(m->clients); c; c = nexttiled(c->next))
    resize(c, m->wx, m->wy, m->ww - 2 * c->bw, m->wh - 2 * c->bw, 0);
}

void
bstack(Monitor *m)
{
  unsigned int i, n, w, mh, mx, tx;
  float mfacts = 0, sfacts = 0;
  Client *c;

  for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) {
    if (n < m->nmaster)
      mfacts += c->cfact;
    else
      sfacts += c->cfact;
  }
  if (n == 0)
    return;
  if(n == 1){
    c = nexttiled(m->clients);
    resize(c, m->wx, m->wy, m->ww - 2 * c->bw, m->wh - 2 * c->bw, 0);
    return;
  }

  if (n > m->nmaster)
    mh = m->nmaster ? m->wh * m->mfact : 0;
  else
    mh = m->wh;
  for (i = 0, mx = tx = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
    if (i < m->nmaster) {
      w = (m->ww - mx) * (c->cfact / mfacts);
      resize(c, m->wx + mx, m->wy, w - (2*c->bw), mh - 2*c->bw, 0);
      if(mx + WIDTH(c) < m->mw)
        mx += WIDTH(c);
      mfacts -= c->cfact;
    } else {
      w = (m->ww - tx) * (c->cfact / sfacts);
      resize(c, m->wx + tx, m->wy + mh, w - (2*c->bw), m->wh - mh - 2*(c->bw), 0);
      if(tx + WIDTH(c) < m->mw)
        tx += WIDTH(c);
      sfacts -= c->cfact;
    }
}

void
bstackhoriz(Monitor *m)
{
  unsigned int i, n, h, mw, mh, my, ty;
  float mfacts = 0, sfacts = 0;
  Client *c;

  for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) {
    if (n < m->nmaster)
      mfacts += c->cfact;
    else
      sfacts += c->cfact;
  }
  if (n == 0)
    return;
  if(n == 1){
    c = nexttiled(m->clients);
    resize(c, m->wx, m->wy, m->ww - 2 * c->bw, m->wh - 2 * c->bw, 0);
    return;
  }

  if (n > m->nmaster)
    mh = m->nmaster ? m->wh * m->mfact : 0;
  else
    mh = m->wh;
  mw = m->ww;

  for (i = ty = 0, my = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
    if (i < m->nmaster) {
      h = (mh - my) * (c->cfact / mfacts);
      resize(c, m->wx, m->wy + my, mw - 2*c->bw, h - 2*c->bw, 0);
      if(my + HEIGHT(c) < m->mh)
        my += HEIGHT(c);
      mfacts -= c->cfact;
    } else {
      h = (m->wh - mh - ty) * (c->cfact / sfacts);
      resize(c, m->wx, m->wy + mh + ty, mw - 2*c->bw, h - (2*c->bw), 0);
      if(ty + HEIGHT(c) < m->mh)
        ty += HEIGHT(c);
      sfacts -= c->cfact;
    }
}

void
centeredmaster(Monitor *m)
{
  unsigned int i, n, h, mw, mx, my, oty, ety, tw;
  float mfacts = 0, lfacts = 0, rfacts = 0;
  Client *c;

  /* count number of clients in the selected monitor */
  for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) {
    if (n < m->nmaster)
      mfacts += c->cfact;
    else if ((n - m->nmaster) % 2) 
      lfacts += c->cfact;
    else
      rfacts += c->cfact;
  }
  if (n == 0)
    return;
  if(n == 1){
    c = nexttiled(m->clients);
    resize(c, m->wx, m->wy, m->ww - 2 * c->bw, m->wh - 2 * c->bw, 0);
    return;
  }

  /* initialize areas */
  mw = m->ww;
  mx = 0;
  my = 0;
  tw = mw;

  if (n > m->nmaster) {
    /* go mfact box in the center if more than nmaster clients */
    mw = m->nmaster ? m->ww * m->mfact : 0;
    tw = m->ww - mw;

    if (n - m->nmaster > 1) {
      /* only one client */
      mx = (m->ww - mw) / 2;
      tw = (m->ww - mw) / 2;
    }
  }

  oty = 0;
  ety = 0;
  for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
  if (i < m->nmaster) {
    /* nmaster clients are stacked vertically, in the center
     * of the screen */
    h = (m->wh - my) * (c->cfact / mfacts);
    resize(c, m->wx + mx, m->wy + my, mw - 2*c->bw,
          h - 2*c->bw, 0);
    if(my + HEIGHT(c) < m->mh)
      my += HEIGHT(c);
    mfacts -= c->cfact;
  } else {
    /* stack clients are stacked vertically */
    if ((i - m->nmaster) % 2) {
      h = (m->wh - ety) * (c->cfact / lfacts);
      if(m->nmaster == 0)
        resize(c, m->wx, m->wy + ety, tw - 2*c->bw,
             h - 2*c->bw, 0);
      else
        resize(c, m->wx, m->wy + ety, tw - 2*c->bw,
             h - 2*c->bw, 0);
      if(ety + HEIGHT(c) < m->mh)
        ety += HEIGHT(c);
      lfacts -= c->cfact;
    } else {
      h = (m->wh - oty) * (c->cfact / rfacts);
      resize(c, m->wx + mx + mw, m->wy + oty,
             tw - 2*c->bw, h - 2*c->bw, 0);
      if(oty + HEIGHT(c) < m->mh)
        oty += HEIGHT(c);
      rfacts -= c->cfact;
    }
  }
}

void
centeredfloatingmaster(Monitor *m)
{
  unsigned int i, n, w, mh, mw, mx, mxo, my, myo, tx;
  float mfacts = 0, sfacts = 0;
  Client *c;

  /* count number of clients in the selected monitor */
  for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) {
    if (n < m->nmaster)
      mfacts += c->cfact;
    else
      sfacts += c->cfact;
  }
  if (n == 0)
    return;
  if(n == 1){
    c = nexttiled(m->clients);
    resize(c, m->wx, m->wy, m->ww - 2 * c->bw, m->wh - 2 * c->bw, 0);
    return;
  }

  /* initialize nmaster area */
  if (n > m->nmaster) {
    /* go mfact box in the center if more than nmaster clients */
    if (m->ww > m->wh) {
      mw = m->nmaster ? m->ww * m->mfact : 0;
      mh = m->nmaster ? m->wh * 0.9 : 0;
    } else {
      mh = m->nmaster ? m->wh * m->mfact : 0;
      mw = m->nmaster ? m->ww * 0.9 : 0;
    }
    mx = mxo = (m->ww - mw) / 2;
    my = myo = (m->wh - mh) / 2;
  } else {
    /* go fullscreen if all clients are in the master area */
    mh = m->wh;
    mw = m->ww;
    mx = mxo = 0;
    my = myo = 0;
  }

  for(i = tx = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
  if (i < m->nmaster) {
    /* nmaster clients are stacked horizontally, in the center
     * of the screen */
    w = (mw + mxo - mx) * (c->cfact / mfacts);
    resize(c, m->wx + mx, m->wy + my, w - 2*c->bw,
           mh - 2*c->bw, 0);
    if(mx + WIDTH(c) < m->mw)
      mx += WIDTH(c);
    mfacts -= c->cfact; 
  } else {
    /* stack clients are stacked horizontally */
    w = (m->ww - tx) * (c->cfact / sfacts);
    resize(c, m->wx + tx, m->wy, w - 2*c->bw,
           m->wh - 2*c->bw, 0);
    if(tx + WIDTH(c) < m->mw)
      tx += WIDTH(c);
    sfacts -= c->cfact; 
  }
}

static void
fibonacci(Monitor *m, int s)
{
  unsigned int i, n;
  int nx, ny, nw, nh;
  Client *c;

  for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
  if (n == 0)
    return;
  if (n == 1) {
    c = nexttiled(m->clients);
    resize(c, m->wx, m->wy, m->ww - 2 * c->bw, m->wh - 2 * c->bw, 0);
    return;
  }

  nx = m->wx + m->gappx;
  ny = m->gappx;
  nw = m->ww - 2*m->gappx;
  nh = m->wh - 2*m->gappx;

  for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next)) {
    if ((i % 2 && nh / 2 > 2*c->bw)
       || (!(i % 2) && nw / 2 > 2*c->bw)) {
      if (i < n - 1) {
        if (i % 2)
          nh = (nh - m->gappx) / 2;
        else
          nw = (nw - m->gappx) / 2;

        if ((i % 4) == 2 && !s)
          nx += nw + m->gappx;
        else if ((i % 4) == 3 && !s)
          ny += nh + m->gappx;
      }
      if ((i % 4) == 0) {
        if (s)
          ny += nh + m->gappx;
        else
          ny -= nh + m->gappx;
      }
      else if ((i % 4) == 1)
        nx += nw + m->gappx;
      else if ((i % 4) == 2)
        ny += nh + m->gappx;
      else if ((i % 4) == 3) {
        if (s)
          nx += nw + m->gappx;
        else
          nx -= nw + m->gappx;
      }
      if (i == 0) {
        if (n != 1)
          nw = (m->ww - 2*m->gappx - m->gappx) * m->mfact;
        ny = m->wy + m->gappx;
      }
      else if (i == 1)
        nw = m->ww - nw - m->gappx - 2*m->gappx;
      i++;
    }

    resize(c, nx, ny, nw - (2*c->bw), nh - (2*c->bw), False);
  }
}

void
dwindle(Monitor *mon) {
  fibonacci(mon, 1);
}

void
spiral(Monitor *mon) {
  fibonacci(mon, 0);
}

void
cyclelayout(const Arg *arg) {
  Layout *lt;

  // find current layout
  for (lt = (Layout *)layouts; lt != selmon->lt[selmon->sellt]; lt++);

  // switch to the next layout
  if (lt->symbol && (lt + 1)->symbol) {
    setlayout( &((Arg){ .v = (lt + 1)}) );
  } else {
    setlayout( &((Arg){ .v = layouts}) );
  }
}

void
setlayout(const Arg *arg)
{
  if (!arg || !arg->v || arg->v != selmon->lt[selmon->sellt])
    selmon->sellt ^= 1;
  if (arg && arg->v)
    selmon->lt[selmon->sellt] = (Layout *)arg->v;
  strncpy(selmon->ltsymbol, selmon->lt[selmon->sellt]->symbol, sizeof selmon->ltsymbol);
  if (selmon->sel)
    arrange(selmon);
  else
    drawbar(selmon);
}

