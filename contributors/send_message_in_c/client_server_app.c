/* App which show's how to send p2p messages via baresip.
  Client role is to send messages to the server.
  Server role is to display those messages on stdout.

  Available environment variables:
    CLIENT_IP - host's ip on which client app is being run, ex: 10.0.0.236:8080
    SERVER_IP - host's ip on which server app is being run, ex: 10.0.0.236:8181
    MESSAGE - message's content, ex: hello world
*/
//
/* IMPORTS */
// C Standard Library
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Libre Library
#include <re.h>
// Baresip Library, order is important
#include <baresip.h>

#ifndef CLIENT_IP
#define CLIENT_IP NULL
#endif

#ifndef SERVER_IP
#define SERVER_IP NULL
#endif

#ifndef MESSAGE
#define MESSAGE "hello world"
#endif

//
/* PRIVATE API DECLARATIONS */
static bool is_server(int argc, char *argv[]);
static int configure_server(void);
static int configure_client(void);
static int initialize_server(void);
static int initialize_client(void);
static int create_server_ua(void);
struct ua *create_client_ua();
static void message_recv_handler(struct ua *ua, const struct pl *peer,
                                 const struct pl *ctype, struct mbuf *body,
                                 void *arg);
static void signal_handler(int sig);
static void ua_exit_handler(void *arg);
static void ua_event_handler(struct ua *ua, enum ua_event ev, struct call *call,
                             const char *prm, void *arg);
//
/* PUBLIC API */
int main(int argc, char *argv[]) {
  bool _enable_server;
  int err;

  if (!MESSAGE || !CLIENT_IP || !SERVER_IP) {
    fprintf(stderr, "Required environment variables not filled\n");
  }

  /* Initialize libre */
  err = libre_init();
  if (err)
    goto out;

  /* Load baresip config */
  err = conf_configure();
  if (err) {
    warning("main: configure failed: %m\n", err);
    goto out;
  }

  /* Configure my app  */
  _enable_server = is_server(argc, argv);
  if (_enable_server)
    configure_server();
  else
    configure_client();

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

  /* Initialize my app */
  if (_enable_server)
    initialize_server();
  else
    initialize_client();

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

//
/* PRIVATE API IMPLEMENTATION */
int configure_server(void) {
  struct config *cfg;
  int err;

  cfg = conf_config();

  // SET CUSTOM IP ADDR
  strcpy(cfg->sip.local, SERVER_IP);

  return 0;
}

int initialize_server(void) {
  struct message *all_messages;
  int err;

  err = create_server_ua();
  if (err) {
    perror("Unable to create server's user agent\n");
    return 1;
  }

  all_messages = baresip_message();
  if (!all_messages) {
    perror("Unable to initialize server messages container\n");
    return 2;
  }

  err = message_listen(all_messages, message_recv_handler, NULL);
  if (err) {
    perror("Unable to listen on server messages container\n");
    return 3;
  }

  return 0;
}

int create_server_ua() {
  struct ua *user_agent;
  int err;
  err = ua_alloc(&user_agent, "<sip:user_0@10.0.0.236:8080>;regint=0");

  if (err != 0) {
    return 1;
  }

  return 0;
}

void message_recv_handler(struct ua *ua, const struct pl *peer,
                          const struct pl *ctype, struct mbuf *body,
                          void *arg) {
  re_printf("recv msg from %r: \"%b\"\n", peer, mbuf_buf(body),
            mbuf_get_left(body));
}

int configure_client(void) {
  struct config *cfg;
  int err;

  cfg = conf_config();

  // SET CUSTOM IP ADDR
  strcpy(cfg->sip.local, CLIENT_IP);

  return 0;
}

int initialize_client(void) {
  struct message *all_messages;
  struct ua *user_agent;
  pthread_t thread;
  int err;

  user_agent = create_client_ua();
  if (!user_agent) {
    perror("Unable to create server's user agent\n");
    return 1;
  }

  all_messages = baresip_message();
  if (!all_messages) {
    perror("Unable to initialize server messages container\n");
    return 2;
  }

  err = pthread_create(&thread, NULL, send_messages, user_agent);
  if (err) {
    perror("Unable to send messages in background\n");
    return 3;
  }

  return 0;
}

int send_messages(struct ua *user_agent) {
  while (1) {
    err = message_send(user_agent, "<sip:user_0@10.0.0.236:8080>", MESSAGE,
                       NULL, NULL);

    puts("Message send");

    sleep(1);
  }
}

struct ua *create_client_ua() {
  struct ua *user_agent;
  int err;
  err = ua_alloc(&user_agent, "<sip:user_1@10.0.0.236:8181>;regint=0");

  if (err != 0) {
    perror("Unable to create user agent.");
    return NULL;
  }

  return user_agent;
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

bool is_server(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: \n <exe> (SERVER|CLIENT) \n");
    return false;
  } else if (strcmp(argv[1], "SERVER") == 0)
    return true;
  else if (strcmp(argv[1], "CLIENT") == 0)
    return false;
  else {
    fprintf(stderr, "Usage: \n <exe> (SERVER|CLIENT) \n");
    return false;
  }
}
