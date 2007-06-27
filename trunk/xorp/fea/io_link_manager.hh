// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-

// Copyright (c) 2007 International Computer Science Institute
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

// $XORP$

#ifndef __FEA_IO_LINK_MANAGER_HH__
#define __FEA_IO_LINK_MANAGER_HH__

#include <list>
#include <vector>
#include <set>
#include <map>

#include "libxorp/mac.hh"

#include "fea/data_plane/io/io_link_pcap.hh"

class IoLinkManager;


/**
 * Simple structure used to cache commonly passed MAC header
 * information when passed around between different components of the FEA.
 */
struct MacHeaderInfo {
    string	if_name;
    string	vif_name;
    Mac		src_address;
    Mac		dst_address;
    uint16_t	ether_type;
};

/**
 * A class that handles raw link I/O communication for a specific protocol.
 *
 * It also allows arbitrary filters to receive the raw link-level data for that
 * protocol.
 */
class IoLinkComm : public IoLinkPcap {
public:
    /**
     * Filter class.
     */
    class InputFilter {
    public:
	InputFilter(IoLinkManager&	io_link_manager,
		    const string&	receiver_name,
		    const string&	if_name,
		    const string&	vif_name,
		    uint16_t		ether_type,
		    const string&	filter_program)
	    : _io_link_manager(io_link_manager),
	      _receiver_name(receiver_name),
	      _if_name(if_name),
	      _vif_name(vif_name),
	      _ether_type(ether_type),
	      _filter_program(filter_program)
	{}
	virtual ~InputFilter() {}

	/**
	 * Get a reference to the I/O Link manager.
	 *
	 * @return a reference to the I/O Link manager.
	 */
	IoLinkManager& io_link_manager() { return (_io_link_manager); }

	/**
	 * Get a const reference to the I/O Link manager.
	 *
	 * @return a const reference to the I/O Link manager.
	 */
	const IoLinkManager& io_link_manager() const { return (_io_link_manager); }

	/**
	 * Get the receiver name.
	 *
	 * @return the receiver name.
	 */
	const string& receiver_name() const { return (_receiver_name); }

	/**
	 * Get the interface name.
	 *
	 * @return the interface name.
	 */
	const string& if_name() const { return (_if_name); }

	/**
	 * Get the vif name.
	 *
	 * @return the vif name.
	 */
	const string& vif_name() const { return (_vif_name); }

	/**
	 * Get the EtherType protocol number.
	 *
	 * @return the EtherType protocol number.
	 */
	uint16_t ether_type() const { return (_ether_type); }

	/**
	 * Get the filter program.
	 *
	 * @return the filter program.
	 */
	const string& filter_program() const { return (_filter_program); }

	/**
	 * Method invoked when data arrives on associated IoLinkComm instance.
	 */
	virtual void recv(const struct MacHeaderInfo& header,
			  const vector<uint8_t>& payload) = 0;

	/**
	 * Method invoked by the destructor of the associated IoLinkComm
	 * instance. This method provides the InputFilter with the
	 * opportunity to delete itself or update its state.
	 * The input filter does not need to call IoLinkComm::remove_filter()
	 * since filter removal is automatically conducted.
	 */
	virtual void bye() = 0;

    private:
	IoLinkManager&	_io_link_manager;
	string		_receiver_name;
	string		_if_name;
	string		_vif_name;
	uint16_t	_ether_type;
	string		_filter_program;
    };

    /**
     * Joined multicast group class.
     */
    class JoinedMulticastGroup {
    public:
	JoinedMulticastGroup(const Mac& group_address)
	    : _group_address(group_address)
	{}
	virtual ~JoinedMulticastGroup() {}

	/**
	 * Less-Than Operator
	 *
	 * @param other the right-hand operand to compare against.
	 * @return true if the left-hand operand is numerically smaller
	 * than the right-hand operand.
	 */
	bool operator<(const JoinedMulticastGroup& other) const {
	    return (_group_address < other._group_address);
	}

	/**
	 * Equality Operator
	 *
	 * @param other the right-hand operand to compare against.
	 * @return true if the left-hand operand is numerically same as the
	 * right-hand operand.
	 */
	bool operator==(const JoinedMulticastGroup& other) const {
	    return (_group_address == other._group_address);
	}

	/**
	 * Add a receiver.
	 *
	 * @param receiver_name the name of the receiver to add.
	 */
	void add_receiver(const string& receiver_name) {
	    _receivers.insert(receiver_name);
	}

	/**
	 * Delete a receiver.
	 *
	 * @param receiver_name the name of the receiver to delete.
	 */
	void delete_receiver(const string& receiver_name) {
	    _receivers.erase(receiver_name);
	}

	/**
	 * @return true if there are no receivers associated with this group.
	 */
	bool empty() const { return _receivers.empty(); }

