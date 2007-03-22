// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-

// Copyright (c) 2001-2007 International Computer Science Institute
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

// $XORP: xorp/fea/fticonfig_entry_set.hh,v 1.23 2006/06/29 11:03:54 bms Exp $

#ifndef __FEA_FTICONFIG_ENTRY_SET_HH__
#define __FEA_FTICONFIG_ENTRY_SET_HH__


#include "libxorp/xorp.h"
#include "libxorp/ipvx.hh"
#include "libxorp/time_slice.hh"
#include "libxorp/timer.hh"

#include "fte.hh"
#include "nexthop_port_mapper.hh"
#include "click_socket.hh"
#include "netlink_socket.hh"
#include "routing_socket.hh"
#include "win_rtm_pipe.hh"

class FtiConfig;

class FtiConfigEntrySet {
public:
    FtiConfigEntrySet(FtiConfig& ftic);
    
    virtual ~FtiConfigEntrySet();
    
    FtiConfig&	ftic() { return _ftic; }
    
    virtual void register_ftic_primary();
    virtual void register_ftic_secondary();
    virtual void set_primary() { _is_primary = true; }
    virtual void set_secondary() { _is_primary = false; }
    virtual bool is_primary() const { return _is_primary; }
    virtual bool is_secondary() const { return !_is_primary; }
    virtual bool is_running() const { return _is_running; }

    /**
     * Start operation.
     * 
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    virtual int start(string& error_msg) = 0;
    
    /**
     * Stop operation.
     *
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    virtual int stop(string& error_msg) = 0;
    
    /**
     * Start a configuration interval. All modifications to FtiConfig
     * state must be within a marked "configuration" interval.
     *
     * This method provides derived classes with a mechanism to perform
     * any actions necessary before forwarding table modifications can
     * be made.
     *
     * @param error_msg the error message (if error).
     * @return true if configuration successfully started.
     */
    virtual bool start_configuration(string& error_msg) {
	// Nothing particular to do, just label start.
	return mark_configuration_start(error_msg);
    }

    /**
     * End of configuration interval.
     *
     * This method provides derived classes with a mechanism to
     * perform any actions necessary at the end of a configuration, eg
     * write a file.
     *
     * @param error_msg the error message (if error).
     * @return true configuration success pushed down into forwarding table.
     */
    virtual bool end_configuration(string& error_msg) {
	// Nothing particular to do, just label start.
	return mark_configuration_end(error_msg);
    }
    
    /**
     * Add a single routing entry.  Must be within a configuration
     * interval.
     *
     * @param fte the entry to add.
     *
     * @return true on success, otherwise false.
     */
    virtual bool add_entry4(const Fte4& fte) = 0;

    /**
     * Delete a single routing entry. Must be with a configuration interval.
     *
     * @param fte the entry to delete. Only destination and netmask are used.
     *
     * @return true on success, otherwise false.
     */
    virtual bool delete_entry4(const Fte4& fte) = 0;

    /**
     * Add a single routing entry. Must be within a configuration
     * interval.
     *
     * @param fte the entry to add.
     *
     * @return true on success, otherwise false.
     */
    virtual bool add_entry6(const Fte6& fte) = 0;

    /**
     * Delete a single routing entry.  Must be within a configuration
     * interval.
     *
     * @param fte the entry to delete. Only destination and netmask are used.
     *
     * @return true on success, otherwise false.
     */
    virtual bool delete_entry6(const Fte6& fte) = 0;

protected:
    /**
     * Mark start of a configuration.
     *
     * @param error_msg the error message (if error).
     * @return true if configuration can be marked as started, false otherwise.
     */
    inline bool mark_configuration_start(string& error_msg) {
	if (false == _in_configuration) {
	    _in_configuration = true;
	    return true;
	}
	error_msg = c_format("Cannot start configuration: "
			     "configuration in progress");
	return false;
    }

    /**
     * Mark end of a configuration.
     *
     * @param error_msg the error message (if error).
     * @return true if configuration can be marked as ended, false otherwise.
     */
    inline bool mark_configuration_end(string& error_msg) {
	if (true == _in_configuration) {
	    _in_configuration = false;
	    return true;
	}
	error_msg = c_format("Cannot end configuration: "
			     "configuration not in progress");
	return false;
    }
    
    inline bool in_configuration() const { return _in_configuration; }

    // Misc other state
    bool	_is_running;

private:
    FtiConfig&	_ftic;
    bool	_in_configuration;
    bool	_is_primary;	// True -> primary, false -> secondary method
};

class FtiConfigEntrySetDummy : public FtiConfigEntrySet {
public:
    FtiConfigEntrySetDummy(FtiConfig& ftic);
    virtual ~FtiConfigEntrySetDummy();

    /**
     * Start operation.
     *
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    virtual int start(string& error_msg);
    
    /**
     * Stop operation.
     *
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    virtual int stop(string& error_msg);

    /**
     * Add a single routing entry.  Must be within a configuration
     * interval.
     *
     * @param fte the entry to add.
     *
     * @return true on success, otherwise false.
     */
    virtual bool add_entry4(const Fte4& fte);

