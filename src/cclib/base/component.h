//
//  COMPONENT.h
//  ATS
//
//  Created by Sebastian Heymann on 8/11/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __CCLIB_COMPONENT_INCLUDED__
#define __CCLIB_COMPONENT_INCLUDED__

#include <cclib.h>
#include "property.h"

namespace cclib
{

    class Component
    {
    public:
        typedef CC_PTR<Component> Ptr;
        typedef CC_PTR<const Component> ConstPtr;

        Component(const std::string &theName = "Component");
        virtual ~Component();

        const std::string& getName() const {
            return m_name;
        };

        void setName(const std::string & theName) {
            m_name = std::string(theName);
        };

        const std::list<Property::Ptr>& getPropertyList() const;
        Property::Ptr getPropertyByName(const std::string & thePropertyName);

        template<typename T>
        inline void set(const std::string & thePropertyName, T & theValue) {
            Property::Ptr myProperty = getPropertyByName(thePropertyName);
            myProperty->setValue<T>(theValue);
        }

        template<typename T>
        inline T & get(const std::string & thePropertyName) {
            Property::Ptr myProperty = getPropertyByName(thePropertyName);
            return myProperty->getValue<T>();
        }

        virtual void updateProperty(const Property::ConstPtr &theProperty){};

    protected:
        void registerProperty(Property::Ptr theProperty);

    private:
        std::string m_name;
        std::list<Property::Ptr> m_propertyList;
    };

    // Exception definitions. TODO: put those to some neat macros
    class PropertyNotFoundException : public Exception
    {
    public:
        PropertyNotFoundException(std::string thePropertyName):
        Exception(std::string("Named Property not found: ")+thePropertyName)
        {}
    };

    class ComponentError: public Exception
    {
    public:
        ComponentError(std::string theErrorString):
        Exception(std::string("ComponentError: ")+theErrorString)
        {}
    };
}

#endif // __CCLIB_COMPONENT_INCLUDED__
