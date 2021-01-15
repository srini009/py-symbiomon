# (C) 2018 The University of Chicago
# See COPYRIGHT in top-level directory.
import _pysymbiomonclient
import pymargo

class SymbiomonClient():
    """
    The SymbiomonClient class wraps a symbiomon_client_t structure at C level.
    It registers the RPCs necessary to interact with a Symbiomon metric.
    It can be used to create metric handles pointing to Symbiomon metrics.
    """

    def __init__(self, mid):
        """
        Constructor. Initializes a new SymbiomonClient with a MargoInstance.

        Args:
            mid (MargoInstance): MargoInstance on which to register RPCs.
        """
        self._client = _pysymbiomonclient.client_init(mid._mid)

    def create_metric_handle(self, addr, metric_id, provider_id):
        """
        Creates a SymbiomonMetricHandle object pointing to the given
        address and metric id.

        Args:
            addr (MargoAddress): Address of the Symbiomon metric.
            metric_id (int): ID of the metric.
	    provider_id(int): ID of the provider
        """
        ph = _pysymbiomonclient.metric_handle_create(self._client, addr.get_hg_addr(), metric_id, provider_id)
        return SymbiomonMetricHandle(ph)

    def finalize(self):
        """

        Finalizes the underlying symbiomon_client_t structure.
        """
        _pysymbiomonclient.client_finalize(self._client)

class SymbiomonMetricHandle():
    """
    The SymbiomonMetricHandle class represents a handle to a remote Symbiomon metric.
    Internally, this class wraps a symbiomon_metric_handle_t C structure.
    """

    def __init__(self, ph):
        """
        Constructor. This is not supposed to be called by users.
        Users should create a SymbiomonMetricHandle from a SymbiomonClient clt
        by calling clt.create_metric_handle.
        """
        self._ph = ph

    def ref_incr(self):
        """
        Call metric_handle_ref_incr on the underlying
        symbiomon_metric_handle_t C structure.
        """
        _pysymbiomonclient.metric_handle_ref_incr(self._ph)

	
    def __del__(self):
        """
        Explicit destructor to call metric_handle_release on the underlying
        symbiomon_metric_handle_t C structure.
        """
        _pysymbiomonclient.metric_handle_release(self._ph)

