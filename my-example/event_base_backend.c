// Last Update:2015-10-15 13:54:14

#include <stdio.h>

#include <event.h>

int main(int argc, const char *argv[])
{
	int i;
	const char **methods = event_get_supported_methods();
	printf("Starting Libevent: %s. Available methods are: \n",
			event_get_version());
	for (i = 0; methods[i] != NULL; i++)
		printf("%s\n", methods[i]);

	printf("Afetr setting avoid method...\n");
	struct event_config *cfg;
	struct event_base *base;

	cfg = event_config_new();
	event_config_avoid_method(cfg, "select");
	// event_config_require_features(cfg, EV_FEATURE_FDS);
	base = event_base_new_with_config(cfg);
	event_config_free(cfg);

	enum event_method_feature f;
	f = event_base_get_features(base);
	if (f & EV_FEATURE_ET)
		printf("Edge-triggered events are supported.\n");
	if (f & EV_FEATURE_O1)
		printf("O(1) event notification is supported.\n");
	if (f & EV_FEATURE_FDS)
		printf("All FD types are supported.\n");
	printf("\n");

	return 0;
}
