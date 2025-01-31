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

#include <xsd/cxx/pre.hxx>

#include "imdn.h"

namespace LinphonePrivate {
namespace Xsd {
namespace Imdn {
// Imdn
//

const Imdn::MessageIdType &Imdn::getMessageId() const {
	return this->message_id_.get();
}

Imdn::MessageIdType &Imdn::getMessageId() {
	return this->message_id_.get();
}

void Imdn::setMessageId(const MessageIdType &x) {
	this->message_id_.set(x);
}

void Imdn::setMessageId(::std::unique_ptr<MessageIdType> x) {
	this->message_id_.set(std::move(x));
}

::std::unique_ptr<Imdn::MessageIdType> Imdn::setDetachMessage_id() {
	return this->message_id_.detach();
}

const Imdn::DatetimeType &Imdn::getDatetime() const {
	return this->datetime_.get();
}

Imdn::DatetimeType &Imdn::getDatetime() {
	return this->datetime_.get();
}

void Imdn::setDatetime(const DatetimeType &x) {
	this->datetime_.set(x);
}

void Imdn::setDatetime(::std::unique_ptr<DatetimeType> x) {
	this->datetime_.set(std::move(x));
}

::std::unique_ptr<Imdn::DatetimeType> Imdn::setDetachDatetime() {
	return this->datetime_.detach();
}

const Imdn::RecipientUriOptional &Imdn::getRecipientUri() const {
	return this->recipient_uri_;
}

Imdn::RecipientUriOptional &Imdn::getRecipientUri() {
	return this->recipient_uri_;
}

void Imdn::setRecipientUri(const RecipientUriType &x) {
	this->recipient_uri_.set(x);
}

void Imdn::setRecipientUri(const RecipientUriOptional &x) {
	this->recipient_uri_ = x;
}

void Imdn::setRecipientUri(::std::unique_ptr<RecipientUriType> x) {
	this->recipient_uri_.set(std::move(x));
}

const Imdn::OriginalRecipientUriOptional &Imdn::getOriginalRecipientUri() const {
	return this->original_recipient_uri_;
}

Imdn::OriginalRecipientUriOptional &Imdn::getOriginalRecipientUri() {
	return this->original_recipient_uri_;
}

void Imdn::setOriginalRecipientUri(const OriginalRecipientUriType &x) {
	this->original_recipient_uri_.set(x);
}

void Imdn::setOriginalRecipientUri(const OriginalRecipientUriOptional &x) {
	this->original_recipient_uri_ = x;
}

void Imdn::setOriginalRecipientUri(::std::unique_ptr<OriginalRecipientUriType> x) {
	this->original_recipient_uri_.set(std::move(x));
}

const Imdn::SubjectOptional &Imdn::getSubject() const {
	return this->subject_;
}

Imdn::SubjectOptional &Imdn::getSubject() {
	return this->subject_;
}

void Imdn::setSubject(const SubjectType &x) {
	this->subject_.set(x);
}

void Imdn::setSubject(const SubjectOptional &x) {
	this->subject_ = x;
}

void Imdn::setSubject(::std::unique_ptr<SubjectType> x) {
	this->subject_.set(std::move(x));
}

const Imdn::DeliveryNotificationOptional &Imdn::getDeliveryNotification() const {
	return this->delivery_notification_;
}

Imdn::DeliveryNotificationOptional &Imdn::getDeliveryNotification() {
	return this->delivery_notification_;
}

void Imdn::setDeliveryNotification(const DeliveryNotificationType &x) {
	this->delivery_notification_.set(x);
}

void Imdn::setDeliveryNotification(const DeliveryNotificationOptional &x) {
	this->delivery_notification_ = x;
}

void Imdn::setDeliveryNotification(::std::unique_ptr<DeliveryNotificationType> x) {
	this->delivery_notification_.set(std::move(x));
}

const Imdn::DisplayNotificationOptional &Imdn::getDisplayNotification() const {
	return this->display_notification_;
}

Imdn::DisplayNotificationOptional &Imdn::getDisplayNotification() {
	return this->display_notification_;
}

void Imdn::setDisplayNotification(const DisplayNotificationType &x) {
	this->display_notification_.set(x);
}

void Imdn::setDisplayNotification(const DisplayNotificationOptional &x) {
	this->display_notification_ = x;
}

void Imdn::setDisplayNotification(::std::unique_ptr<DisplayNotificationType> x) {
	this->display_notification_.set(std::move(x));
}

const Imdn::ProcessingNotificationOptional &Imdn::getProcessingNotification() const {
	return this->processing_notification_;
}

Imdn::ProcessingNotificationOptional &Imdn::getProcessingNotification() {
	return this->processing_notification_;
}

void Imdn::setProcessingNotification(const ProcessingNotificationType &x) {
	this->processing_notification_.set(x);
}

void Imdn::setProcessingNotification(const ProcessingNotificationOptional &x) {
	this->processing_notification_ = x;
}

void Imdn::setProcessingNotification(::std::unique_ptr<ProcessingNotificationType> x) {
	this->processing_notification_.set(std::move(x));
}

const Imdn::AnySequence &Imdn::getAny() const {
	return this->any_;
}

Imdn::AnySequence &Imdn::getAny() {
	return this->any_;
}

void Imdn::setAny(const AnySequence &s) {
	this->any_ = s;
}

const ::xercesc::DOMDocument &Imdn::getDomDocument() const {
	return *this->dom_document_;
}

::xercesc::DOMDocument &Imdn::getDomDocument() {
	return *this->dom_document_;
}

// DeliveryNotification
//

const DeliveryNotification::StatusType &DeliveryNotification::getStatus() const {
	return this->status_.get();
}

DeliveryNotification::StatusType &DeliveryNotification::getStatus() {
	return this->status_.get();
}

void DeliveryNotification::setStatus(const StatusType &x) {
	this->status_.set(x);
}

void DeliveryNotification::setStatus(::std::unique_ptr<StatusType> x) {
	this->status_.set(std::move(x));
}

::std::unique_ptr<DeliveryNotification::StatusType> DeliveryNotification::setDetachStatus() {
	return this->status_.detach();
}

// Delivered
//

// Failed
//

// DisplayNotification
//

const DisplayNotification::StatusType &DisplayNotification::getStatus() const {
	return this->status_.get();
}

DisplayNotification::StatusType &DisplayNotification::getStatus() {
	return this->status_.get();
}

void DisplayNotification::setStatus(const StatusType &x) {
	this->status_.set(x);
}

void DisplayNotification::setStatus(::std::unique_ptr<StatusType> x) {
	this->status_.set(std::move(x));
}

::std::unique_ptr<DisplayNotification::StatusType> DisplayNotification::setDetachStatus() {
	return this->status_.detach();
}

// Displayed
//

// ProcessingNotification
//

const ProcessingNotification::StatusType &ProcessingNotification::getStatus() const {
	return this->status_.get();
}

ProcessingNotification::StatusType &ProcessingNotification::getStatus() {
	return this->status_.get();
}

void ProcessingNotification::setStatus(const StatusType &x) {
	this->status_.set(x);
}

void ProcessingNotification::setStatus(::std::unique_ptr<StatusType> x) {
	this->status_.set(std::move(x));
}

::std::unique_ptr<ProcessingNotification::StatusType> ProcessingNotification::setDetachStatus() {
	return this->status_.detach();
}

// Processed
//

// Stored
//

// Forbidden
//

// Error
//

// Status
//

const Status::DeliveredOptional &Status::getDelivered() const {
	return this->delivered_;
}

Status::DeliveredOptional &Status::getDelivered() {
	return this->delivered_;
}

void Status::setDelivered(const DeliveredType &x) {
	this->delivered_.set(x);
}

void Status::setDelivered(const DeliveredOptional &x) {
	this->delivered_ = x;
}

void Status::setDelivered(::std::unique_ptr<DeliveredType> x) {
	this->delivered_.set(std::move(x));
}

const Status::FailedOptional &Status::getFailed() const {
	return this->failed_;
}

Status::FailedOptional &Status::getFailed() {
	return this->failed_;
}

void Status::setFailed(const FailedType &x) {
	this->failed_.set(x);
}

void Status::setFailed(const FailedOptional &x) {
	this->failed_ = x;
}

void Status::setFailed(::std::unique_ptr<FailedType> x) {
	this->failed_.set(std::move(x));
}

const Status::ForbiddenOptional &Status::getForbidden() const {
	return this->forbidden_;
}

Status::ForbiddenOptional &Status::getForbidden() {
	return this->forbidden_;
}

void Status::setForbidden(const ForbiddenType &x) {
	this->forbidden_.set(x);
}

void Status::setForbidden(const ForbiddenOptional &x) {
	this->forbidden_ = x;
}

void Status::setForbidden(::std::unique_ptr<ForbiddenType> x) {
	this->forbidden_.set(std::move(x));
}

const Status::ErrorOptional &Status::getError() const {
	return this->error_;
}

Status::ErrorOptional &Status::getError() {
	return this->error_;
}

void Status::setError(const ErrorType &x) {
	this->error_.set(x);
}

void Status::setError(const ErrorOptional &x) {
	this->error_ = x;
}

void Status::setError(::std::unique_ptr<ErrorType> x) {
	this->error_.set(std::move(x));
}

const Status::ReasonOptional &Status::getReason() const {
	return this->reason_;
}

Status::ReasonOptional &Status::getReason() {
	return this->reason_;
}

void Status::setReason(const ReasonType &x) {
	this->reason_.set(x);
}

void Status::setReason(const ReasonOptional &x) {
	this->reason_ = x;
}

void Status::setReason(::std::unique_ptr<ReasonType> x) {
	this->reason_.set(std::move(x));
}

// Status1
//

const Status1::DisplayedOptional &Status1::getDisplayed() const {
	return this->displayed_;
}

Status1::DisplayedOptional &Status1::getDisplayed() {
	return this->displayed_;
}

void Status1::setDisplayed(const DisplayedType &x) {
	this->displayed_.set(x);
}

void Status1::setDisplayed(const DisplayedOptional &x) {
	this->displayed_ = x;
}

void Status1::setDisplayed(::std::unique_ptr<DisplayedType> x) {
	this->displayed_.set(std::move(x));
}

const Status1::ForbiddenOptional &Status1::getForbidden() const {
	return this->forbidden_;
}

Status1::ForbiddenOptional &Status1::getForbidden() {
	return this->forbidden_;
}

void Status1::setForbidden(const ForbiddenType &x) {
	this->forbidden_.set(x);
}

void Status1::setForbidden(const ForbiddenOptional &x) {
	this->forbidden_ = x;
}

void Status1::setForbidden(::std::unique_ptr<ForbiddenType> x) {
	this->forbidden_.set(std::move(x));
}

const Status1::ErrorOptional &Status1::getError() const {
	return this->error_;
}

Status1::ErrorOptional &Status1::getError() {
	return this->error_;
}

void Status1::setError(const ErrorType &x) {
	this->error_.set(x);
}

void Status1::setError(const ErrorOptional &x) {
	this->error_ = x;
}

void Status1::setError(::std::unique_ptr<ErrorType> x) {
	this->error_.set(std::move(x));
}

const Status1::AnySequence &Status1::getAny() const {
	return this->any_;
}

Status1::AnySequence &Status1::getAny() {
	return this->any_;
}

void Status1::setAny(const AnySequence &s) {
	this->any_ = s;
}

const ::xercesc::DOMDocument &Status1::getDomDocument() const {
	return *this->dom_document_;
}

::xercesc::DOMDocument &Status1::getDomDocument() {
	return *this->dom_document_;
}

// Status2
//

const Status2::ProcessedOptional &Status2::getProcessed() const {
	return this->processed_;
}

Status2::ProcessedOptional &Status2::getProcessed() {
	return this->processed_;
}

void Status2::setProcessed(const ProcessedType &x) {
	this->processed_.set(x);
}

void Status2::setProcessed(const ProcessedOptional &x) {
	this->processed_ = x;
}

void Status2::setProcessed(::std::unique_ptr<ProcessedType> x) {
	this->processed_.set(std::move(x));
}

const Status2::StoredOptional &Status2::getStored() const {
	return this->stored_;
}

Status2::StoredOptional &Status2::getStored() {
	return this->stored_;
}

void Status2::setStored(const StoredType &x) {
	this->stored_.set(x);
}

void Status2::setStored(const StoredOptional &x) {
	this->stored_ = x;
}

void Status2::setStored(::std::unique_ptr<StoredType> x) {
	this->stored_.set(std::move(x));
}

const Status2::ForbiddenOptional &Status2::getForbidden() const {
	return this->forbidden_;
}

Status2::ForbiddenOptional &Status2::getForbidden() {
	return this->forbidden_;
}

void Status2::setForbidden(const ForbiddenType &x) {
	this->forbidden_.set(x);
}

void Status2::setForbidden(const ForbiddenOptional &x) {
	this->forbidden_ = x;
}

void Status2::setForbidden(::std::unique_ptr<ForbiddenType> x) {
	this->forbidden_.set(std::move(x));
}

const Status2::ErrorOptional &Status2::getError() const {
	return this->error_;
}

Status2::ErrorOptional &Status2::getError() {
	return this->error_;
}

void Status2::setError(const ErrorType &x) {
	this->error_.set(x);
}

void Status2::setError(const ErrorOptional &x) {
	this->error_ = x;
}

void Status2::setError(::std::unique_ptr<ErrorType> x) {
	this->error_.set(std::move(x));
}

const Status2::AnySequence &Status2::getAny() const {
	return this->any_;
}

Status2::AnySequence &Status2::getAny() {
	return this->any_;
}

void Status2::setAny(const AnySequence &s) {
	this->any_ = s;
}

const ::xercesc::DOMDocument &Status2::getDomDocument() const {
	return *this->dom_document_;
}

::xercesc::DOMDocument &Status2::getDomDocument() {
	return *this->dom_document_;
}
} // namespace Imdn
} // namespace Xsd
} // namespace LinphonePrivate

