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
        typedef std::tr1::shared_ptr<Component> Ptr;
        typedef std::tr1::shared_ptr<const Component> ConstPtr;
        
        Component(const std::string &theName = "Component");
        virtual ~Component();
        
        const std::string& getName() const { return m_name; };
        const std::list<Property::Ptr>& getPropertyList() const;
        Property::Ptr getPropertyByName(const std::string & thePropertyName);

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
