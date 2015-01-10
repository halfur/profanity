#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>

#include "chat_session.h"
#include "command/commands.h"
#include "xmpp/xmpp.h"
#include "roster_list.h"

#include "ui/stub_ui.h"

void clears_chat_sessions(void **state)
{
    CommandHelp *help = malloc(sizeof(CommandHelp));

    chat_sessions_init();
    roster_init();
    chat_session_on_recipient_activity("bob@server.org", "laptop");
    chat_session_on_recipient_activity("mike@server.org", "work");

    will_return(jabber_get_connection_status, JABBER_CONNECTED);
    will_return(jabber_get_fulljid, "myjid@myserver.com");
    expect_any_cons_show();

    gboolean result = cmd_disconnect(NULL, *help);

    assert_true(result);

    ChatSession *session1 = chat_session_get("bob@server.org");
    ChatSession *session2 = chat_session_get("mike@server.org");
    assert_null(session1);
    assert_null(session2);
    free(help);
}