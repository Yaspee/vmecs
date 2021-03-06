#include <stdio.h>
#include <signal.h>

#include "pub/type.h"

#include "proto/vmess/inbound.h"
#include "proto/vmess/outbound.h"
#include "proto/vmess/vmess.h"
#include "proto/vmess/tcp.h"

#include "proto/relay/tcp.h"

data128_t user_id;

int main()
{
    signal(SIGPIPE, SIG_IGN);

    vmess_config_t *config;
    target_id_t *local, *proxy;
    vmess_tcp_inbound_t *inbound;
    vmess_tcp_outbound_t *outbound;

    tcp_relay_config_t *relay_conf;

    memset(user_id, 0, sizeof(user_id));

    config = vmess_config_new(user_id);
    local = target_id_new_ipv4((byte_t []) { 127, 0, 0, 1 }, 3131);
    proxy = target_id_new_ipv4((byte_t []) { 127, 0, 0, 1 }, 3132);

    relay_conf = tcp_relay_config_new_default();
    inbound = vmess_tcp_inbound_new(config, local);
    outbound = vmess_tcp_outbound_new(config, proxy);

    tcp_relay(relay_conf, (tcp_inbound_t *)inbound, (tcp_outbound_t *)outbound);

    vmess_config_free(config);
    target_id_free(local);
    target_id_free(proxy);
    tcp_relay_config_free(relay_conf);

    tcp_inbound_free(inbound);
    tcp_outbound_free(outbound);

    return 0;
}
