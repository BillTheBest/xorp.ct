/*
 * Copyright (c) 2001-2003 International Computer Science Institute
 * See LICENSE file for licensing, conditions, and warranties on use.
 *
 * DO NOT EDIT THIS FILE - IT IS PROGRAMMATICALLY GENERATED
 *
 * Generated by 'tgt-gen'.
 */

#ident "$XORP: xorp/xrl/targets/xorp_if_mib_base.cc,v 1.1 2003/05/20 01:20:46 jcardona Exp $"


#include "xorp_if_mib_base.hh"

const XrlCmdError
XrlXorpIfMibTargetBase::handle_common_0_1_get_target_name(const Xrl& xrl, XrlArgs* args)
{
    if (xrl.const_args().size() != 0) { 
	XLOG_ERROR("Wrong number of arguments (%u != 0) handling common/0.1/get_target_name", (uint32_t)xrl.const_args().size());
	return XrlCmdError::BAD_ARGS();
    }

    if (args == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    string name; 
    try {
	XrlCmdError e = common_0_1_get_target_name(
	    name);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for common/0.1/get_target_name failed: %s \"%s\"",
            		 e.str().c_str(), e.note().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }

    /* Marshall return values */
    try {
	args->add("name", name);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlXorpIfMibTargetBase::handle_common_0_1_get_version(const Xrl& xrl, XrlArgs* args)
{
    if (xrl.const_args().size() != 0) { 
	XLOG_ERROR("Wrong number of arguments (%u != 0) handling common/0.1/get_version", (uint32_t)xrl.const_args().size());
	return XrlCmdError::BAD_ARGS();
    }

    if (args == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    string version; 
    try {
	XrlCmdError e = common_0_1_get_version(
	    version);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for common/0.1/get_version failed: %s \"%s\"",
            		 e.str().c_str(), e.note().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }

    /* Marshall return values */
    try {
	args->add("version", version);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlXorpIfMibTargetBase::handle_common_0_1_get_status(const Xrl& xrl, XrlArgs* args)
{
    if (xrl.const_args().size() != 0) { 
	XLOG_ERROR("Wrong number of arguments (%u != 0) handling common/0.1/get_status", (uint32_t)xrl.const_args().size());
	return XrlCmdError::BAD_ARGS();
    }

    if (args == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    uint32_t status; 
    string reason; 
    try {
	XrlCmdError e = common_0_1_get_status(
	    status, 
	    reason);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for common/0.1/get_status failed: %s \"%s\"",
            		 e.str().c_str(), e.note().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }

    /* Marshall return values */
    try {
	args->add("status", status);
	args->add("reason", reason);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlXorpIfMibTargetBase::handle_common_0_1_shutdown(const Xrl& xrl, XrlArgs* /* args */)
{
    if (xrl.const_args().size() != 0) { 
	XLOG_ERROR("Wrong number of arguments (%u != 0) handling common/0.1/shutdown", (uint32_t)xrl.const_args().size());
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    try {
	XrlCmdError e = common_0_1_shutdown();
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for common/0.1/shutdown failed: %s \"%s\"",
            		 e.str().c_str(), e.note().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlXorpIfMibTargetBase::handle_xorp_if_mib_0_1_load_mib(const Xrl& xrl, XrlArgs* args)
{
    if (xrl.const_args().size() != 2) { 
	XLOG_ERROR("Wrong number of arguments (%u != 2) handling xorp_if_mib/0.1/load_mib", (uint32_t)xrl.const_args().size());
	return XrlCmdError::BAD_ARGS();
    }

    if (args == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    uint32_t mib_index; 
    try {
	XrlCmdError e = xorp_if_mib_0_1_load_mib(
	    xrl.const_args().get_string("mod_name"), 
	    xrl.const_args().get_string("abs_path"), 
	    mib_index);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for xorp_if_mib/0.1/load_mib failed: %s \"%s\"",
            		 e.str().c_str(), e.note().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }

    /* Marshall return values */
    try {
	args->add("mib_index", mib_index);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

const XrlCmdError
XrlXorpIfMibTargetBase::handle_xorp_if_mib_0_1_unload_mib(const Xrl& xrl, XrlArgs* args)
{
    if (xrl.const_args().size() != 1) { 
	XLOG_ERROR("Wrong number of arguments (%u != 1) handling xorp_if_mib/0.1/unload_mib", (uint32_t)xrl.const_args().size());
	return XrlCmdError::BAD_ARGS();
    }

    if (args == 0) {
	XLOG_FATAL("Return list empty");
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    bool unloaded; 
    try {
	XrlCmdError e = xorp_if_mib_0_1_unload_mib(
	    xrl.const_args().get_uint32("mib_index"), 
	    unloaded);
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for xorp_if_mib/0.1/unload_mib failed: %s \"%s\"",
            		 e.str().c_str(), e.note().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }

    /* Marshall return values */
    try {
	args->add("unloaded", unloaded);
    } catch (const XrlArgs::XrlAtomFound& ) {
	XLOG_FATAL("Duplicate atom name"); /* XXX Should never happen */
    }
    return XrlCmdError::OKAY();
}

