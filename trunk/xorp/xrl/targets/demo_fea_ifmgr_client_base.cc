/*
 * Copyright (c) 2001-2003 International Computer Science Institute
 * See LICENSE file for licensing, conditions, and warranties on use.
 *
 * DO NOT EDIT THIS FILE - IT IS PROGRAMMATICALLY GENERATED
 *
 * Generated by '../scripts/tgt-gen'.
 */

#ident "$XORP: xorp/xrl/targets/demo_fea_ifmgr_client_base.cc,v 1.2 2003/01/24 06:31:32 pavlin Exp $"


#include "demo_fea_ifmgr_client_base.hh"

const XrlCmdError
XrlDemoFeaIfmgrClientTargetBase::handle_common_0_1_get_target_name(const Xrl& xrl, XrlArgs* args)
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
XrlDemoFeaIfmgrClientTargetBase::handle_common_0_1_get_version(const Xrl& xrl, XrlArgs* args)
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
XrlDemoFeaIfmgrClientTargetBase::handle_fea_ifmgr_client_0_1_interface_update(const Xrl& xrl, XrlArgs* /* args */)
{
    if (xrl.const_args().size() != 2) { 
	XLOG_ERROR("Wrong number of arguments (%u != 2) handling fea_ifmgr_client/0.1/interface_update", (uint32_t)xrl.const_args().size());
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    try {
	XrlCmdError e = fea_ifmgr_client_0_1_interface_update(
	    xrl.const_args().get_string("ifname"), 
	    xrl.const_args().get_uint32("event"));
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for fea_ifmgr_client/0.1/interface_update failed: %s \"%s\"",
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
XrlDemoFeaIfmgrClientTargetBase::handle_fea_ifmgr_client_0_1_vif_update(const Xrl& xrl, XrlArgs* /* args */)
{
    if (xrl.const_args().size() != 3) { 
	XLOG_ERROR("Wrong number of arguments (%u != 3) handling fea_ifmgr_client/0.1/vif_update", (uint32_t)xrl.const_args().size());
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    try {
	XrlCmdError e = fea_ifmgr_client_0_1_vif_update(
	    xrl.const_args().get_string("ifname"), 
	    xrl.const_args().get_string("vifname"), 
	    xrl.const_args().get_uint32("event"));
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for fea_ifmgr_client/0.1/vif_update failed: %s \"%s\"",
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
XrlDemoFeaIfmgrClientTargetBase::handle_fea_ifmgr_client_0_1_vifaddr4_update(const Xrl& xrl, XrlArgs* /* args */)
{
    if (xrl.const_args().size() != 4) { 
	XLOG_ERROR("Wrong number of arguments (%u != 4) handling fea_ifmgr_client/0.1/vifaddr4_update", (uint32_t)xrl.const_args().size());
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    try {
	XrlCmdError e = fea_ifmgr_client_0_1_vifaddr4_update(
	    xrl.const_args().get_string("ifname"), 
	    xrl.const_args().get_string("vifname"), 
	    xrl.const_args().get_ipv4("addr"), 
	    xrl.const_args().get_uint32("event"));
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for fea_ifmgr_client/0.1/vifaddr4_update failed: %s \"%s\"",
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
XrlDemoFeaIfmgrClientTargetBase::handle_fea_ifmgr_client_0_1_vifaddr6_update(const Xrl& xrl, XrlArgs* /* args */)
{
    if (xrl.const_args().size() != 4) { 
	XLOG_ERROR("Wrong number of arguments (%u != 4) handling fea_ifmgr_client/0.1/vifaddr6_update", (uint32_t)xrl.const_args().size());
	return XrlCmdError::BAD_ARGS();
    }

    /* Return value declarations */
    try {
	XrlCmdError e = fea_ifmgr_client_0_1_vifaddr6_update(
	    xrl.const_args().get_string("ifname"), 
	    xrl.const_args().get_string("vifname"), 
	    xrl.const_args().get_ipv6("addr"), 
	    xrl.const_args().get_uint32("event"));
	if (e != XrlCmdError::OKAY()) {
	    XLOG_WARNING("Handling method for fea_ifmgr_client/0.1/vifaddr6_update failed: %s \"%s\"",
            		 e.str().c_str(), e.note().c_str());
	    return e;
        }
    } catch (const XrlArgs::XrlAtomNotFound& e) {
	XLOG_ERROR("Argument not found");
	return XrlCmdError::BAD_ARGS();
    }
    return XrlCmdError::OKAY();
}