#include <xsd/cxx/xml/dom/wildcard-source.hxx>

#include <xsd/cxx/xml/dom/parsing-source.hxx>

#include <xsd/cxx/tree/type-factory-map.hxx>

namespace _xsd {
static const ::xsd::cxx::tree::type_factory_plate<0, char> type_factory_plate_init;
}

namespace LinphonePrivate {
namespace Xsd {
namespace Imdn {
// Imdn
//

Imdn::Imdn(const MessageIdType &message_id, const DatetimeType &datetime)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(), dom_document_(::xsd::cxx::xml::dom::create_document<char>()),
      message_id_(message_id, this), datetime_(datetime, this), recipient_uri_(this), original_recipient_uri_(this),
      subject_(this), delivery_notification_(this), display_notification_(this), processing_notification_(this),
      any_(this->getDomDocument()) {
}

Imdn::Imdn(const Imdn &x, ::LinphonePrivate::Xsd::XmlSchema::Flags f, ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(x, f, c), dom_document_(::xsd::cxx::xml::dom::create_document<char>()),
      message_id_(x.message_id_, f, this), datetime_(x.datetime_, f, this), recipient_uri_(x.recipient_uri_, f, this),
      original_recipient_uri_(x.original_recipient_uri_, f, this), subject_(x.subject_, f, this),
      delivery_notification_(x.delivery_notification_, f, this),
      display_notification_(x.display_notification_, f, this),
      processing_notification_(x.processing_notification_, f, this), any_(x.any_, this->getDomDocument()) {
}

Imdn::Imdn(const ::xercesc::DOMElement &e,
           ::LinphonePrivate::Xsd::XmlSchema::Flags f,
           ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(e, f | ::LinphonePrivate::Xsd::XmlSchema::Flags::base, c),
      dom_document_(::xsd::cxx::xml::dom::create_document<char>()), message_id_(this), datetime_(this),
      recipient_uri_(this), original_recipient_uri_(this), subject_(this), delivery_notification_(this),
      display_notification_(this), processing_notification_(this), any_(this->getDomDocument()) {
	if ((f & ::LinphonePrivate::Xsd::XmlSchema::Flags::base) == 0) {
		::xsd::cxx::xml::dom::parser<char> p(e, true, false, false);
		this->parse(p, f);
	}
}

void Imdn::parse(::xsd::cxx::xml::dom::parser<char> &p, ::LinphonePrivate::Xsd::XmlSchema::Flags f) {
	for (; p.more_content(); p.next_content(false)) {
		const ::xercesc::DOMElement &i(p.cur_element());
		const ::xsd::cxx::xml::qualified_name<char> n(::xsd::cxx::xml::dom::name<char>(i));

		// message-id
		//
		if (n.name() == "message-id" && n.namespace_() == "urn:ietf:params:xml:ns:imdn") {
			::std::unique_ptr<MessageIdType> r(MessageIdTraits::create(i, f, this));

			if (!message_id_.present()) {
				this->message_id_.set(::std::move(r));
				continue;
			}
		}

		// datetime
		//
		if (n.name() == "datetime" && n.namespace_() == "urn:ietf:params:xml:ns:imdn") {
			::std::unique_ptr<DatetimeType> r(DatetimeTraits::create(i, f, this));

			if (!datetime_.present()) {
				this->datetime_.set(::std::move(r));
				continue;
			}
		}

		// recipient-uri
		//
		if (n.name() == "recipient-uri" && n.namespace_() == "urn:ietf:params:xml:ns:imdn") {
			::std::unique_ptr<RecipientUriType> r(RecipientUriTraits::create(i, f, this));

			if (!this->recipient_uri_) {
				this->recipient_uri_.set(::std::move(r));
				continue;
			}
		}

		// original-recipient-uri
		//
		if (n.name() == "original-recipient-uri" && n.namespace_() == "urn:ietf:params:xml:ns:imdn") {
			::std::unique_ptr<OriginalRecipientUriType> r(OriginalRecipientUriTraits::create(i, f, this));

			if (!this->original_recipient_uri_) {
				this->original_recipient_uri_.set(::std::move(r));
				continue;
			}
		}

		// subject
		//
		if (n.name() == "subject" && n.namespace_() == "urn:ietf:params:xml:ns:imdn") {
			::std::unique_ptr<SubjectType> r(SubjectTraits::create(i, f, this));

			if (!this->subject_) {
				this->subject_.set(::std::move(r));
				continue;
			}
		}

		// delivery-notification
		//
		if (n.name() == "delivery-notification" && n.namespace_() == "urn:ietf:params:xml:ns:imdn") {
			::std::unique_ptr<DeliveryNotificationType> r(DeliveryNotificationTraits::create(i, f, this));

			if (!this->delivery_notification_) {
				this->delivery_notification_.set(::std::move(r));
				continue;
			}
		}

		// display-notification
		//
		if (n.name() == "display-notification" && n.namespace_() == "urn:ietf:params:xml:ns:imdn") {
			::std::unique_ptr<DisplayNotificationType> r(DisplayNotificationTraits::create(i, f, this));

			if (!this->display_notification_) {
				this->display_notification_.set(::std::move(r));
				continue;
			}
		}

		// processing-notification
		//
		if (n.name() == "processing-notification" && n.namespace_() == "urn:ietf:params:xml:ns:imdn") {
			::std::unique_ptr<ProcessingNotificationType> r(ProcessingNotificationTraits::create(i, f, this));

			if (!this->processing_notification_) {
				this->processing_notification_.set(::std::move(r));
				continue;
			}
		}

		// any
		//
		if ((!n.namespace_().empty() && n.namespace_() != "urn:ietf:params:xml:ns:imdn")) {
			::xercesc::DOMElement *r(static_cast<::xercesc::DOMElement *>(
			    this->getDomDocument().importNode(const_cast<::xercesc::DOMElement *>(&i), true)));
			this->any_.push_back(r);
			continue;
		}

		break;
	}

	if (!message_id_.present()) {
		throw ::xsd::cxx::tree::expected_element<char>("message-id", "urn:ietf:params:xml:ns:imdn");
	}

	if (!datetime_.present()) {
		throw ::xsd::cxx::tree::expected_element<char>("datetime", "urn:ietf:params:xml:ns:imdn");
	}
}

Imdn *Imdn::_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f, ::LinphonePrivate::Xsd::XmlSchema::Container *c) const {
	return new class Imdn(*this, f, c);
}

Imdn &Imdn::operator=(const Imdn &x) {
	if (this != &x) {
		static_cast<::LinphonePrivate::Xsd::XmlSchema::Type &>(*this) = x;
		this->message_id_ = x.message_id_;
		this->datetime_ = x.datetime_;
		this->recipient_uri_ = x.recipient_uri_;
		this->original_recipient_uri_ = x.original_recipient_uri_;
		this->subject_ = x.subject_;
		this->delivery_notification_ = x.delivery_notification_;
		this->display_notification_ = x.display_notification_;
		this->processing_notification_ = x.processing_notification_;
		this->any_ = x.any_;
	}

	return *this;
}

Imdn::~Imdn() {
}

// DeliveryNotification
//

DeliveryNotification::DeliveryNotification(const StatusType &status)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(), status_(status, this) {
}

DeliveryNotification::DeliveryNotification(::std::unique_ptr<StatusType> status)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(), status_(std::move(status), this) {
}

DeliveryNotification::DeliveryNotification(const DeliveryNotification &x,
                                           ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                                           ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(x, f, c), status_(x.status_, f, this) {
}

DeliveryNotification::DeliveryNotification(const ::xercesc::DOMElement &e,
                                           ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                                           ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(e, f | ::LinphonePrivate::Xsd::XmlSchema::Flags::base, c), status_(this) {
	if ((f & ::LinphonePrivate::Xsd::XmlSchema::Flags::base) == 0) {
		::xsd::cxx::xml::dom::parser<char> p(e, true, false, false);
		this->parse(p, f);
	}
}

void DeliveryNotification::parse(::xsd::cxx::xml::dom::parser<char> &p, ::LinphonePrivate::Xsd::XmlSchema::Flags f) {
	for (; p.more_content(); p.next_content(false)) {
		const ::xercesc::DOMElement &i(p.cur_element());
		const ::xsd::cxx::xml::qualified_name<char> n(::xsd::cxx::xml::dom::name<char>(i));

		// status
		//
		if (n.name() == "status" && n.namespace_() == "urn:ietf:params:xml:ns:imdn") {
			::std::unique_ptr<StatusType> r(StatusTraits::create(i, f, this));

			if (!status_.present()) {
				this->status_.set(::std::move(r));
				continue;
			}
		}

		break;
	}

	if (!status_.present()) {
		throw ::xsd::cxx::tree::expected_element<char>("status", "urn:ietf:params:xml:ns:imdn");
	}
}

DeliveryNotification *DeliveryNotification::_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f,
                                                   ::LinphonePrivate::Xsd::XmlSchema::Container *c) const {
	return new class DeliveryNotification(*this, f, c);
}

DeliveryNotification &DeliveryNotification::operator=(const DeliveryNotification &x) {
	if (this != &x) {
		static_cast<::LinphonePrivate::Xsd::XmlSchema::Type &>(*this) = x;
		this->status_ = x.status_;
	}

	return *this;
}

DeliveryNotification::~DeliveryNotification() {
}

// Delivered
//

Delivered::Delivered() : ::LinphonePrivate::Xsd::XmlSchema::Type() {
}

Delivered::Delivered(const Delivered &x,
                     ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                     ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(x, f, c) {
}

Delivered::Delivered(const ::xercesc::DOMElement &e,
                     ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                     ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(e, f, c) {
}

Delivered::Delivered(const ::xercesc::DOMAttr &a,
                     ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                     ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(a, f, c) {
}

Delivered::Delivered(const ::std::string &s,
                     const ::xercesc::DOMElement *e,
                     ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                     ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(s, e, f, c) {
}

Delivered *Delivered::_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f,
                             ::LinphonePrivate::Xsd::XmlSchema::Container *c) const {
	return new class Delivered(*this, f, c);
}

Delivered::~Delivered() {
}

// Failed
//

Failed::Failed() : ::LinphonePrivate::Xsd::XmlSchema::Type() {
}

Failed::Failed(const Failed &x,
               ::LinphonePrivate::Xsd::XmlSchema::Flags f,
               ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(x, f, c) {
}

Failed::Failed(const ::xercesc::DOMElement &e,
               ::LinphonePrivate::Xsd::XmlSchema::Flags f,
               ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(e, f, c) {
}

Failed::Failed(const ::xercesc::DOMAttr &a,
               ::LinphonePrivate::Xsd::XmlSchema::Flags f,
               ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(a, f, c) {
}

Failed::Failed(const ::std::string &s,
               const ::xercesc::DOMElement *e,
               ::LinphonePrivate::Xsd::XmlSchema::Flags f,
               ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(s, e, f, c) {
}

Failed *Failed::_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f,
                       ::LinphonePrivate::Xsd::XmlSchema::Container *c) const {
	return new class Failed(*this, f, c);
}

Failed::~Failed() {
}

// DisplayNotification
//

DisplayNotification::DisplayNotification(const StatusType &status)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(), status_(status, this) {
}

DisplayNotification::DisplayNotification(::std::unique_ptr<StatusType> status)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(), status_(std::move(status), this) {
}

DisplayNotification::DisplayNotification(const DisplayNotification &x,
                                         ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                                         ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(x, f, c), status_(x.status_, f, this) {
}

DisplayNotification::DisplayNotification(const ::xercesc::DOMElement &e,
                                         ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                                         ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(e, f | ::LinphonePrivate::Xsd::XmlSchema::Flags::base, c), status_(this) {
	if ((f & ::LinphonePrivate::Xsd::XmlSchema::Flags::base) == 0) {
		::xsd::cxx::xml::dom::parser<char> p(e, true, false, false);
		this->parse(p, f);
	}
}

void DisplayNotification::parse(::xsd::cxx::xml::dom::parser<char> &p, ::LinphonePrivate::Xsd::XmlSchema::Flags f) {
	for (; p.more_content(); p.next_content(false)) {
		const ::xercesc::DOMElement &i(p.cur_element());
		const ::xsd::cxx::xml::qualified_name<char> n(::xsd::cxx::xml::dom::name<char>(i));

		// status
		//
		if (n.name() == "status" && n.namespace_() == "urn:ietf:params:xml:ns:imdn") {
			::std::unique_ptr<StatusType> r(StatusTraits::create(i, f, this));

			if (!status_.present()) {
				this->status_.set(::std::move(r));
				continue;
			}
		}

		break;
	}

	if (!status_.present()) {
		throw ::xsd::cxx::tree::expected_element<char>("status", "urn:ietf:params:xml:ns:imdn");
	}
}

DisplayNotification *DisplayNotification::_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f,
                                                 ::LinphonePrivate::Xsd::XmlSchema::Container *c) const {
	return new class DisplayNotification(*this, f, c);
}

DisplayNotification &DisplayNotification::operator=(const DisplayNotification &x) {
	if (this != &x) {
		static_cast<::LinphonePrivate::Xsd::XmlSchema::Type &>(*this) = x;
		this->status_ = x.status_;
	}

	return *this;
}

DisplayNotification::~DisplayNotification() {
}

// Displayed
//

Displayed::Displayed() : ::LinphonePrivate::Xsd::XmlSchema::Type() {
}

Displayed::Displayed(const Displayed &x,
                     ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                     ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(x, f, c) {
}

Displayed::Displayed(const ::xercesc::DOMElement &e,
                     ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                     ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(e, f, c) {
}

Displayed::Displayed(const ::xercesc::DOMAttr &a,
                     ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                     ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(a, f, c) {
}

Displayed::Displayed(const ::std::string &s,
                     const ::xercesc::DOMElement *e,
                     ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                     ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(s, e, f, c) {
}

Displayed *Displayed::_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f,
                             ::LinphonePrivate::Xsd::XmlSchema::Container *c) const {
	return new class Displayed(*this, f, c);
}

Displayed::~Displayed() {
}

// ProcessingNotification
//

ProcessingNotification::ProcessingNotification(const StatusType &status)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(), status_(status, this) {
}

ProcessingNotification::ProcessingNotification(::std::unique_ptr<StatusType> status)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(), status_(std::move(status), this) {
}

ProcessingNotification::ProcessingNotification(const ProcessingNotification &x,
                                               ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                                               ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(x, f, c), status_(x.status_, f, this) {
}

ProcessingNotification::ProcessingNotification(const ::xercesc::DOMElement &e,
                                               ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                                               ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(e, f | ::LinphonePrivate::Xsd::XmlSchema::Flags::base, c), status_(this) {
	if ((f & ::LinphonePrivate::Xsd::XmlSchema::Flags::base) == 0) {
		::xsd::cxx::xml::dom::parser<char> p(e, true, false, false);
		this->parse(p, f);
	}
}

void ProcessingNotification::parse(::xsd::cxx::xml::dom::parser<char> &p, ::LinphonePrivate::Xsd::XmlSchema::Flags f) {
	for (; p.more_content(); p.next_content(false)) {
		const ::xercesc::DOMElement &i(p.cur_element());
		const ::xsd::cxx::xml::qualified_name<char> n(::xsd::cxx::xml::dom::name<char>(i));

		// status
		//
		if (n.name() == "status" && n.namespace_() == "urn:ietf:params:xml:ns:imdn") {
			::std::unique_ptr<StatusType> r(StatusTraits::create(i, f, this));

			if (!status_.present()) {
				this->status_.set(::std::move(r));
				continue;
			}
		}

		break;
	}

	if (!status_.present()) {
		throw ::xsd::cxx::tree::expected_element<char>("status", "urn:ietf:params:xml:ns:imdn");
	}
}

ProcessingNotification *ProcessingNotification::_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f,
                                                       ::LinphonePrivate::Xsd::XmlSchema::Container *c) const {
	return new class ProcessingNotification(*this, f, c);
}

ProcessingNotification &ProcessingNotification::operator=(const ProcessingNotification &x) {
	if (this != &x) {
		static_cast<::LinphonePrivate::Xsd::XmlSchema::Type &>(*this) = x;
		this->status_ = x.status_;
	}

	return *this;
}

ProcessingNotification::~ProcessingNotification() {
}

// Processed
//

Processed::Processed() : ::LinphonePrivate::Xsd::XmlSchema::Type() {
}

Processed::Processed(const Processed &x,
                     ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                     ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(x, f, c) {
}

Processed::Processed(const ::xercesc::DOMElement &e,
                     ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                     ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(e, f, c) {
}

Processed::Processed(const ::xercesc::DOMAttr &a,
                     ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                     ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(a, f, c) {
}

Processed::Processed(const ::std::string &s,
                     const ::xercesc::DOMElement *e,
                     ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                     ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(s, e, f, c) {
}

Processed *Processed::_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f,
                             ::LinphonePrivate::Xsd::XmlSchema::Container *c) const {
	return new class Processed(*this, f, c);
}

Processed::~Processed() {
}

// Stored
//

Stored::Stored() : ::LinphonePrivate::Xsd::XmlSchema::Type() {
}

Stored::Stored(const Stored &x,
               ::LinphonePrivate::Xsd::XmlSchema::Flags f,
               ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(x, f, c) {
}

Stored::Stored(const ::xercesc::DOMElement &e,
               ::LinphonePrivate::Xsd::XmlSchema::Flags f,
               ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(e, f, c) {
}

Stored::Stored(const ::xercesc::DOMAttr &a,
               ::LinphonePrivate::Xsd::XmlSchema::Flags f,
               ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(a, f, c) {
}

Stored::Stored(const ::std::string &s,
               const ::xercesc::DOMElement *e,
               ::LinphonePrivate::Xsd::XmlSchema::Flags f,
               ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(s, e, f, c) {
}

Stored *Stored::_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f,
                       ::LinphonePrivate::Xsd::XmlSchema::Container *c) const {
	return new class Stored(*this, f, c);
}

Stored::~Stored() {
}

// Forbidden
//

Forbidden::Forbidden() : ::LinphonePrivate::Xsd::XmlSchema::Type() {
}

Forbidden::Forbidden(const Forbidden &x,
                     ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                     ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(x, f, c) {
}

Forbidden::Forbidden(const ::xercesc::DOMElement &e,
                     ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                     ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(e, f, c) {
}

Forbidden::Forbidden(const ::xercesc::DOMAttr &a,
                     ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                     ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(a, f, c) {
}

Forbidden::Forbidden(const ::std::string &s,
                     const ::xercesc::DOMElement *e,
                     ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                     ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(s, e, f, c) {
}

Forbidden *Forbidden::_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f,
                             ::LinphonePrivate::Xsd::XmlSchema::Container *c) const {
	return new class Forbidden(*this, f, c);
}

Forbidden::~Forbidden() {
}

// Error
//

Error::Error() : ::LinphonePrivate::Xsd::XmlSchema::Type() {
}

Error::Error(const Error &x,
             ::LinphonePrivate::Xsd::XmlSchema::Flags f,
             ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(x, f, c) {
}

Error::Error(const ::xercesc::DOMElement &e,
             ::LinphonePrivate::Xsd::XmlSchema::Flags f,
             ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(e, f, c) {
}

Error::Error(const ::xercesc::DOMAttr &a,
             ::LinphonePrivate::Xsd::XmlSchema::Flags f,
             ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(a, f, c) {
}

Error::Error(const ::std::string &s,
             const ::xercesc::DOMElement *e,
             ::LinphonePrivate::Xsd::XmlSchema::Flags f,
             ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(s, e, f, c) {
}

Error *Error::_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f,
                     ::LinphonePrivate::Xsd::XmlSchema::Container *c) const {
	return new class Error(*this, f, c);
}

Error::~Error() {
}

// Status
//

Status::Status()
    : ::LinphonePrivate::Xsd::XmlSchema::Type(), delivered_(this), failed_(this), forbidden_(this), error_(this),
      reason_(this) {
}

Status::Status(const Status &x,
               ::LinphonePrivate::Xsd::XmlSchema::Flags f,
               ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(x, f, c), delivered_(x.delivered_, f, this), failed_(x.failed_, f, this),
      forbidden_(x.forbidden_, f, this), error_(x.error_, f, this), reason_(x.reason_, f, this) {
}

Status::Status(const ::xercesc::DOMElement &e,
               ::LinphonePrivate::Xsd::XmlSchema::Flags f,
               ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(e, f | ::LinphonePrivate::Xsd::XmlSchema::Flags::base, c),
      delivered_(this), failed_(this), forbidden_(this), error_(this), reason_(this) {
	if ((f & ::LinphonePrivate::Xsd::XmlSchema::Flags::base) == 0) {
		::xsd::cxx::xml::dom::parser<char> p(e, true, false, false);
		this->parse(p, f);
	}
}

void Status::parse(::xsd::cxx::xml::dom::parser<char> &p, ::LinphonePrivate::Xsd::XmlSchema::Flags f) {
	for (; p.more_content(); p.next_content(false)) {
		const ::xercesc::DOMElement &i(p.cur_element());
		const ::xsd::cxx::xml::qualified_name<char> n(::xsd::cxx::xml::dom::name<char>(i));

		// delivered
		//
		if (n.name() == "delivered" && n.namespace_() == "urn:ietf:params:xml:ns:imdn") {
			::std::unique_ptr<DeliveredType> r(DeliveredTraits::create(i, f, this));

			if (!this->delivered_) {
				this->delivered_.set(::std::move(r));
				continue;
			}
		}

		// failed
		//
		if (n.name() == "failed" && n.namespace_() == "urn:ietf:params:xml:ns:imdn") {
			::std::unique_ptr<FailedType> r(FailedTraits::create(i, f, this));

			if (!this->failed_) {
				this->failed_.set(::std::move(r));
				continue;
			}
		}

		// forbidden
		//
		if (n.name() == "forbidden" && n.namespace_() == "urn:ietf:params:xml:ns:imdn") {
			::std::unique_ptr<ForbiddenType> r(ForbiddenTraits::create(i, f, this));

			if (!this->forbidden_) {
				this->forbidden_.set(::std::move(r));
				continue;
			}
		}

		// error
		//
		if (n.name() == "error" && n.namespace_() == "urn:ietf:params:xml:ns:imdn") {
			::std::unique_ptr<ErrorType> r(ErrorTraits::create(i, f, this));

			if (!this->error_) {
				this->error_.set(::std::move(r));
				continue;
			}
		}

		// reason
		//
		if (n.name() == "reason" && n.namespace_() == "http://www.linphone.org/xsds/imdn.xsd") {
			::std::unique_ptr<ReasonType> r(ReasonTraits::create(i, f, this));

			if (!this->reason_) {
				this->reason_.set(::std::move(r));
				continue;
			}
		}

		break;
	}
}

Status *Status::_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f,
                       ::LinphonePrivate::Xsd::XmlSchema::Container *c) const {
	return new class Status(*this, f, c);
}

Status &Status::operator=(const Status &x) {
	if (this != &x) {
		static_cast<::LinphonePrivate::Xsd::XmlSchema::Type &>(*this) = x;
		this->delivered_ = x.delivered_;
		this->failed_ = x.failed_;
		this->forbidden_ = x.forbidden_;
		this->error_ = x.error_;
		this->reason_ = x.reason_;
	}

	return *this;
}

Status::~Status() {
}

// Status1
//

Status1::Status1()
    : ::LinphonePrivate::Xsd::XmlSchema::Type(), dom_document_(::xsd::cxx::xml::dom::create_document<char>()),
      displayed_(this), forbidden_(this), error_(this), any_(this->getDomDocument()) {
}

Status1::Status1(const Status1 &x,
                 ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                 ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(x, f, c), dom_document_(::xsd::cxx::xml::dom::create_document<char>()),
      displayed_(x.displayed_, f, this), forbidden_(x.forbidden_, f, this), error_(x.error_, f, this),
      any_(x.any_, this->getDomDocument()) {
}

Status1::Status1(const ::xercesc::DOMElement &e,
                 ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                 ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(e, f | ::LinphonePrivate::Xsd::XmlSchema::Flags::base, c),
      dom_document_(::xsd::cxx::xml::dom::create_document<char>()), displayed_(this), forbidden_(this), error_(this),
      any_(this->getDomDocument()) {
	if ((f & ::LinphonePrivate::Xsd::XmlSchema::Flags::base) == 0) {
		::xsd::cxx::xml::dom::parser<char> p(e, true, false, false);
		this->parse(p, f);
	}
}

void Status1::parse(::xsd::cxx::xml::dom::parser<char> &p, ::LinphonePrivate::Xsd::XmlSchema::Flags f) {
	for (; p.more_content(); p.next_content(false)) {
		const ::xercesc::DOMElement &i(p.cur_element());
		const ::xsd::cxx::xml::qualified_name<char> n(::xsd::cxx::xml::dom::name<char>(i));

		// displayed
		//
		if (n.name() == "displayed" && n.namespace_() == "urn:ietf:params:xml:ns:imdn") {
			::std::unique_ptr<DisplayedType> r(DisplayedTraits::create(i, f, this));

			if (!this->displayed_) {
				this->displayed_.set(::std::move(r));
				continue;
			}
		}

		// forbidden
		//
		if (n.name() == "forbidden" && n.namespace_() == "urn:ietf:params:xml:ns:imdn") {
			::std::unique_ptr<ForbiddenType> r(ForbiddenTraits::create(i, f, this));

			if (!this->forbidden_) {
				this->forbidden_.set(::std::move(r));
				continue;
			}
		}

		// error
		//
		if (n.name() == "error" && n.namespace_() == "urn:ietf:params:xml:ns:imdn") {
			::std::unique_ptr<ErrorType> r(ErrorTraits::create(i, f, this));

			if (!this->error_) {
				this->error_.set(::std::move(r));
				continue;
			}
		}

		// any
		//
		if ((!n.namespace_().empty() && n.namespace_() != "urn:ietf:params:xml:ns:imdn")) {
			::xercesc::DOMElement *r(static_cast<::xercesc::DOMElement *>(
			    this->getDomDocument().importNode(const_cast<::xercesc::DOMElement *>(&i), true)));
			this->any_.push_back(r);
			continue;
		}

		break;
	}
}

Status1 *Status1::_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f,
                         ::LinphonePrivate::Xsd::XmlSchema::Container *c) const {
	return new class Status1(*this, f, c);
}

Status1 &Status1::operator=(const Status1 &x) {
	if (this != &x) {
		static_cast<::LinphonePrivate::Xsd::XmlSchema::Type &>(*this) = x;
		this->displayed_ = x.displayed_;
		this->forbidden_ = x.forbidden_;
		this->error_ = x.error_;
		this->any_ = x.any_;
	}

	return *this;
}

Status1::~Status1() {
}

// Status2
//

Status2::Status2()
    : ::LinphonePrivate::Xsd::XmlSchema::Type(), dom_document_(::xsd::cxx::xml::dom::create_document<char>()),
      processed_(this), stored_(this), forbidden_(this), error_(this), any_(this->getDomDocument()) {
}

Status2::Status2(const Status2 &x,
                 ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                 ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(x, f, c), dom_document_(::xsd::cxx::xml::dom::create_document<char>()),
      processed_(x.processed_, f, this), stored_(x.stored_, f, this), forbidden_(x.forbidden_, f, this),
      error_(x.error_, f, this), any_(x.any_, this->getDomDocument()) {
}

Status2::Status2(const ::xercesc::DOMElement &e,
                 ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                 ::LinphonePrivate::Xsd::XmlSchema::Container *c)
    : ::LinphonePrivate::Xsd::XmlSchema::Type(e, f | ::LinphonePrivate::Xsd::XmlSchema::Flags::base, c),
      dom_document_(::xsd::cxx::xml::dom::create_document<char>()), processed_(this), stored_(this), forbidden_(this),
      error_(this), any_(this->getDomDocument()) {
	if ((f & ::LinphonePrivate::Xsd::XmlSchema::Flags::base) == 0) {
		::xsd::cxx::xml::dom::parser<char> p(e, true, false, false);
		this->parse(p, f);
	}
}

void Status2::parse(::xsd::cxx::xml::dom::parser<char> &p, ::LinphonePrivate::Xsd::XmlSchema::Flags f) {
	for (; p.more_content(); p.next_content(false)) {
		const ::xercesc::DOMElement &i(p.cur_element());
		const ::xsd::cxx::xml::qualified_name<char> n(::xsd::cxx::xml::dom::name<char>(i));

		// processed
		//
		if (n.name() == "processed" && n.namespace_() == "urn:ietf:params:xml:ns:imdn") {
			::std::unique_ptr<ProcessedType> r(ProcessedTraits::create(i, f, this));

			if (!this->processed_) {
				this->processed_.set(::std::move(r));
				continue;
			}
		}

		// stored
		//
		if (n.name() == "stored" && n.namespace_() == "urn:ietf:params:xml:ns:imdn") {
			::std::unique_ptr<StoredType> r(StoredTraits::create(i, f, this));

			if (!this->stored_) {
				this->stored_.set(::std::move(r));
				continue;
			}
		}

		// forbidden
		//
		if (n.name() == "forbidden" && n.namespace_() == "urn:ietf:params:xml:ns:imdn") {
			::std::unique_ptr<ForbiddenType> r(ForbiddenTraits::create(i, f, this));

			if (!this->forbidden_) {
				this->forbidden_.set(::std::move(r));
				continue;
			}
		}

		// error
		//
		if (n.name() == "error" && n.namespace_() == "urn:ietf:params:xml:ns:imdn") {
			::std::unique_ptr<ErrorType> r(ErrorTraits::create(i, f, this));

			if (!this->error_) {
				this->error_.set(::std::move(r));
				continue;
			}
		}

		// any
		//
		if ((!n.namespace_().empty() && n.namespace_() != "urn:ietf:params:xml:ns:imdn")) {
			::xercesc::DOMElement *r(static_cast<::xercesc::DOMElement *>(
			    this->getDomDocument().importNode(const_cast<::xercesc::DOMElement *>(&i), true)));
			this->any_.push_back(r);
			continue;
		}

		break;
	}
}

Status2 *Status2::_clone(::LinphonePrivate::Xsd::XmlSchema::Flags f,
                         ::LinphonePrivate::Xsd::XmlSchema::Container *c) const {
	return new class Status2(*this, f, c);
}

Status2 &Status2::operator=(const Status2 &x) {
	if (this != &x) {
		static_cast<::LinphonePrivate::Xsd::XmlSchema::Type &>(*this) = x;
		this->processed_ = x.processed_;
		this->stored_ = x.stored_;
		this->forbidden_ = x.forbidden_;
		this->error_ = x.error_;
		this->any_ = x.any_;
	}

	return *this;
}

Status2::~Status2() {
}
} // namespace Imdn
} // namespace Xsd
} // namespace LinphonePrivate

#include <ostream>

#include <xsd/cxx/tree/std-ostream-map.hxx>

namespace _xsd {
static const ::xsd::cxx::tree::std_ostream_plate<0, char> std_ostream_plate_init;
}

namespace LinphonePrivate {
namespace Xsd {
namespace Imdn {
::std::ostream &operator<<(::std::ostream &o, const Imdn &i) {
	o << ::std::endl << "message-id: " << i.getMessageId();
	o << ::std::endl << "datetime: " << i.getDatetime();
	if (i.getRecipientUri()) {
		o << ::std::endl << "recipient-uri: " << *i.getRecipientUri();
	}

	if (i.getOriginalRecipientUri()) {
		o << ::std::endl << "original-recipient-uri: " << *i.getOriginalRecipientUri();
	}

	if (i.getSubject()) {
		o << ::std::endl << "subject: " << *i.getSubject();
	}

	if (i.getDeliveryNotification()) {
		o << ::std::endl << "delivery-notification: " << *i.getDeliveryNotification();
	}

	if (i.getDisplayNotification()) {
		o << ::std::endl << "display-notification: " << *i.getDisplayNotification();
	}

	if (i.getProcessingNotification()) {
		o << ::std::endl << "processing-notification: " << *i.getProcessingNotification();
	}

	return o;
}

::std::ostream &operator<<(::std::ostream &o, const DeliveryNotification &i) {
	o << ::std::endl << "status: " << i.getStatus();
	return o;
}

::std::ostream &operator<<(::std::ostream &o, const Delivered &) {
	return o;
}

::std::ostream &operator<<(::std::ostream &o, const Failed &) {
	return o;
}

::std::ostream &operator<<(::std::ostream &o, const DisplayNotification &i) {
	o << ::std::endl << "status: " << i.getStatus();
	return o;
}

::std::ostream &operator<<(::std::ostream &o, const Displayed &) {
	return o;
}

::std::ostream &operator<<(::std::ostream &o, const ProcessingNotification &i) {
	o << ::std::endl << "status: " << i.getStatus();
	return o;
}

::std::ostream &operator<<(::std::ostream &o, const Processed &) {
	return o;
}

::std::ostream &operator<<(::std::ostream &o, const Stored &) {
	return o;
}

::std::ostream &operator<<(::std::ostream &o, const Forbidden &) {
	return o;
}

::std::ostream &operator<<(::std::ostream &o, const Error &) {
	return o;
}

::std::ostream &operator<<(::std::ostream &o, const Status &i) {
	if (i.getDelivered()) {
		o << ::std::endl << "delivered: " << *i.getDelivered();
	}

	if (i.getFailed()) {
		o << ::std::endl << "failed: " << *i.getFailed();
	}

	if (i.getForbidden()) {
		o << ::std::endl << "forbidden: " << *i.getForbidden();
	}

	if (i.getError()) {
		o << ::std::endl << "error: " << *i.getError();
	}

	if (i.getReason()) {
		o << ::std::endl << "reason: " << *i.getReason();
	}

	return o;
}

::std::ostream &operator<<(::std::ostream &o, const Status1 &i) {
	if (i.getDisplayed()) {
		o << ::std::endl << "displayed: " << *i.getDisplayed();
	}

	if (i.getForbidden()) {
		o << ::std::endl << "forbidden: " << *i.getForbidden();
	}

	if (i.getError()) {
		o << ::std::endl << "error: " << *i.getError();
	}

	return o;
}

::std::ostream &operator<<(::std::ostream &o, const Status2 &i) {
	if (i.getProcessed()) {
		o << ::std::endl << "processed: " << *i.getProcessed();
	}

	if (i.getStored()) {
		o << ::std::endl << "stored: " << *i.getStored();
	}

	if (i.getForbidden()) {
		o << ::std::endl << "forbidden: " << *i.getForbidden();
	}

	if (i.getError()) {
		o << ::std::endl << "error: " << *i.getError();
	}

	return o;
}
} // namespace Imdn
} // namespace Xsd
} // namespace LinphonePrivate

#include <istream>
#include <xsd/cxx/tree/error-handler.hxx>
#include <xsd/cxx/xml/sax/std-input-source.hxx>

namespace LinphonePrivate {
namespace Xsd {
namespace Imdn {
::std::unique_ptr<::LinphonePrivate::Xsd::Imdn::Imdn>
parseImdn(const ::std::string &u,
          ::LinphonePrivate::Xsd::XmlSchema::Flags f,
          const ::LinphonePrivate::Xsd::XmlSchema::Properties &p) {
	::xsd::cxx::xml::auto_initializer i((f & ::LinphonePrivate::Xsd::XmlSchema::Flags::dont_initialize) == 0,
	                                    (f & ::LinphonePrivate::Xsd::XmlSchema::Flags::keep_dom) == 0);

	::xsd::cxx::tree::error_handler<char> h;

	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> d(
	    ::xsd::cxx::xml::dom::parse<char>(u, h, p, f));

	h.throw_if_failed<::xsd::cxx::tree::parsing<char>>();

	return ::std::unique_ptr<::LinphonePrivate::Xsd::Imdn::Imdn>(::LinphonePrivate::Xsd::Imdn::parseImdn(
	    std::move(d), f | ::LinphonePrivate::Xsd::XmlSchema::Flags::own_dom, p));
}

::std::unique_ptr<::LinphonePrivate::Xsd::Imdn::Imdn>
parseImdn(const ::std::string &u,
          ::LinphonePrivate::Xsd::XmlSchema::ErrorHandler &h,
          ::LinphonePrivate::Xsd::XmlSchema::Flags f,
          const ::LinphonePrivate::Xsd::XmlSchema::Properties &p) {
	::xsd::cxx::xml::auto_initializer i((f & ::LinphonePrivate::Xsd::XmlSchema::Flags::dont_initialize) == 0,
	                                    (f & ::LinphonePrivate::Xsd::XmlSchema::Flags::keep_dom) == 0);

	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> d(
	    ::xsd::cxx::xml::dom::parse<char>(u, h, p, f));

	if (!d.get()) throw ::xsd::cxx::tree::parsing<char>();

	return ::std::unique_ptr<::LinphonePrivate::Xsd::Imdn::Imdn>(::LinphonePrivate::Xsd::Imdn::parseImdn(
	    std::move(d), f | ::LinphonePrivate::Xsd::XmlSchema::Flags::own_dom, p));
}

::std::unique_ptr<::LinphonePrivate::Xsd::Imdn::Imdn>
parseImdn(const ::std::string &u,
          ::xercesc::DOMErrorHandler &h,
          ::LinphonePrivate::Xsd::XmlSchema::Flags f,
          const ::LinphonePrivate::Xsd::XmlSchema::Properties &p) {
	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> d(
	    ::xsd::cxx::xml::dom::parse<char>(u, h, p, f));

	if (!d.get()) throw ::xsd::cxx::tree::parsing<char>();

	return ::std::unique_ptr<::LinphonePrivate::Xsd::Imdn::Imdn>(::LinphonePrivate::Xsd::Imdn::parseImdn(
	    std::move(d), f | ::LinphonePrivate::Xsd::XmlSchema::Flags::own_dom, p));
}

::std::unique_ptr<::LinphonePrivate::Xsd::Imdn::Imdn>
parseImdn(::std::istream &is,
          ::LinphonePrivate::Xsd::XmlSchema::Flags f,
          const ::LinphonePrivate::Xsd::XmlSchema::Properties &p) {
	::xsd::cxx::xml::auto_initializer i((f & ::LinphonePrivate::Xsd::XmlSchema::Flags::dont_initialize) == 0,
	                                    (f & ::LinphonePrivate::Xsd::XmlSchema::Flags::keep_dom) == 0);

	::xsd::cxx::xml::sax::std_input_source isrc(is);
	return ::LinphonePrivate::Xsd::Imdn::parseImdn(isrc, f, p);
}

::std::unique_ptr<::LinphonePrivate::Xsd::Imdn::Imdn>
parseImdn(::std::istream &is,
          ::LinphonePrivate::Xsd::XmlSchema::ErrorHandler &h,
          ::LinphonePrivate::Xsd::XmlSchema::Flags f,
          const ::LinphonePrivate::Xsd::XmlSchema::Properties &p) {
	::xsd::cxx::xml::auto_initializer i((f & ::LinphonePrivate::Xsd::XmlSchema::Flags::dont_initialize) == 0,
	                                    (f & ::LinphonePrivate::Xsd::XmlSchema::Flags::keep_dom) == 0);

	::xsd::cxx::xml::sax::std_input_source isrc(is);
	return ::LinphonePrivate::Xsd::Imdn::parseImdn(isrc, h, f, p);
}

::std::unique_ptr<::LinphonePrivate::Xsd::Imdn::Imdn>
parseImdn(::std::istream &is,
          ::xercesc::DOMErrorHandler &h,
          ::LinphonePrivate::Xsd::XmlSchema::Flags f,
          const ::LinphonePrivate::Xsd::XmlSchema::Properties &p) {
	::xsd::cxx::xml::sax::std_input_source isrc(is);
	return ::LinphonePrivate::Xsd::Imdn::parseImdn(isrc, h, f, p);
}

::std::unique_ptr<::LinphonePrivate::Xsd::Imdn::Imdn>
parseImdn(::std::istream &is,
          const ::std::string &sid,
          ::LinphonePrivate::Xsd::XmlSchema::Flags f,
          const ::LinphonePrivate::Xsd::XmlSchema::Properties &p) {
	::xsd::cxx::xml::auto_initializer i((f & ::LinphonePrivate::Xsd::XmlSchema::Flags::dont_initialize) == 0,
	                                    (f & ::LinphonePrivate::Xsd::XmlSchema::Flags::keep_dom) == 0);

	::xsd::cxx::xml::sax::std_input_source isrc(is, sid);
	return ::LinphonePrivate::Xsd::Imdn::parseImdn(isrc, f, p);
}

::std::unique_ptr<::LinphonePrivate::Xsd::Imdn::Imdn>
parseImdn(::std::istream &is,
          const ::std::string &sid,
          ::LinphonePrivate::Xsd::XmlSchema::ErrorHandler &h,
          ::LinphonePrivate::Xsd::XmlSchema::Flags f,
          const ::LinphonePrivate::Xsd::XmlSchema::Properties &p) {
	::xsd::cxx::xml::auto_initializer i((f & ::LinphonePrivate::Xsd::XmlSchema::Flags::dont_initialize) == 0,
	                                    (f & ::LinphonePrivate::Xsd::XmlSchema::Flags::keep_dom) == 0);

	::xsd::cxx::xml::sax::std_input_source isrc(is, sid);
	return ::LinphonePrivate::Xsd::Imdn::parseImdn(isrc, h, f, p);
}

::std::unique_ptr<::LinphonePrivate::Xsd::Imdn::Imdn>
parseImdn(::std::istream &is,
          const ::std::string &sid,
          ::xercesc::DOMErrorHandler &h,
          ::LinphonePrivate::Xsd::XmlSchema::Flags f,
          const ::LinphonePrivate::Xsd::XmlSchema::Properties &p) {
	::xsd::cxx::xml::sax::std_input_source isrc(is, sid);
	return ::LinphonePrivate::Xsd::Imdn::parseImdn(isrc, h, f, p);
}

::std::unique_ptr<::LinphonePrivate::Xsd::Imdn::Imdn>
parseImdn(::xercesc::InputSource &i,
          ::LinphonePrivate::Xsd::XmlSchema::Flags f,
          const ::LinphonePrivate::Xsd::XmlSchema::Properties &p) {
	::xsd::cxx::tree::error_handler<char> h;

	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> d(
	    ::xsd::cxx::xml::dom::parse<char>(i, h, p, f));

	h.throw_if_failed<::xsd::cxx::tree::parsing<char>>();

	return ::std::unique_ptr<::LinphonePrivate::Xsd::Imdn::Imdn>(::LinphonePrivate::Xsd::Imdn::parseImdn(
	    std::move(d), f | ::LinphonePrivate::Xsd::XmlSchema::Flags::own_dom, p));
}

::std::unique_ptr<::LinphonePrivate::Xsd::Imdn::Imdn>
parseImdn(::xercesc::InputSource &i,
          ::LinphonePrivate::Xsd::XmlSchema::ErrorHandler &h,
          ::LinphonePrivate::Xsd::XmlSchema::Flags f,
          const ::LinphonePrivate::Xsd::XmlSchema::Properties &p) {
	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> d(
	    ::xsd::cxx::xml::dom::parse<char>(i, h, p, f));

	if (!d.get()) throw ::xsd::cxx::tree::parsing<char>();

	return ::std::unique_ptr<::LinphonePrivate::Xsd::Imdn::Imdn>(::LinphonePrivate::Xsd::Imdn::parseImdn(
	    std::move(d), f | ::LinphonePrivate::Xsd::XmlSchema::Flags::own_dom, p));
}

::std::unique_ptr<::LinphonePrivate::Xsd::Imdn::Imdn>
parseImdn(::xercesc::InputSource &i,
          ::xercesc::DOMErrorHandler &h,
          ::LinphonePrivate::Xsd::XmlSchema::Flags f,
          const ::LinphonePrivate::Xsd::XmlSchema::Properties &p) {
	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> d(
	    ::xsd::cxx::xml::dom::parse<char>(i, h, p, f));

	if (!d.get()) throw ::xsd::cxx::tree::parsing<char>();

	return ::std::unique_ptr<::LinphonePrivate::Xsd::Imdn::Imdn>(::LinphonePrivate::Xsd::Imdn::parseImdn(
	    std::move(d), f | ::LinphonePrivate::Xsd::XmlSchema::Flags::own_dom, p));
}

::std::unique_ptr<::LinphonePrivate::Xsd::Imdn::Imdn>
parseImdn(const ::xercesc::DOMDocument &doc,
          ::LinphonePrivate::Xsd::XmlSchema::Flags f,
          const ::LinphonePrivate::Xsd::XmlSchema::Properties &p) {
	if (f & ::LinphonePrivate::Xsd::XmlSchema::Flags::keep_dom) {
		::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> d(
		    static_cast<::xercesc::DOMDocument *>(doc.cloneNode(true)));

		return ::std::unique_ptr<::LinphonePrivate::Xsd::Imdn::Imdn>(::LinphonePrivate::Xsd::Imdn::parseImdn(
		    std::move(d), f | ::LinphonePrivate::Xsd::XmlSchema::Flags::own_dom, p));
	}

	const ::xercesc::DOMElement &e(*doc.getDocumentElement());
	const ::xsd::cxx::xml::qualified_name<char> n(::xsd::cxx::xml::dom::name<char>(e));

	if (n.name() == "imdn" && n.namespace_() == "urn:ietf:params:xml:ns:imdn") {
		::std::unique_ptr<::LinphonePrivate::Xsd::Imdn::Imdn> r(
		    ::xsd::cxx::tree::traits<::LinphonePrivate::Xsd::Imdn::Imdn, char>::create(e, f, 0));
		return r;
	}

	throw ::xsd::cxx::tree::unexpected_element<char>(n.name(), n.namespace_(), "imdn", "urn:ietf:params:xml:ns:imdn");
}

::std::unique_ptr<::LinphonePrivate::Xsd::Imdn::Imdn>
parseImdn(::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> d,
          ::LinphonePrivate::Xsd::XmlSchema::Flags f,
          const ::LinphonePrivate::Xsd::XmlSchema::Properties &) {
	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> c(
	    ((f & ::LinphonePrivate::Xsd::XmlSchema::Flags::keep_dom) &&
	     !(f & ::LinphonePrivate::Xsd::XmlSchema::Flags::own_dom))
	        ? static_cast<::xercesc::DOMDocument *>(d->cloneNode(true))
	        : 0);

	::xercesc::DOMDocument &doc(c.get() ? *c : *d);
	const ::xercesc::DOMElement &e(*doc.getDocumentElement());

	const ::xsd::cxx::xml::qualified_name<char> n(::xsd::cxx::xml::dom::name<char>(e));

	if (f & ::LinphonePrivate::Xsd::XmlSchema::Flags::keep_dom)
		doc.setUserData(::LinphonePrivate::Xsd::XmlSchema::dom::treeNodeKey, (c.get() ? &c : &d), 0);

	if (n.name() == "imdn" && n.namespace_() == "urn:ietf:params:xml:ns:imdn") {
		::std::unique_ptr<::LinphonePrivate::Xsd::Imdn::Imdn> r(
		    ::xsd::cxx::tree::traits<::LinphonePrivate::Xsd::Imdn::Imdn, char>::create(e, f, 0));
		return r;
	}

	throw ::xsd::cxx::tree::unexpected_element<char>(n.name(), n.namespace_(), "imdn", "urn:ietf:params:xml:ns:imdn");
}
} // namespace Imdn
} // namespace Xsd
} // namespace LinphonePrivate

#include <ostream>
#include <xsd/cxx/tree/error-handler.hxx>
#include <xsd/cxx/xml/dom/serialization-source.hxx>

#include <xsd/cxx/tree/type-serializer-map.hxx>

namespace _xsd {
static const ::xsd::cxx::tree::type_serializer_plate<0, char> type_serializer_plate_init;
}

namespace LinphonePrivate {
namespace Xsd {
namespace Imdn {
void serializeImdn(::std::ostream &o,
                   const ::LinphonePrivate::Xsd::Imdn::Imdn &s,
                   const ::LinphonePrivate::Xsd::XmlSchema::NamespaceInfomap &m,
                   const ::std::string &e,
                   ::LinphonePrivate::Xsd::XmlSchema::Flags f) {
	::xsd::cxx::xml::auto_initializer i((f & ::LinphonePrivate::Xsd::XmlSchema::Flags::dont_initialize) == 0);

	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> d(
	    ::LinphonePrivate::Xsd::Imdn::serializeImdn(s, m, f));

	::xsd::cxx::tree::error_handler<char> h;

	::xsd::cxx::xml::dom::ostream_format_target t(o);
	if (!::xsd::cxx::xml::dom::serialize(t, *d, e, h, f)) {
		h.throw_if_failed<::xsd::cxx::tree::serialization<char>>();
	}
}

void serializeImdn(::std::ostream &o,
                   const ::LinphonePrivate::Xsd::Imdn::Imdn &s,
                   ::LinphonePrivate::Xsd::XmlSchema::ErrorHandler &h,
                   const ::LinphonePrivate::Xsd::XmlSchema::NamespaceInfomap &m,
                   const ::std::string &e,
                   ::LinphonePrivate::Xsd::XmlSchema::Flags f) {
	::xsd::cxx::xml::auto_initializer i((f & ::LinphonePrivate::Xsd::XmlSchema::Flags::dont_initialize) == 0);

	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> d(
	    ::LinphonePrivate::Xsd::Imdn::serializeImdn(s, m, f));
	::xsd::cxx::xml::dom::ostream_format_target t(o);
	if (!::xsd::cxx::xml::dom::serialize(t, *d, e, h, f)) {
		throw ::xsd::cxx::tree::serialization<char>();
	}
}

void serializeImdn(::std::ostream &o,
                   const ::LinphonePrivate::Xsd::Imdn::Imdn &s,
                   ::xercesc::DOMErrorHandler &h,
                   const ::LinphonePrivate::Xsd::XmlSchema::NamespaceInfomap &m,
                   const ::std::string &e,
                   ::LinphonePrivate::Xsd::XmlSchema::Flags f) {
	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> d(
	    ::LinphonePrivate::Xsd::Imdn::serializeImdn(s, m, f));
	::xsd::cxx::xml::dom::ostream_format_target t(o);
	if (!::xsd::cxx::xml::dom::serialize(t, *d, e, h, f)) {
		throw ::xsd::cxx::tree::serialization<char>();
	}
}

void serializeImdn(::xercesc::XMLFormatTarget &t,
                   const ::LinphonePrivate::Xsd::Imdn::Imdn &s,
                   const ::LinphonePrivate::Xsd::XmlSchema::NamespaceInfomap &m,
                   const ::std::string &e,
                   ::LinphonePrivate::Xsd::XmlSchema::Flags f) {
	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> d(
	    ::LinphonePrivate::Xsd::Imdn::serializeImdn(s, m, f));

	::xsd::cxx::tree::error_handler<char> h;

	if (!::xsd::cxx::xml::dom::serialize(t, *d, e, h, f)) {
		h.throw_if_failed<::xsd::cxx::tree::serialization<char>>();
	}
}

void serializeImdn(::xercesc::XMLFormatTarget &t,
                   const ::LinphonePrivate::Xsd::Imdn::Imdn &s,
                   ::LinphonePrivate::Xsd::XmlSchema::ErrorHandler &h,
                   const ::LinphonePrivate::Xsd::XmlSchema::NamespaceInfomap &m,
                   const ::std::string &e,
                   ::LinphonePrivate::Xsd::XmlSchema::Flags f) {
	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> d(
	    ::LinphonePrivate::Xsd::Imdn::serializeImdn(s, m, f));
	if (!::xsd::cxx::xml::dom::serialize(t, *d, e, h, f)) {
		throw ::xsd::cxx::tree::serialization<char>();
	}
}

void serializeImdn(::xercesc::XMLFormatTarget &t,
                   const ::LinphonePrivate::Xsd::Imdn::Imdn &s,
                   ::xercesc::DOMErrorHandler &h,
                   const ::LinphonePrivate::Xsd::XmlSchema::NamespaceInfomap &m,
                   const ::std::string &e,
                   ::LinphonePrivate::Xsd::XmlSchema::Flags f) {
	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> d(
	    ::LinphonePrivate::Xsd::Imdn::serializeImdn(s, m, f));
	if (!::xsd::cxx::xml::dom::serialize(t, *d, e, h, f)) {
		throw ::xsd::cxx::tree::serialization<char>();
	}
}

void serializeImdn(::xercesc::DOMDocument &d,
                   const ::LinphonePrivate::Xsd::Imdn::Imdn &s,
                   ::LinphonePrivate::Xsd::XmlSchema::Flags) {
	::xercesc::DOMElement &e(*d.getDocumentElement());
	const ::xsd::cxx::xml::qualified_name<char> n(::xsd::cxx::xml::dom::name<char>(e));

	if (n.name() == "imdn" && n.namespace_() == "urn:ietf:params:xml:ns:imdn") {
		e << s;
	} else {
		throw ::xsd::cxx::tree::unexpected_element<char>(n.name(), n.namespace_(), "imdn",
		                                                 "urn:ietf:params:xml:ns:imdn");
	}
}

::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument>
serializeImdn(const ::LinphonePrivate::Xsd::Imdn::Imdn &s,
              const ::LinphonePrivate::Xsd::XmlSchema::NamespaceInfomap &m,
              ::LinphonePrivate::Xsd::XmlSchema::Flags f) {
	::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr<::xercesc::DOMDocument> d(
	    ::xsd::cxx::xml::dom::serialize<char>("imdn", "urn:ietf:params:xml:ns:imdn", m, f));

	::LinphonePrivate::Xsd::Imdn::serializeImdn(*d, s, f);
	return d;
}

void operator<<(::xercesc::DOMElement &e, const Imdn &i) {
	e << static_cast<const ::LinphonePrivate::Xsd::XmlSchema::Type &>(i);

	// message-id
	//
	{
		::xercesc::DOMElement &s(::xsd::cxx::xml::dom::create_element("message-id", "urn:ietf:params:xml:ns:imdn", e));

		s << i.getMessageId();
	}

	// datetime
	//
	{
		::xercesc::DOMElement &s(::xsd::cxx::xml::dom::create_element("datetime", "urn:ietf:params:xml:ns:imdn", e));

		s << i.getDatetime();
	}

	// recipient-uri
	//
	if (i.getRecipientUri()) {
		::xercesc::DOMElement &s(
		    ::xsd::cxx::xml::dom::create_element("recipient-uri", "urn:ietf:params:xml:ns:imdn", e));

		s << *i.getRecipientUri();
	}

	// original-recipient-uri
	//
	if (i.getOriginalRecipientUri()) {
		::xercesc::DOMElement &s(
		    ::xsd::cxx::xml::dom::create_element("original-recipient-uri", "urn:ietf:params:xml:ns:imdn", e));

		s << *i.getOriginalRecipientUri();
	}

	// subject
	//
	if (i.getSubject()) {
		::xercesc::DOMElement &s(::xsd::cxx::xml::dom::create_element("subject", "urn:ietf:params:xml:ns:imdn", e));

		s << *i.getSubject();
	}

	// delivery-notification
	//
	if (i.getDeliveryNotification()) {
		::xercesc::DOMElement &s(
		    ::xsd::cxx::xml::dom::create_element("delivery-notification", "urn:ietf:params:xml:ns:imdn", e));

		s << *i.getDeliveryNotification();
	}

	// display-notification
	//
	if (i.getDisplayNotification()) {
		::xercesc::DOMElement &s(
		    ::xsd::cxx::xml::dom::create_element("display-notification", "urn:ietf:params:xml:ns:imdn", e));

		s << *i.getDisplayNotification();
	}

	// processing-notification
	//
	if (i.getProcessingNotification()) {
		::xercesc::DOMElement &s(
		    ::xsd::cxx::xml::dom::create_element("processing-notification", "urn:ietf:params:xml:ns:imdn", e));

		s << *i.getProcessingNotification();
	}

	// any
	//
	for (Imdn::AnyConstIterator b(i.getAny().begin()), n(i.getAny().end()); b != n; ++b) {
		e.appendChild(e.getOwnerDocument()->importNode(const_cast<::xercesc::DOMElement *>(&(*b)), true));
	}
}

void operator<<(::xercesc::DOMElement &e, const DeliveryNotification &i) {
	e << static_cast<const ::LinphonePrivate::Xsd::XmlSchema::Type &>(i);

	// status
	//
	{
		::xercesc::DOMElement &s(::xsd::cxx::xml::dom::create_element("status", "urn:ietf:params:xml:ns:imdn", e));

		s << i.getStatus();
	}
}

void operator<<(::xercesc::DOMElement &e, const Delivered &i) {
	e << static_cast<const ::LinphonePrivate::Xsd::XmlSchema::Type &>(i);
}

void operator<<(::xercesc::DOMAttr &, const Delivered &) {
}

void operator<<(::LinphonePrivate::Xsd::XmlSchema::ListStream &, const Delivered &) {
}

void operator<<(::xercesc::DOMElement &e, const Failed &i) {
	e << static_cast<const ::LinphonePrivate::Xsd::XmlSchema::Type &>(i);
}

void operator<<(::xercesc::DOMAttr &, const Failed &) {
}

void operator<<(::LinphonePrivate::Xsd::XmlSchema::ListStream &, const Failed &) {
}

void operator<<(::xercesc::DOMElement &e, const DisplayNotification &i) {
	e << static_cast<const ::LinphonePrivate::Xsd::XmlSchema::Type &>(i);

	// status
	//
	{
		::xercesc::DOMElement &s(::xsd::cxx::xml::dom::create_element("status", "urn:ietf:params:xml:ns:imdn", e));

		s << i.getStatus();
	}
}

void operator<<(::xercesc::DOMElement &e, const Displayed &i) {
	e << static_cast<const ::LinphonePrivate::Xsd::XmlSchema::Type &>(i);
}

void operator<<(::xercesc::DOMAttr &, const Displayed &) {
}

void operator<<(::LinphonePrivate::Xsd::XmlSchema::ListStream &, const Displayed &) {
}

void operator<<(::xercesc::DOMElement &e, const ProcessingNotification &i) {
	e << static_cast<const ::LinphonePrivate::Xsd::XmlSchema::Type &>(i);

	// status
	//
	{
		::xercesc::DOMElement &s(::xsd::cxx::xml::dom::create_element("status", "urn:ietf:params:xml:ns:imdn", e));

		s << i.getStatus();
	}
}

void operator<<(::xercesc::DOMElement &e, const Processed &i) {
	e << static_cast<const ::LinphonePrivate::Xsd::XmlSchema::Type &>(i);
}

void operator<<(::xercesc::DOMAttr &, const Processed &) {
}

void operator<<(::LinphonePrivate::Xsd::XmlSchema::ListStream &, const Processed &) {
}

void operator<<(::xercesc::DOMElement &e, const Stored &i) {
	e << static_cast<const ::LinphonePrivate::Xsd::XmlSchema::Type &>(i);
}

void operator<<(::xercesc::DOMAttr &, const Stored &) {
}

void operator<<(::LinphonePrivate::Xsd::XmlSchema::ListStream &, const Stored &) {
}

void operator<<(::xercesc::DOMElement &e, const Forbidden &i) {
	e << static_cast<const ::LinphonePrivate::Xsd::XmlSchema::Type &>(i);
}

void operator<<(::xercesc::DOMAttr &, const Forbidden &) {
}

void operator<<(::LinphonePrivate::Xsd::XmlSchema::ListStream &, const Forbidden &) {
}

void operator<<(::xercesc::DOMElement &e, const Error &i) {
	e << static_cast<const ::LinphonePrivate::Xsd::XmlSchema::Type &>(i);
}

void operator<<(::xercesc::DOMAttr &, const Error &) {
}

void operator<<(::LinphonePrivate::Xsd::XmlSchema::ListStream &, const Error &) {
}

void operator<<(::xercesc::DOMElement &e, const Status &i) {
	e << static_cast<const ::LinphonePrivate::Xsd::XmlSchema::Type &>(i);

	// delivered
	//
	if (i.getDelivered()) {
		::xercesc::DOMElement &s(::xsd::cxx::xml::dom::create_element("delivered", "urn:ietf:params:xml:ns:imdn", e));

		s << *i.getDelivered();
	}

	// failed
	//
	if (i.getFailed()) {
		::xercesc::DOMElement &s(::xsd::cxx::xml::dom::create_element("failed", "urn:ietf:params:xml:ns:imdn", e));

		s << *i.getFailed();
	}

	// forbidden
	//
	if (i.getForbidden()) {
		::xercesc::DOMElement &s(::xsd::cxx::xml::dom::create_element("forbidden", "urn:ietf:params:xml:ns:imdn", e));

		s << *i.getForbidden();
	}

	// error
	//
	if (i.getError()) {
		::xercesc::DOMElement &s(::xsd::cxx::xml::dom::create_element("error", "urn:ietf:params:xml:ns:imdn", e));

		s << *i.getError();
	}

	// reason
	//
	if (i.getReason()) {
		::xercesc::DOMElement &s(
		    ::xsd::cxx::xml::dom::create_element("reason", "http://www.linphone.org/xsds/imdn.xsd", e));

		s << *i.getReason();
	}
}

void operator<<(::xercesc::DOMElement &e, const Status1 &i) {
	e << static_cast<const ::LinphonePrivate::Xsd::XmlSchema::Type &>(i);

	// displayed
	//
	if (i.getDisplayed()) {
		::xercesc::DOMElement &s(::xsd::cxx::xml::dom::create_element("displayed", "urn:ietf:params:xml:ns:imdn", e));

		s << *i.getDisplayed();
	}

	// forbidden
	//
	if (i.getForbidden()) {
		::xercesc::DOMElement &s(::xsd::cxx::xml::dom::create_element("forbidden", "urn:ietf:params:xml:ns:imdn", e));

		s << *i.getForbidden();
	}

	// error
	//
	if (i.getError()) {
		::xercesc::DOMElement &s(::xsd::cxx::xml::dom::create_element("error", "urn:ietf:params:xml:ns:imdn", e));

		s << *i.getError();
	}

	// any
	//
	for (Status1::AnyConstIterator b(i.getAny().begin()), n(i.getAny().end()); b != n; ++b) {
		e.appendChild(e.getOwnerDocument()->importNode(const_cast<::xercesc::DOMElement *>(&(*b)), true));
	}
}

void operator<<(::xercesc::DOMElement &e, const Status2 &i) {
	e << static_cast<const ::LinphonePrivate::Xsd::XmlSchema::Type &>(i);

	// processed
	//
	if (i.getProcessed()) {
		::xercesc::DOMElement &s(::xsd::cxx::xml::dom::create_element("processed", "urn:ietf:params:xml:ns:imdn", e));

		s << *i.getProcessed();
	}

	// stored
	//
	if (i.getStored()) {
		::xercesc::DOMElement &s(::xsd::cxx::xml::dom::create_element("stored", "urn:ietf:params:xml:ns:imdn", e));

		s << *i.getStored();
	}

	// forbidden
	//
	if (i.getForbidden()) {
		::xercesc::DOMElement &s(::xsd::cxx::xml::dom::create_element("forbidden", "urn:ietf:params:xml:ns:imdn", e));

		s << *i.getForbidden();
	}

	// error
	//
	if (i.getError()) {
		::xercesc::DOMElement &s(::xsd::cxx::xml::dom::create_element("error", "urn:ietf:params:xml:ns:imdn", e));

		s << *i.getError();
	}

	// any
	//
	for (Status2::AnyConstIterator b(i.getAny().begin()), n(i.getAny().end()); b != n; ++b) {
		e.appendChild(e.getOwnerDocument()->importNode(const_cast<::xercesc::DOMElement *>(&(*b)), true));
	}
}
} // namespace Imdn
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