    /**
     * Delete a single routing entry. Must be with a configuration interval.
     *
     * @param fte the entry to delete. Only destination and netmask are used.
     *
     * @return true on success, otherwise false.
     */
    virtual bool delete_entry4(const Fte4& fte);

    /**
     * Add a single routing entry. Must be within a configuration
     * interval.
     *
     * @param fte the entry to add.
     *
     * @return true on success, otherwise false.
     */
    virtual bool add_entry6(const Fte6& fte);

    /**
     * Delete a single routing entry.  Must be within a configuration
     * interval.
     *
     * @param fte the entry to delete. Only destination and netmask are used.
     *
     * @return true on success, otherwise false.
     */
    virtual bool delete_entry6(const Fte6& fte);
    
private:
};

class FtiConfigEntrySetRtsock : public FtiConfigEntrySet,
				public RoutingSocket {
public:
    FtiConfigEntrySetRtsock(FtiConfig& ftic);
    virtual ~FtiConfigEntrySetRtsock();

    /**
     * Start operation.
     *
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    virtual int start(string& error_msg);

    /**
     * Stop operation.
     *
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    virtual int stop(string& error_msg);

    /**
     * Add a single routing entry.  Must be within a configuration
     * interval.
     *
     * @param fte the entry to add.
     *
     * @return true on success, otherwise false.
     */
    virtual bool add_entry4(const Fte4& fte);

    /**
     * Delete a single routing entry. Must be with a configuration interval.
     *
     * @param fte the entry to delete. Only destination and netmask are used.
     *
     * @return true on success, otherwise false.
     */
    virtual bool delete_entry4(const Fte4& fte);

    /**
     * Add a single routing entry. Must be within a configuration
     * interval.
     *
     * @param fte the entry to add.
     *
     * @return true on success, otherwise false.
     */
    virtual bool add_entry6(const Fte6& fte);

    /**
     * Delete a single routing entry.  Must be within a configuration
     * interval.
     *
     * @param fte the entry to delete. Only destination and netmask are used.
     *
     * @return true on success, otherwise false.
     */
    virtual bool delete_entry6(const Fte6& fte);

private:
    bool add_entry(const FteX& fte);
    bool delete_entry(const FteX& fte);
};

class FtiConfigEntrySetNetlink : public FtiConfigEntrySet,
				 public NetlinkSocket {
public:
    FtiConfigEntrySetNetlink(FtiConfig& ftic);
    virtual ~FtiConfigEntrySetNetlink();

    /**
     * Start operation.
     *
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    virtual int start(string& error_msg);

    /**
     * Stop operation.
     *
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    virtual int stop(string& error_msg);

    /**
     * Add a single routing entry.  Must be within a configuration
     * interval.
     *
     * @param fte the entry to add.
     *
     * @return true on success, otherwise false.
     */
    virtual bool add_entry4(const Fte4& fte);

    /**
     * Delete a single routing entry. Must be with a configuration interval.
     *
     * @param fte the entry to delete. Only destination and netmask are used.
     *
     * @return true on success, otherwise false.
     */
    virtual bool delete_entry4(const Fte4& fte);

    /**
     * Add a single routing entry. Must be within a configuration
     * interval.
     *
     * @param fte the entry to add.
     *
     * @return true on success, otherwise false.
     */
    virtual bool add_entry6(const Fte6& fte);

    /**
     * Delete a single routing entry.  Must be within a configuration
     * interval.
     *
     * @param fte the entry to delete. Only destination and netmask are used.
     *
     * @return true on success, otherwise false.
     */
    virtual bool delete_entry6(const Fte6& fte);

private:
    bool add_entry(const FteX& fte);
    bool delete_entry(const FteX& fte);

    NetlinkSocketReader _ns_reader;
};

