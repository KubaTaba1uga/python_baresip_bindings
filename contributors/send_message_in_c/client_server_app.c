/* Client role is to send messages to the server.
   Server role is to display those messages on stdout.
*/
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <re.h>

#include <baresip.h>

#define SERVER_IP "10.0.0.236:8080"
#define CLIENT_IP "10.0.0.236:8181"

struct account *create_server_account();
static void message_recv_handler(struct ua *ua, const struct pl *peer,
                                 const struct pl *ctype, struct mbuf *body,
                                 void *arg);
/* APP's LOGIC */
int server_app(void) {

  struct account *server_account;
  struct contacts *all_contacts;
  struct pl pl_addr;
  struct message *all_messages;
  int err;

  server_account = create_server_account();
  if (!server_account)
    return 1;

  err = contact_init(&all_contacts);
  if (err) {
    fprintf(stderr, "Unable to initialize server contacts\n");
    return 2;
  }

  pl_set_str(&pl_addr, "User 1 <sip:user_1@10.0.0.236:8181>");

  err = contact_add(all_contacts, NULL, &pl_addr);
  if (err) {
    fprintf(stderr, "Unable to add server contact\n");
    return 3;
  }

  err = message_init(&all_messages);
  if (err) {
    fprintf(stderr, "Unable to initialize server messages container\n");
    return 4;
  }

  err = message_listen(all_messages, message_recv_handler, NULL);

  sleep(-1);

  return 0;
}
int client_app(void) { struct account *client_account; }

struct account *create_server_account() {
  /* This account is used to receive mesages from client. */
  struct account *account;
  int err;

  err = account_alloc(&account, "<sip:user_0@10.0.0.236:8080>;regint=0");

  if (err != 0) {
    puts("Unable to allocate memory for the account.");
    return NULL;
  }

  printf("\nServer account: %s\n\n", account_aor(account));

  return account;
}

/* struct contact *create_client_contact(struct contacts* contacts){ */
/*   /\* This account is used to receive mesages from client. *\/ */
/*   struct account* account; */
/*   int err; */

/*   err = account_alloc(&account, "<sip:user_0@10.0.0.1:8080>;regint=0"); */

/*   if (err != 0){ */
/*     puts("Unable to allocate memory for the account."); */
/*     return NULL; */
/*   } */

/*   printf("\nServer account: %s\n\n", account_aor(account)); */

/*   return account; */
/* } */

/* Main private declarations */
static void signal_handler(int sig);
static void ua_exit_handler(void *arg);
static void ua_event_handler(struct ua *ua, enum ua_event ev, struct call *call,
                             const char *prm, void *arg);

/* Main */
int main(int argc, char *argv[]) {
  /* Parse user input */
  bool is_server;
  /* printf("%i %i", strlen(argv[1]), strlen("SERVER")); */
  if (argc != 2) {
    fprintf(stderr, "Usage: \n <exe> (SERVER|CLIENT) \n");
    return 1;
  } else if (strcmp(argv[1], "SERVER") == 0)
    is_server = true;
  else if (strcmp(argv[1], "CLIENT") == 0)
    is_server = false;
  else {
    fprintf(stderr, "Usage: \n <exe> (SERVER|CLIENT) \n");
    return 1;
  }

  /* BARESIP BOILERCODE START */
  int err;

  err = libre_init();
  if (err)
    goto out;

  err = conf_configure();
  if (err) {
    warning("main: configure failed: %m\n", err);
    goto out;
  }
  struct config *cfg;
  cfg = conf_config();
  // SET CUSTOM IP ADDR
  strcpy(cfg->sip.local, "0.0.0.0:8080");

  /*
   * Initialise the top-level baresip struct, must be
   * done AFTER configuration is complete.
   */
  err = baresip_init(cfg);
  if (err) {
    warning("main: baresip init failed (%m)\n", err);
    goto out;
  }

  /* Initialise User Agents */
  err = ua_init("baresip v" BARESIP_VERSION " (" ARCH "/" OS ")", true, true,
                true);
  if (err)
    goto out;

  uag_set_exit_handler(ua_exit_handler, NULL);

  uag_event_register(ua_event_handler, NULL);

  /* Load modules */
  err = conf_modules();
  if (err)
    goto out;

  /* BARESIP BOILERCODE END */

  /* APP's LOGIC */
  if (is_server)
    err = server_app();
  else
    err = client_app();

out:
  /* BARESIP BOILERCODE START */
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

  /* BARESIP BOILERCODE END */

  return err;
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

static void message_recv_handler(struct ua *ua, const struct pl *peer,
                                 const struct pl *ctype, struct mbuf *body,
                                 void *arg) {

  info("recv msg from %r: \"%b\"\n", peer, mbuf_buf(body), mbuf_get_left(body));
}
