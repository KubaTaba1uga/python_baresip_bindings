
#include <baresip.h>
#include <re.h>
#include <stdint.h>
#include <stdlib.h>

static int configure_server(void);
static int initialize_server(void);
static int create_server_ua(void);
static void signal_handler(int sig);
static void ua_exit_handler(void *arg);
static void ua_event_handler(struct ua *ua, enum ua_event ev, struct call *call,
                             const char *prm, void *arg);

int main(int argc, char *argv[]) {
  int err;

  err = libre_init();
  if (err)
    goto out;

  err = conf_configure();
  if (err) {
    warning("main: configure failed: %m\n", err);
    goto out;
  }

  // MY APP START
  configure_server();
  // MY APP END

  /*
   * Initialise the top-level baresip struct, must be
   * done AFTER configuration is complete.
   */
  err = baresip_init(conf_config());
  if (err) {
    warning("main: baresip init failed (%m)\n", err);
    goto out;
  }

  /* Initialise User Agents */
  err = ua_init("baresip v" BARESIP_VERSION " (" ARCH "/" OS ")", true, true,
                true);
  if (err)
    goto out;

  // MY APP START
  initialize_server(); // this needs to be after `ua_init`
  // MY APP END

  uag_set_exit_handler(ua_exit_handler, NULL);

  uag_event_register(ua_event_handler, NULL);

  /* Load modules */
  err = conf_modules();
  if (err)
    goto out;

  /* Main loop */
  err = re_main(signal_handler);

out:
  if (err)
    ua_stop_all(true);

  ua_close();
  module_app_unload();
  conf_close();

  baresip_close();

  /* NOTE: modules must be unloaded after all application
   *       activity has stopped.
   */
  debug("main: unloading modules..\n");
  mod_close();

  re_thread_async_close();

  /* Check for open timers */
  tmr_debug();

  libre_close();

  /* Check for memory leaks */
  mem_debug();

  return err;
}

int configure_server(void) {
  struct config *cfg;
  int err;

  cfg = conf_config();

  // SET CUSTOM IP ADDR
  strcpy(cfg->sip.local, "0.0.0.0:8080");
}

int initialize_server(void) { create_server_ua(); }

int create_server_ua() {
  struct ua *user_agent;
  int err;
  err = ua_alloc(&user_agent, "<sip:user_0@10.0.0.236:8080>;regint=0");

  if (err != 0) {
    puts("Unable to create user agent.");
    return 1;
  }

  return 0;
}

static void signal_handler(int sig) {
  static bool term = false;

  if (term) {
    mod_close();
    exit(0);
  }

  term = true;

  info("terminated by signal %d\n", sig);

  ua_stop_all(false);
}

static void ua_exit_handler(void *arg) {
  (void)arg;
  debug("ua exited -- stopping main runloop\n");

  /* The main run-loop can be stopped now */
  re_cancel();
}

static void ua_event_handler(struct ua *ua, enum ua_event ev, struct call *call,
                             const char *prm, void *arg) {
  re_printf("ua event: %s\n", uag_event_str(ev));
}
