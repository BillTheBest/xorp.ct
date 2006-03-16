// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-
// vim:set sts=4 ts=8: 

// Copyright (c) 2001-2005 International Computer Science Institute
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software")
// to deal in the Software without restriction, subject to the conditions
// listed in the XORP LICENSE file. These conditions include: you must
// preserve this copyright notice, and you cannot mention the copyright
// holders in advertising related to the Software without their permission.
// The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
// notice is a summary of the XORP LICENSE file; the license in that file is
// legally binding.

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "libxorp/xorp.h"

// #define DEBUG_LOGGING

#include "libxorp/debug.h"
#include "libxorp/status_codes.h"
#include "libxipc/xrl_router.hh"

#include "constants.hh"
#include "system.hh"
#include "xrl_process_spy.hh"
#include "xrl_port_manager.hh"
#include "xrl_redist_manager.hh"
#include "xrl_target_ripng.hh"
#include "xrl_target_common.hh"

XrlRipngTarget::XrlRipngTarget(EventLoop&		el,
			       XrlRouter&		xr,
			       XrlProcessSpy&		xps,
			       XrlPortManager<IPv6>& 	xpm,
			       XrlRedistManager<IPv6>&	xrm,
			       bool&			should_exit,
			       System<IPv6>&		rip_system)
    : XrlRipngTargetBase(&xr), _e(el)
{
    _ct = new XrlRipCommonTarget<IPv6>(xps, xpm, xrm, should_exit, rip_system);
}

XrlRipngTarget::~XrlRipngTarget()
{
    delete _ct;
}

XrlCmdError
XrlRipngTarget::common_0_1_get_target_name(string& n)
{
    n = name();
    return XrlCmdError::OKAY();
}

XrlCmdError
XrlRipngTarget::common_0_1_get_version(string& v)
{
    v = string(version());
    return XrlCmdError::OKAY();
}

void
XrlRipngTarget::set_status(ProcessStatus status, const string& note)
{
    _ct->set_status(status, note);
}

XrlCmdError
XrlRipngTarget::common_0_1_get_status(uint32_t& status, string& reason)
{
    return _ct->common_0_1_get_status(status, reason);
}

XrlCmdError
XrlRipngTarget::common_0_1_shutdown()
{
    return _ct->common_0_1_shutdown();
}

XrlCmdError
XrlRipngTarget::finder_event_observer_0_1_xrl_target_birth(const string& cname,
							   const string& iname)
{
    return _ct->finder_event_observer_0_1_xrl_target_birth(cname, iname);
}

