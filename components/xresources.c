/* F7 - xresources */

#define PATCH_COLOR(S,D)  if(S != NULL)\
                                  strncpy(D, S, strlen(S))

static char *res_manager = NULL;

char *
setup_xresources(void) {
  char *resource_manager;

  // get pointer to the display resource manager
  resource_manager = XResourceManagerString(dpy);
  if (!resource_manager) {
    return NULL;
  }

  // return resource manager
  return resource_manager;
}


void
refresh_xresources(void) {
  // Update default colors
  PATCH_COLOR(custom_normbg, normbg);
  PATCH_COLOR(custom_normborder, normborder);
  PATCH_COLOR(custom_normfg, normfg);
  PATCH_COLOR(custom_selbg, selbg);
  PATCH_COLOR(custom_selborder, selborder);
  PATCH_COLOR(custom_selfg, selfg);

  // build color table
  for (int i=0; i < LENGTH(colors); i++) {
    scheme[i] = drw_scm_create(drw, colors[i], alphas[i], 3);
  }
}

void
update_xresources(char *resource_manager) {
  XrmDatabase db;

  // get the database
  db = XrmGetStringDatabase(resource_manager);

  // load resources from database
  for (int index = 0; index < resource_inventory_size; index++) {
    xresource_load(db, configurable_resources[index].xrdb_entry, configurable_resources[index].type, configurable_resources[index].target);
  }

  // refresh
  refresh_xresources();
}

int
xresource_load(XrmDatabase db, char *resource_name, enum xresource_type type, void *target) {
  // build scoped resource name
  // all valid resources start with the "dwm." prefix
  char scoped_resource[256];
  bzero(scoped_resource, sizeof(scoped_resource));
  snprintf(scoped_resource, sizeof(scoped_resource), "%s.%s", "dwm", resource_name);

  // XrmResource placeholder
  XrmValue resource_value;

  // holders for typed returns
  char *res_type;
  char **string_resource_type = target;
  int *integer_resource_type = target;
  float *float_resource_type = target;

  // load resource from the resources database...
  XrmGetResource(db, scoped_resource, scoped_resource, &res_type, &resource_value);
  if (ISNULL(resource_value.addr)) {
    return 1;
  }

  // convert returned value
  switch (type) {
    case STRING:
      *string_resource_type = resource_value.addr;
      break;
    case INTEGER:
      *integer_resource_type = strtoul(resource_value.addr, NULL, 10);
      break;
    case FLOAT:
      *float_resource_type = strtof(resource_value.addr, NULL);
      break;
  }

  return 0;
}

void
xrdbreload(const Arg *arg) {
  // load resources from xrdb
  update_xresources(res_manager);

  // refresh xresources
  refresh_xresources();

  // rearrange
  focus(NULL);
  arrange(NULL);
}