    private:
	Mac		_group_address;
	set<string>	_receivers;
    };

public:
    /**
     * Constructor for IoLinkComm.
     *
     * @param eventloop the event loop to use.
     * @param iftree the interface tree to use (@see IfTree).
     * @param if_name the interface name.
     * @param vif_name the vif name.
     * @param ether_type the EtherType protocol number.
     * @param filter_program the optional filter program to be applied on
     * the received packets. The program uses tcpdump(1) style expression.
     */
    IoLinkComm(EventLoop& eventloop, const IfTree& iftree,
	       const string& if_name, const string& vif_name,
	       uint16_t ether_type, const string& filter_program);
    virtual ~IoLinkComm();

    /**
     * Add a filter to list of input filters.  The IoLinkComm class
     * assumes that the callee will be responsible for managing the memory
     * associated with the filter and will call remove_filter() if the
     * filter is deleted or goes out of scope.
     */
    bool add_filter(InputFilter* filter);

    /**
     * Remove filter from list of input filters.
     */
    bool remove_filter(InputFilter* filter);

    /**
     * @return true if there are no filters associated with this instance.
     */
    bool no_input_filters() const { return _input_filters.empty(); }

    /**
     * Send a raw link-level packet.
     *
     * @param src_address the MAC source address.
     * @param dst_address the MAC destination address.
     * @param ether_type the EtherType protocol number.
     * @param payload the payload, everything after the MAC header.
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    int		send_packet(const Mac&		src_address,
			    const Mac&		dst_address,
			    uint16_t		ether_type,
			    const vector<uint8_t>& payload,
			    string&		error_msg);

    /**
     * Received a raw link-level packet.
     *
     * @param src_address the MAC source address.
     * @param dst_address the MAC destination address.
     * @param ether_type the EtherType protocol number.
     * @param packet the payload, everything after the MAC header.
     */
    void	process_recv_data(const Mac&		src_address,
				  const Mac&		dst_address,
				  uint16_t		ether_type,
				  const vector<uint8_t>& payload);

    /**
     * Join a MAC multicast group.
     * 
     * @param group_address the multicast group address to join.
     * @param receiver_name the name of the receiver.
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    int		join_multicast_group(const Mac&		group_address,
				     const string&	receiver_name,
				     string&		error_msg);

    /**
     * Leave a MAC multicast group.
     * 
     * @param group_address the multicast group address to leave.
     * @param receiver_name the name of the receiver.
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    int		leave_multicast_group(const Mac&	group_address,
				      const string&	receiver_name,
				      string&		error_msg);

private:
    IoLinkComm(const IoLinkComm&);		// Not implemented.
    IoLinkComm& operator=(const IoLinkComm&);	// Not implemented.

    list<InputFilter*>		_input_filters;
    typedef map<JoinedMulticastGroup, JoinedMulticastGroup> JoinedGroupsTable;
    JoinedGroupsTable		_joined_groups_table;
};

/**
 * @short A class that manages raw link-level I/O.
 *
 * The IoLinkManager has two containers: a container for link-level handlers
 * (@see IoLinkComm) indexed by the protocol associated with the handler, and
 * a container for the filters associated with each receiver_name.  When
 * a receiver registers for interest in a particular type of raw
 * packet a handler (@see IoLinkComm) is created if necessary, then the
 * relevent filter is created and associated with the IoLinkComm.
 */
class IoLinkManager {
public:
    typedef XorpCallback2<int, const uint8_t*, size_t>::RefPtr UpcallReceiverCb;

    /**
     * Constructor for IoLinkManager.
     */
    IoLinkManager(EventLoop& eventloop, const IfTree& iftree);

    virtual ~IoLinkManager();

    /**
     * @short Class that implements the API for sending raw link-level packet
     * to a receiver.
     */
    class SendToReceiverBase {
    public:
	virtual ~SendToReceiverBase() {}
	/**
	 * Send a raw link-level packet to a receiver.
	 *
	 * @param receiver_name the name of the receiver to send the
	 * raw link-level packet to.
	 * @param header the MAC header information.
	 * @param payload the payload, everything after the MAC header.
	 */
	virtual void send_to_receiver(const string&		receiver_name,
				      const struct MacHeaderInfo& header,
				      const vector<uint8_t>&	payload) = 0;
    };

