/* F8 - rounded corners */

// determines round window corners mode
unsigned int have_to_round_corners(Client *c) {
  // 2 means round all windows
  if (round_windows == 2) return 1;

  // 0 and window is floating
  if (c->isfloating && round_windows == 0) return 1;

  // 1 and window is fullscreen
  if (c->isfullscreen && round_windows == 1) return 1;

  // any other combination means no rounded corners
  return 0;
}

// use X11 shape extension to draw windows with round corners
void shape_window_round_corners(Client *c) {
  if (!c) return;
  Window win = c->win;
  unsigned long white_pixel = 0, black_pixel = 0;
  unsigned int diameter = 0;
  int event_base, error_base;

  // Get correct white and black pixel values from the current screen
  white_pixel = WhitePixel(dpy, DefaultScreen(dpy));
  black_pixel = BlackPixel(dpy, DefaultScreen(dpy));

  // check whether the SHAPE extension is supported
  if (! XShapeQueryExtension(dpy, &event_base, &error_base)) {
    printf("X SHAPE extension is not supported. Won't render corners on windows.");
    return;
  }

  // do we have to round corners?
  if (!(corner_radius > 0)) {
    return;
  } else {
    diameter = corner_radius << 1;
  }

  // allocate arc and rectangle resources
  XArc window_corners[] = {
    {0, 0, diameter, diameter, 0, 360*64}, // top left
    {c->w - diameter - 1, 0, diameter, diameter, 0, 360*64}, // top right
    {0, c->h - diameter - 1, diameter, diameter, 0, 360*64}, // bottom left
    {c->w - diameter - 1, c->h - diameter - 1, diameter, diameter, 0, 360*64}, // bottom right
  };

  XRectangle mask_rectangles[] = {
    {corner_radius, 0, c->w - diameter, c->h},
    {0, corner_radius, c->w, c->h - diameter},
  };

  // allocate a pixmap for the draw mask
  Pixmap mask = XCreatePixmap(dpy, win, c->w, c->h, 1);
  if (!mask) return;

  // create a graphic context to draw on
  XGCValues valuemask;
  GC graphic_context;
  graphic_context = XCreateGC(dpy, mask, 0, &valuemask);
  // failed to allocate graphic context.
  // clean up mask drawable and return
  if (!graphic_context) {
    XFreePixmap(dpy, mask);
    return;
  }

  // 1- fill mask rectangle
  XSetForeground(dpy, graphic_context, black_pixel);
  XFillRectangle(dpy, mask, graphic_context, 0, 0, c->w, c->h);

  // 2- draw corner arcs
  XSetForeground(dpy, graphic_context, white_pixel);
  XFillArcs(dpy, mask, graphic_context, window_corners, 4);
  XFillRectangles(dpy, mask, graphic_context, mask_rectangles, 2);

  // 3- combine masks
  XShapeCombineMask(dpy, win, ShapeBounding, 0, 0, mask, ShapeSet);

  // free mask drawable
  XFreePixmap(dpy, mask);
  // clear and deallocate graphic context
  XFreeGC(dpy, graphic_context);
}
