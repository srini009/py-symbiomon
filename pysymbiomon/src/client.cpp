/*
 * (C) 2018 The University of Chicago
 * 
 * See COPYRIGHT in top-level directory.
 */
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <string>
#include <vector>
#include <utility>
#include <cstring>
#include <iostream>
#include <symbiomon/symbiomon-client.h>
#include <symbiomon/symbiomon-metric.h>

namespace py11 = pybind11;

typedef py11::capsule pymargo_instance_id;
typedef py11::capsule pyhg_addr_t;
typedef py11::capsule pysymbiomon_client_t;
typedef py11::capsule pysymbiomon_metric_handle_t;

#define MID2CAPSULE(__mid)    py11::capsule((void*)(__mid),  "margo_instance_id", nullptr)
#define ADDR2CAPSULE(__addr)  py11::capsule((void*)(__addr), "hg_addr_t", nullptr)
#define SYMBIOMONMH2CAPSULE(__rph) py11::capsule((void*)(__rph),  "symbiomon_metric_handle_t", nullptr)
#define SYMBIOMONCL2CAPSULE(__rcl) py11::capsule((void*)(__rcl),  "symbiomon_client_t", nullptr)

static pysymbiomon_client_t pysymbiomon_client_init(pymargo_instance_id mid) {
    symbiomon_client_t result = SYMBIOMON_CLIENT_NULL;
    symbiomon_client_init(mid, &result);
    return SYMBIOMONCL2CAPSULE(result);
}

static pysymbiomon_metric_handle_t pysymbiomon_remote_metric_handle_create(
        pysymbiomon_client_t client,
        pyhg_addr_t addr,
	symbiomon_metric_id_t id,
        uint8_t provider_id) {

    symbiomon_metric_handle_t metricHandle = SYMBIOMON_METRIC_HANDLE_NULL;
    symbiomon_remote_metric_handle_create(client, addr, provider_id, id, &metricHandle);
    return SYMBIOMONMH2CAPSULE(metricHandle);
}


static std::vector<std::pair <double, double> > pysymbiomon_remote_metric_fetch(
        pysymbiomon_metric_handle_t handle,
        int64_t num_samples_requested) {

    char *name = NULL;
    char *ns = NULL;
    symbiomon_metric_buffer buf;
    int64_t num = num_samples_requested;
    int ret = symbiomon_remote_metric_fetch(handle, &num, &buf, &name, &ns);
    std::vector<std::pair <double, double> > buffer;
    fprintf(stderr, "Requested for %u samples, returned : %u\n", num_samples_requested, num);

    /*if(ret == SYMBIOMON_SUCCESS) {
       for(int i = 0; i < num; i++) {
          buffer.push_back(std::make_pair(buf[i].val, buf[i].time));
          std::cout << "Time: " << buf[i].val << ", Val: " << buf[i].time;
       }
    } else {
       buffer.push_back(std::make_pair(0.0, -1.0));
    }*/
    return buffer;
}       

    
static symbiomon_metric_id_t pysymbiomon_remote_metric_get_id(
        char *ns, 
        char *name, 
        const std::vector<std::string> &taglist,
        uint32_t num_tags) {

    symbiomon_metric_id_t id;
    symbiomon_taglist_t t = (symbiomon_taglist_t)malloc(sizeof(*t));
    t->num_tags = num_tags;
    t->taglist = (char**)malloc(num_tags*sizeof(char*));
    for (auto item = taglist.begin(); item != taglist.end(); item++) {
        t->taglist[item - taglist.begin()] = (char*)malloc(36*sizeof(char));
        strcpy(t->taglist[item - taglist.begin()], (*item).c_str());
        std::cout << (*item).c_str() << " ";
    }
    int ret = symbiomon_remote_metric_get_id(ns, name, t, &id);
    if(ret == SYMBIOMON_SUCCESS) {
       return id;
    } else {
       return 0;
    }
}

PYBIND11_MODULE(_pysymbiomonclient, m)
{
    m.def("client_init", &pysymbiomon_client_init);
    m.def("client_finalize", [](pysymbiomon_client_t clt) {
            int ret;
            ret = symbiomon_client_finalize(clt);
            if(ret == SYMBIOMON_SUCCESS) return;
            throw std::runtime_error(std::string("symbiomon_client_finalize returned ")+std::to_string(ret));});
    m.def("metric_handle_create", &pysymbiomon_remote_metric_handle_create);
    m.def("metric_get_id", &pysymbiomon_remote_metric_get_id);
    m.def("metric_fetch", &pysymbiomon_remote_metric_fetch);
    m.def("metric_handle_ref_incr", [](pysymbiomon_metric_handle_t prmh) {
            int ret;
            ret = symbiomon_remote_metric_handle_ref_incr(prmh);
            if(ret == SYMBIOMON_SUCCESS) return;
            throw std::runtime_error(std::string("symbiomon_remote_metric_handle_ref_incr returned ")+std::to_string(ret));});
    m.def("metric_handle_release", [](pysymbiomon_metric_handle_t prmh) {
            int ret;
            ret = symbiomon_remote_metric_handle_release(prmh);
            if(ret == SYMBIOMON_SUCCESS) return;
            throw std::runtime_error(std::string("symbiomon_remote_metric_handle_release returned ")+std::to_string(ret));});
}
