#ifndef MOD_NOROUTE_H_INCLUDED
#define MOD_NOROUTE_H_INCLUDED

int noRoute(
    const char* url,
    struct MHD_Response *response,
    struct MHD_Connection *connection,
    void **con_cls);

#define NOROUTE_BODY "<html><head><title>%s</title></head>\
<body>\
<h1>No route found for %s</h1>\
</body>\
</html>"

#endif