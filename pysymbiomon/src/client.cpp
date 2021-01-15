/*
 * (C) 2018 The University of Chicago
 * 
 * See COPYRIGHT in top-level directory.
 */
#include <pybind11/pybind11.h>
#include <string>
#include <vector>
#include <cstring>
#include <iostream>
#include <symbiomon/symbiomon-client.h>
#include <symbiomon/symbiomon-metric.h>

namespace py11 = pybind11;

typedef py11::capsule pymargo_instance_id;
typedef py11::capsule pymargo_addr;
typedef py11::capsule pysymbiomon_client_t;
typedef py11::capsule pysymbiomon_metric_handle_t;
typedef py11::capsule pysymbiomon_taglist_t;
typedef py11::capsule pysymbiomon_metric_id_t;

#define MID2CAPSULE(__mid)    py11::capsule((void*)(__mid),  "margo_instance_id", nullptr)
#define ADDR2CAPSULE(__addr)  py11::capsule((void*)(__addr), "hg_addr_t", nullptr)
#define SYMBIOMONMH2CAPSULE(__rph) py11::capsule((void*)(__rph),  "symbiomon_metric_handle_t", nullptr)
#define SYMBIOMONCL2CAPSULE(__rcl) py11::capsule((void*)(__rcl),  "symbiomon_client_t", nullptr)
#define SYMBIOMONTL2CAPSULE(__rtl) py11::capsule((void*)(__rtl),  "symbiomon_taglist_t", nullptr)
#define SYMBIOMONMI2CAPSULE(__rtl) py11::capsule((void*)(__rmi),  "symbiomon_metric_id_t", nullptr)

static pysymbiomon_client_t pysymbiomon_client_init(pymargo_instance_id mid) {
    symbiomon_client_t result = SYMBIOMON_CLIENT_NULL;
    symbiomon_client_init(mid, &result);
    return SYMBIOMONCL2CAPSULE(result);
}

static pysymbiomon_metric_handle_t pysymbiomon_remote_metric_handle_create(
        pysymbiomon_client_t client,
        pymargo_addr addr,
	pysymbiomon_metric_id_t id,
        uint8_t provider_id) {

    symbiomon_metric_handle_t metricHandle = SYMBIOMON_METRIC_HANDLE_NULL;
    symbiomon_remote_metric_handle_create(client, addr, provider_id, id, &metricHandle);
    return SYMBIOMONMH2CAPSULE(metricHandle);
}


PYBIND11_MODULE(_pysymbiomonclient, m)
{
    m.def("client_init", &pysymbiomon_client_init);
    m.def("client_finalize", [](pysymbiomon_client_t clt) {
            return symbiomon_client_finalize(clt);} );
    m.def("metric_handle_create", &pysymbiomon_metric_handle_create);
    m.def("metric_handle_ref_incr", [](pysymbiomon_metric_handle_t prmh) {
            return symbiomon_remote_metric_handle_ref_incr(prmh); });
    m.def("metric_handle_release", [](pysymbiomon_metric_handle_t prmh) {
            return symbiomon_remote_metric_handle_release(prmh); });
}
