/* F7 - xresources */

void
setup_xresources(void) {
  XrmDatabase db;
  char *resource_manager;

  // initialize Xresources Manager
  XrmInitialize();
  // get pointer to the display resource manager
  resource_manager = XResourceManagerString(dpy);
  if (!resource_manager) {
    return;
  }

  // get the database
  db = XrmGetStringDatabase(resource_manager);

  // load resources from database
  for (int index = 0; index < resource_inventory_size; index++) {
    xresource_load(db, configurable_resources[index].xrdb_entry, configurable_resources[index].type, configurable_resources[index].target);
  }
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