XrlCmdError
XrlRipngTarget::finder_event_observer_0_1_xrl_target_death(const string& cname,
							   const string& iname)
{
    return _ct->finder_event_observer_0_1_xrl_target_death(cname, iname);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_add_rip_address(const string& ifn,
					  const string& vifn,
					  const IPv6&   addr)
{
    return _ct->ripx_0_1_add_rip_address(ifn, vifn, addr);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_remove_rip_address(const string& ifn,
					     const string& vifn,
					     const IPv6&   addr)
{
    return _ct->ripx_0_1_remove_rip_address(ifn, vifn, addr);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_set_rip_address_enabled(const string&	ifn,
						  const string&	vifn,
						  const IPv6&	a,
						  const bool&	en)
{
    return _ct->ripx_0_1_set_rip_address_enabled(ifn, vifn, a, en);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_rip_address_enabled(const string&	ifn,
					      const string&	vifn,
					      const IPv6&	a,
					      bool&		en)
{
    return _ct->ripx_0_1_rip_address_enabled(ifn, vifn, a, en);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_set_cost(const string&	ifn,
				   const string&	vifn,
				   const IPv6&		a,
				   const uint32_t&	cost)
{
    return _ct->ripx_0_1_set_cost(ifn, vifn, a, cost);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_cost(const string&	ifn,
			       const string&	vifn,
			       const IPv6&	a,
			       uint32_t&	cost)
{
    return _ct->ripx_0_1_cost(ifn, vifn, a, cost);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_set_horizon(const string&	ifn,
				      const string&	vifn,
				      const IPv6&	a,
				      const string&	horizon)
{
    return _ct->ripx_0_1_set_horizon(ifn, vifn, a, horizon);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_horizon(const string&	ifn,
				  const string&	vifn,
				  const IPv6&	a,
				  string&	horizon)
{
    return _ct->ripx_0_1_horizon(ifn, vifn, a, horizon);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_set_passive(const string&	ifn,
				      const string&	vifn,
				      const IPv6&	a,
				      const bool&	passive)
{
    return _ct->ripx_0_1_set_passive(ifn, vifn, a, passive);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_passive(const string&	ifn,
				  const string&	vifn,
				  const IPv6&	a,
				  bool&		passive)
{
    return _ct->ripx_0_1_passive(ifn, vifn, a, passive);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_set_accept_non_rip_requests(const string&	ifn,
						      const string&	vifn,
						      const IPv6&	addr,
						      const bool&	accept)
{
    return _ct->ripx_0_1_set_accept_non_rip_requests(ifn, vifn, addr, accept);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_accept_non_rip_requests(const string&	ifn,
						  const string&	vifn,
						  const IPv6&	addr,
						  bool&		accept)
{
    return _ct->ripx_0_1_accept_non_rip_requests(ifn, vifn, addr, accept);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_set_accept_default_route(const string&	ifn,
						   const string&	vifn,
						   const IPv6&		addr,
						   const bool&		accept)
{
    return _ct->ripx_0_1_set_accept_default_route(ifn, vifn, addr, accept);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_accept_default_route(const string&	ifn,
					     const string&	vifn,
					     const IPv6&	addr,
					     bool&		accept)
{
    return _ct->ripx_0_1_accept_default_route(ifn, vifn, addr, accept);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_set_advertise_default_route(const string&	ifn,
						      const string&	vifn,
						      const IPv6&	addr,
						      const bool&	adv)
{
    return _ct->ripx_0_1_set_advertise_default_route(ifn, vifn, addr, adv);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_advertise_default_route(const string&	ifn,
						  const string&	vifn,
						  const IPv6&	addr,
						  bool&		adv)
{
    return _ct->ripx_0_1_advertise_default_route(ifn, vifn, addr, adv);
}


XrlCmdError
XrlRipngTarget::ripng_0_1_set_route_expiry_seconds(
						const string&	ifn,
						const string&	vifn,
						const IPv6&	a,
						const uint32_t&	t
						)
{
    return _ct->ripx_0_1_set_route_expiry_seconds(ifn, vifn, a, t);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_route_expiry_seconds(const string&	ifn,
					       const string&	vifn,
					       const IPv6&	a,
					       uint32_t&	t)
{
    return _ct->ripx_0_1_route_expiry_seconds(ifn, vifn, a, t);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_set_route_deletion_seconds(const string&	ifn,
						     const string&	vifn,
						     const IPv6&	a,
						     const uint32_t&	t)
{
    return _ct->ripx_0_1_set_route_deletion_seconds(ifn, vifn, a, t);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_route_deletion_seconds(const string&	ifn,
						 const string&	vifn,
						 const IPv6&	a,
						 uint32_t&	t)
{
    return _ct->ripx_0_1_route_deletion_seconds(ifn, vifn, a, t);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_set_table_request_seconds(const string&	ifn,
						    const string&	vifn,
						    const IPv6&		a,
						    const uint32_t&	t)
{
    return _ct->ripx_0_1_set_table_request_seconds(ifn, vifn, a, t);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_table_request_seconds(const string&	ifn,
						const string&	vifn,
						const IPv6&	a,
						uint32_t&	t)
{
    return _ct->ripx_0_1_table_request_seconds(ifn, vifn, a, t);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_set_unsolicited_response_min_seconds(
						const string&	ifn,
						const string&	vifn,
						const IPv6&	a,
						const uint32_t& t
						)
{
    return _ct->ripx_0_1_set_unsolicited_response_min_seconds(ifn, vifn, a, t);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_unsolicited_response_min_seconds(
						const string&	ifn,
						const string&	vifn,
						const IPv6&	a,
						uint32_t&	t
						)
{
    return _ct->ripx_0_1_unsolicited_response_min_seconds(ifn, vifn, a, t);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_set_unsolicited_response_max_seconds(
						const string&	ifn,
						const string&	vifn,
						const IPv6&	a,
						const uint32_t& t
						)
{
    return _ct->ripx_0_1_set_unsolicited_response_max_seconds(ifn, vifn, a, t);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_unsolicited_response_max_seconds(
						const string&	ifn,
						const string&	vifn,
						const IPv6&	a,
						uint32_t&	t
						)
{
    return _ct->ripx_0_1_unsolicited_response_max_seconds(ifn, vifn, a, t);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_set_triggered_update_min_seconds(
						const string&	ifn,
						const string&	vifn,
						const IPv6&	a,
						const uint32_t&	t
						)
{
    return _ct->ripx_0_1_set_triggered_update_min_seconds(ifn, vifn, a, t);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_triggered_update_min_seconds(
						const string&	ifn,
						const string&	vifn,
						const IPv6&	a,
						uint32_t&	t
						)
{
    return _ct->ripx_0_1_triggered_update_min_seconds(ifn, vifn, a, t);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_set_triggered_update_max_seconds(
						const string&	ifn,
						const string&	vifn,
						const IPv6&	a,
						const uint32_t&	t
						)
{
    return _ct->ripx_0_1_set_triggered_update_max_seconds(ifn, vifn, a, t);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_triggered_update_max_seconds(
						const string&	ifn,
						const string&	vifn,
						const IPv6&	a,
						uint32_t&	t
						)
{
    return _ct->ripx_0_1_triggered_update_max_seconds(ifn, vifn, a, t);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_set_interpacket_delay_milliseconds(
						const string&	ifn,
						const string&	vifn,
						const IPv6&	a,
						const uint32_t&	t
						)
{
    return _ct->ripx_0_1_set_interpacket_delay_milliseconds(ifn, vifn, a, t);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_interpacket_delay_milliseconds(
						const string&	ifn,
						const string&	vifn,
						const IPv6&	a,
						uint32_t&	t
						)
{
    return _ct->ripx_0_1_set_interpacket_delay_milliseconds(ifn, vifn, a, t);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_rip_address_status(const string&	ifn,
					     const string&	vifn,
					     const IPv6&	a,
					     string&		status)
{
    return _ct->ripx_0_1_rip_address_status(ifn, vifn, a, status);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_get_all_addresses(XrlAtomList&	ifnames,
					    XrlAtomList&	vifnames,
					    XrlAtomList&	addrs)
{
    return _ct->ripx_0_1_get_all_addresses(ifnames, vifnames, addrs);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_get_peers(const string&	ifn,
				    const string&	vifn,
				    const IPv6&		a,
				    XrlAtomList&	peers)
{
    return _ct->ripx_0_1_get_peers(ifn, vifn, a, peers);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_get_all_peers(XrlAtomList&	peers,
					XrlAtomList&	ifnames,
					XrlAtomList&	vifnames,
					XrlAtomList&	addrs)
{
    return _ct->ripx_0_1_get_all_peers(peers, ifnames, vifnames, addrs);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_get_counters(const string&	ifname,
				       const string&	vifname,
				       const IPv6&	addr,
				       XrlAtomList&	descs,
				       XrlAtomList&	values)
{
    return _ct->ripx_0_1_get_counters(ifname, vifname, addr, descs, values);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_get_peer_counters(const string&	ifn,
					    const string&	vifn,
					    const IPv6&		addr,
					    const IPv6&		peer,
					    XrlAtomList&	descs,
					    XrlAtomList&	vals,
					    uint32_t&		last_active)
{
    return _ct->ripx_0_1_get_peer_counters(ifn, vifn, addr, peer,
					   descs, vals, last_active);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_redist_protocol_routes(const string&	 protocol,
						 const uint32_t& cost,
						 const uint32_t& tag)
{
    return _ct->ripx_0_1_redist_protocol_routes(protocol, cost, tag);
}

XrlCmdError
XrlRipngTarget::ripng_0_1_no_redist_protocol_routes(const string& protocol)
{
    return _ct->ripx_0_1_no_redist_protocol_routes(protocol);
}

XrlCmdError
XrlRipngTarget::redist6_0_1_add_route(const IPv6Net&	net,
				      const IPv6&	nexthop,
				      const string&	ifname,
				      const string&	vifname,
				      const uint32_t&	metric,
				      const uint32_t&	admin_distance,
				      const string&	cookie,
				      const string&	protocol_origin)
{
    return _ct->redistx_0_1_add_route(net, nexthop, ifname, vifname, metric,
				      admin_distance, cookie, protocol_origin);
}

XrlCmdError
XrlRipngTarget::redist6_0_1_delete_route(const IPv6Net&	net,
					 const string&	cookie,
					 const string&	protocol_origin)
{
    return _ct->redistx_0_1_delete_route(net, cookie, protocol_origin);
}

XrlCmdError
XrlRipngTarget::redist6_0_1_starting_route_dump(const string& /* cookie */)
{
    return XrlCmdError::OKAY();
}

XrlCmdError
XrlRipngTarget::redist6_0_1_finishing_route_dump(const string& /* cookie */)
{
    return XrlCmdError::OKAY();
}


XrlCmdError
XrlRipngTarget::socket6_user_0_1_recv_event(
					const string&		sockid,
					const IPv6&		src_host,
					const uint32_t&		src_port,
					const vector<uint8_t>&	pdata
					)
{
    return _ct->socketx_user_0_1_recv_event(sockid, src_host, src_port, pdata);
}

XrlCmdError
XrlRipngTarget::socket6_user_0_1_connect_event(const string&	sockid,
					     const IPv6&	src_host,
					     const uint32_t&	src_port,
					     const string&	new_sockid,
					     bool&		accept)
{
    return _ct->socketx_user_0_1_connect_event(sockid,
					       src_host,
					       src_port,
					       new_sockid,
					       accept);
}

XrlCmdError
XrlRipngTarget::socket6_user_0_1_error_event(const string&	sockid,
					     const string& 	reason,
					     const bool&	fatal)
{
    return _ct->socketx_user_0_1_error_event(sockid, reason, fatal);
}

XrlCmdError
XrlRipngTarget::socket6_user_0_1_close_event(const string&	sockid,
					     const string&	reason)
{
    return _ct->socketx_user_0_1_close_event(sockid, reason);
}

XrlCmdError
XrlRipngTarget::policy_backend_0_1_configure(const uint32_t& filter,
                                           const string& conf)
{
    return _ct->policy_backend_0_1_configure(filter, conf);
}

XrlCmdError
XrlRipngTarget::policy_backend_0_1_reset(const uint32_t& filter)
{
    return _ct->policy_backend_0_1_reset(filter);
}

XrlCmdError
XrlRipngTarget::policy_backend_0_1_push_routes()
{
    return _ct->policy_backend_0_1_push_routes();
}

XrlCmdError 
XrlRipngTarget::policy_redist6_0_1_add_route6(const IPv6Net&	    network,
					      const bool&	    unicast,
					      const bool&	    multicast,
				              const IPv6&	    nexthop,
				              const uint32_t&	    metric,
				              const XrlAtomList&    policytags)
{
    return _ct->policy_redistx_0_1_add_routex(network, unicast, multicast,
					      nexthop, metric, policytags);
}

XrlCmdError 
XrlRipngTarget::policy_redist6_0_1_delete_route6(const IPv6Net&  network,
						 const bool&     unicast,
					         const bool&     multicast)
{
    return _ct->policy_redistx_0_1_delete_routex(network, unicast, multicast);
}