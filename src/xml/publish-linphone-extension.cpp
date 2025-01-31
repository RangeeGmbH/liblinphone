/*
 * Copyright (c) 2010-2023 Belledonne Communications SARL.
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
#if __GNUC__ >=7
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
#endif
//
// End prologue.

#include <xsd/cxx/pre.hxx>

#include "publish-linphone-extension.h"

namespace LinphonePrivate
{
  namespace Xsd
  {
    namespace PublishLinphoneExtension
    {
      // CryptoType
      // 

      const CryptoType::SspiType& CryptoType::
      getSspi () const
      {
        return this->sspi_.get ();
      }

      CryptoType::SspiType& CryptoType::
      getSspi ()
      {
        return this->sspi_.get ();
      }

      void CryptoType::
      setSspi (const SspiType& x)
      {
        this->sspi_.set (x);
      }

      const CryptoType::CspiType& CryptoType::
      getCspi () const
      {
        return this->cspi_.get ();
      }

      CryptoType::CspiType& CryptoType::
      getCspi ()
      {
        return this->cspi_.get ();
      }

      void CryptoType::
      setCspi (const CspiType& x)
      {
        this->cspi_.set (x);
      }

      void CryptoType::
      setCspi (::std::unique_ptr< CspiType > x)
      {
        this->cspi_.set (std::move (x));
      }

      ::std::unique_ptr< CryptoType::CspiType > CryptoType::
      setDetachCspi ()
      {
        return this->cspi_.detach ();
      }

      const CryptoType::CiphersOptional& CryptoType::
      getCiphers () const
      {
        return this->ciphers_;
      }

      CryptoType::CiphersOptional& CryptoType::
      getCiphers ()
      {
        return this->ciphers_;
      }

      void CryptoType::
      setCiphers (const CiphersType& x)
      {
        this->ciphers_.set (x);
      }

      void CryptoType::
      setCiphers (const CiphersOptional& x)
      {
        this->ciphers_ = x;
      }

      void CryptoType::
      setCiphers (::std::unique_ptr< CiphersType > x)
      {
        this->ciphers_.set (std::move (x));
      }

      const CryptoType::EntityType& CryptoType::
      getEntity () const
      {
        return this->entity_.get ();
      }

      CryptoType::EntityType& CryptoType::
      getEntity ()
      {
        return this->entity_.get ();
      }

      void CryptoType::
      setEntity (const EntityType& x)
      {
        this->entity_.set (x);
      }

      void CryptoType::
      setEntity (::std::unique_ptr< EntityType > x)
      {
        this->entity_.set (std::move (x));
      }

      ::std::unique_ptr< CryptoType::EntityType > CryptoType::
      setDetachEntity ()
      {
        return this->entity_.detach ();
      }


      // CiphersType
      // 

      const CiphersType::EncryptedektSequence& CiphersType::
      getEncryptedekt () const
      {
        return this->encryptedekt_;
      }

      CiphersType::EncryptedektSequence& CiphersType::
      getEncryptedekt ()
      {
        return this->encryptedekt_;
      }

      void CiphersType::
      setEncryptedekt (const EncryptedektSequence& s)
      {
        this->encryptedekt_ = s;
      }


      // EncryptedektType
      // 

      const EncryptedektType::DestType& EncryptedektType::
      getDest () const
      {
        return this->dest_.get ();
      }

      EncryptedektType::DestType& EncryptedektType::
      getDest ()
      {
        return this->dest_.get ();
      }

      void EncryptedektType::
      setDest (const DestType& x)
      {
        this->dest_.set (x);
      }

      void EncryptedektType::
      setDest (::std::unique_ptr< DestType > x)
      {
        this->dest_.set (std::move (x));
      }

      ::std::unique_ptr< EncryptedektType::DestType > EncryptedektType::
      setDetachDest ()
      {
        return this->dest_.detach ();
      }
    }
  }
}

#include <xsd/cxx/xml/dom/wildcard-source.hxx>

#include <xsd/cxx/xml/dom/parsing-source.hxx>

#include <xsd/cxx/tree/type-factory-map.hxx>

namespace _xsd
{
  static
  const ::xsd::cxx::tree::type_factory_plate< 0, char >
  type_factory_plate_init;
}

namespace LinphonePrivate
{
  namespace Xsd
  {
    namespace PublishLinphoneExtension
    {
      // CryptoType
      //

      CryptoType::
      CryptoType (const SspiType& sspi,
                  const CspiType& cspi,
                  const EntityType& entity)
      : ::LinphonePrivate::Xsd::XmlSchema::Type (),
        sspi_ (sspi, this),
        cspi_ (cspi, this),
        ciphers_ (this),
        entity_ (entity, this)
      {
      }

      CryptoType::
      CryptoType (const CryptoType& x,
                  ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                  ::LinphonePrivate::Xsd::XmlSchema::Container* c)
      : ::LinphonePrivate::Xsd::XmlSchema::Type (x, f, c),
        sspi_ (x.sspi_, f, this),
        cspi_ (x.cspi_, f, this),
        ciphers_ (x.ciphers_, f, this),
        entity_ (x.entity_, f, this)
      {
      }

      CryptoType::
      CryptoType (const ::xercesc::DOMElement& e,
                  ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                  ::LinphonePrivate::Xsd::XmlSchema::Container* c)
      : ::LinphonePrivate::Xsd::XmlSchema::Type (e, f | ::LinphonePrivate::Xsd::XmlSchema::Flags::base, c),
        sspi_ (this),
        cspi_ (this),
        ciphers_ (this),
        entity_ (this)
      {
        if ((f & ::LinphonePrivate::Xsd::XmlSchema::Flags::base) == 0)
        {
          ::xsd::cxx::xml::dom::parser< char > p (e, true, false, true);
          this->parse (p, f);
        }
      }

      void CryptoType::
      parse (::xsd::cxx::xml::dom::parser< char >& p,
             ::LinphonePrivate::Xsd::XmlSchema::Flags f)
      {
        for (; p.more_content (); p.next_content (false))
        {
          const ::xercesc::DOMElement& i (p.cur_element ());
          const ::xsd::cxx::xml::qualified_name< char > n (
            ::xsd::cxx::xml::dom::name< char > (i));

          // sspi
          //
          if (n.name () == "sspi" && n.namespace_ () == "linphone:xml:ns:publish-linphone-extension")
          {
            if (!sspi_.present ())
            {
              this->sspi_.set (SspiTraits::create (i, f, this));
              continue;
            }
          }

          // cspi
          //
          if (n.name () == "cspi" && n.namespace_ () == "linphone:xml:ns:publish-linphone-extension")
          {
            ::std::unique_ptr< CspiType > r (
              CspiTraits::create (i, f, this));

            if (!cspi_.present ())
            {
              this->cspi_.set (::std::move (r));
              continue;
            }
          }

          // ciphers
          //
          if (n.name () == "ciphers" && n.namespace_ () == "linphone:xml:ns:publish-linphone-extension")
          {
            ::std::unique_ptr< CiphersType > r (
              CiphersTraits::create (i, f, this));

            if (!this->ciphers_)
            {
              this->ciphers_.set (::std::move (r));
              continue;
            }
          }

          break;
        }

        if (!sspi_.present ())
        {
          throw ::xsd::cxx::tree::expected_element< char > (
            "sspi",
            "linphone:xml:ns:publish-linphone-extension");
        }

        if (!cspi_.present ())
        {
          throw ::xsd::cxx::tree::expected_element< char > (
            "cspi",
            "linphone:xml:ns:publish-linphone-extension");
        }

        while (p.more_attributes ())
        {
          const ::xercesc::DOMAttr& i (p.next_attribute ());
          const ::xsd::cxx::xml::qualified_name< char > n (
            ::xsd::cxx::xml::dom::name< char > (i));

          if (n.name () == "entity" && n.namespace_ ().empty ())
          {
            this->entity_.set (EntityTraits::create (i, f, this));
            continue;
          }
        }

        if (!entity_.present ())
        {
          throw ::xsd::cxx::tree::expected_attribute< char > (
            "entity",
            "");
        }
      }

      CryptoType* CryptoType::
      _clone (::LinphonePrivate::Xsd::XmlSchema::Flags f,
              ::LinphonePrivate::Xsd::XmlSchema::Container* c) const
      {
        return new class CryptoType (*this, f, c);
      }

      CryptoType& CryptoType::
      operator= (const CryptoType& x)
      {
        if (this != &x)
        {
          static_cast< ::LinphonePrivate::Xsd::XmlSchema::Type& > (*this) = x;
          this->sspi_ = x.sspi_;
          this->cspi_ = x.cspi_;
          this->ciphers_ = x.ciphers_;
          this->entity_ = x.entity_;
        }

        return *this;
      }

      CryptoType::
      ~CryptoType ()
      {
      }

      // CiphersType
      //

      CiphersType::
      CiphersType ()
      : ::LinphonePrivate::Xsd::XmlSchema::Type (),
        encryptedekt_ (this)
      {
      }

      CiphersType::
      CiphersType (const CiphersType& x,
                   ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                   ::LinphonePrivate::Xsd::XmlSchema::Container* c)
      : ::LinphonePrivate::Xsd::XmlSchema::Type (x, f, c),
        encryptedekt_ (x.encryptedekt_, f, this)
      {
      }

      CiphersType::
      CiphersType (const ::xercesc::DOMElement& e,
                   ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                   ::LinphonePrivate::Xsd::XmlSchema::Container* c)
      : ::LinphonePrivate::Xsd::XmlSchema::Type (e, f | ::LinphonePrivate::Xsd::XmlSchema::Flags::base, c),
        encryptedekt_ (this)
      {
        if ((f & ::LinphonePrivate::Xsd::XmlSchema::Flags::base) == 0)
        {
          ::xsd::cxx::xml::dom::parser< char > p (e, true, false, false);
          this->parse (p, f);
        }
      }

      void CiphersType::
      parse (::xsd::cxx::xml::dom::parser< char >& p,
             ::LinphonePrivate::Xsd::XmlSchema::Flags f)
      {
        for (; p.more_content (); p.next_content (false))
        {
          const ::xercesc::DOMElement& i (p.cur_element ());
          const ::xsd::cxx::xml::qualified_name< char > n (
            ::xsd::cxx::xml::dom::name< char > (i));

          // encryptedekt
          //
          if (n.name () == "encryptedekt" && n.namespace_ () == "linphone:xml:ns:publish-linphone-extension")
          {
            ::std::unique_ptr< EncryptedektType > r (
              EncryptedektTraits::create (i, f, this));

            this->encryptedekt_.push_back (::std::move (r));
            continue;
          }

          break;
        }
      }

      CiphersType* CiphersType::
      _clone (::LinphonePrivate::Xsd::XmlSchema::Flags f,
              ::LinphonePrivate::Xsd::XmlSchema::Container* c) const
      {
        return new class CiphersType (*this, f, c);
      }

      CiphersType& CiphersType::
      operator= (const CiphersType& x)
      {
        if (this != &x)
        {
          static_cast< ::LinphonePrivate::Xsd::XmlSchema::Type& > (*this) = x;
          this->encryptedekt_ = x.encryptedekt_;
        }

        return *this;
      }

      CiphersType::
      ~CiphersType ()
      {
      }

      // EncryptedektType
      //

      EncryptedektType::
      EncryptedektType (const DestType& dest)
      : ::LinphonePrivate::Xsd::XmlSchema::String (),
        dest_ (dest, this)
      {
      }

      EncryptedektType::
      EncryptedektType (const char* _xsd_String_base,
                        const DestType& dest)
      : ::LinphonePrivate::Xsd::XmlSchema::String (_xsd_String_base),
        dest_ (dest, this)
      {
      }

      EncryptedektType::
      EncryptedektType (const ::std::string& _xsd_String_base,
                        const DestType& dest)
      : ::LinphonePrivate::Xsd::XmlSchema::String (_xsd_String_base),
        dest_ (dest, this)
      {
      }

      EncryptedektType::
      EncryptedektType (const ::LinphonePrivate::Xsd::XmlSchema::String& _xsd_String_base,
                        const DestType& dest)
      : ::LinphonePrivate::Xsd::XmlSchema::String (_xsd_String_base),
        dest_ (dest, this)
      {
      }

      EncryptedektType::
      EncryptedektType (const EncryptedektType& x,
                        ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                        ::LinphonePrivate::Xsd::XmlSchema::Container* c)
      : ::LinphonePrivate::Xsd::XmlSchema::String (x, f, c),
        dest_ (x.dest_, f, this)
      {
      }

      EncryptedektType::
      EncryptedektType (const ::xercesc::DOMElement& e,
                        ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                        ::LinphonePrivate::Xsd::XmlSchema::Container* c)
      : ::LinphonePrivate::Xsd::XmlSchema::String (e, f | ::LinphonePrivate::Xsd::XmlSchema::Flags::base, c),
        dest_ (this)
      {
        if ((f & ::LinphonePrivate::Xsd::XmlSchema::Flags::base) == 0)
        {
          ::xsd::cxx::xml::dom::parser< char > p (e, false, false, true);
          this->parse (p, f);
        }
      }

      void EncryptedektType::
      parse (::xsd::cxx::xml::dom::parser< char >& p,
             ::LinphonePrivate::Xsd::XmlSchema::Flags f)
      {
        while (p.more_attributes ())
        {
          const ::xercesc::DOMAttr& i (p.next_attribute ());
          const ::xsd::cxx::xml::qualified_name< char > n (
            ::xsd::cxx::xml::dom::name< char > (i));

          if (n.name () == "dest" && n.namespace_ ().empty ())
          {
            this->dest_.set (DestTraits::create (i, f, this));
            continue;
          }
        }

        if (!dest_.present ())
        {
          throw ::xsd::cxx::tree::expected_attribute< char > (
            "dest",
            "");
        }
      }

      EncryptedektType* EncryptedektType::
      _clone (::LinphonePrivate::Xsd::XmlSchema::Flags f,
              ::LinphonePrivate::Xsd::XmlSchema::Container* c) const
      {
        return new class EncryptedektType (*this, f, c);
      }

      EncryptedektType& EncryptedektType::
      operator= (const EncryptedektType& x)
      {
        if (this != &x)
        {
          static_cast< ::LinphonePrivate::Xsd::XmlSchema::String& > (*this) = x;
          this->dest_ = x.dest_;
        }

        return *this;
      }

      EncryptedektType::
      ~EncryptedektType ()
      {
      }
    }
  }
}

#include <ostream>

#include <xsd/cxx/tree/std-ostream-map.hxx>

namespace _xsd
{
  static
  const ::xsd::cxx::tree::std_ostream_plate< 0, char >
  std_ostream_plate_init;
}

namespace LinphonePrivate
{
  namespace Xsd
  {
    namespace PublishLinphoneExtension
    {
      ::std::ostream&
      operator<< (::std::ostream& o, const CryptoType& i)
      {
        o << ::std::endl << "sspi: " << i.getSspi ();
        o << ::std::endl << "cspi: " << i.getCspi ();
        if (i.getCiphers ())
        {
          o << ::std::endl << "ciphers: " << *i.getCiphers ();
        }

        o << ::std::endl << "entity: " << i.getEntity ();
        return o;
      }

      ::std::ostream&
      operator<< (::std::ostream& o, const CiphersType& i)
      {
        for (CiphersType::EncryptedektConstIterator
             b (i.getEncryptedekt ().begin ()), e (i.getEncryptedekt ().end ());
             b != e; ++b)
        {
          o << ::std::endl << "encryptedekt: " << *b;
        }

        return o;
      }

      ::std::ostream&
      operator<< (::std::ostream& o, const EncryptedektType& i)
      {
        o << static_cast< const ::LinphonePrivate::Xsd::XmlSchema::String& > (i);

        o << ::std::endl << "dest: " << i.getDest ();
        return o;
      }
    }
  }
}

#include <istream>
#include <xsd/cxx/xml/sax/std-input-source.hxx>
#include <xsd/cxx/tree/error-handler.hxx>

namespace LinphonePrivate
{
  namespace Xsd
  {
    namespace PublishLinphoneExtension
    {
      ::std::unique_ptr< ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType >
      parseCrypto (const ::std::string& u,
                   ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                   const ::LinphonePrivate::Xsd::XmlSchema::Properties& p)
      {
        ::xsd::cxx::xml::auto_initializer i (
          (f & ::LinphonePrivate::Xsd::XmlSchema::Flags::dont_initialize) == 0,
          (f & ::LinphonePrivate::Xsd::XmlSchema::Flags::keep_dom) == 0);

        ::xsd::cxx::tree::error_handler< char > h;

        ::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr< ::xercesc::DOMDocument > d (
          ::xsd::cxx::xml::dom::parse< char > (
            u, h, p, f));

        h.throw_if_failed< ::xsd::cxx::tree::parsing< char > > ();

        return ::std::unique_ptr< ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType > (
          ::LinphonePrivate::Xsd::PublishLinphoneExtension::parseCrypto (
            std::move (d), f | ::LinphonePrivate::Xsd::XmlSchema::Flags::own_dom, p));
      }

      ::std::unique_ptr< ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType >
      parseCrypto (const ::std::string& u,
                   ::LinphonePrivate::Xsd::XmlSchema::ErrorHandler& h,
                   ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                   const ::LinphonePrivate::Xsd::XmlSchema::Properties& p)
      {
        ::xsd::cxx::xml::auto_initializer i (
          (f & ::LinphonePrivate::Xsd::XmlSchema::Flags::dont_initialize) == 0,
          (f & ::LinphonePrivate::Xsd::XmlSchema::Flags::keep_dom) == 0);

        ::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr< ::xercesc::DOMDocument > d (
          ::xsd::cxx::xml::dom::parse< char > (
            u, h, p, f));

        if (!d.get ())
          throw ::xsd::cxx::tree::parsing< char > ();

        return ::std::unique_ptr< ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType > (
          ::LinphonePrivate::Xsd::PublishLinphoneExtension::parseCrypto (
            std::move (d), f | ::LinphonePrivate::Xsd::XmlSchema::Flags::own_dom, p));
      }

      ::std::unique_ptr< ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType >
      parseCrypto (const ::std::string& u,
                   ::xercesc::DOMErrorHandler& h,
                   ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                   const ::LinphonePrivate::Xsd::XmlSchema::Properties& p)
      {
        ::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr< ::xercesc::DOMDocument > d (
          ::xsd::cxx::xml::dom::parse< char > (
            u, h, p, f));

        if (!d.get ())
          throw ::xsd::cxx::tree::parsing< char > ();

        return ::std::unique_ptr< ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType > (
          ::LinphonePrivate::Xsd::PublishLinphoneExtension::parseCrypto (
            std::move (d), f | ::LinphonePrivate::Xsd::XmlSchema::Flags::own_dom, p));
      }

      ::std::unique_ptr< ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType >
      parseCrypto (::std::istream& is,
                   ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                   const ::LinphonePrivate::Xsd::XmlSchema::Properties& p)
      {
        ::xsd::cxx::xml::auto_initializer i (
          (f & ::LinphonePrivate::Xsd::XmlSchema::Flags::dont_initialize) == 0,
          (f & ::LinphonePrivate::Xsd::XmlSchema::Flags::keep_dom) == 0);

        ::xsd::cxx::xml::sax::std_input_source isrc (is);
        return ::LinphonePrivate::Xsd::PublishLinphoneExtension::parseCrypto (isrc, f, p);
      }

      ::std::unique_ptr< ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType >
      parseCrypto (::std::istream& is,
                   ::LinphonePrivate::Xsd::XmlSchema::ErrorHandler& h,
                   ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                   const ::LinphonePrivate::Xsd::XmlSchema::Properties& p)
      {
        ::xsd::cxx::xml::auto_initializer i (
          (f & ::LinphonePrivate::Xsd::XmlSchema::Flags::dont_initialize) == 0,
          (f & ::LinphonePrivate::Xsd::XmlSchema::Flags::keep_dom) == 0);

        ::xsd::cxx::xml::sax::std_input_source isrc (is);
        return ::LinphonePrivate::Xsd::PublishLinphoneExtension::parseCrypto (isrc, h, f, p);
      }

      ::std::unique_ptr< ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType >
      parseCrypto (::std::istream& is,
                   ::xercesc::DOMErrorHandler& h,
                   ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                   const ::LinphonePrivate::Xsd::XmlSchema::Properties& p)
      {
        ::xsd::cxx::xml::sax::std_input_source isrc (is);
        return ::LinphonePrivate::Xsd::PublishLinphoneExtension::parseCrypto (isrc, h, f, p);
      }

      ::std::unique_ptr< ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType >
      parseCrypto (::std::istream& is,
                   const ::std::string& sid,
                   ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                   const ::LinphonePrivate::Xsd::XmlSchema::Properties& p)
      {
        ::xsd::cxx::xml::auto_initializer i (
          (f & ::LinphonePrivate::Xsd::XmlSchema::Flags::dont_initialize) == 0,
          (f & ::LinphonePrivate::Xsd::XmlSchema::Flags::keep_dom) == 0);

        ::xsd::cxx::xml::sax::std_input_source isrc (is, sid);
        return ::LinphonePrivate::Xsd::PublishLinphoneExtension::parseCrypto (isrc, f, p);
      }

      ::std::unique_ptr< ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType >
      parseCrypto (::std::istream& is,
                   const ::std::string& sid,
                   ::LinphonePrivate::Xsd::XmlSchema::ErrorHandler& h,
                   ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                   const ::LinphonePrivate::Xsd::XmlSchema::Properties& p)
      {
        ::xsd::cxx::xml::auto_initializer i (
          (f & ::LinphonePrivate::Xsd::XmlSchema::Flags::dont_initialize) == 0,
          (f & ::LinphonePrivate::Xsd::XmlSchema::Flags::keep_dom) == 0);

        ::xsd::cxx::xml::sax::std_input_source isrc (is, sid);
        return ::LinphonePrivate::Xsd::PublishLinphoneExtension::parseCrypto (isrc, h, f, p);
      }

      ::std::unique_ptr< ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType >
      parseCrypto (::std::istream& is,
                   const ::std::string& sid,
                   ::xercesc::DOMErrorHandler& h,
                   ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                   const ::LinphonePrivate::Xsd::XmlSchema::Properties& p)
      {
        ::xsd::cxx::xml::sax::std_input_source isrc (is, sid);
        return ::LinphonePrivate::Xsd::PublishLinphoneExtension::parseCrypto (isrc, h, f, p);
      }

      ::std::unique_ptr< ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType >
      parseCrypto (::xercesc::InputSource& i,
                   ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                   const ::LinphonePrivate::Xsd::XmlSchema::Properties& p)
      {
        ::xsd::cxx::tree::error_handler< char > h;

        ::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr< ::xercesc::DOMDocument > d (
          ::xsd::cxx::xml::dom::parse< char > (
            i, h, p, f));

        h.throw_if_failed< ::xsd::cxx::tree::parsing< char > > ();

        return ::std::unique_ptr< ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType > (
          ::LinphonePrivate::Xsd::PublishLinphoneExtension::parseCrypto (
            std::move (d), f | ::LinphonePrivate::Xsd::XmlSchema::Flags::own_dom, p));
      }

      ::std::unique_ptr< ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType >
      parseCrypto (::xercesc::InputSource& i,
                   ::LinphonePrivate::Xsd::XmlSchema::ErrorHandler& h,
                   ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                   const ::LinphonePrivate::Xsd::XmlSchema::Properties& p)
      {
        ::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr< ::xercesc::DOMDocument > d (
          ::xsd::cxx::xml::dom::parse< char > (
            i, h, p, f));

        if (!d.get ())
          throw ::xsd::cxx::tree::parsing< char > ();

        return ::std::unique_ptr< ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType > (
          ::LinphonePrivate::Xsd::PublishLinphoneExtension::parseCrypto (
            std::move (d), f | ::LinphonePrivate::Xsd::XmlSchema::Flags::own_dom, p));
      }

      ::std::unique_ptr< ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType >
      parseCrypto (::xercesc::InputSource& i,
                   ::xercesc::DOMErrorHandler& h,
                   ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                   const ::LinphonePrivate::Xsd::XmlSchema::Properties& p)
      {
        ::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr< ::xercesc::DOMDocument > d (
          ::xsd::cxx::xml::dom::parse< char > (
            i, h, p, f));

        if (!d.get ())
          throw ::xsd::cxx::tree::parsing< char > ();

        return ::std::unique_ptr< ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType > (
          ::LinphonePrivate::Xsd::PublishLinphoneExtension::parseCrypto (
            std::move (d), f | ::LinphonePrivate::Xsd::XmlSchema::Flags::own_dom, p));
      }

      ::std::unique_ptr< ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType >
      parseCrypto (const ::xercesc::DOMDocument& doc,
                   ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                   const ::LinphonePrivate::Xsd::XmlSchema::Properties& p)
      {
        if (f & ::LinphonePrivate::Xsd::XmlSchema::Flags::keep_dom)
        {
          ::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr< ::xercesc::DOMDocument > d (
            static_cast< ::xercesc::DOMDocument* > (doc.cloneNode (true)));

          return ::std::unique_ptr< ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType > (
            ::LinphonePrivate::Xsd::PublishLinphoneExtension::parseCrypto (
              std::move (d), f | ::LinphonePrivate::Xsd::XmlSchema::Flags::own_dom, p));
        }

        const ::xercesc::DOMElement& e (*doc.getDocumentElement ());
        const ::xsd::cxx::xml::qualified_name< char > n (
          ::xsd::cxx::xml::dom::name< char > (e));

        if (n.name () == "crypto" &&
            n.namespace_ () == "linphone:xml:ns:publish-linphone-extension")
        {
          ::std::unique_ptr< ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType > r (
            ::xsd::cxx::tree::traits< ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType, char >::create (
              e, f, 0));
          return r;
        }

        throw ::xsd::cxx::tree::unexpected_element < char > (
          n.name (),
          n.namespace_ (),
          "crypto",
          "linphone:xml:ns:publish-linphone-extension");
      }

      ::std::unique_ptr< ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType >
      parseCrypto (::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr< ::xercesc::DOMDocument > d,
                   ::LinphonePrivate::Xsd::XmlSchema::Flags f,
                   const ::LinphonePrivate::Xsd::XmlSchema::Properties&)
      {
        ::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr< ::xercesc::DOMDocument > c (
          ((f & ::LinphonePrivate::Xsd::XmlSchema::Flags::keep_dom) &&
           !(f & ::LinphonePrivate::Xsd::XmlSchema::Flags::own_dom))
          ? static_cast< ::xercesc::DOMDocument* > (d->cloneNode (true))
          : 0);

        ::xercesc::DOMDocument& doc (c.get () ? *c : *d);
        const ::xercesc::DOMElement& e (*doc.getDocumentElement ());

        const ::xsd::cxx::xml::qualified_name< char > n (
          ::xsd::cxx::xml::dom::name< char > (e));

        if (f & ::LinphonePrivate::Xsd::XmlSchema::Flags::keep_dom)
          doc.setUserData (::LinphonePrivate::Xsd::XmlSchema::dom::treeNodeKey,
                           (c.get () ? &c : &d),
                           0);

        if (n.name () == "crypto" &&
            n.namespace_ () == "linphone:xml:ns:publish-linphone-extension")
        {
          ::std::unique_ptr< ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType > r (
            ::xsd::cxx::tree::traits< ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType, char >::create (
              e, f, 0));
          return r;
        }

        throw ::xsd::cxx::tree::unexpected_element < char > (
          n.name (),
          n.namespace_ (),
          "crypto",
          "linphone:xml:ns:publish-linphone-extension");
      }
    }
  }
}

#include <ostream>
#include <xsd/cxx/tree/error-handler.hxx>
#include <xsd/cxx/xml/dom/serialization-source.hxx>

#include <xsd/cxx/tree/type-serializer-map.hxx>

namespace _xsd
{
  static
  const ::xsd::cxx::tree::type_serializer_plate< 0, char >
  type_serializer_plate_init;
}

namespace LinphonePrivate
{
  namespace Xsd
  {
    namespace PublishLinphoneExtension
    {
      void
      serializeCrypto (::std::ostream& o,
                       const ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType& s,
                       const ::LinphonePrivate::Xsd::XmlSchema::NamespaceInfomap& m,
                       const ::std::string& e,
                       ::LinphonePrivate::Xsd::XmlSchema::Flags f)
      {
        ::xsd::cxx::xml::auto_initializer i (
          (f & ::LinphonePrivate::Xsd::XmlSchema::Flags::dont_initialize) == 0);

        ::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr< ::xercesc::DOMDocument > d (
          ::LinphonePrivate::Xsd::PublishLinphoneExtension::serializeCrypto (s, m, f));

        ::xsd::cxx::tree::error_handler< char > h;

        ::xsd::cxx::xml::dom::ostream_format_target t (o);
        if (!::xsd::cxx::xml::dom::serialize (t, *d, e, h, f))
        {
          h.throw_if_failed< ::xsd::cxx::tree::serialization< char > > ();
        }
      }

      void
      serializeCrypto (::std::ostream& o,
                       const ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType& s,
                       ::LinphonePrivate::Xsd::XmlSchema::ErrorHandler& h,
                       const ::LinphonePrivate::Xsd::XmlSchema::NamespaceInfomap& m,
                       const ::std::string& e,
                       ::LinphonePrivate::Xsd::XmlSchema::Flags f)
      {
        ::xsd::cxx::xml::auto_initializer i (
          (f & ::LinphonePrivate::Xsd::XmlSchema::Flags::dont_initialize) == 0);

        ::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr< ::xercesc::DOMDocument > d (
          ::LinphonePrivate::Xsd::PublishLinphoneExtension::serializeCrypto (s, m, f));
        ::xsd::cxx::xml::dom::ostream_format_target t (o);
        if (!::xsd::cxx::xml::dom::serialize (t, *d, e, h, f))
        {
          throw ::xsd::cxx::tree::serialization< char > ();
        }
      }

      void
      serializeCrypto (::std::ostream& o,
                       const ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType& s,
                       ::xercesc::DOMErrorHandler& h,
                       const ::LinphonePrivate::Xsd::XmlSchema::NamespaceInfomap& m,
                       const ::std::string& e,
                       ::LinphonePrivate::Xsd::XmlSchema::Flags f)
      {
        ::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr< ::xercesc::DOMDocument > d (
          ::LinphonePrivate::Xsd::PublishLinphoneExtension::serializeCrypto (s, m, f));
        ::xsd::cxx::xml::dom::ostream_format_target t (o);
        if (!::xsd::cxx::xml::dom::serialize (t, *d, e, h, f))
        {
          throw ::xsd::cxx::tree::serialization< char > ();
        }
      }

      void
      serializeCrypto (::xercesc::XMLFormatTarget& t,
                       const ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType& s,
                       const ::LinphonePrivate::Xsd::XmlSchema::NamespaceInfomap& m,
                       const ::std::string& e,
                       ::LinphonePrivate::Xsd::XmlSchema::Flags f)
      {
        ::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr< ::xercesc::DOMDocument > d (
          ::LinphonePrivate::Xsd::PublishLinphoneExtension::serializeCrypto (s, m, f));

        ::xsd::cxx::tree::error_handler< char > h;

        if (!::xsd::cxx::xml::dom::serialize (t, *d, e, h, f))
        {
          h.throw_if_failed< ::xsd::cxx::tree::serialization< char > > ();
        }
      }

      void
      serializeCrypto (::xercesc::XMLFormatTarget& t,
                       const ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType& s,
                       ::LinphonePrivate::Xsd::XmlSchema::ErrorHandler& h,
                       const ::LinphonePrivate::Xsd::XmlSchema::NamespaceInfomap& m,
                       const ::std::string& e,
                       ::LinphonePrivate::Xsd::XmlSchema::Flags f)
      {
        ::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr< ::xercesc::DOMDocument > d (
          ::LinphonePrivate::Xsd::PublishLinphoneExtension::serializeCrypto (s, m, f));
        if (!::xsd::cxx::xml::dom::serialize (t, *d, e, h, f))
        {
          throw ::xsd::cxx::tree::serialization< char > ();
        }
      }

      void
      serializeCrypto (::xercesc::XMLFormatTarget& t,
                       const ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType& s,
                       ::xercesc::DOMErrorHandler& h,
                       const ::LinphonePrivate::Xsd::XmlSchema::NamespaceInfomap& m,
                       const ::std::string& e,
                       ::LinphonePrivate::Xsd::XmlSchema::Flags f)
      {
        ::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr< ::xercesc::DOMDocument > d (
          ::LinphonePrivate::Xsd::PublishLinphoneExtension::serializeCrypto (s, m, f));
        if (!::xsd::cxx::xml::dom::serialize (t, *d, e, h, f))
        {
          throw ::xsd::cxx::tree::serialization< char > ();
        }
      }

      void
      serializeCrypto (::xercesc::DOMDocument& d,
                       const ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType& s,
                       ::LinphonePrivate::Xsd::XmlSchema::Flags)
      {
        ::xercesc::DOMElement& e (*d.getDocumentElement ());
        const ::xsd::cxx::xml::qualified_name< char > n (
          ::xsd::cxx::xml::dom::name< char > (e));

        if (n.name () == "crypto" &&
            n.namespace_ () == "linphone:xml:ns:publish-linphone-extension")
        {
          e << s;
        }
        else
        {
          throw ::xsd::cxx::tree::unexpected_element < char > (
            n.name (),
            n.namespace_ (),
            "crypto",
            "linphone:xml:ns:publish-linphone-extension");
        }
      }

      ::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr< ::xercesc::DOMDocument >
      serializeCrypto (const ::LinphonePrivate::Xsd::PublishLinphoneExtension::CryptoType& s,
                       const ::LinphonePrivate::Xsd::XmlSchema::NamespaceInfomap& m,
                       ::LinphonePrivate::Xsd::XmlSchema::Flags f)
      {
        ::LinphonePrivate::Xsd::XmlSchema::dom::unique_ptr< ::xercesc::DOMDocument > d (
          ::xsd::cxx::xml::dom::serialize< char > (
            "crypto",
            "linphone:xml:ns:publish-linphone-extension",
            m, f));

        ::LinphonePrivate::Xsd::PublishLinphoneExtension::serializeCrypto (*d, s, f);
        return d;
      }

      void
      operator<< (::xercesc::DOMElement& e, const CryptoType& i)
      {
        e << static_cast< const ::LinphonePrivate::Xsd::XmlSchema::Type& > (i);

        // sspi
        //
        {
          ::xercesc::DOMElement& s (
            ::xsd::cxx::xml::dom::create_element (
              "sspi",
              "linphone:xml:ns:publish-linphone-extension",
              e));

          s << i.getSspi ();
        }

        // cspi
        //
        {
          ::xercesc::DOMElement& s (
            ::xsd::cxx::xml::dom::create_element (
              "cspi",
              "linphone:xml:ns:publish-linphone-extension",
              e));

          s << i.getCspi ();
        }

        // ciphers
        //
        if (i.getCiphers ())
        {
          ::xercesc::DOMElement& s (
            ::xsd::cxx::xml::dom::create_element (
              "ciphers",
              "linphone:xml:ns:publish-linphone-extension",
              e));

          s << *i.getCiphers ();
        }

        // entity
        //
        {
          ::xercesc::DOMAttr& a (
            ::xsd::cxx::xml::dom::create_attribute (
              "entity",
              e));

          a << i.getEntity ();
        }
      }

      void
      operator<< (::xercesc::DOMElement& e, const CiphersType& i)
      {
        e << static_cast< const ::LinphonePrivate::Xsd::XmlSchema::Type& > (i);

        // encryptedekt
        //
        for (CiphersType::EncryptedektConstIterator
             b (i.getEncryptedekt ().begin ()), n (i.getEncryptedekt ().end ());
             b != n; ++b)
        {
          ::xercesc::DOMElement& s (
            ::xsd::cxx::xml::dom::create_element (
              "encryptedekt",
              "linphone:xml:ns:publish-linphone-extension",
              e));

          s << *b;
        }
      }

      void
      operator<< (::xercesc::DOMElement& e, const EncryptedektType& i)
      {
        e << static_cast< const ::LinphonePrivate::Xsd::XmlSchema::String& > (i);

        // dest
        //
        {
          ::xercesc::DOMAttr& a (
            ::xsd::cxx::xml::dom::create_attribute (
              "dest",
              e));

          a << i.getDest ();
        }
      }
    }
  }
}

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

