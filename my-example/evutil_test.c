// Last Update:2015-11-05 11:33:13

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <evutil.h>

void data_type_test()
{
	printf("%lu\n", sizeof(void));
	printf("%lu\n", sizeof(void *));

	printf("==================8bit==================\n");
	printf(" sizeof(ev_int8_t): %lu,  EV_INT8_MAX: %d, EV_INT8_MIN: %d\n", 
			sizeof(ev_int8_t), EV_INT8_MAX, EV_INT8_MIN);
	printf("sizeof(ev_uint8_t): %lu, EV_UINT8_MAX: %d,         MIN: 0\n", 
			sizeof(ev_uint8_t), EV_UINT8_MAX);

	printf("==================16bit==================\n");
	printf(" sizeof(ev_int16_t): %lu,  EV_INT16_MAX: %d, EV_INT16_MIN: %d\n", 
			sizeof(ev_int16_t), EV_INT16_MAX, EV_INT16_MIN);
	printf("sizeof(ev_uint16_t): %lu, EV_UINT16_MAX: %d,          MIN: 0\n", 
			sizeof(ev_uint16_t), EV_UINT16_MAX);

	printf("==================32bit==================\n");
	printf(" sizeof(ev_int32_t): %lu,  EV_INT32_MAX: %d, EV_INT32_MIN: %d\n", 
			sizeof(ev_int32_t), EV_INT32_MAX, EV_INT32_MIN);
	printf("sizeof(ev_uint32_t): %lu, EV_UINT32_MAX: %u,          MIN: 0\n", 
			sizeof(ev_uint32_t), EV_UINT32_MAX);

	printf("==================64bit==================\n");
	printf(" sizeof(ev_int64_t): %lu,  EV_INT64_MAX: %ld, EV_INT64_MIN: %ld\n", 
			sizeof(ev_int64_t), EV_INT64_MAX, EV_INT64_MIN);
	printf("sizeof(ev_uint64_t): %lu, EV_UINT32_MAX: %lu,          MIN: 0\n", 
			sizeof(ev_uint64_t), EV_UINT64_MAX);

	printf("sizeof(ev_ssize_t): %lu, EV_SSIZE_MAX: %ld, EV_SSIZE_MIN: %ld\n",
			sizeof(ev_ssize_t), EV_SSIZE_MAX, EV_SSIZE_MIN);
	printf("EV_SIZE_MAX: %lu, EV_SIZE_MIN: 0\n", EV_SIZE_MAX);

	printf("sizeof(ev_off_t): %lu\n", sizeof(ev_off_t));
	printf("sizeof(ev_socklen_t): %lu\n", sizeof(ev_socklen_t));
	printf("sizeof(ev_intptr_t): %lu\n", sizeof(ev_intptr_t));
	printf("sizeof(ev_uintptr_t): %lu\n", sizeof(ev_uintptr_t));
}

void timer_util_test()
{
	// #define evutil_timeradd(tvp, uvp, vvp) timeradd((tvp), (uvp), (vvp))
	// #define evutil_timersub(tvp, uvp, vvp) timersub((tvp), (uvp), (vvp))
	struct timeval tva, tvb, res;
	memset(&tva, 0, sizeof(tva));
	memset(&tvb, 0, sizeof(tvb));
	memset(&res, 0, sizeof(res));
	tva.tv_sec = 12; tva.tv_usec = 102;
	tvb.tv_sec = 23; tvb.tv_usec = 234455;
	timeradd(&tva, &tvb, &res);
	printf("%lds, %ldus\n", res.tv_sec, res.tv_usec);
	timersub(&tvb, &tva, &res);
	printf("%lds, %ldus\n", res.tv_sec, res.tv_usec);

	timerclear(&tva);
	timeradd(&tva, &tvb, &res);
	printf("%lds, %ldus\n", res.tv_sec, res.tv_usec);
	printf("timerisset(&tvb): %d\n", timerisset(&tvb));

	printf("tva > tvb ? ==> %d\n", timercmp(&tva, &tvb, >));
	printf("tva < tvb ? ==> %d\n", timercmp(&tva, &tvb, <));
	printf("tva == tvb ? ==> %d\n", timercmp(&tva, &tvb, ==));
	printf("tva != tvb ? ==> %d\n", timercmp(&tva, &tvb, !=));
	printf("tva >= tvb ? ==> %d\n", timercmp(&tva, &tvb, >=));
	printf("tva <= tvb ? ==> %d\n", timercmp(&tva, &tvb, <=));
}

void random_test()
{
	int rval = 0;
	evutil_secure_rng_get_bytes(&rval, sizeof(rval));
	printf("random_data: %d\n", rval);
}

void socket_util_test()
{
	struct sockaddr_in6 sin6;
	char ipv6_str[64] = {0};
	int outlen = sizeof(sin6);
	char *ipstr = "[fe80::20c:29ff:febc:7264]:80";

	evutil_parse_sockaddr_port(ipstr, (struct sockaddr *)&sin6, &outlen);
	printf("outlen: %d\n", outlen);

	snprintf(ipv6_str, sizeof(ipv6_str), "%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x",
			sin6.sin6_addr.s6_addr[0], sin6.sin6_addr.s6_addr[1], sin6.sin6_addr.s6_addr[2], sin6.sin6_addr.s6_addr[3],
			sin6.sin6_addr.s6_addr[4], sin6.sin6_addr.s6_addr[5], sin6.sin6_addr.s6_addr[6], sin6.sin6_addr.s6_addr[7],
			sin6.sin6_addr.s6_addr[8], sin6.sin6_addr.s6_addr[9], sin6.sin6_addr.s6_addr[10], sin6.sin6_addr.s6_addr[11],
			sin6.sin6_addr.s6_addr[12], sin6.sin6_addr.s6_addr[13], sin6.sin6_addr.s6_addr[14], sin6.sin6_addr.s6_addr[15]);
	printf("ipv6 addr: %s\n", ipv6_str);
	char dst[32] = {0};
	evutil_inet_ntop(AF_INET6, &sin6.sin6_addr, dst, sizeof(dst));
	printf("%s\n", dst);
	printf("port: %d\n", ntohs(sin6.sin6_port));
}

int main(int argc, const char *argv[])
{
	//data_type_test();
	//timer_util_test();
	//socket_util_test();
	random_test();

	return 0;
}