class FtiConfigEntrySetClick : public FtiConfigEntrySet,
			       public ClickSocket,
			       public NexthopPortMapperObserver {
public:
    FtiConfigEntrySetClick(FtiConfig& ftic);
    virtual ~FtiConfigEntrySetClick();

    /**
     * Start operation.
     *
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    virtual int start(string& error_msg);

    /**
     * Stop operation.
     *
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    virtual int stop(string& error_msg);

    /**
     * Add a single routing entry.  Must be within a configuration
     * interval.
     *
     * @param fte the entry to add.
     *
     * @return true on success, otherwise false.
     */
    virtual bool add_entry4(const Fte4& fte);

    /**
     * Delete a single routing entry. Must be with a configuration interval.
     *
     * @param fte the entry to delete. Only destination and netmask are used.
     *
     * @return true on success, otherwise false.
     */
    virtual bool delete_entry4(const Fte4& fte);

    /**
     * Add a single routing entry. Must be within a configuration
     * interval.
     *
     * @param fte the entry to add.
     *
     * @return true on success, otherwise false.
     */
    virtual bool add_entry6(const Fte6& fte);

    /**
     * Delete a single routing entry.  Must be within a configuration
     * interval.
     *
     * @param fte the entry to delete. Only destination and netmask are used.
     *
     * @return true on success, otherwise false.
     */
    virtual bool delete_entry6(const Fte6& fte);

    /**
     * Obtain a reference to the table with the IPv4 forwarding entries.
     *
     * @return a reference to the table with the IPv4 forwarding entries.
     */
    const map<IPv4Net, Fte4>& fte_table4() const { return _fte_table4; }

    /**
     * Obtain a reference to the table with the IPv6 forwarding entries.
     *
     * @return a reference to the table with the IPv6 forwarding entries.
     */
    const map<IPv6Net, Fte6>& fte_table6() const { return _fte_table6; }

private:
    virtual void nexthop_port_mapper_event(bool is_mapping_changed);

    bool add_entry(const FteX& fte);
    bool delete_entry(const FteX& fte);

    // Methods related to reinstalling all IPv4 and IPv6 forwarding entries
    void start_task_reinstall_all_entries();
    void run_task_reinstall_all_entries();
    bool reinstall_all_entries4();
    bool reinstall_all_entries6();

    ClickSocketReader _cs_reader;

    map<IPv4Net, Fte4>	_fte_table4;
    map<IPv6Net, Fte6>	_fte_table6;

    // State related to reinstalling all IPv4 and IPv6 forwarding entries
    XorpTimer _reinstall_all_entries_timer;
    TimeSlice _reinstall_all_entries_time_slice;
    bool _start_reinstalling_fte_table4;
    bool _is_reinstalling_fte_table4;
    bool _start_reinstalling_fte_table6;
    bool _is_reinstalling_fte_table6;
    IPv4Net _reinstalling_ipv4net;
    IPv6Net _reinstalling_ipv6net;
};

class FtiConfigEntrySetIPHelper : public FtiConfigEntrySet {
public:
    FtiConfigEntrySetIPHelper(FtiConfig& ftic);
    virtual ~FtiConfigEntrySetIPHelper();

    /**
     * Start operation.
     *
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    virtual int start(string& error_msg);

    /**
     * Stop operation.
     *
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    virtual int stop(string& error_msg);

    /**
     * Add a single routing entry.  Must be within a configuration
     * interval.
     *
     * @param fte the entry to add.
     *
     * @return true on success, otherwise false.
     */
    virtual bool add_entry4(const Fte4& fte);

    /**
     * Delete a single routing entry. Must be with a configuration interval.
     *
     * @param fte the entry to delete. Only destination and netmask are used.
     *
     * @return true on success, otherwise false.
     */
    virtual bool delete_entry4(const Fte4& fte);

    /**
     * Add a single routing entry. Must be within a configuration
     * interval.
     *
     * @param fte the entry to add.
     *
     * @return true on success, otherwise false.
     */
    virtual bool add_entry6(const Fte6& fte);

    /**
     * Delete a single routing entry.  Must be within a configuration
     * interval.
     *
     * @param fte the entry to delete. Only destination and netmask are used.
     *
     * @return true on success, otherwise false.
     */
    virtual bool delete_entry6(const Fte6& fte);

private:
    bool add_entry(const FteX& fte);
    bool delete_entry(const FteX& fte);
};

class FtiConfigEntrySetRtmV2 : public FtiConfigEntrySet {
public:
    FtiConfigEntrySetRtmV2(FtiConfig& ftic);
    virtual ~FtiConfigEntrySetRtmV2();

    /**
     * Start operation.
     *
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    virtual int start(string& error_msg);

    /**
     * Stop operation.
     *
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    virtual int stop(string& error_msg);

    /**
     * Add a single routing entry.  Must be within a configuration
     * interval.
     *
     * @param fte the entry to add.
     *
     * @return true on success, otherwise false.
     */
    virtual bool add_entry4(const Fte4& fte);

    /**
     * Delete a single routing entry. Must be with a configuration interval.
     *
     * @param fte the entry to delete. Only destination and netmask are used.
     *
     * @return true on success, otherwise false.
     */
    virtual bool delete_entry4(const Fte4& fte);

    /**
     * Add a single routing entry. Must be within a configuration
     * interval.
     *
     * @param fte the entry to add.
     *
     * @return true on success, otherwise false.
     */
    virtual bool add_entry6(const Fte6& fte);

    /**
     * Delete a single routing entry.  Must be within a configuration
     * interval.
     *
     * @param fte the entry to delete. Only destination and netmask are used.
     *
     * @return true on success, otherwise false.
     */
    virtual bool delete_entry6(const Fte6& fte);

private:
    WinRtmPipe*		_rs4;
    WinRtmPipe*		_rs6;
    bool add_entry(const FteX& fte);
    bool delete_entry(const FteX& fte);
};

#endif // __FEA_FTICONFIG_ENTRY_SET_HH__