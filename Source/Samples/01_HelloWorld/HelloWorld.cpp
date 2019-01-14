//
// Copyright (c) 2008-2019 the Urho3D project.
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

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Core/ProcessUtils.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/UI.h>

#include "Urho3D/Scene/Serialization.h"
#include "HelloWorld.h"

#include <Urho3D/DebugNew.h>

// Expands to this example's entry-point
URHO3D_DEFINE_APPLICATION_MAIN(HelloWorld)

HelloWorld::HelloWorld(Context* context) :
    Sample(context)
{
}

class PolymorphicTest : public Serializable
{
    URHO3D_OBJECT(PolymorphicTest, Serializable);
    virtual void Serialize(::cereal::BinaryInputArchive& ar) { ar(*this); }
    virtual void Serialize(::cereal::BinaryOutputArchive& ar) { ar(*this); }
    virtual void Serialize(::cereal::PortableBinaryInputArchive& ar) { ar(*this); }
    virtual void Serialize(::cereal::PortableBinaryOutputArchive& ar) { ar(*this); }
    virtual void Serialize(::cereal::XMLInputArchive& ar) { ar(*this); }
    virtual void Serialize(::cereal::XMLOutputArchive& ar) { ar(*this); }
    virtual void Serialize(::cereal::JSONInputArchive& ar) { ar(*this); }
    virtual void Serialize(::cereal::JSONOutputArchive& ar) { ar(*this); }

public:
    explicit PolymorphicTest(Context* context) : Serializable(context)
    {
    }

    static void RegisterObject(Context* context)
    {
        context->RegisterFactory<PolymorphicTest>();
        URHO3D_ATTRIBUTE("Poly", int, poly_, 999, AM_DEFAULT);
    }

    int poly_ = 999;
};

class CerealTest : public Serializable
{
    URHO3D_OBJECT(CerealTest, Serializable);
    virtual void Serialize(::cereal::BinaryInputArchive& ar) { ar(*this); }
    virtual void Serialize(::cereal::BinaryOutputArchive& ar) { ar(*this); }
    virtual void Serialize(::cereal::PortableBinaryInputArchive& ar) { ar(*this); }
    virtual void Serialize(::cereal::PortableBinaryOutputArchive& ar) { ar(*this); }
    virtual void Serialize(::cereal::XMLInputArchive& ar) { ar(*this); }
    virtual void Serialize(::cereal::XMLOutputArchive& ar) { ar(*this); }
    virtual void Serialize(::cereal::JSONInputArchive& ar) { ar(*this); }
    virtual void Serialize(::cereal::JSONOutputArchive& ar) { ar(*this); }

public:
    explicit CerealTest(Context* context) : Serializable(context)
    {
    }

    static void RegisterObject(Context* context)
    {
        context->RegisterFactory<CerealTest>();
        URHO3D_ATTRIBUTE("Integer Foo", int, foo_, 0, AM_DEFAULT);
    }

    template<typename Archive>
    void serialize(Archive& ar)
    {
        ar(::cereal::make_nvp(BaseClassName::GetTypeNameStatic().CString(), cereal::base_class<BaseClassName>(this))
            ,CEREAL_NVP(hash_)
            ,CEREAL_NVP(vector2_)
            ,CEREAL_NVP(string_)
            ,CEREAL_NVP(poly_)
        );
    }

    int foo_ = 0;
    Vector2 vector2_{12, 34};
    StringHash hash_;
    String string_{"foobar"};
    SharedPtr<PolymorphicTest> poly_;
};

//URHO3D_SERIALIZABLE(CerealTest, 1);


void HelloWorld::Start()
{
    CerealTest::RegisterObject(context_);
    PolymorphicTest::RegisterObject(context_);

    {
        SharedPtr<CerealTest> test = context_->CreateObject<CerealTest>();
        {
            test->foo_ = 123;
            test->hash_ = StringHash(555);
            File file(context_, "/tmp/cereal_test.xml", FILE_WRITE);
            test->Save(file, SF_XML);
        }
        {
            test->foo_ = 1234;
            test->hash_ = StringHash(555);
            File file(context_, "/tmp/cereal_test.json", FILE_WRITE);
            test->Save(file, SF_JSON);
        }
        {
            test->foo_ = 12345;
            test->hash_ = StringHash(555);
            File file(context_, "/tmp/cereal_test.bin", FILE_WRITE);
            test->Save(file, SF_BINARY);
        }
    }

    {
        SharedPtr<CerealTest> test = context_->CreateObject<CerealTest>();
        {
            File file(context_, "/tmp/cereal_test.xml");
            test->Load(file, SF_XML);
            assert(test->foo_ == 123);
            assert(test->hash_.Value() == 555);
        }
        {
            File file(context_, "/tmp/cereal_test.json");
            test->Load(file, SF_JSON);
            assert(test->foo_ == 1234);
            assert(test->hash_.Value() == 555);
        }
        {
            File file(context_, "/tmp/cereal_test.bin");
            test->Load(file, SF_BINARY);
            assert(test->foo_ == 12345);
            assert(test->hash_.Value() == 555);
        }
    }


    exit(0);

    // Execute base class startup
    Sample::Start();

    // Create "Hello World" Text
    CreateText();

    // Finally subscribe to the update event. Note that by subscribing events at this point we have already missed some events
    // like the ScreenMode event sent by the Graphics subsystem when opening the application window. To catch those as well we
    // could subscribe in the constructor instead.
    SubscribeToEvents();

    // Set the mouse mode to use in the sample
    Sample::InitMouseMode(MM_FREE);
}

void HelloWorld::CreateText()
{
    auto* cache = GetSubsystem<ResourceCache>();

    // Construct new Text object
    SharedPtr<Text> helloText(new Text(context_));

    // Set String to display
    helloText->SetText("Hello World from Urho3D!");

    // Set font and text color
    helloText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 30);
    helloText->SetColor(Color(0.0f, 1.0f, 0.0f));

    // Align Text center-screen
    helloText->SetHorizontalAlignment(HA_CENTER);
    helloText->SetVerticalAlignment(VA_CENTER);

    // Add Text instance to the UI root element
    GetSubsystem<UI>()->GetRoot()->AddChild(helloText);
}

void HelloWorld::SubscribeToEvents()
{
    // Subscribe HandleUpdate() function for processing update events
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(HelloWorld, HandleUpdate));
}

void HelloWorld::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    // Do nothing for now, could be extended to eg. animate the display
}
