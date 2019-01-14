//
// Copyright (c) 2017-2019 Rokas Kupstys.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#ifndef CEREAL_FUTURE_EXPERIMENTAL
#   define CEREAL_FUTURE_EXPERIMENTAL
#endif
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/adapters.hpp>
#include <fstream>

#include "../Container/Ptr.h"
#include "../Core/Context.h"
#include "../Core/Object.h"
#include "../Core/StringUtils.h"
#include "../Core/Variant.h"
#include "../IO/Serializer.h"
#include "../IO/Deserializer.h"

namespace cereal
{

#define URHO3D_SERIALIZE_PLAIN(Format, Type)                         \
    inline void prologue(Format##OutputArchive& ar, Type const&) { } \
    inline void epilogue(Format##OutputArchive& ar, Type const&) { } \
    inline void prologue(Format##InputArchive& ar, Type const&) { }  \
    inline void epilogue(Format##InputArchive& ar, Type const&) { }  \


// Allow constructing subclasses of Object. They need Context* passed to the constructor.
template <> struct LoadAndConstruct<Urho3D::Object>
{
    template <class Archive, class T>
    static
    typename std::enable_if<std::is_base_of<Urho3D::Object, T>::value, void>::type
    load_and_construct(Archive& ar, cereal::construct<T>& construct, uint32_t version)
    {
        construct(cereal::get_user_data<Urho3D::Context*>(ar));
    }
};

// Serialize Variant
template <class Archive> inline
void CEREAL_SAVE_FUNCTION_NAME(Archive& ar, const Urho3D::Variant& variant)
{
    using namespace Urho3D;
    // Arbitrary pointers can not be serialized.
    if (variant.GetType() == VAR_VOIDPTR || variant.GetType() == VAR_CUSTOM_HEAP || variant.GetType() == VAR_CUSTOM_STACK)
        return;

//    // Pointers to Serializable can be serialized.
//    Serializable* serializable = nullptr;
//    if (variant.GetType() == VAR_PTR)
//    {
//        serializable = dynamic_cast<Serializable*>(variant.GetPtr());
//        if (serializable == nullptr)
//            return;
//    }

    switch (variant.GetType())
    {
    case VAR_NONE:
        break;
    case VAR_INT:
        ar(variant.GetInt());
        break;
    case VAR_BOOL:
        ar(variant.GetBool());
        break;
    case VAR_FLOAT:
        ar(variant.GetFloat());
        break;
    case VAR_VECTOR2:
        ar(variant.GetVector2());
        break;
//    case VAR_VECTOR3:
//        ar(variant.GetVector3());
//        break;
//    case VAR_VECTOR4:
//        ar(variant.GetVector4());
//        break;
//    case VAR_QUATERNION:
//        ar(variant.GetQuaternion());
//        break;
//    case VAR_COLOR:
//        ar(variant.GetColor());
//        break;
//    case VAR_STRING:
//        ar(variant.GetString());
//        break;
//    case VAR_BUFFER:
//        ar(variant.GetBuffer());
//        break;
//    case VAR_RESOURCEREF:
//        ar(variant.GetResourceRef());
//        break;
//    case VAR_RESOURCEREFLIST:
//        ar(variant.GetResourceRefList());
//        break;
//    case VAR_VARIANTVECTOR:
//        ar(variant.GetVariantVector());
//        break;
//    case VAR_VARIANTMAP:
//        ar(variant.GetVariantMap());
//        break;
//    case VAR_INTRECT:
//        ar(variant.GetIntRect());
//        break;
//    case VAR_INTVECTOR2:
//        ar(variant.GetIntVector2());
//        break;
//    case VAR_PTR:
//        break;
//    case VAR_MATRIX3:
//        ar(variant.GetMatrix3());
//        break;
//    case VAR_MATRIX3X4:
//        ar(variant.GetMatrix3x4());
//        break;
//    case VAR_MATRIX4:
//        ar(variant.GetMatrix4());
//        break;
    case VAR_DOUBLE:
        ar(variant.GetDouble());
        break;
//    case VAR_STRINGVECTOR:
//        ar(variant.GetStringVector());
//        break;
//    case VAR_RECT:
//        ar(variant.GetRect());
//        break;
//    case VAR_INTVECTOR3:
//        ar(variant.GetIntVector3());
//        break;
    case VAR_INT64:
        ar(variant.GetInt64());
        break;
    default:
        break;
    }
}

template <class Archive> inline
void CEREAL_LOAD_FUNCTION_NAME(Archive& ar, Urho3D::Variant& variant)
{
    using namespace Urho3D;
    // Arbitrary pointers can not be serialized.
    if (variant.GetType() == VAR_VOIDPTR || variant.GetType() == VAR_CUSTOM_HEAP || variant.GetType() == VAR_CUSTOM_STACK)
        return;

#define DESERIALIZE_VALUE(Type)     \
    {                               \
        Type value{};               \
        ar(value);                  \
        variant = std::move(value); \
    }

    switch (variant.GetType())
    {
    case VAR_NONE:
        break;
    case VAR_INT:
        DESERIALIZE_VALUE(int);
        break;
    case VAR_BOOL:
        DESERIALIZE_VALUE(bool);
        break;
    case VAR_FLOAT:
        DESERIALIZE_VALUE(float);
        break;
    case VAR_VECTOR2:
        DESERIALIZE_VALUE(Vector2);
        break;
//    case VAR_VECTOR3:
//        DESERIALIZE_VALUE(Vector3);
//        break;
//    case VAR_VECTOR4:
//        DESERIALIZE_VALUE(Vector4);
//        break;
//    case VAR_QUATERNION:
//        DESERIALIZE_VALUE(Quaternion);
//        break;
//    case VAR_COLOR:
//        DESERIALIZE_VALUE(Color);
//        break;
//    case VAR_STRING:
//        DESERIALIZE_VALUE(String);
//        break;
//    case VAR_BUFFER:
//        DESERIALIZE_VALUE(PODVector<unsigned char>);
//        break;
//    case VAR_RESOURCEREF:
//        DESERIALIZE_VALUE(ResourceRef);
//        break;
//    case VAR_RESOURCEREFLIST:
//        DESERIALIZE_VALUE(ResourceRefList);
//        break;
//    case VAR_VARIANTVECTOR:
//        DESERIALIZE_VALUE(VariantVector);
//        break;
//    case VAR_VARIANTMAP:
//        DESERIALIZE_VALUE(VariantMap);
//        break;
//    case VAR_INTRECT:
//        DESERIALIZE_VALUE(IntRect);
//        break;
//    case VAR_INTVECTOR2:
//        DESERIALIZE_VALUE(IntVector2);
//        break;
//    case VAR_PTR:
//        break;
//    case VAR_MATRIX3:
//        DESERIALIZE_VALUE(Matrix3);
//        break;
//    case VAR_MATRIX3X4:
//        DESERIALIZE_VALUE(Matrix3x4);
//        break;
//    case VAR_MATRIX4:
//        DESERIALIZE_VALUE(Matrix4);
//        break;
    case VAR_DOUBLE:
        DESERIALIZE_VALUE(double);
        break;
//    case VAR_STRINGVECTOR:
//        DESERIALIZE_VALUE(StringVector);
//        break;
//    case VAR_RECT:
//        DESERIALIZE_VALUE(Rect);
//        break;
//    case VAR_INTVECTOR3:
//        DESERIALIZE_VALUE(IntVector3);
//        break;
    case VAR_INT64:
        DESERIALIZE_VALUE(long long int);
        break;
    default:
        break;
    }

#undef DESERIALIZE_VALUE
}

URHO3D_SERIALIZE_PLAIN(XML, Urho3D::Variant);
URHO3D_SERIALIZE_PLAIN(JSON, Urho3D::Variant);

// StringHash
template <class Archive> inline
void CEREAL_SAVE_FUNCTION_NAME(Archive& ar, const Urho3D::StringHash& value)
{
    ar(value.Value());
}

template <class Archive> inline
void CEREAL_LOAD_FUNCTION_NAME(Archive& ar, Urho3D::StringHash& value)
{
    unsigned hash{};
    ar(hash);
    value = Urho3D::StringHash(hash);
}

URHO3D_SERIALIZE_PLAIN(XML, Urho3D::StringHash);
URHO3D_SERIALIZE_PLAIN(JSON, Urho3D::StringHash);

// String
template<typename Archive> inline void
CEREAL_SAVE_FUNCTION_NAME(Archive& ar, const Urho3D::String& value)
{
    // TODO: cereal should be patched to support `const char*` as string so we can avoid extra copies here.
    ar(std::string(value.CString()));
}

template <class Archive> inline void
CEREAL_LOAD_FUNCTION_NAME(Archive& ar, Urho3D::String& value)
{
    std::string str;
    ar(str);
    value = str;
}
URHO3D_SERIALIZE_PLAIN(XML, Urho3D::String);
URHO3D_SERIALIZE_PLAIN(JSON, Urho3D::String);

// Vector2
template <class Archive> inline
void CEREAL_SAVE_FUNCTION_NAME(Archive& ar, const Urho3D::Vector2& value)
{
    if (std::is_same<Archive, JSONInputArchive>::value || std::is_same<Archive, JSONOutputArchive>::value)
    {
        size_type size = 2;
        ar(make_size_tag(size));
        ar(value.x_, value.y_);
    }
    else if (std::is_same<Archive, XMLInputArchive>::value || std::is_same<Archive, XMLOutputArchive>::value)
        ar(std::string(value.ToString().CString()));
    else
        ar(value.x_, value.y_);
}

template <class Archive> inline
void CEREAL_LOAD_FUNCTION_NAME(Archive& ar, Urho3D::Vector2& value)
{
    if (std::is_same<Archive, JSONInputArchive>::value || std::is_same<Archive, JSONOutputArchive>::value)
    {
        size_type size = 2;
        ar(make_size_tag(size));
        ar(value.x_, value.y_);
    }
    else if (std::is_same<Archive, XMLInputArchive>::value || std::is_same<Archive, XMLOutputArchive>::value)
    {
        std::string text;
        ar(text);
        value = Urho3D::ToVector2(text.c_str());
    }
    else
        ar(value.x_, value.y_);
}
URHO3D_SERIALIZE_PLAIN(XML, Urho3D::Vector2);



#undef URHO3D_SERIALIZE_PLAIN



//// Shared pointers
//
////! Saving Urho3D::SharedPtr for non polymorphic types
//template <class Archive, class T> inline
//typename std::enable_if<!std::is_polymorphic<T>::value, void>::type
//CEREAL_SAVE_FUNCTION_NAME( Archive & ar, Urho3D::SharedPtr<T> const & ptr )
//{
//    ar( CEREAL_NVP_("ptr_wrapper", memory_detail::make_ptr_wrapper( ptr )) );
//}
//
////! Loading Urho3D::SharedPtr, case when no user load and construct for non polymorphic types
//template <class Archive, class T> inline
//typename std::enable_if<!std::is_polymorphic<T>::value, void>::type
//CEREAL_LOAD_FUNCTION_NAME( Archive & ar, Urho3D::SharedPtr<T> & ptr )
//{
//    ar( CEREAL_NVP_("ptr_wrapper", memory_detail::make_ptr_wrapper( ptr )) );
//}
//
////! Saving Urho3D::WeakPtr for non polymorphic types
//template <class Archive, class T> inline
//typename std::enable_if<!std::is_polymorphic<T>::value, void>::type
//CEREAL_SAVE_FUNCTION_NAME( Archive & ar, Urho3D::WeakPtr<T> const & ptr )
//{
//    auto const sptr = ptr.lock();
//    ar( CEREAL_NVP_("ptr_wrapper", memory_detail::make_ptr_wrapper( sptr )) );
//}
//
////! Loading Urho3D::WeakPtr for non polymorphic types
//template <class Archive, class T> inline
//typename std::enable_if<!std::is_polymorphic<T>::value, void>::type
//CEREAL_LOAD_FUNCTION_NAME( Archive & ar, Urho3D::WeakPtr<T> & ptr )
//{
//    Urho3D::SharedPtr<T> sptr;
//    ar( CEREAL_NVP_("ptr_wrapper", memory_detail::make_ptr_wrapper( sptr )) );
//    ptr = sptr;
//}
//
//
////! Saving Urho3D::SharedPtr (wrapper implementation)
///*! @internal */
//template <class Archive, class T> inline
//void CEREAL_SAVE_FUNCTION_NAME( Archive & ar, memory_detail::PtrWrapper<Urho3D::SharedPtr<T> const &> const & wrapper )
//{
//    auto & ptr = wrapper.ptr;
//
//    uint32_t id = ar.registerSharedPointer( ptr.get() );
//    ar( CEREAL_NVP_("id", id) );
//
//    if( id & detail::msb_32bit )
//    {
//        ar( CEREAL_NVP_("data", *ptr) );
//    }
//}
//
////! Loading Urho3D::SharedPtr, case when user load and construct (wrapper implementation)
///*! @internal */
//template <class Archive, class T> inline
//typename std::enable_if<traits::has_load_and_construct<T, Archive>::value, void>::type
//CEREAL_LOAD_FUNCTION_NAME( Archive & ar, memory_detail::PtrWrapper<Urho3D::SharedPtr<T> &> & wrapper )
//{
//    auto & ptr = wrapper.ptr;
//
//    uint32_t id;
//
//    ar( CEREAL_NVP_("id", id) );
//
//    if( id & detail::msb_32bit )
//    {
//        // Storage type for the pointer - since we can't default construct this type,
//        // we'll allocate it using std::aligned_storage and use a custom deleter
//        using ST = typename std::aligned_storage<sizeof(T), CEREAL_ALIGNOF(T)>::type;
//
//
//        Urho3D::Context* context = cereal::get_user_data<Urho3D::Context*>(ar);
//        context->GetObjectFactories()
//
//        // Valid flag - set to true once construction finishes
//        //  This prevents us from calling the destructor on
//        //  uninitialized data.
//        auto valid = std::make_shared<bool>( false );
//
//        // Allocate our storage, which we will treat as
//        //  uninitialized until initialized with placement new
//        ptr.reset( reinterpret_cast<T *>( new ST() ),
//            [=]( T * t )
//            {
//                if( *valid )
//                    t->~T();
//
//                delete reinterpret_cast<ST *>( t );
//            } );
//
//        // Register the pointer
//        ar.registerSharedPointer( id, ptr );
//
//        // Perform the actual loading and allocation
//        memory_detail::loadAndConstructSharedPtr( ar, ptr.get(), typename ::cereal::traits::has_shared_from_this<T>::type() );
//
//        // Mark pointer as valid (initialized)
//        *valid = true;
//    }
//    else
//        ptr = std::static_pointer_cast<T>(ar.getSharedPointer(id));
//}
//







}

/// Register a class with serialization library. This macro can be optional, but most of the time it is not. Macro
/// should be called from .cpp file of the class in a global namespace. `Version` will be serialized with the class and
/// passed to serialization function when loading.
#define URHO3D_SERIALIZABLE(Class, Version)                                                                \
    CEREAL_CLASS_VERSION(Class, Version);                                                                  \
    CEREAL_REGISTER_TYPE(Class);                                                                           \
    CEREAL_REGISTER_POLYMORPHIC_RELATION(Class::BaseClassName, Class)                                      \

namespace Urho3D
{

/// Simple bridge allowing std::ostream to output to %Urho3D::Serializer.
struct SerializerStream : public std::streambuf
{
    /// Construct.
    explicit SerializerStream(Serializer& destination, unsigned bufferSize=1024 * 1024)
        : output_(destination)
        , buffer_(bufferSize)
    {
        auto* buffer = buffer_.Buffer();
        setp(buffer, buffer + buffer_.Size());
    }

    /// Destruct.
    ~SerializerStream() override
    {
        Flush();
    }

protected:
    /// Write out all pending data.
    int Flush()
    {
        if (pptr() - pbase() > 0)
            return output_.Write(pbase(), static_cast<unsigned int>(pptr() - pbase()));
        return 0;
    }

    /// Too much data in the buffer, write it out.
    int_type overflow(int_type c) override
    {
        if (pptr() - pbase() > 0)
        {
            if (Flush() == 0)
                return traits_type::not_eof(c);
        }
        auto* buffer = buffer_.Buffer();
        setp(buffer, buffer + buffer_.Size());
        buffer_[0] = traits_type::to_char_type(c);
        pbump(1);
        return traits_type::not_eof(c);
    }

    /// Output device.
    Serializer& output_;
    /// Output buffer.
    PODVector<char> buffer_;
};

/// Simple bridge allowing std::istream to read from Urho3D::Deserializer.
struct DeserializerStream : public std::streambuf
{
    /// Construct.
    explicit DeserializerStream(Deserializer& source, unsigned bufferSize=1024 * 1024)
        : source_(source)
        , buffer_(bufferSize)
    {
        auto* buffer = buffer_.Buffer();
        setg(buffer, buffer, buffer);
    }

protected:
    /// Not enough data available in the input buffer, read some more.
    int_type underflow() override
    {
        unsigned len = source_.Read(buffer_.Buffer(), buffer_.Size());
        if (len == 0)
            return traits_type::eof();

        auto* buffer = buffer_.Buffer();
        setg(buffer, buffer, buffer + len);
        return traits_type::to_int_type(buffer_[0]);
    }

    /// Input device.
    Deserializer& source_;
    /// Input buffer.
    PODVector<char> buffer_;
};

}
