/* F10 - gaps */

void
updategaps(const Arg *arg) {
  // update gaps settings for the current selected monitor, or reset to default value
  ((arg->i == 0) || ((selmon->gappx + arg->i) < gappx)) ? (selmon->gappx = gappx) : (selmon->gappx += arg->i);

  // arrange windows
  arrange(selmon);
}