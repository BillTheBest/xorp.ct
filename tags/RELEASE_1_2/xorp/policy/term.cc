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

#ident "$XORP: xorp/policy/term.cc,v 1.14 2005/10/02 18:19:15 pavlin Exp $"

// #define DEBUG_LOGGING
// #define DEBUG_PRINT_FUNCTION_NAME

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "policy_module.h"
#include "libxorp/xlog.h"
#include "libxorp/debug.h"
#include "term.hh"
#include "policy/common/policy_utils.hh"
#include "parser.hh"

using namespace policy_utils;

Term::Term(const string& name) : _name(name), 
				 _source_nodes(_block_nodes[SOURCE]),
				 _dest_nodes(_block_nodes[DEST]),
				 _action_nodes(_block_nodes[ACTION])
{
    for(unsigned int i = 0; i < LAST_BLOCK; i++)
	_block_nodes[i] = new Nodes;
}

Term::~Term()
{
    for(unsigned int i = 0; i < LAST_BLOCK; i++) {
	clear_map_container(*_block_nodes[i]);
	delete _block_nodes[i];

	list<pair<ConfigNodeId, Node*> >::iterator iter;
	for (iter = _out_of_order_nodes[i].begin();
	     iter != _out_of_order_nodes[i].end();
	     ++iter) {
	    delete iter->second;
	}
    }
}

void
Term::set_block(const uint32_t& block, const ConfigNodeId& order,
		const string& statement)
{
    if (block >= LAST_BLOCK) {
	throw term_syntax_error("Unknown block: " + to_str(block));
    }

    // check if we want to delete
    if (statement.empty()) {
	del_block(block, order);
	return;
    }

    // check that position is empty... 
    // if it's not delete and then add...  This usually occurs when "replacing"
    // an existing statement. like:
    // localpref: 101
    // setting localpref: 102 in same node will cause this...
    Nodes& conf_block = *_block_nodes[block];
    if ((conf_block.find(order) != conf_block.end())
	|| (find_out_of_order_node(block, order)
	    != _out_of_order_nodes[block].end())) {
	debug_msg("[POLICY] Deleting previous statement...\n");
	del_block(block, order);
/*	
	throw term_syntax_error("A statement is already present in position: " 
				+ to_str(order));
*/				
    }

    debug_msg("[POLICY] Statement=(%s)\n", statement.c_str());

    // parse and check syntax error
    Parser parser;
    // cast should be safe... because of check earlier in method.
    Parser::Nodes* nodes = parser.parse(static_cast<BLOCKS>(block), statement);
    if (!nodes) {
	string err = parser.last_error();
	// XXX convert block from int to string... [human readable]
	throw term_syntax_error("Syntax error in term " + _name + 
				" block " + block2str(block) + " statement=("
				+ statement + "): " + err);
    }
    XLOG_ASSERT(nodes->size() == 1); // XXX a single statement!

    pair<Nodes::iterator, bool> res;
    res = conf_block.insert(order, nodes->front());
    if (res.second != true) {
	//
	// Failed to add the entry, probably because it was received out of
	// order. Add it to the list of entries that need to be added later.
	//
	_out_of_order_nodes[block].push_back(make_pair(order, nodes->front()));
	return;
    }

    //
    // Try to add any entries that are out of order.
    // Note that we need to keep trying traversing the list until
    // no entry is added.
    //
    while (true) {
	bool entry_added = false;
	list<pair<ConfigNodeId, Node*> >::iterator iter;
	for (iter = _out_of_order_nodes[block].begin();
	     iter != _out_of_order_nodes[block].end();
	     ++iter) {
	    res = conf_block.insert(iter->first, iter->second);
	    if (res.second == true) {
		// Entry added successfully
		entry_added = true;
		_out_of_order_nodes[block].erase(iter);
		break;
	    }
	}
	
	if (! entry_added)
	    break;
    }
}

void
Term::del_block(const uint32_t& block, const ConfigNodeId& order)
{
    XLOG_ASSERT (block < LAST_BLOCK);

    Nodes& conf_block = *_block_nodes[block];

    Nodes::iterator i = conf_block.find(order);
    if (i != conf_block.end()) {
	conf_block.erase(i);
	return;
    }

    // Try to delete from the list of out-of-order nodes
    list<pair<ConfigNodeId, Node*> >::iterator iter;
    iter = find_out_of_order_node(block, order);
    if (iter != _out_of_order_nodes[block].end()) {
	_out_of_order_nodes[block].erase(iter);
	return;
    }

    throw term_syntax_error("Want to delete an empty position: " 
			    + order.str());
}

list<pair<ConfigNodeId, Node*> >::iterator
Term::find_out_of_order_node(const uint32_t& block, const ConfigNodeId& order)
{
    list<pair<ConfigNodeId, Node*> >::iterator iter;

    XLOG_ASSERT (block < LAST_BLOCK);

    for (iter = _out_of_order_nodes[block].begin();
	 iter != _out_of_order_nodes[block].end();
	 ++iter) {
	const ConfigNodeId& list_order = iter->first;
	if (list_order.unique_node_id() == order.unique_node_id())
	    return (iter);
    }

    return (_out_of_order_nodes[block].end());
}

string
Term::block2str(uint32_t block)
{
    switch (block) {
	case SOURCE:
	    return "source";

	case DEST:
	    return "dest";
	    
	case ACTION:
	    return "action";
    
	default:
	    return "UNKNOWN";
    }
}