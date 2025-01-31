/*
 * Copyright (c) 2010-2022 Belledonne Communications SARL.
 *
 * This file is part of Liblinphone
 * (see https://gitlab.linphone.org/BC/public/liblinphone).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CONFERENCE_INFO_H
#define CONFERENCE_INFO_H

#ifndef XSD_CXX11
#define XSD_CXX11
#endif

#ifndef XSD_USE_CHAR
#define XSD_USE_CHAR
#endif

#ifndef XSD_CXX_TREE_USE_CHAR
#define XSD_CXX_TREE_USE_CHAR
#endif

// Begin prologue.
//
#if __clang__ || __GNUC__ >= 4
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wconversion"
#endif
#if __GNUC__ > 5 || (__GNUC__ == 5 && __GNUC_MINOR__ >= 1)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-override"
#endif
#if __GNUC__ >= 7
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
#endif
//
// End prologue.

#include <xsd/cxx/config.hxx>

#if (XSD_INT_VERSION != 4000000L)
#error XSD runtime version mismatch
#endif

#include <xsd/cxx/pre.hxx>

#include <xsd/cxx/xml/char-utf8.hxx>

#include <xsd/cxx/tree/elements.hxx>
#include <xsd/cxx/tree/exceptions.hxx>
#include <xsd/cxx/tree/types.hxx>

#include <xsd/cxx/xml/error-handler.hxx>

#include <xsd/cxx/xml/dom/auto-ptr.hxx>

#include <xsd/cxx/tree/parsing.hxx>
#include <xsd/cxx/tree/parsing/boolean.hxx>
#include <xsd/cxx/tree/parsing/byte.hxx>
#include <xsd/cxx/tree/parsing/decimal.hxx>
#include <xsd/cxx/tree/parsing/double.hxx>
#include <xsd/cxx/tree/parsing/float.hxx>
#include <xsd/cxx/tree/parsing/int.hxx>
#include <xsd/cxx/tree/parsing/long.hxx>
#include <xsd/cxx/tree/parsing/short.hxx>
#include <xsd/cxx/tree/parsing/unsigned-byte.hxx>
#include <xsd/cxx/tree/parsing/unsigned-int.hxx>
#include <xsd/cxx/tree/parsing/unsigned-long.hxx>
#include <xsd/cxx/tree/parsing/unsigned-short.hxx>

#include <xsd/cxx/tree/serialization.hxx>
#include <xsd/cxx/tree/serialization/boolean.hxx>
#include <xsd/cxx/tree/serialization/byte.hxx>
#include <xsd/cxx/tree/serialization/decimal.hxx>
#include <xsd/cxx/tree/serialization/double.hxx>
#include <xsd/cxx/tree/serialization/float.hxx>
#include <xsd/cxx/tree/serialization/int.hxx>
#include <xsd/cxx/tree/serialization/long.hxx>
#include <xsd/cxx/tree/serialization/short.hxx>
#include <xsd/cxx/tree/serialization/unsigned-byte.hxx>
#include <xsd/cxx/tree/serialization/unsigned-int.hxx>
#include <xsd/cxx/tree/serialization/unsigned-long.hxx>
#include <xsd/cxx/tree/serialization/unsigned-short.hxx>
#include <xsd/cxx/xml/dom/serialization-header.hxx>

#include <xsd/cxx/tree/std-ostream-operators.hxx>

namespace LinphonePrivate {
namespace Xsd {
namespace XmlSchema {
// anyType and anySimpleType.
//
typedef ::xsd::cxx::tree::type Type;
typedef ::xsd::cxx::tree::simple_type<char, Type> SimpleType;
typedef ::xsd::cxx::tree::type Container;

// 8-bit
//
typedef signed char Byte;
typedef unsigned char UnsignedByte;

// 16-bit
//
typedef short Short;
typedef unsigned short UnsignedShort;

// 32-bit
//
typedef int Int;
typedef unsigned int UnsignedInt;

// 64-bit
//
typedef long long Long;
typedef unsigned long long UnsignedLong;

// Supposed to be arbitrary-length integral types.
//
typedef long long Integer;
typedef long long NonPositiveInteger;
typedef unsigned long long NonNegativeInteger;
typedef unsigned long long PositiveInteger;
typedef long long NegativeInteger;

// Boolean.
//
typedef bool Boolean;

// Floating-point types.
//
typedef float Float;
typedef double Double;
typedef double Decimal;

// String types.
//
typedef ::xsd::cxx::tree::string<char, SimpleType> String;
typedef ::xsd::cxx::tree::normalized_string<char, String> NormalizedString;
typedef ::xsd::cxx::tree::token<char, NormalizedString> Token;
typedef ::xsd::cxx::tree::name<char, Token> Name;
typedef ::xsd::cxx::tree::nmtoken<char, Token> Nmtoken;
typedef ::xsd::cxx::tree::nmtokens<char, SimpleType, Nmtoken> Nmtokens;
typedef ::xsd::cxx::tree::ncname<char, Name> Ncname;
typedef ::xsd::cxx::tree::language<char, Token> Language;

// ID/IDREF.
//
typedef ::xsd::cxx::tree::id<char, Ncname> Id;
typedef ::xsd::cxx::tree::idref<char, Ncname, Type> Idref;
typedef ::xsd::cxx::tree::idrefs<char, SimpleType, Idref> Idrefs;

// URI.
//
typedef ::xsd::cxx::tree::uri<char, SimpleType> Uri;

// Qualified name.
//
typedef ::xsd::cxx::tree::qname<char, SimpleType, Uri, Ncname> Qname;

// Binary.
//
typedef ::xsd::cxx::tree::buffer<char> Buffer;
typedef ::xsd::cxx::tree::base64_binary<char, SimpleType> Base64Binary;
typedef ::xsd::cxx::tree::hex_binary<char, SimpleType> HexBinary;

// Date/time.
//
typedef ::xsd::cxx::tree::time_zone TimeZone;
typedef ::xsd::cxx::tree::date<char, SimpleType> Date;
typedef ::xsd::cxx::tree::date_time<char, SimpleType> DateTime;
typedef ::xsd::cxx::tree::duration<char, SimpleType> Duration;
typedef ::xsd::cxx::tree::gday<char, SimpleType> Gday;
typedef ::xsd::cxx::tree::gmonth<char, SimpleType> Gmonth;
typedef ::xsd::cxx::tree::gmonth_day<char, SimpleType> GmonthDay;
typedef ::xsd::cxx::tree::gyear<char, SimpleType> Gyear;
typedef ::xsd::cxx::tree::gyear_month<char, SimpleType> GyearMonth;
typedef ::xsd::cxx::tree::time<char, SimpleType> Time;

// Entity.
//
typedef ::xsd::cxx::tree::entity<char, Ncname> Entity;
typedef ::xsd::cxx::tree::entities<char, SimpleType, Entity> Entities;

typedef ::xsd::cxx::tree::content_order ContentOrder;
// Namespace information and list stream. Used in
// serialization functions.
//
typedef ::xsd::cxx::xml::dom::namespace_info<char> NamespaceInfo;
typedef ::xsd::cxx::xml::dom::namespace_infomap<char> NamespaceInfomap;
typedef ::xsd::cxx::tree::list_stream<char> ListStream;
typedef ::xsd::cxx::tree::as_double<Double> AsDouble;
typedef ::xsd::cxx::tree::as_decimal<Decimal> AsDecimal;
typedef ::xsd::cxx::tree::facet Facet;

// Flags and properties.
//
typedef ::xsd::cxx::tree::flags Flags;
typedef ::xsd::cxx::tree::properties<char> Properties;

// Parsing/serialization diagnostics.
//
typedef ::xsd::cxx::tree::severity Severity;
typedef ::xsd::cxx::tree::error<char> Error;
typedef ::xsd::cxx::tree::diagnostics<char> Diagnostics;

// Exceptions.
//
typedef ::xsd::cxx::tree::exception<char> Exception;
typedef ::xsd::cxx::tree::bounds<char> Bounds;
typedef ::xsd::cxx::tree::duplicate_id<char> DuplicateId;
typedef ::xsd::cxx::tree::parsing<char> Parsing;
typedef ::xsd::cxx::tree::expected_element<char> ExpectedElement;
typedef ::xsd::cxx::tree::unexpected_element<char> UnexpectedElement;
typedef ::xsd::cxx::tree::expected_attribute<char> ExpectedAttribute;
typedef ::xsd::cxx::tree::unexpected_enumerator<char> UnexpectedEnumerator;
typedef ::xsd::cxx::tree::expected_text_content<char> ExpectedTextContent;
typedef ::xsd::cxx::tree::no_prefix_mapping<char> NoPrefixMapping;
typedef ::xsd::cxx::tree::no_type_info<char> NoTypeInfo;
typedef ::xsd::cxx::tree::not_derived<char> NotDerived;
typedef ::xsd::cxx::tree::serialization<char> Serialization;

// Error handler callback interface.
//
typedef ::xsd::cxx::xml::error_handler<char> ErrorHandler;

// DOM interaction.
//
namespace dom {
// Automatic pointer for DOMDocument.
//
using ::xsd::cxx::xml::dom::unique_ptr;

#ifndef XSD_CXX_TREE_TREE_NODE_KEY__LINPHONEPRIVATE__XSD__XMLSCHEMA
#define XSD_CXX_TREE_TREE_NODE_KEY__LINPHONEPRIVATE__XSD__XMLSCHEMA
// DOM user data key for back pointers to tree nodes.
//
const XMLCh *const treeNodeKey = ::xsd::cxx::tree::user_data_keys::node;
#endif
} // namespace dom
} // namespace XmlSchema
} // namespace Xsd
} // namespace LinphonePrivate

// Forward declarations.
//
namespace LinphonePrivate {
namespace Xsd {
namespace ConferenceInfo {
class ConferenceType;
class StateType;
class ConferenceDescriptionType;
class HostType;
class ConferenceStateType;
class ConferenceMediaType;
class ConferenceMediumType;
class UrisType;
class UriType;
class KeywordsType;
class UsersType;
class UserType;
class UserRolesType;
class UserLanguagesType;
class EndpointType;
class EndpointStatusType;
class JoiningType;
class DisconnectionType;
class ExecutionType;
class CallType;
class SipDialogIdType;
class MediaType;
class MediaStatusType;
class SidebarsByValType;
} // namespace ConferenceInfo
} // namespace Xsd
} // namespace LinphonePrivate

#include <algorithm> // std::binary_search
#include <limits>    // std::numeric_limits
#include <memory>    // ::std::unique_ptr
#include <utility>   // std::move

#include <xsd/cxx/xml/char-utf8.hxx>

#include <xsd/cxx/tree/containers.hxx>
#include <xsd/cxx/tree/elements.hxx>
#include <xsd/cxx/tree/exceptions.hxx>
#include <xsd/cxx/tree/list.hxx>

#include <xsd/cxx/xml/dom/parsing-header.hxx>

#include <xsd/cxx/tree/containers-wildcard.hxx>

#include "xml.h"

namespace LinphonePrivate {
namespace Xsd {
namespace ConferenceInfo {
class ConferenceType : public ::LinphonePrivate::Xsd::XmlSchema::Type {
public:
	// conference-description
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::ConferenceDescriptionType ConferenceDescriptionType;
	typedef ::xsd::cxx::tree::optional<ConferenceDescriptionType> ConferenceDescriptionOptional;
	typedef ::xsd::cxx::tree::traits<ConferenceDescriptionType, char> ConferenceDescriptionTraits;

	const ConferenceDescriptionOptional &getConferenceDescription() const;

	ConferenceDescriptionOptional &getConferenceDescription();

	void setConferenceDescription(const ConferenceDescriptionType &x);

	void setConferenceDescription(const ConferenceDescriptionOptional &x);

	void setConferenceDescription(::std::unique_ptr<ConferenceDescriptionType> p);

	// host-info
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::HostType HostInfoType;
	typedef ::xsd::cxx::tree::optional<HostInfoType> HostInfoOptional;
	typedef ::xsd::cxx::tree::traits<HostInfoType, char> HostInfoTraits;

	const HostInfoOptional &getHostInfo() const;

	HostInfoOptional &getHostInfo();

	void setHostInfo(const HostInfoType &x);

	void setHostInfo(const HostInfoOptional &x);

	void setHostInfo(::std::unique_ptr<HostInfoType> p);

	// conference-state
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::ConferenceStateType ConferenceStateType;
	typedef ::xsd::cxx::tree::optional<ConferenceStateType> ConferenceStateOptional;
	typedef ::xsd::cxx::tree::traits<ConferenceStateType, char> ConferenceStateTraits;

	const ConferenceStateOptional &getConferenceState() const;

	ConferenceStateOptional &getConferenceState();

	void setConferenceState(const ConferenceStateType &x);

	void setConferenceState(const ConferenceStateOptional &x);

	void setConferenceState(::std::unique_ptr<ConferenceStateType> p);

	// users
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::UsersType UsersType;
	typedef ::xsd::cxx::tree::optional<UsersType> UsersOptional;
	typedef ::xsd::cxx::tree::traits<UsersType, char> UsersTraits;

	const UsersOptional &getUsers() const;

	UsersOptional &getUsers();

	void setUsers(const UsersType &x);

	void setUsers(const UsersOptional &x);

	void setUsers(::std::unique_ptr<UsersType> p);

	// sidebars-by-ref
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::UrisType SidebarsByRefType;
	typedef ::xsd::cxx::tree::optional<SidebarsByRefType> SidebarsByRefOptional;
	typedef ::xsd::cxx::tree::traits<SidebarsByRefType, char> SidebarsByRefTraits;

	const SidebarsByRefOptional &getSidebarsByRef() const;

	SidebarsByRefOptional &getSidebarsByRef();

	void setSidebarsByRef(const SidebarsByRefType &x);

	void setSidebarsByRef(const SidebarsByRefOptional &x);

	void setSidebarsByRef(::std::unique_ptr<SidebarsByRefType> p);

	// sidebars-by-val
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::SidebarsByValType SidebarsByValType;
	typedef ::xsd::cxx::tree::optional<SidebarsByValType> SidebarsByValOptional;
	typedef ::xsd::cxx::tree::traits<SidebarsByValType, char> SidebarsByValTraits;

	const SidebarsByValOptional &getSidebarsByVal() const;

	SidebarsByValOptional &getSidebarsByVal();

	void setSidebarsByVal(const SidebarsByValType &x);

	void setSidebarsByVal(const SidebarsByValOptional &x);

	void setSidebarsByVal(::std::unique_ptr<SidebarsByValType> p);

	// any
	//
	typedef ::xsd::cxx::tree::element_sequence AnySequence;
	typedef AnySequence::iterator AnyIterator;
	typedef AnySequence::const_iterator AnyConstIterator;

	const AnySequence &getAny() const;

	AnySequence &getAny();

	void setAny(const AnySequence &s);

	// entity
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::Uri EntityType;
	typedef ::xsd::cxx::tree::traits<EntityType, char> EntityTraits;

	const EntityType &getEntity() const;

	EntityType &getEntity();

	void setEntity(const EntityType &x);

	void setEntity(::std::unique_ptr<EntityType> p);

	::std::unique_ptr<EntityType> setDetachEntity();

	// state
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::StateType StateType;
	typedef ::xsd::cxx::tree::traits<StateType, char> StateTraits;

	const StateType &getState() const;

	StateType &getState();

	void setState(const StateType &x);

	void setState(::std::unique_ptr<StateType> p);

	::std::unique_ptr<StateType> setDetachState();

	static const StateType &getStateDefaultValue();

	// version
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::UnsignedInt VersionType;
	typedef ::xsd::cxx::tree::optional<VersionType> VersionOptional;
	typedef ::xsd::cxx::tree::traits<VersionType, char> VersionTraits;

	const VersionOptional &getVersion() const;

	VersionOptional &getVersion();

	void setVersion(const VersionType &x);

	void setVersion(const VersionOptional &x);

	// any_attribute
	//
	typedef ::xsd::cxx::tree::attribute_set<char> AnyAttributeSet;
	typedef AnyAttributeSet::iterator AnyAttributeIterator;
	typedef AnyAttributeSet::const_iterator AnyAttributeConstIterator;

	const AnyAttributeSet &getAnyAttribute() const;

	AnyAttributeSet &getAnyAttribute();

	void setAnyAttribute(const AnyAttributeSet &s);

	// DOMDocument for wildcard content.
	//
	const ::xercesc::DOMDocument &getDomDocument() const;

	::xercesc::DOMDocument &getDomDocument();

	// Constructors.
	//
	ConferenceType(const EntityType &);

	ConferenceType(const ::xercesc::DOMElement &e,
	               ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	               ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	ConferenceType(const ConferenceType &x,
	               ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	               ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	virtual ConferenceType *_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                               ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0) const;

	ConferenceType &operator=(const ConferenceType &x);

	virtual ~ConferenceType();

	// Implementation.
	//
protected:
	void parse(::xsd::cxx::xml::dom::parser<char> &, ::LinphonePrivate::Xsd::XmlSchema::Flags);

protected:
	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> dom_document_;

	ConferenceDescriptionOptional conference_description_;
	HostInfoOptional host_info_;
	ConferenceStateOptional conference_state_;
	UsersOptional users_;
	SidebarsByRefOptional sidebars_by_ref_;
	SidebarsByValOptional sidebars_by_val_;
	AnySequence any_;
	::xsd::cxx::tree::one<EntityType> entity_;
	::xsd::cxx::tree::one<StateType> state_;
	static const StateType state_default_value_;
	VersionOptional version_;
	AnyAttributeSet any_attribute_;
};

class StateType : public ::LinphonePrivate::Xsd::XmlSchema::String {
public:
	enum Value { full, partial, deleted };

	StateType(Value v);

	StateType(const char *v);

	StateType(const ::std::string &v);

	StateType(const ::LinphonePrivate::Xsd::XmlSchema::String &v);

	StateType(const ::xercesc::DOMElement &e,
	          ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	          ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	StateType(const ::xercesc::DOMAttr &a,
	          ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	          ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	StateType(const ::std::string &s,
	          const ::xercesc::DOMElement *e,
	          ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	          ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	StateType(const StateType &x,
	          ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	          ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	virtual StateType *_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                          ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0) const;

	StateType &operator=(Value v);

	virtual operator Value() const {
		return _xsd_StateType_convert();
	}

protected:
	Value _xsd_StateType_convert() const;

public:
	static const char *const _xsd_StateType_literals_[3];
	static const Value _xsd_StateType_indexes_[3];
};

class ConferenceDescriptionType : public ::LinphonePrivate::Xsd::XmlSchema::Type {
public:
	// display-text
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::String DisplayTextType;
	typedef ::xsd::cxx::tree::optional<DisplayTextType> DisplayTextOptional;
	typedef ::xsd::cxx::tree::traits<DisplayTextType, char> DisplayTextTraits;

	const DisplayTextOptional &getDisplayText() const;

	DisplayTextOptional &getDisplayText();

	void setDisplayText(const DisplayTextType &x);

	void setDisplayText(const DisplayTextOptional &x);

	void setDisplayText(::std::unique_ptr<DisplayTextType> p);

	// subject
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::String SubjectType;
	typedef ::xsd::cxx::tree::optional<SubjectType> SubjectOptional;
	typedef ::xsd::cxx::tree::traits<SubjectType, char> SubjectTraits;

	const SubjectOptional &getSubject() const;

	SubjectOptional &getSubject();

	void setSubject(const SubjectType &x);

	void setSubject(const SubjectOptional &x);

	void setSubject(::std::unique_ptr<SubjectType> p);

	// free-text
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::String FreeTextType;
	typedef ::xsd::cxx::tree::optional<FreeTextType> FreeTextOptional;
	typedef ::xsd::cxx::tree::traits<FreeTextType, char> FreeTextTraits;

	const FreeTextOptional &getFreeText() const;

	FreeTextOptional &getFreeText();

	void setFreeText(const FreeTextType &x);

	void setFreeText(const FreeTextOptional &x);

	void setFreeText(::std::unique_ptr<FreeTextType> p);

	// keywords
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::KeywordsType KeywordsType;
	typedef ::xsd::cxx::tree::optional<KeywordsType> KeywordsOptional;
	typedef ::xsd::cxx::tree::traits<KeywordsType, char> KeywordsTraits;

	const KeywordsOptional &getKeywords() const;

	KeywordsOptional &getKeywords();

	void setKeywords(const KeywordsType &x);

	void setKeywords(const KeywordsOptional &x);

	void setKeywords(::std::unique_ptr<KeywordsType> p);

	// conf-uris
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::UrisType ConfUrisType;
	typedef ::xsd::cxx::tree::optional<ConfUrisType> ConfUrisOptional;
	typedef ::xsd::cxx::tree::traits<ConfUrisType, char> ConfUrisTraits;

	const ConfUrisOptional &getConfUris() const;

	ConfUrisOptional &getConfUris();

	void setConfUris(const ConfUrisType &x);

	void setConfUris(const ConfUrisOptional &x);

	void setConfUris(::std::unique_ptr<ConfUrisType> p);

	// service-uris
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::UrisType ServiceUrisType;
	typedef ::xsd::cxx::tree::optional<ServiceUrisType> ServiceUrisOptional;
	typedef ::xsd::cxx::tree::traits<ServiceUrisType, char> ServiceUrisTraits;

	const ServiceUrisOptional &getServiceUris() const;

	ServiceUrisOptional &getServiceUris();

	void setServiceUris(const ServiceUrisType &x);

	void setServiceUris(const ServiceUrisOptional &x);

	void setServiceUris(::std::unique_ptr<ServiceUrisType> p);

	// maximum-user-count
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::UnsignedInt MaximumUserCountType;
	typedef ::xsd::cxx::tree::optional<MaximumUserCountType> MaximumUserCountOptional;
	typedef ::xsd::cxx::tree::traits<MaximumUserCountType, char> MaximumUserCountTraits;

	const MaximumUserCountOptional &getMaximumUserCount() const;

	MaximumUserCountOptional &getMaximumUserCount();

	void setMaximumUserCount(const MaximumUserCountType &x);

	void setMaximumUserCount(const MaximumUserCountOptional &x);

	// available-media
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::ConferenceMediaType AvailableMediaType;
	typedef ::xsd::cxx::tree::optional<AvailableMediaType> AvailableMediaOptional;
	typedef ::xsd::cxx::tree::traits<AvailableMediaType, char> AvailableMediaTraits;

	const AvailableMediaOptional &getAvailableMedia() const;

	AvailableMediaOptional &getAvailableMedia();

	void setAvailableMedia(const AvailableMediaType &x);

	void setAvailableMedia(const AvailableMediaOptional &x);

	void setAvailableMedia(::std::unique_ptr<AvailableMediaType> p);

	// any
	//
	typedef ::xsd::cxx::tree::element_sequence AnySequence;
	typedef AnySequence::iterator AnyIterator;
	typedef AnySequence::const_iterator AnyConstIterator;

	const AnySequence &getAny() const;

	AnySequence &getAny();

	void setAny(const AnySequence &s);

	// any_attribute
	//
	typedef ::xsd::cxx::tree::attribute_set<char> AnyAttributeSet;
	typedef AnyAttributeSet::iterator AnyAttributeIterator;
	typedef AnyAttributeSet::const_iterator AnyAttributeConstIterator;

	const AnyAttributeSet &getAnyAttribute() const;

	AnyAttributeSet &getAnyAttribute();

	void setAnyAttribute(const AnyAttributeSet &s);

	// DOMDocument for wildcard content.
	//
	const ::xercesc::DOMDocument &getDomDocument() const;

	::xercesc::DOMDocument &getDomDocument();

	// Constructors.
	//
	ConferenceDescriptionType();

	ConferenceDescriptionType(const ::xercesc::DOMElement &e,
	                          ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                          ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	ConferenceDescriptionType(const ConferenceDescriptionType &x,
	                          ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                          ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	virtual ConferenceDescriptionType *_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                                          ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0) const;

	ConferenceDescriptionType &operator=(const ConferenceDescriptionType &x);

	virtual ~ConferenceDescriptionType();

	// Implementation.
	//
protected:
	void parse(::xsd::cxx::xml::dom::parser<char> &, ::LinphonePrivate::Xsd::XmlSchema::Flags);

protected:
	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> dom_document_;

	DisplayTextOptional display_text_;
	SubjectOptional subject_;
	FreeTextOptional free_text_;
	KeywordsOptional keywords_;
	ConfUrisOptional conf_uris_;
	ServiceUrisOptional service_uris_;
	MaximumUserCountOptional maximum_user_count_;
	AvailableMediaOptional available_media_;
	AnySequence any_;
	AnyAttributeSet any_attribute_;
};

class HostType : public ::LinphonePrivate::Xsd::XmlSchema::Type {
public:
	// display-text
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::String DisplayTextType;
	typedef ::xsd::cxx::tree::optional<DisplayTextType> DisplayTextOptional;
	typedef ::xsd::cxx::tree::traits<DisplayTextType, char> DisplayTextTraits;

	const DisplayTextOptional &getDisplayText() const;

	DisplayTextOptional &getDisplayText();

	void setDisplayText(const DisplayTextType &x);

	void setDisplayText(const DisplayTextOptional &x);

	void setDisplayText(::std::unique_ptr<DisplayTextType> p);

	// web-page
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::Uri WebPageType;
	typedef ::xsd::cxx::tree::optional<WebPageType> WebPageOptional;
	typedef ::xsd::cxx::tree::traits<WebPageType, char> WebPageTraits;

	const WebPageOptional &getWebPage() const;

	WebPageOptional &getWebPage();

	void setWebPage(const WebPageType &x);

	void setWebPage(const WebPageOptional &x);

	void setWebPage(::std::unique_ptr<WebPageType> p);

	// uris
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::UrisType UrisType;
	typedef ::xsd::cxx::tree::optional<UrisType> UrisOptional;
	typedef ::xsd::cxx::tree::traits<UrisType, char> UrisTraits;

	const UrisOptional &getUris() const;

	UrisOptional &getUris();

	void setUris(const UrisType &x);

	void setUris(const UrisOptional &x);

	void setUris(::std::unique_ptr<UrisType> p);

	// any
	//
	typedef ::xsd::cxx::tree::element_sequence AnySequence;
	typedef AnySequence::iterator AnyIterator;
	typedef AnySequence::const_iterator AnyConstIterator;

	const AnySequence &getAny() const;

	AnySequence &getAny();

	void setAny(const AnySequence &s);

	// any_attribute
	//
	typedef ::xsd::cxx::tree::attribute_set<char> AnyAttributeSet;
	typedef AnyAttributeSet::iterator AnyAttributeIterator;
	typedef AnyAttributeSet::const_iterator AnyAttributeConstIterator;

	const AnyAttributeSet &getAnyAttribute() const;

	AnyAttributeSet &getAnyAttribute();

	void setAnyAttribute(const AnyAttributeSet &s);

	// DOMDocument for wildcard content.
	//
	const ::xercesc::DOMDocument &getDomDocument() const;

	::xercesc::DOMDocument &getDomDocument();

	// Constructors.
	//
	HostType();

	HostType(const ::xercesc::DOMElement &e,
	         ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	         ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	HostType(const HostType &x,
	         ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	         ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	virtual HostType *_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                         ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0) const;

	HostType &operator=(const HostType &x);

	virtual ~HostType();

	// Implementation.
	//
protected:
	void parse(::xsd::cxx::xml::dom::parser<char> &, ::LinphonePrivate::Xsd::XmlSchema::Flags);

protected:
	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> dom_document_;

	DisplayTextOptional display_text_;
	WebPageOptional web_page_;
	UrisOptional uris_;
	AnySequence any_;
	AnyAttributeSet any_attribute_;
};

class ConferenceStateType : public ::LinphonePrivate::Xsd::XmlSchema::Type {
public:
	// user-count
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::UnsignedInt UserCountType;
	typedef ::xsd::cxx::tree::optional<UserCountType> UserCountOptional;
	typedef ::xsd::cxx::tree::traits<UserCountType, char> UserCountTraits;

	const UserCountOptional &getUserCount() const;

	UserCountOptional &getUserCount();

	void setUserCount(const UserCountType &x);

	void setUserCount(const UserCountOptional &x);

	// active
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::Boolean ActiveType;
	typedef ::xsd::cxx::tree::optional<ActiveType> ActiveOptional;
	typedef ::xsd::cxx::tree::traits<ActiveType, char> ActiveTraits;

	const ActiveOptional &getActive() const;

	ActiveOptional &getActive();

	void setActive(const ActiveType &x);

	void setActive(const ActiveOptional &x);

	// locked
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::Boolean LockedType;
	typedef ::xsd::cxx::tree::optional<LockedType> LockedOptional;
	typedef ::xsd::cxx::tree::traits<LockedType, char> LockedTraits;

	const LockedOptional &getLocked() const;

	LockedOptional &getLocked();

	void setLocked(const LockedType &x);

	void setLocked(const LockedOptional &x);

	// any
	//
	typedef ::xsd::cxx::tree::element_sequence AnySequence;
	typedef AnySequence::iterator AnyIterator;
	typedef AnySequence::const_iterator AnyConstIterator;

	const AnySequence &getAny() const;

	AnySequence &getAny();

	void setAny(const AnySequence &s);

	// any_attribute
	//
	typedef ::xsd::cxx::tree::attribute_set<char> AnyAttributeSet;
	typedef AnyAttributeSet::iterator AnyAttributeIterator;
	typedef AnyAttributeSet::const_iterator AnyAttributeConstIterator;

	const AnyAttributeSet &getAnyAttribute() const;

	AnyAttributeSet &getAnyAttribute();

	void setAnyAttribute(const AnyAttributeSet &s);

	// DOMDocument for wildcard content.
	//
	const ::xercesc::DOMDocument &getDomDocument() const;

	::xercesc::DOMDocument &getDomDocument();

	// Constructors.
	//
	ConferenceStateType();

	ConferenceStateType(const ::xercesc::DOMElement &e,
	                    ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                    ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	ConferenceStateType(const ConferenceStateType &x,
	                    ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                    ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	virtual ConferenceStateType *_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                                    ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0) const;

	ConferenceStateType &operator=(const ConferenceStateType &x);

	virtual ~ConferenceStateType();

	// Implementation.
	//
protected:
	void parse(::xsd::cxx::xml::dom::parser<char> &, ::LinphonePrivate::Xsd::XmlSchema::Flags);

protected:
	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> dom_document_;

	UserCountOptional user_count_;
	ActiveOptional active_;
	LockedOptional locked_;
	AnySequence any_;
	AnyAttributeSet any_attribute_;
};

class ConferenceMediaType : public ::LinphonePrivate::Xsd::XmlSchema::Type {
public:
	// entry
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::ConferenceMediumType EntryType;
	typedef ::xsd::cxx::tree::sequence<EntryType> EntrySequence;
	typedef EntrySequence::iterator EntryIterator;
	typedef EntrySequence::const_iterator EntryConstIterator;
	typedef ::xsd::cxx::tree::traits<EntryType, char> EntryTraits;

	const EntrySequence &getEntry() const;

	EntrySequence &getEntry();

	void setEntry(const EntrySequence &s);

	// any_attribute
	//
	typedef ::xsd::cxx::tree::attribute_set<char> AnyAttributeSet;
	typedef AnyAttributeSet::iterator AnyAttributeIterator;
	typedef AnyAttributeSet::const_iterator AnyAttributeConstIterator;

	const AnyAttributeSet &getAnyAttribute() const;

	AnyAttributeSet &getAnyAttribute();

	void setAnyAttribute(const AnyAttributeSet &s);

	// DOMDocument for wildcard content.
	//
	const ::xercesc::DOMDocument &getDomDocument() const;

	::xercesc::DOMDocument &getDomDocument();

	// Constructors.
	//
	ConferenceMediaType();

	ConferenceMediaType(const ::xercesc::DOMElement &e,
	                    ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                    ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	ConferenceMediaType(const ConferenceMediaType &x,
	                    ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                    ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	virtual ConferenceMediaType *_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                                    ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0) const;

	ConferenceMediaType &operator=(const ConferenceMediaType &x);

	virtual ~ConferenceMediaType();

	// Implementation.
	//
protected:
	void parse(::xsd::cxx::xml::dom::parser<char> &, ::LinphonePrivate::Xsd::XmlSchema::Flags);

protected:
	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> dom_document_;

	EntrySequence entry_;
	AnyAttributeSet any_attribute_;
};

class ConferenceMediumType : public ::LinphonePrivate::Xsd::XmlSchema::Type {
public:
	// display-text
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::String DisplayTextType;
	typedef ::xsd::cxx::tree::optional<DisplayTextType> DisplayTextOptional;
	typedef ::xsd::cxx::tree::traits<DisplayTextType, char> DisplayTextTraits;

	const DisplayTextOptional &getDisplayText() const;

	DisplayTextOptional &getDisplayText();

	void setDisplayText(const DisplayTextType &x);

	void setDisplayText(const DisplayTextOptional &x);

	void setDisplayText(::std::unique_ptr<DisplayTextType> p);

	// type
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::String TypeType;
	typedef ::xsd::cxx::tree::traits<TypeType, char> TypeTraits;

	const TypeType &getType() const;

	TypeType &getType();

	void setType(const TypeType &x);

	void setType(::std::unique_ptr<TypeType> p);

	::std::unique_ptr<TypeType> setDetachType();

	// status
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::MediaStatusType StatusType;
	typedef ::xsd::cxx::tree::optional<StatusType> StatusOptional;
	typedef ::xsd::cxx::tree::traits<StatusType, char> StatusTraits;

	const StatusOptional &getStatus() const;

	StatusOptional &getStatus();

	void setStatus(const StatusType &x);

	void setStatus(const StatusOptional &x);

	void setStatus(::std::unique_ptr<StatusType> p);

	// any
	//
	typedef ::xsd::cxx::tree::element_sequence AnySequence;
	typedef AnySequence::iterator AnyIterator;
	typedef AnySequence::const_iterator AnyConstIterator;

	const AnySequence &getAny() const;

	AnySequence &getAny();

	void setAny(const AnySequence &s);

	// label
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::String LabelType;
	typedef ::xsd::cxx::tree::traits<LabelType, char> LabelTraits;

	const LabelType &getLabel() const;

	LabelType &getLabel();

	void setLabel(const LabelType &x);

	void setLabel(::std::unique_ptr<LabelType> p);

	::std::unique_ptr<LabelType> setDetachLabel();

	// any_attribute
	//
	typedef ::xsd::cxx::tree::attribute_set<char> AnyAttributeSet;
	typedef AnyAttributeSet::iterator AnyAttributeIterator;
	typedef AnyAttributeSet::const_iterator AnyAttributeConstIterator;

	const AnyAttributeSet &getAnyAttribute() const;

	AnyAttributeSet &getAnyAttribute();

	void setAnyAttribute(const AnyAttributeSet &s);

	// DOMDocument for wildcard content.
	//
	const ::xercesc::DOMDocument &getDomDocument() const;

	::xercesc::DOMDocument &getDomDocument();

	// Constructors.
	//
	ConferenceMediumType(const TypeType &, const LabelType &);

	ConferenceMediumType(const ::xercesc::DOMElement &e,
	                     ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                     ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	ConferenceMediumType(const ConferenceMediumType &x,
	                     ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                     ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	virtual ConferenceMediumType *_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                                     ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0) const;

	ConferenceMediumType &operator=(const ConferenceMediumType &x);

	virtual ~ConferenceMediumType();

	// Implementation.
	//
protected:
	void parse(::xsd::cxx::xml::dom::parser<char> &, ::LinphonePrivate::Xsd::XmlSchema::Flags);

protected:
	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> dom_document_;

	DisplayTextOptional display_text_;
	::xsd::cxx::tree::one<TypeType> type_;
	StatusOptional status_;
	AnySequence any_;
	::xsd::cxx::tree::one<LabelType> label_;
	AnyAttributeSet any_attribute_;
};

class UrisType : public ::LinphonePrivate::Xsd::XmlSchema::Type {
public:
	// entry
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::UriType EntryType;
	typedef ::xsd::cxx::tree::sequence<EntryType> EntrySequence;
	typedef EntrySequence::iterator EntryIterator;
	typedef EntrySequence::const_iterator EntryConstIterator;
	typedef ::xsd::cxx::tree::traits<EntryType, char> EntryTraits;

	const EntrySequence &getEntry() const;

	EntrySequence &getEntry();

	void setEntry(const EntrySequence &s);

	// state
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::StateType StateType;
	typedef ::xsd::cxx::tree::traits<StateType, char> StateTraits;

	const StateType &getState() const;

	StateType &getState();

	void setState(const StateType &x);

	void setState(::std::unique_ptr<StateType> p);

	::std::unique_ptr<StateType> setDetachState();

	static const StateType &getStateDefaultValue();

	// any_attribute
	//
	typedef ::xsd::cxx::tree::attribute_set<char> AnyAttributeSet;
	typedef AnyAttributeSet::iterator AnyAttributeIterator;
	typedef AnyAttributeSet::const_iterator AnyAttributeConstIterator;

	const AnyAttributeSet &getAnyAttribute() const;

	AnyAttributeSet &getAnyAttribute();

	void setAnyAttribute(const AnyAttributeSet &s);

	// DOMDocument for wildcard content.
	//
	const ::xercesc::DOMDocument &getDomDocument() const;

	::xercesc::DOMDocument &getDomDocument();

	// Constructors.
	//
	UrisType();

	UrisType(const ::xercesc::DOMElement &e,
	         ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	         ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	UrisType(const UrisType &x,
	         ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	         ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	virtual UrisType *_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                         ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0) const;

	UrisType &operator=(const UrisType &x);

	virtual ~UrisType();

	// Implementation.
	//
protected:
	void parse(::xsd::cxx::xml::dom::parser<char> &, ::LinphonePrivate::Xsd::XmlSchema::Flags);

protected:
	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> dom_document_;

	EntrySequence entry_;
	::xsd::cxx::tree::one<StateType> state_;
	static const StateType state_default_value_;
	AnyAttributeSet any_attribute_;
};

class UriType : public ::LinphonePrivate::Xsd::XmlSchema::Type {
public:
	// uri
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::Uri UriType1;
	typedef ::xsd::cxx::tree::traits<UriType1, char> UriTraits;

	const UriType1 &getUri() const;

	UriType1 &getUri();

	void setUri(const UriType1 &x);

	void setUri(::std::unique_ptr<UriType1> p);

	::std::unique_ptr<UriType1> setDetachUri();

	// display-text
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::String DisplayTextType;
	typedef ::xsd::cxx::tree::optional<DisplayTextType> DisplayTextOptional;
	typedef ::xsd::cxx::tree::traits<DisplayTextType, char> DisplayTextTraits;

	const DisplayTextOptional &getDisplayText() const;

	DisplayTextOptional &getDisplayText();

	void setDisplayText(const DisplayTextType &x);

	void setDisplayText(const DisplayTextOptional &x);

	void setDisplayText(::std::unique_ptr<DisplayTextType> p);

	// purpose
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::String PurposeType;
	typedef ::xsd::cxx::tree::optional<PurposeType> PurposeOptional;
	typedef ::xsd::cxx::tree::traits<PurposeType, char> PurposeTraits;

	const PurposeOptional &getPurpose() const;

	PurposeOptional &getPurpose();

	void setPurpose(const PurposeType &x);

	void setPurpose(const PurposeOptional &x);

	void setPurpose(::std::unique_ptr<PurposeType> p);

	// modified
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::ExecutionType ModifiedType;
	typedef ::xsd::cxx::tree::optional<ModifiedType> ModifiedOptional;
	typedef ::xsd::cxx::tree::traits<ModifiedType, char> ModifiedTraits;

	const ModifiedOptional &getModified() const;

	ModifiedOptional &getModified();

	void setModified(const ModifiedType &x);

	void setModified(const ModifiedOptional &x);

	void setModified(::std::unique_ptr<ModifiedType> p);

	// any
	//
	typedef ::xsd::cxx::tree::element_sequence AnySequence;
	typedef AnySequence::iterator AnyIterator;
	typedef AnySequence::const_iterator AnyConstIterator;

	const AnySequence &getAny() const;

	AnySequence &getAny();

	void setAny(const AnySequence &s);

	// any_attribute
	//
	typedef ::xsd::cxx::tree::attribute_set<char> AnyAttributeSet;
	typedef AnyAttributeSet::iterator AnyAttributeIterator;
	typedef AnyAttributeSet::const_iterator AnyAttributeConstIterator;

	const AnyAttributeSet &getAnyAttribute() const;

	AnyAttributeSet &getAnyAttribute();

	void setAnyAttribute(const AnyAttributeSet &s);

	// DOMDocument for wildcard content.
	//
	const ::xercesc::DOMDocument &getDomDocument() const;

	::xercesc::DOMDocument &getDomDocument();

	// Constructors.
	//
	UriType(const UriType1 &);

	UriType(const ::xercesc::DOMElement &e,
	        ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	        ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	UriType(const UriType &x,
	        ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	        ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	virtual UriType *_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                        ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0) const;

	UriType &operator=(const UriType &x);

	virtual ~UriType();

	// Implementation.
	//
protected:
	void parse(::xsd::cxx::xml::dom::parser<char> &, ::LinphonePrivate::Xsd::XmlSchema::Flags);

protected:
	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> dom_document_;

	::xsd::cxx::tree::one<UriType1> uri_;
	DisplayTextOptional display_text_;
	PurposeOptional purpose_;
	ModifiedOptional modified_;
	AnySequence any_;
	AnyAttributeSet any_attribute_;
};

class KeywordsType : public ::LinphonePrivate::Xsd::XmlSchema::SimpleType,
                     public ::xsd::cxx::tree::list<::LinphonePrivate::Xsd::XmlSchema::String, char> {
public:
	KeywordsType();

	KeywordsType(size_type n, const ::LinphonePrivate::Xsd::XmlSchema::String &x);

	template <typename I>
	KeywordsType(const I &begin, const I &end)
	    : ::xsd::cxx::tree::list<::LinphonePrivate::Xsd::XmlSchema::String, char>(begin, end, this) {
	}

	KeywordsType(const ::xercesc::DOMElement &e,
	             ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	             ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	KeywordsType(const ::xercesc::DOMAttr &a,
	             ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	             ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	KeywordsType(const ::std::string &s,
	             const ::xercesc::DOMElement *e,
	             ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	             ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	KeywordsType(const KeywordsType &x,
	             ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	             ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	virtual KeywordsType *_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                             ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0) const;

	virtual ~KeywordsType();
};

class UsersType : public ::LinphonePrivate::Xsd::XmlSchema::Type {
public:
	// user
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::UserType UserType;
	typedef ::xsd::cxx::tree::sequence<UserType> UserSequence;
	typedef UserSequence::iterator UserIterator;
	typedef UserSequence::const_iterator UserConstIterator;
	typedef ::xsd::cxx::tree::traits<UserType, char> UserTraits;

	const UserSequence &getUser() const;

	UserSequence &getUser();

	void setUser(const UserSequence &s);

	// any
	//
	typedef ::xsd::cxx::tree::element_sequence AnySequence;
	typedef AnySequence::iterator AnyIterator;
	typedef AnySequence::const_iterator AnyConstIterator;

	const AnySequence &getAny() const;

	AnySequence &getAny();

	void setAny(const AnySequence &s);

	// state
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::StateType StateType;
	typedef ::xsd::cxx::tree::traits<StateType, char> StateTraits;

	const StateType &getState() const;

	StateType &getState();

	void setState(const StateType &x);

	void setState(::std::unique_ptr<StateType> p);

	::std::unique_ptr<StateType> setDetachState();

	static const StateType &getStateDefaultValue();

	// any_attribute
	//
	typedef ::xsd::cxx::tree::attribute_set<char> AnyAttributeSet;
	typedef AnyAttributeSet::iterator AnyAttributeIterator;
	typedef AnyAttributeSet::const_iterator AnyAttributeConstIterator;

	const AnyAttributeSet &getAnyAttribute() const;

	AnyAttributeSet &getAnyAttribute();

	void setAnyAttribute(const AnyAttributeSet &s);

	// DOMDocument for wildcard content.
	//
	const ::xercesc::DOMDocument &getDomDocument() const;

	::xercesc::DOMDocument &getDomDocument();

	// Constructors.
	//
	UsersType();

	UsersType(const ::xercesc::DOMElement &e,
	          ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	          ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	UsersType(const UsersType &x,
	          ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	          ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	virtual UsersType *_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                          ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0) const;

	UsersType &operator=(const UsersType &x);

	virtual ~UsersType();

	// Implementation.
	//
protected:
	void parse(::xsd::cxx::xml::dom::parser<char> &, ::LinphonePrivate::Xsd::XmlSchema::Flags);

protected:
	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> dom_document_;

	UserSequence user_;
	AnySequence any_;
	::xsd::cxx::tree::one<StateType> state_;
	static const StateType state_default_value_;
	AnyAttributeSet any_attribute_;
};

class UserType : public ::LinphonePrivate::Xsd::XmlSchema::Type {
public:
	// display-text
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::String DisplayTextType;
	typedef ::xsd::cxx::tree::optional<DisplayTextType> DisplayTextOptional;
	typedef ::xsd::cxx::tree::traits<DisplayTextType, char> DisplayTextTraits;

	const DisplayTextOptional &getDisplayText() const;

	DisplayTextOptional &getDisplayText();

	void setDisplayText(const DisplayTextType &x);

	void setDisplayText(const DisplayTextOptional &x);

	void setDisplayText(::std::unique_ptr<DisplayTextType> p);

	// associated-aors
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::UrisType AssociatedAorsType;
	typedef ::xsd::cxx::tree::optional<AssociatedAorsType> AssociatedAorsOptional;
	typedef ::xsd::cxx::tree::traits<AssociatedAorsType, char> AssociatedAorsTraits;

	const AssociatedAorsOptional &getAssociatedAors() const;

	AssociatedAorsOptional &getAssociatedAors();

	void setAssociatedAors(const AssociatedAorsType &x);

	void setAssociatedAors(const AssociatedAorsOptional &x);

	void setAssociatedAors(::std::unique_ptr<AssociatedAorsType> p);

	// roles
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::UserRolesType RolesType;
	typedef ::xsd::cxx::tree::optional<RolesType> RolesOptional;
	typedef ::xsd::cxx::tree::traits<RolesType, char> RolesTraits;

	const RolesOptional &getRoles() const;

	RolesOptional &getRoles();

	void setRoles(const RolesType &x);

	void setRoles(const RolesOptional &x);

	void setRoles(::std::unique_ptr<RolesType> p);

	// languages
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::UserLanguagesType LanguagesType;
	typedef ::xsd::cxx::tree::optional<LanguagesType> LanguagesOptional;
	typedef ::xsd::cxx::tree::traits<LanguagesType, char> LanguagesTraits;

	const LanguagesOptional &getLanguages() const;

	LanguagesOptional &getLanguages();

	void setLanguages(const LanguagesType &x);

	void setLanguages(const LanguagesOptional &x);

	void setLanguages(::std::unique_ptr<LanguagesType> p);

	// cascaded-focus
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::Uri CascadedFocusType;
	typedef ::xsd::cxx::tree::optional<CascadedFocusType> CascadedFocusOptional;
	typedef ::xsd::cxx::tree::traits<CascadedFocusType, char> CascadedFocusTraits;

	const CascadedFocusOptional &getCascadedFocus() const;

	CascadedFocusOptional &getCascadedFocus();

	void setCascadedFocus(const CascadedFocusType &x);

	void setCascadedFocus(const CascadedFocusOptional &x);

	void setCascadedFocus(::std::unique_ptr<CascadedFocusType> p);

	// endpoint
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::EndpointType EndpointType;
	typedef ::xsd::cxx::tree::sequence<EndpointType> EndpointSequence;
	typedef EndpointSequence::iterator EndpointIterator;
	typedef EndpointSequence::const_iterator EndpointConstIterator;
	typedef ::xsd::cxx::tree::traits<EndpointType, char> EndpointTraits;

	const EndpointSequence &getEndpoint() const;

	EndpointSequence &getEndpoint();

	void setEndpoint(const EndpointSequence &s);

	// any
	//
	typedef ::xsd::cxx::tree::element_sequence AnySequence;
	typedef AnySequence::iterator AnyIterator;
	typedef AnySequence::const_iterator AnyConstIterator;

	const AnySequence &getAny() const;

	AnySequence &getAny();

	void setAny(const AnySequence &s);

	// entity
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::Uri EntityType;
	typedef ::xsd::cxx::tree::optional<EntityType> EntityOptional;
	typedef ::xsd::cxx::tree::traits<EntityType, char> EntityTraits;

	const EntityOptional &getEntity() const;

	EntityOptional &getEntity();

	void setEntity(const EntityType &x);

	void setEntity(const EntityOptional &x);

	void setEntity(::std::unique_ptr<EntityType> p);

	// state
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::StateType StateType;
	typedef ::xsd::cxx::tree::traits<StateType, char> StateTraits;

	const StateType &getState() const;

	StateType &getState();

	void setState(const StateType &x);

	void setState(::std::unique_ptr<StateType> p);

	::std::unique_ptr<StateType> setDetachState();

	static const StateType &getStateDefaultValue();

	// any_attribute
	//
	typedef ::xsd::cxx::tree::attribute_set<char> AnyAttributeSet;
	typedef AnyAttributeSet::iterator AnyAttributeIterator;
	typedef AnyAttributeSet::const_iterator AnyAttributeConstIterator;

	const AnyAttributeSet &getAnyAttribute() const;

	AnyAttributeSet &getAnyAttribute();

	void setAnyAttribute(const AnyAttributeSet &s);

	// DOMDocument for wildcard content.
	//
	const ::xercesc::DOMDocument &getDomDocument() const;

	::xercesc::DOMDocument &getDomDocument();

	// Constructors.
	//
	UserType();

	UserType(const ::xercesc::DOMElement &e,
	         ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	         ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	UserType(const UserType &x,
	         ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	         ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	virtual UserType *_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                         ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0) const;

	UserType &operator=(const UserType &x);

	virtual ~UserType();

	// Implementation.
	//
protected:
	void parse(::xsd::cxx::xml::dom::parser<char> &, ::LinphonePrivate::Xsd::XmlSchema::Flags);

protected:
	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> dom_document_;

	DisplayTextOptional display_text_;
	AssociatedAorsOptional associated_aors_;
	RolesOptional roles_;
	LanguagesOptional languages_;
	CascadedFocusOptional cascaded_focus_;
	EndpointSequence endpoint_;
	AnySequence any_;
	EntityOptional entity_;
	::xsd::cxx::tree::one<StateType> state_;
	static const StateType state_default_value_;
	AnyAttributeSet any_attribute_;
};

class UserRolesType : public ::LinphonePrivate::Xsd::XmlSchema::Type {
public:
	// entry
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::String EntryType;
	typedef ::xsd::cxx::tree::sequence<EntryType> EntrySequence;
	typedef EntrySequence::iterator EntryIterator;
	typedef EntrySequence::const_iterator EntryConstIterator;
	typedef ::xsd::cxx::tree::traits<EntryType, char> EntryTraits;

	const EntrySequence &getEntry() const;

	EntrySequence &getEntry();

	void setEntry(const EntrySequence &s);

	// any_attribute
	//
	typedef ::xsd::cxx::tree::attribute_set<char> AnyAttributeSet;
	typedef AnyAttributeSet::iterator AnyAttributeIterator;
	typedef AnyAttributeSet::const_iterator AnyAttributeConstIterator;

	const AnyAttributeSet &getAnyAttribute() const;

	AnyAttributeSet &getAnyAttribute();

	void setAnyAttribute(const AnyAttributeSet &s);

	// DOMDocument for wildcard content.
	//
	const ::xercesc::DOMDocument &getDomDocument() const;

	::xercesc::DOMDocument &getDomDocument();

	// Constructors.
	//
	UserRolesType();

	UserRolesType(const ::xercesc::DOMElement &e,
	              ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	              ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	UserRolesType(const UserRolesType &x,
	              ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	              ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	virtual UserRolesType *_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                              ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0) const;

	UserRolesType &operator=(const UserRolesType &x);

	virtual ~UserRolesType();

	// Implementation.
	//
protected:
	void parse(::xsd::cxx::xml::dom::parser<char> &, ::LinphonePrivate::Xsd::XmlSchema::Flags);

protected:
	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> dom_document_;

	EntrySequence entry_;
	AnyAttributeSet any_attribute_;
};

class UserLanguagesType : public ::LinphonePrivate::Xsd::XmlSchema::SimpleType,
                          public ::xsd::cxx::tree::list<::LinphonePrivate::Xsd::XmlSchema::Language, char> {
public:
	UserLanguagesType();

	UserLanguagesType(size_type n, const ::LinphonePrivate::Xsd::XmlSchema::Language &x);

	template <typename I>
	UserLanguagesType(const I &begin, const I &end)
	    : ::xsd::cxx::tree::list<::LinphonePrivate::Xsd::XmlSchema::Language, char>(begin, end, this) {
	}

	UserLanguagesType(const ::xercesc::DOMElement &e,
	                  ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                  ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	UserLanguagesType(const ::xercesc::DOMAttr &a,
	                  ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                  ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	UserLanguagesType(const ::std::string &s,
	                  const ::xercesc::DOMElement *e,
	                  ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                  ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	UserLanguagesType(const UserLanguagesType &x,
	                  ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                  ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	virtual UserLanguagesType *_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                                  ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0) const;

	virtual ~UserLanguagesType();
};

class EndpointType : public ::LinphonePrivate::Xsd::XmlSchema::Type {
public:
	// display-text
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::String DisplayTextType;
	typedef ::xsd::cxx::tree::optional<DisplayTextType> DisplayTextOptional;
	typedef ::xsd::cxx::tree::traits<DisplayTextType, char> DisplayTextTraits;

	const DisplayTextOptional &getDisplayText() const;

	DisplayTextOptional &getDisplayText();

	void setDisplayText(const DisplayTextType &x);

	void setDisplayText(const DisplayTextOptional &x);

	void setDisplayText(::std::unique_ptr<DisplayTextType> p);

	// referred
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::ExecutionType ReferredType;
	typedef ::xsd::cxx::tree::optional<ReferredType> ReferredOptional;
	typedef ::xsd::cxx::tree::traits<ReferredType, char> ReferredTraits;

	const ReferredOptional &getReferred() const;

	ReferredOptional &getReferred();

	void setReferred(const ReferredType &x);

	void setReferred(const ReferredOptional &x);

	void setReferred(::std::unique_ptr<ReferredType> p);

	// status
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::EndpointStatusType StatusType;
	typedef ::xsd::cxx::tree::optional<StatusType> StatusOptional;
	typedef ::xsd::cxx::tree::traits<StatusType, char> StatusTraits;

	const StatusOptional &getStatus() const;

	StatusOptional &getStatus();

	void setStatus(const StatusType &x);

	void setStatus(const StatusOptional &x);

	void setStatus(::std::unique_ptr<StatusType> p);

	// joining-method
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::JoiningType JoiningMethodType;
	typedef ::xsd::cxx::tree::optional<JoiningMethodType> JoiningMethodOptional;
	typedef ::xsd::cxx::tree::traits<JoiningMethodType, char> JoiningMethodTraits;

	const JoiningMethodOptional &getJoiningMethod() const;

	JoiningMethodOptional &getJoiningMethod();

	void setJoiningMethod(const JoiningMethodType &x);

	void setJoiningMethod(const JoiningMethodOptional &x);

	void setJoiningMethod(::std::unique_ptr<JoiningMethodType> p);

	// joining-info
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::ExecutionType JoiningInfoType;
	typedef ::xsd::cxx::tree::optional<JoiningInfoType> JoiningInfoOptional;
	typedef ::xsd::cxx::tree::traits<JoiningInfoType, char> JoiningInfoTraits;

	const JoiningInfoOptional &getJoiningInfo() const;

	JoiningInfoOptional &getJoiningInfo();

	void setJoiningInfo(const JoiningInfoType &x);

	void setJoiningInfo(const JoiningInfoOptional &x);

	void setJoiningInfo(::std::unique_ptr<JoiningInfoType> p);

	// disconnection-method
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::DisconnectionType DisconnectionMethodType;
	typedef ::xsd::cxx::tree::optional<DisconnectionMethodType> DisconnectionMethodOptional;
	typedef ::xsd::cxx::tree::traits<DisconnectionMethodType, char> DisconnectionMethodTraits;

	const DisconnectionMethodOptional &getDisconnectionMethod() const;

	DisconnectionMethodOptional &getDisconnectionMethod();

	void setDisconnectionMethod(const DisconnectionMethodType &x);

	void setDisconnectionMethod(const DisconnectionMethodOptional &x);

	void setDisconnectionMethod(::std::unique_ptr<DisconnectionMethodType> p);

	// disconnection-info
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::ExecutionType DisconnectionInfoType;
	typedef ::xsd::cxx::tree::optional<DisconnectionInfoType> DisconnectionInfoOptional;
	typedef ::xsd::cxx::tree::traits<DisconnectionInfoType, char> DisconnectionInfoTraits;

	const DisconnectionInfoOptional &getDisconnectionInfo() const;

	DisconnectionInfoOptional &getDisconnectionInfo();

	void setDisconnectionInfo(const DisconnectionInfoType &x);

	void setDisconnectionInfo(const DisconnectionInfoOptional &x);

	void setDisconnectionInfo(::std::unique_ptr<DisconnectionInfoType> p);

	// media
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::MediaType MediaType;
	typedef ::xsd::cxx::tree::sequence<MediaType> MediaSequence;
	typedef MediaSequence::iterator MediaIterator;
	typedef MediaSequence::const_iterator MediaConstIterator;
	typedef ::xsd::cxx::tree::traits<MediaType, char> MediaTraits;

	const MediaSequence &getMedia() const;

	MediaSequence &getMedia();

	void setMedia(const MediaSequence &s);

	// call-info
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::CallType CallInfoType;
	typedef ::xsd::cxx::tree::optional<CallInfoType> CallInfoOptional;
	typedef ::xsd::cxx::tree::traits<CallInfoType, char> CallInfoTraits;

	const CallInfoOptional &getCallInfo() const;

	CallInfoOptional &getCallInfo();

	void setCallInfo(const CallInfoType &x);

	void setCallInfo(const CallInfoOptional &x);

	void setCallInfo(::std::unique_ptr<CallInfoType> p);

	// any
	//
	typedef ::xsd::cxx::tree::element_sequence AnySequence;
	typedef AnySequence::iterator AnyIterator;
	typedef AnySequence::const_iterator AnyConstIterator;

	const AnySequence &getAny() const;

	AnySequence &getAny();

	void setAny(const AnySequence &s);

	// entity
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::String EntityType;
	typedef ::xsd::cxx::tree::optional<EntityType> EntityOptional;
	typedef ::xsd::cxx::tree::traits<EntityType, char> EntityTraits;

	const EntityOptional &getEntity() const;

	EntityOptional &getEntity();

	void setEntity(const EntityType &x);

	void setEntity(const EntityOptional &x);

	void setEntity(::std::unique_ptr<EntityType> p);

	// state
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::StateType StateType;
	typedef ::xsd::cxx::tree::traits<StateType, char> StateTraits;

	const StateType &getState() const;

	StateType &getState();

	void setState(const StateType &x);

	void setState(::std::unique_ptr<StateType> p);

	::std::unique_ptr<StateType> setDetachState();

	static const StateType &getStateDefaultValue();

	// any_attribute
	//
	typedef ::xsd::cxx::tree::attribute_set<char> AnyAttributeSet;
	typedef AnyAttributeSet::iterator AnyAttributeIterator;
	typedef AnyAttributeSet::const_iterator AnyAttributeConstIterator;

	const AnyAttributeSet &getAnyAttribute() const;

	AnyAttributeSet &getAnyAttribute();

	void setAnyAttribute(const AnyAttributeSet &s);

	// DOMDocument for wildcard content.
	//
	const ::xercesc::DOMDocument &getDomDocument() const;

	::xercesc::DOMDocument &getDomDocument();

	// Constructors.
	//
	EndpointType();

	EndpointType(const ::xercesc::DOMElement &e,
	             ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	             ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	EndpointType(const EndpointType &x,
	             ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	             ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	virtual EndpointType *_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                             ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0) const;

	EndpointType &operator=(const EndpointType &x);

	virtual ~EndpointType();

	// Implementation.
	//
protected:
	void parse(::xsd::cxx::xml::dom::parser<char> &, ::LinphonePrivate::Xsd::XmlSchema::Flags);

protected:
	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> dom_document_;

	DisplayTextOptional display_text_;
	ReferredOptional referred_;
	StatusOptional status_;
	JoiningMethodOptional joining_method_;
	JoiningInfoOptional joining_info_;
	DisconnectionMethodOptional disconnection_method_;
	DisconnectionInfoOptional disconnection_info_;
	MediaSequence media_;
	CallInfoOptional call_info_;
	AnySequence any_;
	EntityOptional entity_;
	::xsd::cxx::tree::one<StateType> state_;
	static const StateType state_default_value_;
	AnyAttributeSet any_attribute_;
};

class EndpointStatusType : public ::LinphonePrivate::Xsd::XmlSchema::String {
public:
	enum Value {
		pending,
		dialing_out,
		dialing_in,
		alerting,
		on_hold,
		connected,
		muted_via_focus,
		disconnecting,
		disconnected
	};

	EndpointStatusType(Value v);

	EndpointStatusType(const char *v);

	EndpointStatusType(const ::std::string &v);

	EndpointStatusType(const ::LinphonePrivate::Xsd::XmlSchema::String &v);

	EndpointStatusType(const ::xercesc::DOMElement &e,
	                   ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                   ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	EndpointStatusType(const ::xercesc::DOMAttr &a,
	                   ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                   ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	EndpointStatusType(const ::std::string &s,
	                   const ::xercesc::DOMElement *e,
	                   ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                   ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	EndpointStatusType(const EndpointStatusType &x,
	                   ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                   ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	virtual EndpointStatusType *_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                                   ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0) const;

	EndpointStatusType &operator=(Value v);

	virtual operator Value() const {
		return _xsd_EndpointStatusType_convert();
	}

protected:
	Value _xsd_EndpointStatusType_convert() const;

public:
	static const char *const _xsd_EndpointStatusType_literals_[9];
	static const Value _xsd_EndpointStatusType_indexes_[9];
};

class JoiningType : public ::LinphonePrivate::Xsd::XmlSchema::String {
public:
	enum Value { dialed_in, dialed_out, focus_owner };

	JoiningType(Value v);

	JoiningType(const char *v);

	JoiningType(const ::std::string &v);

	JoiningType(const ::LinphonePrivate::Xsd::XmlSchema::String &v);

	JoiningType(const ::xercesc::DOMElement &e,
	            ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	            ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	JoiningType(const ::xercesc::DOMAttr &a,
	            ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	            ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	JoiningType(const ::std::string &s,
	            const ::xercesc::DOMElement *e,
	            ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	            ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	JoiningType(const JoiningType &x,
	            ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	            ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	virtual JoiningType *_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                            ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0) const;

	JoiningType &operator=(Value v);

	virtual operator Value() const {
		return _xsd_JoiningType_convert();
	}

protected:
	Value _xsd_JoiningType_convert() const;

public:
	static const char *const _xsd_JoiningType_literals_[3];
	static const Value _xsd_JoiningType_indexes_[3];
};

class DisconnectionType : public ::LinphonePrivate::Xsd::XmlSchema::String {
public:
	enum Value { departed, booted, failed, busy };

	DisconnectionType(Value v);

	DisconnectionType(const char *v);

	DisconnectionType(const ::std::string &v);

	DisconnectionType(const ::LinphonePrivate::Xsd::XmlSchema::String &v);

	DisconnectionType(const ::xercesc::DOMElement &e,
	                  ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                  ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	DisconnectionType(const ::xercesc::DOMAttr &a,
	                  ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                  ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	DisconnectionType(const ::std::string &s,
	                  const ::xercesc::DOMElement *e,
	                  ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                  ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	DisconnectionType(const DisconnectionType &x,
	                  ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                  ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	virtual DisconnectionType *_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                                  ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0) const;

	DisconnectionType &operator=(Value v);

	virtual operator Value() const {
		return _xsd_DisconnectionType_convert();
	}

protected:
	Value _xsd_DisconnectionType_convert() const;

public:
	static const char *const _xsd_DisconnectionType_literals_[4];
	static const Value _xsd_DisconnectionType_indexes_[4];
};

class ExecutionType : public ::LinphonePrivate::Xsd::XmlSchema::Type {
public:
	// when
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::DateTime WhenType;
	typedef ::xsd::cxx::tree::optional<WhenType> WhenOptional;
	typedef ::xsd::cxx::tree::traits<WhenType, char> WhenTraits;

	const WhenOptional &getWhen() const;

	WhenOptional &getWhen();

	void setWhen(const WhenType &x);

	void setWhen(const WhenOptional &x);

	void setWhen(::std::unique_ptr<WhenType> p);

	// reason
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::String ReasonType;
	typedef ::xsd::cxx::tree::optional<ReasonType> ReasonOptional;
	typedef ::xsd::cxx::tree::traits<ReasonType, char> ReasonTraits;

	const ReasonOptional &getReason() const;

	ReasonOptional &getReason();

	void setReason(const ReasonType &x);

	void setReason(const ReasonOptional &x);

	void setReason(::std::unique_ptr<ReasonType> p);

	// by
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::Uri ByType;
	typedef ::xsd::cxx::tree::optional<ByType> ByOptional;
	typedef ::xsd::cxx::tree::traits<ByType, char> ByTraits;

	const ByOptional &getBy() const;

	ByOptional &getBy();

	void setBy(const ByType &x);

	void setBy(const ByOptional &x);

	void setBy(::std::unique_ptr<ByType> p);

	// any_attribute
	//
	typedef ::xsd::cxx::tree::attribute_set<char> AnyAttributeSet;
	typedef AnyAttributeSet::iterator AnyAttributeIterator;
	typedef AnyAttributeSet::const_iterator AnyAttributeConstIterator;

	const AnyAttributeSet &getAnyAttribute() const;

	AnyAttributeSet &getAnyAttribute();

	void setAnyAttribute(const AnyAttributeSet &s);

	// DOMDocument for wildcard content.
	//
	const ::xercesc::DOMDocument &getDomDocument() const;

	::xercesc::DOMDocument &getDomDocument();

	// Constructors.
	//
	ExecutionType();

	ExecutionType(const ::xercesc::DOMElement &e,
	              ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	              ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	ExecutionType(const ExecutionType &x,
	              ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	              ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	virtual ExecutionType *_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                              ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0) const;

	ExecutionType &operator=(const ExecutionType &x);

	virtual ~ExecutionType();

	// Implementation.
	//
protected:
	void parse(::xsd::cxx::xml::dom::parser<char> &, ::LinphonePrivate::Xsd::XmlSchema::Flags);

protected:
	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> dom_document_;

	WhenOptional when_;
	ReasonOptional reason_;
	ByOptional by_;
	AnyAttributeSet any_attribute_;
};

class CallType : public ::LinphonePrivate::Xsd::XmlSchema::Type {
public:
	// sip
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::SipDialogIdType SipType;
	typedef ::xsd::cxx::tree::optional<SipType> SipOptional;
	typedef ::xsd::cxx::tree::traits<SipType, char> SipTraits;

	const SipOptional &getSip() const;

	SipOptional &getSip();

	void setSip(const SipType &x);

	void setSip(const SipOptional &x);

	void setSip(::std::unique_ptr<SipType> p);

	// any
	//
	typedef ::xsd::cxx::tree::element_sequence AnySequence;
	typedef AnySequence::iterator AnyIterator;
	typedef AnySequence::const_iterator AnyConstIterator;

	const AnySequence &getAny() const;

	AnySequence &getAny();

	void setAny(const AnySequence &s);

	// any_attribute
	//
	typedef ::xsd::cxx::tree::attribute_set<char> AnyAttributeSet;
	typedef AnyAttributeSet::iterator AnyAttributeIterator;
	typedef AnyAttributeSet::const_iterator AnyAttributeConstIterator;

	const AnyAttributeSet &getAnyAttribute() const;

	AnyAttributeSet &getAnyAttribute();

	void setAnyAttribute(const AnyAttributeSet &s);

	// DOMDocument for wildcard content.
	//
	const ::xercesc::DOMDocument &getDomDocument() const;

	::xercesc::DOMDocument &getDomDocument();

	// Constructors.
	//
	CallType();

	CallType(const ::xercesc::DOMElement &e,
	         ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	         ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	CallType(const CallType &x,
	         ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	         ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	virtual CallType *_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                         ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0) const;

	CallType &operator=(const CallType &x);

	virtual ~CallType();

	// Implementation.
	//
protected:
	void parse(::xsd::cxx::xml::dom::parser<char> &, ::LinphonePrivate::Xsd::XmlSchema::Flags);

protected:
	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> dom_document_;

	SipOptional sip_;
	AnySequence any_;
	AnyAttributeSet any_attribute_;
};

class SipDialogIdType : public ::LinphonePrivate::Xsd::XmlSchema::Type {
public:
	// display-text
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::String DisplayTextType;
	typedef ::xsd::cxx::tree::optional<DisplayTextType> DisplayTextOptional;
	typedef ::xsd::cxx::tree::traits<DisplayTextType, char> DisplayTextTraits;

	const DisplayTextOptional &getDisplayText() const;

	DisplayTextOptional &getDisplayText();

	void setDisplayText(const DisplayTextType &x);

	void setDisplayText(const DisplayTextOptional &x);

	void setDisplayText(::std::unique_ptr<DisplayTextType> p);

	// call-id
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::String CallIdType;
	typedef ::xsd::cxx::tree::traits<CallIdType, char> CallIdTraits;

	const CallIdType &getCallId() const;

	CallIdType &getCallId();

	void setCallId(const CallIdType &x);

	void setCallId(::std::unique_ptr<CallIdType> p);

	::std::unique_ptr<CallIdType> setDetachCall_id();

	// from-tag
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::String FromTagType;
	typedef ::xsd::cxx::tree::traits<FromTagType, char> FromTagTraits;

	const FromTagType &getFromTag() const;

	FromTagType &getFromTag();

	void setFromTag(const FromTagType &x);

	void setFromTag(::std::unique_ptr<FromTagType> p);

	::std::unique_ptr<FromTagType> setDetachFrom_tag();

	// to-tag
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::String ToTagType;
	typedef ::xsd::cxx::tree::traits<ToTagType, char> ToTagTraits;

	const ToTagType &getToTag() const;

	ToTagType &getToTag();

	void setToTag(const ToTagType &x);

	void setToTag(::std::unique_ptr<ToTagType> p);

	::std::unique_ptr<ToTagType> setDetachTo_tag();

	// any
	//
	typedef ::xsd::cxx::tree::element_sequence AnySequence;
	typedef AnySequence::iterator AnyIterator;
	typedef AnySequence::const_iterator AnyConstIterator;

	const AnySequence &getAny() const;

	AnySequence &getAny();

	void setAny(const AnySequence &s);

	// any_attribute
	//
	typedef ::xsd::cxx::tree::attribute_set<char> AnyAttributeSet;
	typedef AnyAttributeSet::iterator AnyAttributeIterator;
	typedef AnyAttributeSet::const_iterator AnyAttributeConstIterator;

	const AnyAttributeSet &getAnyAttribute() const;

	AnyAttributeSet &getAnyAttribute();

	void setAnyAttribute(const AnyAttributeSet &s);

	// DOMDocument for wildcard content.
	//
	const ::xercesc::DOMDocument &getDomDocument() const;

	::xercesc::DOMDocument &getDomDocument();

	// Constructors.
	//
	SipDialogIdType(const CallIdType &, const FromTagType &, const ToTagType &);

	SipDialogIdType(const ::xercesc::DOMElement &e,
	                ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	SipDialogIdType(const SipDialogIdType &x,
	                ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	virtual SipDialogIdType *_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                                ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0) const;

	SipDialogIdType &operator=(const SipDialogIdType &x);

	virtual ~SipDialogIdType();

	// Implementation.
	//
protected:
	void parse(::xsd::cxx::xml::dom::parser<char> &, ::LinphonePrivate::Xsd::XmlSchema::Flags);

protected:
	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> dom_document_;

	DisplayTextOptional display_text_;
	::xsd::cxx::tree::one<CallIdType> call_id_;
	::xsd::cxx::tree::one<FromTagType> from_tag_;
	::xsd::cxx::tree::one<ToTagType> to_tag_;
	AnySequence any_;
	AnyAttributeSet any_attribute_;
};

class MediaType : public ::LinphonePrivate::Xsd::XmlSchema::Type {
public:
	// display-text
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::String DisplayTextType;
	typedef ::xsd::cxx::tree::optional<DisplayTextType> DisplayTextOptional;
	typedef ::xsd::cxx::tree::traits<DisplayTextType, char> DisplayTextTraits;

	const DisplayTextOptional &getDisplayText() const;

	DisplayTextOptional &getDisplayText();

	void setDisplayText(const DisplayTextType &x);

	void setDisplayText(const DisplayTextOptional &x);

	void setDisplayText(::std::unique_ptr<DisplayTextType> p);

	// type
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::String TypeType;
	typedef ::xsd::cxx::tree::optional<TypeType> TypeOptional;
	typedef ::xsd::cxx::tree::traits<TypeType, char> TypeTraits;

	const TypeOptional &getType() const;

	TypeOptional &getType();

	void setType(const TypeType &x);

	void setType(const TypeOptional &x);

	void setType(::std::unique_ptr<TypeType> p);

	// label
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::String LabelType;
	typedef ::xsd::cxx::tree::optional<LabelType> LabelOptional;
	typedef ::xsd::cxx::tree::traits<LabelType, char> LabelTraits;

	const LabelOptional &getLabel() const;

	LabelOptional &getLabel();

	void setLabel(const LabelType &x);

	void setLabel(const LabelOptional &x);

	void setLabel(::std::unique_ptr<LabelType> p);

	// src-id
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::String SrcIdType;
	typedef ::xsd::cxx::tree::optional<SrcIdType> SrcIdOptional;
	typedef ::xsd::cxx::tree::traits<SrcIdType, char> SrcIdTraits;

	const SrcIdOptional &getSrcId() const;

	SrcIdOptional &getSrcId();

	void setSrcId(const SrcIdType &x);

	void setSrcId(const SrcIdOptional &x);

	void setSrcId(::std::unique_ptr<SrcIdType> p);

	// status
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::MediaStatusType StatusType;
	typedef ::xsd::cxx::tree::optional<StatusType> StatusOptional;
	typedef ::xsd::cxx::tree::traits<StatusType, char> StatusTraits;

	const StatusOptional &getStatus() const;

	StatusOptional &getStatus();

	void setStatus(const StatusType &x);

	void setStatus(const StatusOptional &x);

	void setStatus(::std::unique_ptr<StatusType> p);

	// any
	//
	typedef ::xsd::cxx::tree::element_sequence AnySequence;
	typedef AnySequence::iterator AnyIterator;
	typedef AnySequence::const_iterator AnyConstIterator;

	const AnySequence &getAny() const;

	AnySequence &getAny();

	void setAny(const AnySequence &s);

	// id
	//
	typedef ::LinphonePrivate::Xsd::XmlSchema::String IdType;
	typedef ::xsd::cxx::tree::traits<IdType, char> IdTraits;

	const IdType &getId() const;

	IdType &getId();

	void setId(const IdType &x);

	void setId(::std::unique_ptr<IdType> p);

	::std::unique_ptr<IdType> setDetachId();

	// any_attribute
	//
	typedef ::xsd::cxx::tree::attribute_set<char> AnyAttributeSet;
	typedef AnyAttributeSet::iterator AnyAttributeIterator;
	typedef AnyAttributeSet::const_iterator AnyAttributeConstIterator;

	const AnyAttributeSet &getAnyAttribute() const;

	AnyAttributeSet &getAnyAttribute();

	void setAnyAttribute(const AnyAttributeSet &s);

	// DOMDocument for wildcard content.
	//
	const ::xercesc::DOMDocument &getDomDocument() const;

	::xercesc::DOMDocument &getDomDocument();

	// Constructors.
	//
	MediaType(const IdType &);

	MediaType(const ::xercesc::DOMElement &e,
	          ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	          ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	MediaType(const MediaType &x,
	          ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	          ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	virtual MediaType *_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                          ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0) const;

	MediaType &operator=(const MediaType &x);

	virtual ~MediaType();

	// Implementation.
	//
protected:
	void parse(::xsd::cxx::xml::dom::parser<char> &, ::LinphonePrivate::Xsd::XmlSchema::Flags);

protected:
	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> dom_document_;

	DisplayTextOptional display_text_;
	TypeOptional type_;
	LabelOptional label_;
	SrcIdOptional src_id_;
	StatusOptional status_;
	AnySequence any_;
	::xsd::cxx::tree::one<IdType> id_;
	AnyAttributeSet any_attribute_;
};

class MediaStatusType : public ::LinphonePrivate::Xsd::XmlSchema::String {
public:
	enum Value { recvonly, sendonly, sendrecv, inactive };

	MediaStatusType(Value v);

	MediaStatusType(const char *v);

	MediaStatusType(const ::std::string &v);

	MediaStatusType(const ::LinphonePrivate::Xsd::XmlSchema::String &v);

	MediaStatusType(const ::xercesc::DOMElement &e,
	                ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	MediaStatusType(const ::xercesc::DOMAttr &a,
	                ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	MediaStatusType(const ::std::string &s,
	                const ::xercesc::DOMElement *e,
	                ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	MediaStatusType(const MediaStatusType &x,
	                ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	virtual MediaStatusType *_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                                ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0) const;

	MediaStatusType &operator=(Value v);

	virtual operator Value() const {
		return _xsd_MediaStatusType_convert();
	}

protected:
	Value _xsd_MediaStatusType_convert() const;

public:
	static const char *const _xsd_MediaStatusType_literals_[4];
	static const Value _xsd_MediaStatusType_indexes_[4];
};

class SidebarsByValType : public ::LinphonePrivate::Xsd::XmlSchema::Type {
public:
	// entry
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::ConferenceType EntryType;
	typedef ::xsd::cxx::tree::sequence<EntryType> EntrySequence;
	typedef EntrySequence::iterator EntryIterator;
	typedef EntrySequence::const_iterator EntryConstIterator;
	typedef ::xsd::cxx::tree::traits<EntryType, char> EntryTraits;

	const EntrySequence &getEntry() const;

	EntrySequence &getEntry();

	void setEntry(const EntrySequence &s);

	// state
	//
	typedef ::LinphonePrivate::Xsd::ConferenceInfo::StateType StateType;
	typedef ::xsd::cxx::tree::traits<StateType, char> StateTraits;

	const StateType &getState() const;

	StateType &getState();

	void setState(const StateType &x);

	void setState(::std::unique_ptr<StateType> p);

	::std::unique_ptr<StateType> setDetachState();

	static const StateType &getStateDefaultValue();

	// any_attribute
	//
	typedef ::xsd::cxx::tree::attribute_set<char> AnyAttributeSet;
	typedef AnyAttributeSet::iterator AnyAttributeIterator;
	typedef AnyAttributeSet::const_iterator AnyAttributeConstIterator;

	const AnyAttributeSet &getAnyAttribute() const;

	AnyAttributeSet &getAnyAttribute();

	void setAnyAttribute(const AnyAttributeSet &s);

	// DOMDocument for wildcard content.
	//
	const ::xercesc::DOMDocument &getDomDocument() const;

	::xercesc::DOMDocument &getDomDocument();

	// Constructors.
	//
	SidebarsByValType();

	SidebarsByValType(const ::xercesc::DOMElement &e,
	                  ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                  ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	SidebarsByValType(const SidebarsByValType &x,
	                  ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                  ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0);

	virtual SidebarsByValType *_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
	                                  ::LinphonePrivate::Xsd::XmlSchema::Container *c = 0) const;

	SidebarsByValType &operator=(const SidebarsByValType &x);

	virtual ~SidebarsByValType();

	// Implementation.
	//
protected:
	void parse(::xsd::cxx::xml::dom::parser<char> &, ::LinphonePrivate::Xsd::XmlSchema::Flags);

protected:
	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> dom_document_;

	EntrySequence entry_;
	::xsd::cxx::tree::one<StateType> state_;
	static const StateType state_default_value_;
	AnyAttributeSet any_attribute_;
};
} // namespace ConferenceInfo
} // namespace Xsd
} // namespace LinphonePrivate

#include <iosfwd>

namespace LinphonePrivate {
namespace Xsd {
namespace ConferenceInfo {
::std::ostream &operator<<(::std::ostream &, const ConferenceType &);

::std::ostream &operator<<(::std::ostream &, StateType::Value);

::std::ostream &operator<<(::std::ostream &, const StateType &);

::std::ostream &operator<<(::std::ostream &, const ConferenceDescriptionType &);

::std::ostream &operator<<(::std::ostream &, const HostType &);

::std::ostream &operator<<(::std::ostream &, const ConferenceStateType &);

::std::ostream &operator<<(::std::ostream &, const ConferenceMediaType &);

::std::ostream &operator<<(::std::ostream &, const ConferenceMediumType &);

::std::ostream &operator<<(::std::ostream &, const UrisType &);

::std::ostream &operator<<(::std::ostream &, const UriType &);

::std::ostream &operator<<(::std::ostream &, const KeywordsType &);

::std::ostream &operator<<(::std::ostream &, const UsersType &);

::std::ostream &operator<<(::std::ostream &, const UserType &);

::std::ostream &operator<<(::std::ostream &, const UserRolesType &);

::std::ostream &operator<<(::std::ostream &, const UserLanguagesType &);

::std::ostream &operator<<(::std::ostream &, const EndpointType &);

::std::ostream &operator<<(::std::ostream &, EndpointStatusType::Value);

::std::ostream &operator<<(::std::ostream &, const EndpointStatusType &);

::std::ostream &operator<<(::std::ostream &, JoiningType::Value);

::std::ostream &operator<<(::std::ostream &, const JoiningType &);

::std::ostream &operator<<(::std::ostream &, DisconnectionType::Value);

::std::ostream &operator<<(::std::ostream &, const DisconnectionType &);

::std::ostream &operator<<(::std::ostream &, const ExecutionType &);

::std::ostream &operator<<(::std::ostream &, const CallType &);

::std::ostream &operator<<(::std::ostream &, const SipDialogIdType &);

::std::ostream &operator<<(::std::ostream &, const MediaType &);

::std::ostream &operator<<(::std::ostream &, MediaStatusType::Value);

::std::ostream &operator<<(::std::ostream &, const MediaStatusType &);

::std::ostream &operator<<(::std::ostream &, const SidebarsByValType &);
} // namespace ConferenceInfo
} // namespace Xsd
} // namespace LinphonePrivate

#include <iosfwd>

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/sax/InputSource.hpp>

namespace LinphonePrivate {
namespace Xsd {
namespace ConferenceInfo {
// Parse a URI or a local file.
//

::std::unique_ptr<::LinphonePrivate::Xsd::ConferenceInfo::ConferenceType> parseConferenceInfo(
    const ::std::string &uri,
    ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
    const ::LinphonePrivate::Xsd::XmlSchema::Properties &p = ::LinphonePrivate::Xsd::XmlSchema::Properties());

::std::unique_ptr<::LinphonePrivate::Xsd::ConferenceInfo::ConferenceType> parseConferenceInfo(
    const ::std::string &uri,
    ::LinphonePrivate::Xsd::XmlSchema::ErrorHandler &eh,
    ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
    const ::LinphonePrivate::Xsd::XmlSchema::Properties &p = ::LinphonePrivate::Xsd::XmlSchema::Properties());

::std::unique_ptr<::LinphonePrivate::Xsd::ConferenceInfo::ConferenceType> parseConferenceInfo(
    const ::std::string &uri,
    ::xercesc::DOMErrorHandler &eh,
    ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
    const ::LinphonePrivate::Xsd::XmlSchema::Properties &p = ::LinphonePrivate::Xsd::XmlSchema::Properties());

// Parse std::istream.
//

::std::unique_ptr<::LinphonePrivate::Xsd::ConferenceInfo::ConferenceType> parseConferenceInfo(
    ::std::istream &is,
    ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
    const ::LinphonePrivate::Xsd::XmlSchema::Properties &p = ::LinphonePrivate::Xsd::XmlSchema::Properties());

::std::unique_ptr<::LinphonePrivate::Xsd::ConferenceInfo::ConferenceType> parseConferenceInfo(
    ::std::istream &is,
    ::LinphonePrivate::Xsd::XmlSchema::ErrorHandler &eh,
    ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
    const ::LinphonePrivate::Xsd::XmlSchema::Properties &p = ::LinphonePrivate::Xsd::XmlSchema::Properties());

::std::unique_ptr<::LinphonePrivate::Xsd::ConferenceInfo::ConferenceType> parseConferenceInfo(
    ::std::istream &is,
    ::xercesc::DOMErrorHandler &eh,
    ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
    const ::LinphonePrivate::Xsd::XmlSchema::Properties &p = ::LinphonePrivate::Xsd::XmlSchema::Properties());

::std::unique_ptr<::LinphonePrivate::Xsd::ConferenceInfo::ConferenceType> parseConferenceInfo(
    ::std::istream &is,
    const ::std::string &id,
    ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
    const ::LinphonePrivate::Xsd::XmlSchema::Properties &p = ::LinphonePrivate::Xsd::XmlSchema::Properties());

::std::unique_ptr<::LinphonePrivate::Xsd::ConferenceInfo::ConferenceType> parseConferenceInfo(
    ::std::istream &is,
    const ::std::string &id,
    ::LinphonePrivate::Xsd::XmlSchema::ErrorHandler &eh,
    ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
    const ::LinphonePrivate::Xsd::XmlSchema::Properties &p = ::LinphonePrivate::Xsd::XmlSchema::Properties());

::std::unique_ptr<::LinphonePrivate::Xsd::ConferenceInfo::ConferenceType> parseConferenceInfo(
    ::std::istream &is,
    const ::std::string &id,
    ::xercesc::DOMErrorHandler &eh,
    ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
    const ::LinphonePrivate::Xsd::XmlSchema::Properties &p = ::LinphonePrivate::Xsd::XmlSchema::Properties());

// Parse xercesc::InputSource.
//

::std::unique_ptr<::LinphonePrivate::Xsd::ConferenceInfo::ConferenceType> parseConferenceInfo(
    ::xercesc::InputSource &is,
    ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
    const ::LinphonePrivate::Xsd::XmlSchema::Properties &p = ::LinphonePrivate::Xsd::XmlSchema::Properties());

::std::unique_ptr<::LinphonePrivate::Xsd::ConferenceInfo::ConferenceType> parseConferenceInfo(
    ::xercesc::InputSource &is,
    ::LinphonePrivate::Xsd::XmlSchema::ErrorHandler &eh,
    ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
    const ::LinphonePrivate::Xsd::XmlSchema::Properties &p = ::LinphonePrivate::Xsd::XmlSchema::Properties());

::std::unique_ptr<::LinphonePrivate::Xsd::ConferenceInfo::ConferenceType> parseConferenceInfo(
    ::xercesc::InputSource &is,
    ::xercesc::DOMErrorHandler &eh,
    ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
    const ::LinphonePrivate::Xsd::XmlSchema::Properties &p = ::LinphonePrivate::Xsd::XmlSchema::Properties());

// Parse xercesc::DOMDocument.
//

::std::unique_ptr<::LinphonePrivate::Xsd::ConferenceInfo::ConferenceType> parseConferenceInfo(
    const ::xercesc::DOMDocument &d,
    ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
    const ::LinphonePrivate::Xsd::XmlSchema::Properties &p = ::LinphonePrivate::Xsd::XmlSchema::Properties());

::std::unique_ptr<::LinphonePrivate::Xsd::ConferenceInfo::ConferenceType> parseConferenceInfo(
    ::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> d,
    ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0,
    const ::LinphonePrivate::Xsd::XmlSchema::Properties &p = ::LinphonePrivate::Xsd::XmlSchema::Properties());
} // namespace ConferenceInfo
} // namespace Xsd
} // namespace LinphonePrivate

#include <iosfwd>

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/framework/XMLFormatter.hpp>

#include <xsd/cxx/xml/dom/auto-ptr.hxx>

namespace LinphonePrivate {
namespace Xsd {
namespace ConferenceInfo {
// Serialize to std::ostream.
//

void serializeConferenceInfo(::std::ostream &os,
                             const ::LinphonePrivate::Xsd::ConferenceInfo::ConferenceType &x,
                             const ::LinphonePrivate::Xsd::XmlSchema::NamespaceInfomap &m =
                                 ::LinphonePrivate::Xsd::XmlSchema::NamespaceInfomap(),
                             const ::std::string &e = "UTF-8",
                             ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0);

void serializeConferenceInfo(::std::ostream &os,
                             const ::LinphonePrivate::Xsd::ConferenceInfo::ConferenceType &x,
                             ::LinphonePrivate::Xsd::XmlSchema::ErrorHandler &eh,
                             const ::LinphonePrivate::Xsd::XmlSchema::NamespaceInfomap &m =
                                 ::LinphonePrivate::Xsd::XmlSchema::NamespaceInfomap(),
                             const ::std::string &e = "UTF-8",
                             ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0);

void serializeConferenceInfo(::std::ostream &os,
                             const ::LinphonePrivate::Xsd::ConferenceInfo::ConferenceType &x,
                             ::xercesc::DOMErrorHandler &eh,
                             const ::LinphonePrivate::Xsd::XmlSchema::NamespaceInfomap &m =
                                 ::LinphonePrivate::Xsd::XmlSchema::NamespaceInfomap(),
                             const ::std::string &e = "UTF-8",
                             ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0);

// Serialize to xercesc::XMLFormatTarget.
//

void serializeConferenceInfo(::xercesc::XMLFormatTarget &ft,
                             const ::LinphonePrivate::Xsd::ConferenceInfo::ConferenceType &x,
                             const ::LinphonePrivate::Xsd::XmlSchema::NamespaceInfomap &m =
                                 ::LinphonePrivate::Xsd::XmlSchema::NamespaceInfomap(),
                             const ::std::string &e = "UTF-8",
                             ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0);

void serializeConferenceInfo(::xercesc::XMLFormatTarget &ft,
                             const ::LinphonePrivate::Xsd::ConferenceInfo::ConferenceType &x,
                             ::LinphonePrivate::Xsd::XmlSchema::ErrorHandler &eh,
                             const ::LinphonePrivate::Xsd::XmlSchema::NamespaceInfomap &m =
                                 ::LinphonePrivate::Xsd::XmlSchema::NamespaceInfomap(),
                             const ::std::string &e = "UTF-8",
                             ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0);

void serializeConferenceInfo(::xercesc::XMLFormatTarget &ft,
                             const ::LinphonePrivate::Xsd::ConferenceInfo::ConferenceType &x,
                             ::xercesc::DOMErrorHandler &eh,
                             const ::LinphonePrivate::Xsd::XmlSchema::NamespaceInfomap &m =
                                 ::LinphonePrivate::Xsd::XmlSchema::NamespaceInfomap(),
                             const ::std::string &e = "UTF-8",
                             ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0);

// Serialize to an existing xercesc::DOMDocument.
//

void serializeConferenceInfo(::xercesc::DOMDocument &d,
                             const ::LinphonePrivate::Xsd::ConferenceInfo::ConferenceType &x,
                             ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0);

// Serialize to a new xercesc::DOMDocument.
//

::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument>
serializeConferenceInfo(const ::LinphonePrivate::Xsd::ConferenceInfo::ConferenceType &x,
                        const ::LinphonePrivate::Xsd::XmlSchema::NamespaceInfomap &m =
                            ::LinphonePrivate::Xsd::XmlSchema::NamespaceInfomap(),
                        ::LinphonePrivate::Xsd::XmlSchema::Flags f = 0);

void operator<<(::xercesc::DOMElement &, const ConferenceType &);

void operator<<(::xercesc::DOMElement &, const StateType &);

void operator<<(::xercesc::DOMAttr &, const StateType &);

void operator<<(::LinphonePrivate::Xsd::XmlSchema::ListStream &, const StateType &);

void operator<<(::xercesc::DOMElement &, const ConferenceDescriptionType &);

void operator<<(::xercesc::DOMElement &, const HostType &);

void operator<<(::xercesc::DOMElement &, const ConferenceStateType &);

void operator<<(::xercesc::DOMElement &, const ConferenceMediaType &);

void operator<<(::xercesc::DOMElement &, const ConferenceMediumType &);

void operator<<(::xercesc::DOMElement &, const UrisType &);

void operator<<(::xercesc::DOMElement &, const UriType &);

void operator<<(::xercesc::DOMElement &, const KeywordsType &);

void operator<<(::xercesc::DOMAttr &, const KeywordsType &);

void operator<<(::LinphonePrivate::Xsd::XmlSchema::ListStream &, const KeywordsType &);

void operator<<(::xercesc::DOMElement &, const UsersType &);

void operator<<(::xercesc::DOMElement &, const UserType &);

void operator<<(::xercesc::DOMElement &, const UserRolesType &);

void operator<<(::xercesc::DOMElement &, const UserLanguagesType &);

void operator<<(::xercesc::DOMAttr &, const UserLanguagesType &);

void operator<<(::LinphonePrivate::Xsd::XmlSchema::ListStream &, const UserLanguagesType &);

void operator<<(::xercesc::DOMElement &, const EndpointType &);

void operator<<(::xercesc::DOMElement &, const EndpointStatusType &);

void operator<<(::xercesc::DOMAttr &, const EndpointStatusType &);

void operator<<(::LinphonePrivate::Xsd::XmlSchema::ListStream &, const EndpointStatusType &);

void operator<<(::xercesc::DOMElement &, const JoiningType &);

void operator<<(::xercesc::DOMAttr &, const JoiningType &);

void operator<<(::LinphonePrivate::Xsd::XmlSchema::ListStream &, const JoiningType &);

void operator<<(::xercesc::DOMElement &, const DisconnectionType &);

void operator<<(::xercesc::DOMAttr &, const DisconnectionType &);

void operator<<(::LinphonePrivate::Xsd::XmlSchema::ListStream &, const DisconnectionType &);

void operator<<(::xercesc::DOMElement &, const ExecutionType &);

void operator<<(::xercesc::DOMElement &, const CallType &);

void operator<<(::xercesc::DOMElement &, const SipDialogIdType &);

void operator<<(::xercesc::DOMElement &, const MediaType &);

void operator<<(::xercesc::DOMElement &, const MediaStatusType &);

void operator<<(::xercesc::DOMAttr &, const MediaStatusType &);

void operator<<(::LinphonePrivate::Xsd::XmlSchema::ListStream &, const MediaStatusType &);

void operator<<(::xercesc::DOMElement &, const SidebarsByValType &);
} // namespace ConferenceInfo
} // namespace Xsd
} // namespace LinphonePrivate

#include <xsd/cxx/post.hxx>

// Begin epilogue.
//
#if __GNUC__ >= 7
#pragma GCC diagnostic pop
#endif
#if __GNUC__ > 5 || (__GNUC__ == 5 && __GNUC_MINOR__ >= 1)
#pragma GCC diagnostic pop
#endif
#if __clang__ || __GNUC__ >= 4
#pragma GCC diagnostic pop
#endif
//
// End epilogue.

#endif // CONFERENCE_INFO_H