    /**
     * Send a raw link-level packet on an interface.
     *
     * @param if_name the interface to send the packet on.
     * @param vif_name the vif to send the packet on.
     * @param src_address the MAC source address.
     * @param dst_address the MAC destination address.
     * @param ether_type the EtherType protocol number. It must be between
     * 1536 and 65535.
     * @param payload the payload, everything after the MAC header.
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    int send(const string&	if_name,
	     const string&	vif_name,
	     const Mac&		src_address,
	     const Mac&		dst_address,
	     uint16_t		ether_type,
	     const vector<uint8_t>&	payload,
	     string&		error_msg);

    /**
     * Register to receive raw link-level packets.
     *
     * @param receiver_name the name of the receiver.
     * @param if_name the interface through which packets should be accepted.
     * @param vif_name the vif through which packets should be accepted.
     * @param ether_type the EtherType protocol number that the receiver is
     * interested in. It must be between 1536 and 65535. A protocol number
     * of 0 is used to specify all protocols.
     * @param filter_program the optional filter program to be applied on the
     * received packets. The program uses tcpdump(1) style expression.
     * @param enable_multicast_loopback if true then enable delivering of
     * multicast datagrams back to this host (assuming the host is a member of
     * the same multicast group.
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    int register_receiver(const string&	receiver_name,
			  const string&	if_name,
			  const string&	vif_name,
			  uint16_t	ether_type,
			  const string&	filter_program,
			  bool		enable_multicast_loopback,
			  string&	error_msg);

    /**
     * Unregister to receive raw link-level packets.
     *
     * @param receiver_name the name of the receiver.
     * @param if_name the interface through which packets should not be
     * accepted.
     * @param vif_name the vif through which packets should not be accepted.
     * @param ether_type the EtherType protocol number that the receiver is
     * not interested in anymore. It must be between 1536 and 65535.
     * A protocol number of 0 is used to specify all protocols.
     * @param filter_program the filter program that was applied on the
     * received packets. The program uses tcpdump(1) style expression.
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    int unregister_receiver(const string&	receiver_name,
			    const string&	if_name,
			    const string&	vif_name,
			    uint16_t		ether_type,
			    const string&	filter_program,
			    string&		error_msg);

    /**
     * Join a MAC multicast group.
     *
     * @param receiver_name the name of the receiver.
     * @param if_name the interface through which packets should be accepted.
     * @param vif_name the vif through which packets should be accepted.
     * @param ether_type the EtherType protocol number that the receiver is
     * interested in. It must be between 1536 and 65535. A protocol number
     * of 0 is used to specify all protocols.
     * @param filter_program the optional filter program to be applied on the
     * received packets. The program uses tcpdump(1) style expression.
     * @param group_address the multicast group address to join.
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    int join_multicast_group(const string&	receiver_name,
			     const string&	if_name,
			     const string&	vif_name,
			     uint16_t		ether_type,
			     const string&	filter_program,
			     const Mac&		group_address,
			     string&		error_msg);

    /**
     * Leave a MAC multicast group.
     *
     * @param receiver_name the name of the receiver.
     * @param if_name the interface through which packets should not be
     * accepted.
     * @param vif_name the vif through which packets should not be accepted.
     * @param ether_type the EtherType protocol number that the receiver is not
     * interested in anymore. It must be between 1536 and 65535. A protocol
     * number of 0 is used to specify all protocols.
     * @param filter_program the filter program that was applied on the
     * received packets. The program uses tcpdump(1) style expression.
     * @param group_address the multicast group address to leave.
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    int leave_multicast_group(const string&	receiver_name,
			      const string&	if_name,
			      const string&	vif_name,
			      uint16_t		ether_type,
			      const string&	filter_program,
			      const Mac&	group_address,
			      string&		error_msg);

    /**
     * Send a raw link-level packet to a receiver.
     *
     * @param receiver_name the name of the receiver to send the raw link-level
     * packet to.
     * @param header the MAC header information.
     * @param payload the payload, everything after the MAC header.
     */
    void send_to_receiver(const string&			receiver_name,
			  const struct MacHeaderInfo&	header,
			  const vector<uint8_t>&	payload);

    /**
     * Set the instance that is responsible for sending raw link-level packets
     * to a receiver.
     */
    void set_send_to_receiver_base(SendToReceiverBase* v) {
	_send_to_receiver_base = v;
    }

    /**
     * Erase filters for a given receiver name.
     *
     * @param receiver_name the name of the receiver.
     */
    void erase_filters_by_name(const string& receiver_name);

    /**
     * Get a reference to the interface tree.
     *
     * @return a reference to the interface tree (@see IfTree).
     */
    const IfTree&	iftree() const { return _iftree; }

private:
    class CommTableKey {
    public:
	CommTableKey(const string& if_name, const string& vif_name,
		     uint16_t ether_type, const string& filter_program)
	    : _if_name(if_name),
	      _vif_name(vif_name),
	      _ether_type(ether_type),
	      _filter_program(filter_program)
	{}
	bool operator<(const CommTableKey& other) const {
	    if (_ether_type != other._ether_type)
		return (_ether_type < other._ether_type);
	    if (_if_name != other._if_name)
		return (_if_name < other._if_name);
	    if (_vif_name != other._vif_name)
		return (_vif_name < other._vif_name);
	    return (_filter_program < other._filter_program);
	}

    private:
	string		_if_name;
	string		_vif_name;
	uint16_t	_ether_type;
	string		_filter_program;
    };

    typedef map<CommTableKey, IoLinkComm*> CommTable;
    typedef multimap<string, IoLinkComm::InputFilter*> FilterBag;

    void erase_filters(CommTable& comm_table, FilterBag& filters,
		       const FilterBag::iterator& begin,
		       const FilterBag::iterator& end);

    EventLoop&		_eventloop;
    const IfTree&	_iftree;

    // Collection of raw link-level communication handlers keyed by protocol.
    CommTable		_comm_table;

    // Collection of input filters created by IoLinkManager
    FilterBag		_filters;

    SendToReceiverBase*	_send_to_receiver_base;
};

#endif // __FEA_IO_LINK_MANAGER_HH__
